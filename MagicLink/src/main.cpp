#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

#include "FileExplorer.h"

int _tmain(int argc, TCHAR *argv[])
{
	FileExplorer fe(argv[1]);

	fe.getFileTree().print(L"");

	return 0;
}
