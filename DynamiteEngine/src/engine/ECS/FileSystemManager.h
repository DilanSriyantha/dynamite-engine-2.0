#ifndef FILE_SYSTEM_MANAGER_H
#define FILE_SYSTEM_MANAGER_H

#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <fstream>
#include <string>
#include <iostream>
#include <functional>

class FileSystemManager {
public:
	template <typename Type> using ReadSuccessCallback = std::function<void(Type)>;
	template <typename Type> using ReadCompleteCallback = std::function<void(Type, bool)>;
	using ReadErrorCallback = std::function<void(std::string)>;

	using WriteCompleteCallback = std::function<void()>;

	FileSystemManager() : stop_flag(false)
	{
		t_worker = std::thread(&FileSystemManager::processQueue, this);
	}

	~FileSystemManager()
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			stop_flag = true;
		}
		cv.notify_all();

		if (t_worker.joinable()) t_worker.join();
	}

	template<typename Type>
	void write(const std::string& filename, Type& obj, WriteCompleteCallback callback = NULL)
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, filename, obj, callback] {
				writeBinFile(filename, obj);

				if (callback) callback();
			});
		}
		cv.notify_one();
	}

	template<typename Type>
	void read(const std::string& filename, ReadSuccessCallback<Type> success = NULL, ReadErrorCallback error = NULL)
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, filename, success, error] {
				Type obj{};
				if (readBinFile(filename, &obj))
				{
					if(success) success(obj);
				}
				else
				{
					if (error) error("something went wrong");
				}
			});
		}
		cv.notify_one();
	}

	template<typename Type>
	void read(const std::string& filename, ReadCompleteCallback<Type> callback = NULL)
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			tasks.emplace([this, filename, callback] {
				Type obj{};
				if (readBinFile(filename, &obj))
				{
					if (callback) callback(obj, true);
				}
				else
				{
					if (callback) callback(obj, false);
				}
			});
		}
		cv.notify_one();
	}

private:
	std::queue<std::function<void()>> tasks;

	std::thread t_worker;
	std::mutex mutex;
	std::condition_variable cv;

	bool stop_flag;

	void processQueue()
	{
		while (true)
		{
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(mutex);
				cv.wait(lock, [&] {
					return !tasks.empty() || stop_flag;
				});

				if (tasks.empty() || stop_flag)
					break;

				task = std::move(tasks.front());
				tasks.pop();
			}
			task();
		}
	}

	template<typename Type>
	bool writeBinFile(const std::string& filename, Type& src)
	{
		std::fstream file(filename, std::ios::out | std::ios::binary | std::ios::trunc);

		if (!file.is_open())
		{
			std::cout << "failed to open file " << filename << "\n";
			return 0;
		}

		// write the size of the object
		const size_t size = sizeof(src);
		file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));

		// write the content/object
		file.write(reinterpret_cast<const char*>(&src), size);

		file.close();

		if (!file.good())
		{
			std::cout << "could not write to file " << filename << "\n";
			return 0;
		}

		std::cout << "file has been written >> " << filename << "\n";

		return 1;
	}

	template <typename Type>
	bool readBinFile(const std::string& filename, Type* dest)
	{
		std::fstream file{ filename, std::ios::in | std::ios::binary };

		if (!file.is_open())
		{
			std::cout << "failed to open file " << filename << "\n";
			return 0;
		}

		// read the size of the content/object
		size_t size = 0;
		file.read(reinterpret_cast<char*>(&size), sizeof(size_t));

		// read the content/object
		file.read(reinterpret_cast<char*>(dest), size);

		if (!file)
		{
			std::cout << "error reading file " << filename << "\n";
			return 0;
		}

		file.close();

		if (!file.good())
		{
			std::cout << "error reading from file " << filename << "\n";
			return 0;
		}

		return 1;
	}
};

#endif