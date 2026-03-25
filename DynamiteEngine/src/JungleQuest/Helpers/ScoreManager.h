#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <unordered_map>
#include "Game.h"
#include "ConfigTypes.h"
#include <functional>

using ScoreMap = std::unordered_map<std::string, size_t>;
using ScoreMapReadCompleteCallback = std::function<void(ScoreMap, bool)>;

class ScoreManager {
public:
	ScoreManager() = default;

	void recordScore(std::string name, size_t score)
	{
		readFromStorage([this, &name, &score](ScoreMap score_map, bool is_successfull) { 
			auto it = score_map.find(name);
			if (it == score_map.end())
			{
				score_map[name] = score;
				saveScoreMap(score_map);
			}
			else
			{
				if (it->second > score) return;

				score_map[name] = score;
				saveScoreMap(score_map);
			}
		});
	}

	void getScore(std::string key, std::function<void(size_t)> callback)
	{
		readFromStorage([&key, callback](ScoreMap score_map, bool is_successfull) {
			auto it = score_map.find(key);
			if (it == score_map.end())
			{
				callback(0);
				return;
			}
			callback(it->second);
		});
	}

private:
	void readFromStorage(ScoreMapReadCompleteCallback callback)
	{
		auto& fsm = Game::getInstance().getFileSystemManager();

		fsm.read<ScoreMap>(SCORE_MAP_FILE, callback);
	}

	void saveScoreMap(ScoreMap& score_map)
	{
		auto& fsm = Game::getInstance().getFileSystemManager();

		fsm.write(SCORE_MAP_FILE, score_map, []() {
			std::cout << "score map has been saved successfully.\n";
		});
	}
};

#endif