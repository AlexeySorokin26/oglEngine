#pragma once

#include <fstream>
#include <string>

// Чтение шейдеров из файла
static bool ReadFile(const char* pFileName, std::string& outFile) {
	std::ifstream f(pFileName);
	bool ret = false;
	if (f.is_open()) {
		std::string line;
		while (getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}
		f.close();
		ret = true;
	}
	else {
		throw std::runtime_error("file not opened: " + std::string(pFileName));
	}
	return ret;
}