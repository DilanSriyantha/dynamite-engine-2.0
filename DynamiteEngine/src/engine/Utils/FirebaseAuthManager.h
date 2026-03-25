#ifndef FIREBASE_AUTH_MANAGER_H
#define FIREBASE_AUTH_MANAGER_H

#include <string>
#include <functional>
#include "HttpRequestManager.h"
#include "Game.h"
#include "ConfigTypes.h"

struct AuthResult {
	bool success = false;
	char id_token[2048];
	char email[256];
	char refresh_token[2048];
	char local_id[2048];
	char error_message[512];
	long long expires_at;
};

struct RefreshResult {
	bool success = false;
	char id_token[2048];
	char refresh_token[2048];
	char error_message[512];
	long long expires_at;
};

enum class LoginState {
	LOGGED_IN,
	NOT_LOGGED_IN
};

class FirebaseAuthManager {
public:
	using AuthCallback = std::function<void(AuthResult)>;
	using LoginStateCallback = std::function<void(LoginState)>;
	using UserLoggedInCallback = std::function<void(AuthResult)>;
	using UserNotLoggedInCallback = std::function<void()>;

	FirebaseAuthManager() = default;

	void signUp(const std::string& email, const std::string& password, AuthCallback callback)
	{
		auto& http = Game::getInstance().getHttpRequestManager();
		
		http.post(signUpUrl(), makePayload(email, password), [callback](json response) {
			if (callback) callback(parseAuthResult(response));
		});
	}

	void signIn(const std::string& email, const std::string& password, AuthCallback callback)
	{
		auto& http = Game::getInstance().getHttpRequestManager();
		
		http.post(signInUrl(), makePayload(email, password), [this, callback](json response) {
			AuthResult p_res = parseAuthResult(response);

			if (!p_res.success)
			{
				callback(p_res);
				return;
			}

			saveAuthCache(p_res, [callback, p_res](){
				callback(p_res);
			});
		});
	}

	void checkLogin(UserLoggedInCallback logged_in, UserNotLoggedInCallback not_logged_in)
	{
		loadAuthCache(
			[this, logged_in, not_logged_in](AuthResult cache) {
				if (!isSessionExpired(cache))
				{
					logged_in(cache);
					return;
				}

				refresh(cache, [this, logged_in, not_logged_in](AuthResult res) {
					if (res.success)
						saveAuthCache(res, [logged_in, res]() {
							logged_in(res);
						});
					else
						not_logged_in();
				});
			},
			[not_logged_in](std::string error) {
				std::cout << "firebase_auth_manager error: " << error << "\n";

				not_logged_in();
			}
		);
	}

private:
	const std::string FIREBASE_API_KEY = "AIzaSyBmibkHIDwQv3EeSGZ8FF7CozXixexd6Zk";

	std::string signUpUrl() const 
	{
		return "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + FIREBASE_API_KEY;
	}

	std::string signInUrl() const
	{
		return "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + FIREBASE_API_KEY;
	}

	std::string refreshUrl() const
	{
		return  "https://securetoken.googleapis.com/v1/token?key=" + FIREBASE_API_KEY;
	}

	static std::string makePayload(const std::string& email, const std::string& password)
	{
		json j;
		j["email"] = email;
		j["password"] = password;
		j["returnSecureToken"] = true;

		return j.dump();
	}

	static std::string makeRefreshPayload(const AuthResult& cached_auth)
	{
		json j;
		j["grant_type"] = "refresh_token";
		j["refresh_token"] = std::string(cached_auth.refresh_token);

		return j.dump();
	}

	static AuthResult parseAuthResult(const json& j)
	{
		AuthResult result;

		if (j.contains("idToken"))
		{
			result.success = true;
			strcpy_s(result.id_token, sizeof(result.id_token), j.value("idToken", "").c_str());
			strcpy_s(result.email, sizeof(result.email), j.value("email", "").c_str());
			strcpy_s(result.refresh_token, sizeof(result.refresh_token), j.value("refreshToken", "").c_str());
			strcpy_s(result.local_id, sizeof(result.local_id), j.value("localId", "").c_str());
			result.expires_at = std::stol(j.value("expiresIn", "0")) + std::time(nullptr);
		}
		else
		{
			result.success = false;
			if (j.contains("error"))
				strcpy_s(result.error_message, sizeof(result.error_message), j["error"].value("message", "Unknown error").c_str());
		}

		return result;
	}

	static RefreshResult parseRefreshResult(const json& j)
	{
		RefreshResult result;

		if (j.contains("id_token"))
		{
			result.success = true;
			strcpy_s(result.id_token, sizeof(result.id_token), j.value("id_token", "").c_str());
			strcpy_s(result.refresh_token, sizeof(result.refresh_token), j.value("refresh_token", "").c_str());
			result.expires_at = std::stol(j.value("expires_in", "0")) + std::time(nullptr);
		}
		else
		{
			result.success = false;
			if (j.contains("error"))
				strcpy_s(result.error_message, sizeof(result.error_message), j["error"].value("message", "Unknown error").c_str());
		}

		return result;
	}

	bool isSessionExpired(AuthResult res)
	{
		long long now = std::time(nullptr);

		if (now < res.expires_at)
			return false;
		return true;
	}

	void refresh(AuthResult& auth_res, AuthCallback callback)
	{
		auto& http = Game::getInstance().getHttpRequestManager();

		std::cout << signInUrl() << "\n";
		std::cout << refreshUrl() << "\n";
		std::cout << makeRefreshPayload(auth_res) << "\n";

		http.post(refreshUrl(), makeRefreshPayload(auth_res), [callback, &auth_res](json response) {
			std::cout << response << "\n";

			RefreshResult ref_res = parseRefreshResult(response);

			if (!ref_res.success)
			{
				auth_res.success = false;
				callback(auth_res);

				return;
			}

			strcpy_s(auth_res.id_token, 2048, ref_res.id_token);
			strcpy_s(auth_res.refresh_token, 2048, ref_res.refresh_token);
			auth_res.expires_at = ref_res.expires_at;

			callback(auth_res);
		});
	}

	void saveAuthCache(AuthResult auth_res, std::function<void()> callback)
	{
		auto& fsm = Game::getInstance().getFileSystemManager();
		fsm.write(AUTH_CACHE_FILE, auth_res, callback);
	}

	void loadAuthCache(std::function<void(AuthResult)> success, std::function<void(std::string)> error)
	{
		auto& fsm = Game::getInstance().getFileSystemManager();
		fsm.read(AUTH_CACHE_FILE, success, error);
	}
};

#endif