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

	std::cout << "Loaded " << buffer.size() << " bytes" << std::endl;

	return buffer;
}

void sendData(const std::string& ip, int port, const char * data, size_t length)
{
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(ip, port);

	//std::cout << "Trying to connect to: " << ip << ":" << port << "..." << std::endl;
	if (status != sf::Socket::Done)
	{
		std::cout << "[ERROR] Cannot connect to " << ip << ":" << port << std::endl;
	}
	else
	{
		//std::cout << "[OK] Connected" << std::endl;
	}

	size_t sent = 0;
	if (socket.send(data, length, sent) != sf::Socket::Done)
	{
		std::cout << "[ERROR] Cannot send data" << std::endl;
	}

	std::cout << "Sent: " << sent << " bytes (" << int(100 * sent / double(length)) << "%)" << std::endl;

	socket.disconnect();
}

std::string toStr(const std::wstring & wstr)
{
	return std::string(wstr.begin(), wstr.end());
}

std::wstring toWstr(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}
