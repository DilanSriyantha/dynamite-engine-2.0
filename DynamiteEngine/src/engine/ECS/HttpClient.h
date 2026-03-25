#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct MemoryStruct {
	char* memory;
	size_t size;
};

class HttpClient {
public:
	HttpClient()
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
	}

	~HttpClient()
	{
		curl_global_cleanup();
	}

	std::string get(const std::string& url)
	{
		CURL* curl = curl_easy_init();
		std::string response;

		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "DynamiteEngine/2.0");
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
			
			CURLcode res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
			curl_easy_cleanup(curl);
		}

		return response;
	}

	std::string getWithExtraHeaders(const std::string& url, const std::vector<std::string>& extra_headers)
	{
		CURL* curl = curl_easy_init();
		std::string response;

		if (curl)
		{
			struct curl_slist* headers = nullptr;
			headers = curl_slist_append(headers, ("Content-Type: application/json"));
			for (auto& h : extra_headers)
				headers = curl_slist_append(headers, h.c_str());

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "DynamiteEngine/2.0");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

			CURLcode res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
			curl_easy_cleanup(curl);
		}

		return response;
	}

	std::string post(const std::string& url, const std::string& data, const std::string& contentType = "application/json")
	{
		CURL* curl = curl_easy_init();
		std::string response;

		if (curl)
		{
			struct curl_slist* headers = nullptr;
			headers = curl_slist_append(headers, ("Content-Type: " + contentType).c_str());

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
			curl_easy_perform(curl);
			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
		}

		return response;
	}

	std::string postWithExtraHeaders(const std::string& url, const std::string& data, const std::vector<std::string>& extra_headers)
	{
		CURL* curl = curl_easy_init();
		std::string response;

		if (curl)
		{
			struct curl_slist* headers = nullptr;

			// add headers
			headers = curl_slist_append(headers, ("Content-Type: application/json"));

			// add extra headers
			for (const auto& h : extra_headers)
				headers = curl_slist_append(headers, h.c_str());

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
			curl_easy_perform(curl);
			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
		}

		return response;
	}

	std::string patchWithExtraHeaders(const std::string& url, const std::string& data, const std::vector<std::string>& extra_headers)
	{
		CURL* curl = curl_easy_init();
		std::string response;

		if (curl)
		{
			struct curl_slist* headers = nullptr;

			// add headers
			headers = curl_slist_append(headers, ("Content-Type: application/json"));

			// add extra headers
			for (const auto& h : extra_headers)
				headers = curl_slist_append(headers, h.c_str());

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
			curl_easy_perform(curl);
			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
		}

		return response;
	}

	MemoryStruct downloadToMemory(const std::string& url)
	{
		CURL* curl = curl_easy_init();
		MemoryStruct chunk;
		chunk.memory = (char*)malloc(1);
		chunk.size = 0;

		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "DynamiteEngine/2.0");
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

			CURLcode res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
			curl_easy_cleanup(curl);
		}

		return chunk;
	}

private:
	static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		size_t total_size = size * nmemb;
		((std::string*)userp)->append((char*)contents, total_size);
		
		return total_size;
	}

	static size_t writeMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		size_t total_size = size * nmemb;
		MemoryStruct* mem = (MemoryStruct*)userp;

		char* ptr = (char*)realloc(mem->memory, mem->size + total_size + 1);
		if (!ptr)
		{
			std::cout << "not enough memory (realloc returned NULL)\n";
			return 0;
		}

		mem->memory = ptr;
		memcpy(&(mem->memory[mem->size]), contents, total_size);
		mem->size += total_size;
		mem->memory[mem->size] = 0;

		return total_size;
	}
};

#endif