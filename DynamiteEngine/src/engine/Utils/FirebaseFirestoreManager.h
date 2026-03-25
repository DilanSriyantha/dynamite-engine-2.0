#ifndef FIREBASE_FIRESTORE_MANAGER_H
#define FIREBASE_FIRESTORE_MANAGER_H

#include <string>
#include <functional>
#include "Game.h"

struct LeaderboardResult {
	bool success = false;
	char username[256];
	int score;
	char error_message[256];
};

using CreateCompleteCallback = std::function<void(bool)>;
using GetCompleteCallback = std::function<void(LeaderboardResult)>;

class FirebaseFirestoreManager {
public:
	void createEntry(std::string user_id, std::string id_token, std::string username, size_t high_score, CreateCompleteCallback callback)
	{
		auto& http = Game::getInstance().getHttpRequestManager();

		std::vector<std::string> headers = {
			"Authorization: Bearer " + id_token
		};

		std::string url = docUrl(user_id);
		std::string payload = makePayload(user_id, id_token, username, high_score);

		http.patchWithExtraHeaders(url, payload, headers, [callback](json response) {
			std::cout << response << "\n";
			
			if (response.contains("createTime"))
			{
				callback(true);
				return;
			}
			
			callback(false);
		});
	}

	void readEntry(const std::string& user_id, const std::string& id_token, GetCompleteCallback callback)
	{
		auto& http = Game::getInstance().getHttpRequestManager();

		std::vector<std::string> headers = {
			"Authorization: Bearer " + id_token
		};

		std::string url = docUrl(user_id);
		
		http.getWithExtraHeaders(url, headers, [callback](json response) {
			callback(parseLeaderboardResult(response));
		});
	}

private:
	static std::string makePayload(std::string user_id, std::string id_token, std::string username, size_t high_score)
	{
		json body = {
			{ "fields", {
				{ "username", { { "stringValue", username } }},
				{ "highscore", { { "integerValue", std::to_string(high_score)} }}
			}}
		};

		return body.dump();
	}

	static LeaderboardResult parseLeaderboardResult(const json& j)
	{
		LeaderboardResult result;

		std::cout << j << std::endl;

		if (j.contains("fields"))
		{
			result.success = true;
			strcpy_s(result.username, sizeof(result.username), j["fields"]["username"].value("stringValue", "").c_str());
			result.score = std::stoi(j["fields"]["highscore"].value("integerValue", "0"));
		}
		else
		{
			result.success = false;
			if (j.contains("error"))
				strcpy_s(result.error_message, sizeof(result.error_message), j["error"].value("message", "Unknown error").c_str());
		}

		return result;
	}

	std::string docUrl(const std::string& doc_id)
	{
		return "https://firestore.googleapis.com/v1/projects/junglequest-a5e98/databases/(default)/documents/leaderboard/" + doc_id;
	}
};

#endif