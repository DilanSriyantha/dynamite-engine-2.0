#ifndef HTTP_REQUEST_MANAGER_H
#define HTTP_REQUEST_MANAGER_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include "HttpClient.h"

class HttpRequestManager {
public:
	using ResponseCallback = std::function<void(json)>;

	HttpRequestManager()
		: stop_flag(false)
	{
		t_worker = std::thread(&HttpRequestManager::processQueue, this);
	}

	~HttpRequestManager()
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			stop_flag = true;
		}
		cv.notify_all();

		if (t_worker.joinable())
			t_worker.join();
	}

	std::future<json> getAsync(const std::string& url)
	{
		auto promise = std::make_shared<std::promise<json>>();

		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, url, promise] {
				std::string response = client.get(url);
				promise->set_value(json::parse(response));
				});
		}
		cv.notify_one();
		return promise->get_future();
	}

	void get(const std::string& url, ResponseCallback callback)
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, url, callback]() {
				std::string response = client.get(url);
				callback(json::parse(response));
				});
		}
		cv.notify_one();
	}

	void getWithExtraHeaders(const std::string& url, const std::vector<std::string>& headers, ResponseCallback callback)
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, url, headers, callback]() {
				std::string response = client.getWithExtraHeaders(url, headers);
				callback(json::parse(response));
			});
		}
		cv.notify_one();
	}

	std::future<json> postAsync(const std::string& url, const std::string& data)
	{
		auto promise = std::make_shared<std::promise<json>>();
		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, url, data, promise] {
				std::string response = client.post(url, data);
				promise->set_value(json::parse(response));
				});
		}
		cv.notify_one();
		return promise->get_future();
	}

	void post(const std::string& url, const std::string& data, ResponseCallback callback)
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, url, data, callback]() {
				std::string response = client.post(url, data);
				callback(json::parse(response));
			});
		}
		cv.notify_one();
	}

	void postWithExtraHeaders(const std::string& url, const std::string& data, const std::vector<std::string>& extra_headers, ResponseCallback callback)
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, url, data, extra_headers, callback]() {
				std::string response = client.postWithExtraHeaders(url, data, extra_headers);
				callback(json::parse(response));
			});
		}
		cv.notify_one();
	}

	void patchWithExtraHeaders(const std::string& url, const std::string& data, const std::vector<std::string>& extra_headers, ResponseCallback callback)
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, url, data, extra_headers, callback]() {
				std::string response = client.patchWithExtraHeaders(url, data, extra_headers);
				callback(json::parse(response));
			});
		}
		cv.notify_one();
	}

	void downloadToMemory(const std::string& url, std::function<void(MemoryStruct mem_struct)> callback)
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, url, callback] {
				auto response = client.downloadToMemory(url);
				if (callback) callback(response);
			});
		}
		cv.notify_one();
	}

	std::future<MemoryStruct> downloadToMemoryAsync(const std::string& url)
	{
		auto promise = std::make_shared<std::promise<MemoryStruct>>();
		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, url, promise] {
				auto response = client.downloadToMemory(url);
				promise->set_value(response);
			});
		}
		cv.notify_one();
		return promise->get_future();
	}

private:
	HttpClient client;
	std::queue<std::function<void()>> tasks;
	std::thread t_worker;
	std::condition_variable cv;
	std::mutex mutex;

	bool stop_flag;

	void processQueue()
	{
		while (true)
		{
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(mutex);
				cv.wait(lock, [&]() { return !tasks.empty() || stop_flag; });

				if (tasks.empty() || stop_flag)
					break;

				task = std::move(tasks.front());
				tasks.pop();
			}
			task();
		}
	}
};

#endif