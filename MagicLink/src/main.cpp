#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
#pragma comment(lib, "User32.lib")

#include "MagicSynchronizer.h"
#include "FileExplorer.h"

void RefreshDirectory(const std::wstring&, const std::wstring&);
void WatchDirectory(LPTSTR, LPTSTR);

int _tmain(int argc, TCHAR *argv[])
{
	if (argc != 3)
	{
		_tprintf(TEXT("Usage: %s <dir>\n"), argv[0]);
		return -1;
	}

	FileExplorer fe(argv[1]);
	fe.getFileTree().print(L"   ");

	MagicSynchronizer sync(argv[1], argv[2]);
	sync.startSync();

	return 0;
}