#include "utils.h"
#include <fstream>

void createFile(std::wstring filename)
{
	std::ofstream ofs;
	ofs.open(filename, std::ofstream::out | std::ofstream::trunc);

	if (ofs)
	{
		ofs << "Sync file" << std::endl;

		ofs.close();
	}
}

bool operator==(const FILETIME & ft1, const FILETIME & ft2)
{
	return ft1.dwHighDateTime == ft2.dwHighDateTime && ft1.dwLowDateTime == ft2.dwLowDateTime;
}
