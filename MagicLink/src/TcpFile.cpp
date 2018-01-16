#include "TcpFile.h"
#include "utils.h"
#include <iostream>

void TcpFile::send_file(const std::string& filename, int start, const std::string& ip, int port)
{
	std::cout << "Starting transfer of '" << filename << "' to " << ip << ":" << port << std::endl;
	// TCP socket
	sf::TcpSocket socket;

	// Stores message data
	std::vector<char> header;
	std::vector<char> data;

	// Add protocol code
	header.push_back(TCP_FILE_REQUEST_CODE::tfrc_file_from_bytes);

	// Add filename size
	header.push_back(char(filename.size()));

	// Add filename
	header.insert(header.end(), filename.begin(), filename.end());

	std::cout << "Filename OK" << std::endl;

	// Load file
	std::vector<char> file = importFile(toWstr(filename));

	std::cout << "File loaded OK" << std::endl;

	// Add filesize
	const int n = file.size();
	char bytes[sizeof(int)];
	memcpy(bytes, (char*)&n, sizeof(int));
	
	for (char byte : bytes)
	{
		header.push_back(byte);
	}

	std::cout << "Header OK" << std::endl;

	// Add file
	data.insert(data.end(), header.begin(), header.end());
	data.insert(data.end(), file.begin(), file.end());

	std::cout << "File data OK" << std::endl;

	const auto status = socket.connect(ip, port);

	//std::cout << "Trying to connect to: " << ip << ":" << port << "..." << std::endl;
	if (status != sf::Socket::Done)
	{
		std::cout << "[ERROR] Cannot connect to " << ip << ":" << port << std::endl;
	}

	std::cout << "Connection OK" << std::endl;

	if (socket.send(data.data(), data.size()) != sf::Socket::Done)
	{
		std::cout << "[ERROR] Cannot send data" << std::endl;
	}

	std::cout << "Sent." << std::endl;

	int response_code = tfrc_file_from_bytes;
	/*while (response_code != tfrc_done)
	{
		Data response = wait_data(socket);

		response_code = response.raw_data[0];

		if (response_code == TCP_FILE_REQUEST_CODE::tfrc_send_from_bytes)
		{
			int start_byte;// = (response.raw_data[1] << 24) + (response.raw_data[2] << 16) + (response.raw_data[3] << 8) + response.raw_data[4];
			memcpy(&start_byte, (int*)&response.raw_data[1], sizeof(int));

			std::cout << "Requested file from bytes " << start_byte << std::endl;

			data.clear();
			data.insert(data.end(), header.begin(), header.end());
			data.insert(data.end(), file.begin()+start_byte, file.end());

			if (socket.send(data.data(), data.size()) != sf::Socket::Done)
			{
				std::cout << "[ERROR] Cannot send data" << std::endl;
			}
		}
	}*/
}

FileViaTcp TcpFile::wait_file(int port)
{
	FileViaTcp result;
	std::vector<char> data;
	
	sf::TcpSocket socket;
	wait_connection(socket, port);

	Data response = wait_data(socket);
	TcpFileMessage message(response.raw_data);

	std::cout << "Received '" << message.filename << "' of size " << message.file_size << " bytes" << std::endl;
	result.filename = message.filename;

	int file_data_size = response.raw_data.size() - message.data_start_index;
	data.insert(data.end(), response.raw_data.begin() + message.data_start_index, response.raw_data.end());

	while (file_data_size < message.file_size)
	{
		Data res = wait_data(socket);
		file_data_size += res.raw_data.size();
		data.insert(data.end(), res.raw_data.begin(), res.raw_data.end());
		std::cout << "File data received size " << file_data_size << " bytes (" << int(100 * file_data_size / double(message.file_size)) << "%)" << std::endl;
	}

	result.data = data;

	return result;
}

void TcpFile::wait_connection(sf::TcpSocket& socket, int port)
{
	sf::TcpListener listener;

	if (listener.listen(port) != sf::Socket::Done)
	{
		std::cout << "[ERROR] Cannot bind port " << port << std::endl;
	}

	if (listener.accept(socket) != sf::Socket::Done) 
	{
	}
}

Data TcpFile::wait_data(sf::TcpSocket& socket)
{
	Data result;

	std::cout << "Waiting on port " << socket.getLocalPort() << std::endl;

	const int MAX_DATA = 1000000;
	result.raw_data.resize(MAX_DATA);
	size_t received;

	if (socket.receive(result.raw_data.data(), MAX_DATA, received) != sf::Socket::Done) { std::cout << "ERROR" << std::endl; }
	result.raw_data.resize(received);
	result.address = socket.getRemoteAddress();
	result.port = socket.getRemotePort();

	std::cout << "Received " << received << " bytes" << std::endl;

	return result;
}

TcpFileMessage::TcpFileMessage(std::vector<char>& message)
{
	code = message[0];
	const int filename_size(message[1]);
	std::cout << "Filename size " << filename_size << std::endl;
	filename = std::string(&message[2], filename_size);

	const int start_file_size_index = 2 + filename_size;
	memcpy(&file_size, reinterpret_cast<int*>(&message[start_file_size_index]), sizeof(int));

	data_start_index = 2 + filename_size + 4;
}
