#ifndef GLOBALS_H
#define GLOBALS_H

#include <Windows.h>
#include <string>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

inline std::string getExecutableDir()
{
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);

	std::string exe_path(path);
	size_t pos = exe_path.find_last_of("\\/");

	return exe_path.substr(0, pos);
}

#endif