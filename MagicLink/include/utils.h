#pragma once

#include <windows.h>
#include <string>

void createFile(std::wstring filename);

bool operator==(const FILETIME& ft1, const FILETIME& ft2);