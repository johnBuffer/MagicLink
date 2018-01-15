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
	while (response_code != tfrc_done)
	{
		std::vector<char> response = wait_data();

		response_code = response[0];

		if (response_code == TCP_FILE_REQUEST_CODE::tfrc_send_from_bytes)
		{
			const int start_byte = (response[1] << 24) + (response[2] << 16) + (response[3] << 8) + response[4];
			data.clear();
			data.insert(data.end(), header.begin(), header.end());
			data.insert(data.end(), file.begin()+start_byte, file.end());
		}

		break;
	}
}

std::vector<char> TcpFile::wait_file(int port)
{
	std::vector<char> data;
	
	std::vector<char> received_data = wait_data(port);

	TcpFileMessage message(received_data);

	std::cout << "Received '" << message.filename << "' of size " << message.file_size << " bytes" << std::endl;

	return data;
}

std::vector<char> TcpFile::wait_data(int port)
{
	sf::TcpListener listener;

	if (listener.listen(port) != sf::Socket::Done)
	{
		std::cout << "[ERROR] Cannot bind port " << port << std::endl;
	}

	sf::TcpSocket client;
	if (listener.accept(client) != sf::Socket::Done) { /* ERROR */ }

	std::cout << "[OK] New client " << client.getRemoteAddress() << ":" << client.getRemotePort() << std::endl;

	const int MAX_DATA = 1000000;
	std::vector<char> data(MAX_DATA);
	size_t received;

	if (client.receive(data.data(), MAX_DATA, received) != sf::Socket::Done) { /* ERROR */ }

	client.disconnect();

	listener.close();

	return data;
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
