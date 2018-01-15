#include <windows.h>
#include <tchar.h> 
#include <cstdio>
#include <strsafe.h>
#include <iostream>
#pragma comment(lib, "User32.lib")

#include "MagicSynchronizer.h"
#include "FileExplorer.h"
#include "MagicServer.h"
#include "utils.h"
#include "TcpFile.h"

int _tmain(int argc, TCHAR *argv[])
{
	if (argc == 1)
	{
		std::cout << "Usage: \n" << "send / serv" << std::endl;
		return -1;
	}

	std::cout << "send path_to_file ip port" << std::endl;
	std::cout << "rec" << std::endl;

	std::vector<std::wstring> args;
	for (int i(0); i < argc; ++i)
	{
		args.push_back(argv[i]);
	}

	/*FileExplorer fe(args[1]);
	json j = fe.getFileTree().toJson();

	FileTree ft(j);

	fe.getFileTree().print(L"    ");
	ft.print(L"    ");

	return 0;*/

	MagicServer server;

	if(args[1] == L"serv")
	{
		server.loadConfigFile(toStr(args[2]));
		server.setPort(std::stoi(args[3]));
		server.start();
	}
	else if (args[1] == L"-l")
	{
		server.loadConfigFile(toStr(args[2]));
		server.listExposedDirectories();
	}
	else if (args[1] == L"test")
	{
		if (args[2] == L"send")
		{
			int port = std::stoi(args[5]);
			TcpFile::send_file(toStr(args[3]), 0, toStr(args[4]), port);
		}
		else if (args[2] == L"rec")
		{
			const int port = 8081;
			std::cout << "Waiting data on port " << port << std::endl;
			TcpFile::wait_file(port);
		}
	}



	/*FileExplorer fe(argv[1]);
	fe.getFileTree().print(L"   ");

	MagicSynchronizer sync(argv[1], argv[2]);
	sync.startSync();*/

	return 0;
}
