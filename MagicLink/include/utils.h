#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <SFML\Network.hpp>

#include "DistantDirectory.h"

void createFile(std::wstring filename, char* data, size_t length);

bool operator==(const FILETIME& ft1, const FILETIME& ft2);

std::vector<char> importFile(const std::wstring& filename);

void sendData(const std::string& ip, int port, const char* data, size_t length);

std::string toStr(const std::wstring& wstr); 

std::wstring toWstr(const std::string& str);