#pragma once

#include <windows.h>
#include <string>
#include <vector>

void createFile(std::wstring filename, char* data, size_t length);

bool operator==(const FILETIME& ft1, const FILETIME& ft2);

std::vector<char> importFile(const std::wstring& filename);