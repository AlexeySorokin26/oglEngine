#pragma once

#include <fstream>
#include <string>

// Чтение шейдеров из файла
static bool ReadFile(const char* pFileName, std::string& outFile) {
	std::ifstream f(pFileName);
	if (f.is_open()) {
		std::string line;
		while (getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}
		f.close();
		return true;
	}
	return false;
}