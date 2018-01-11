#include "utils.h"
#include <fstream>
#include <iostream>

void createFile(std::wstring filename, char* data, size_t length)
{
	std::ofstream ofs;
	ofs.open(filename, std::ofstream::trunc | std::ofstream::binary);

	std::cout << "Creating file of size: " << length << " bytes" << std::endl;

	if (ofs)
	{
		ofs.write(data, length);
		ofs.close();
	}
}

bool operator==(const FILETIME& ft1, const FILETIME& ft2)
{
	return ft1.dwHighDateTime == ft2.dwHighDateTime && ft1.dwLowDateTime == ft2.dwLowDateTime;
}

std::vector<char> importFile(const std::wstring& filename)
{
	std::ifstream input(filename, std::ios::binary);
	// copies all data into buffer
	
	if (!input)
		std::wcout << L"[ERROR] Cannot open file: '" << filename << L"'" << std::endl;

	std::vector<char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));

	return buffer;
}
