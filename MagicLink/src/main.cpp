#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
#pragma comment(lib, "User32.lib")

#include "MagicSynchronizer.h"
#include "FileExplorer.h"
#include "MagicServer.h"
#include "utils.h"

int _tmain(int argc, TCHAR *argv[])
{
	if (argc == 1)
	{
		std::cout << "Usage: \n" << "send / serv" << std::endl;
		return -1;
	}

	std::vector<std::wstring> args;
	for (int i(0); i < argc; ++i)
	{
		args.push_back(argv[i]);
	}

	FileExplorer fe(args[1]);
	json j = fe.getFileTree().toJson();

	std::cout << std::setw(4) << j << std::endl;

	return 0;

	MagicServer server;

	if (args[1].compare(L"send") == 0)
	{
		std::cout << "Send data" << std::endl;
		if (args.size() > 4)
		{
			std::string ip(args[2].begin(), args[2].end());
			int port = std::stoi(args[3]);

			server.sendFile(ip, port, argv[4]);
		}
	}
	else if(args[1].compare(L"serv") == 0)
	{
		server.start();
	}


	/*FileExplorer fe(argv[1]);
	fe.getFileTree().print(L"   ");

	MagicSynchronizer sync(argv[1], argv[2]);
	sync.startSync();*/

	return 0;
}