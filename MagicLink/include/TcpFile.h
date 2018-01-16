#pragma once
#include <SFML/Network.hpp>

enum TCP_FILE_REQUEST_CODE
{
	tfrc_file_from_bytes,
	tfrc_send_from_bytes,
	tfrc_done
};

struct Data
{
	sf::IpAddress address;
	int port;
	std::vector<char> raw_data;
};

struct FileViaTcp
{
	std::string filename;
	std::vector<char> data;
};

struct TcpFileMessage
{
	TcpFileMessage(std::vector<char>& message);
	char code;
	int file_size;
	std::string filename;

	int data_start_index, file_data_size;
};

class TcpFile
{
public:
	static void send_file(const std::string& filename, int start, const std::string& ip, int port);
	static FileViaTcp wait_file(int port = 8080);

	static void wait_connection(sf::TcpSocket& socket, int port);
	static Data first_wait_data(sf::TcpSocket& socket);
	static Data wait_data(sf::TcpSocket& socket);

private:

};