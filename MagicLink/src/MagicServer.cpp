#include "MagicServer.h"
#include <iostream>
#include "utils.h"

MagicServer::MagicServer():
	m_port(8080)
{
}

void MagicServer::start()
{
	if (m_listener.listen(m_port) != sf::Socket::Done)
	{
		std::cout << "[ERROR] Cannot bind port " << m_port << std::endl;
	}

	std::cout << "[OK] Waiting client on port " << m_port << std::endl;

	sf::TcpSocket client;
	if (m_listener.accept(client) != sf::Socket::Done)
	{
		// Error
	}

	std::cout << "[OK] New client " << client.getRemoteAddress() << ":" << client.getRemotePort() << std::endl;
	std::cout << "Waiting for data..." << std::endl;

	const size_t MAX_DATA = 1000000;
	char data[MAX_DATA];
	std::size_t received;

	// socket TCP:
	if (client.receive(data, MAX_DATA, received) != sf::Socket::Done)
	{
		// erreur...
	}

	int size = data[0];
	std::string filename(&data[1], size);

	std::cout << "'" << filename << "' received (" << received-size-1 << " bytes)" << std::endl;
	createFile(std::wstring(filename.begin(), filename.end()), &data[1 + size], received - size - 1);
}

void MagicServer::stop()
{

}

void MagicServer::sendFile(const std::string& ip, int port, const std::wstring& filePath)
{

	std::vector<char> data;
	std::wstring name = filePath.substr(filePath.find_last_of('\\') + 1);
	std::string filename(name.begin(), name.end());

	data.push_back(char(filename.size()));
	// Add filename
	for (char c : filename)
		data.push_back(c);

	std::vector<char> fileData = importFile(filePath);
	// Add file
	for (char c : fileData)
		data.push_back(c);

	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(ip, port);

	std::cout << "Trying to connect to: " << ip << ":" << port << "..." << std::endl;
	if (status != sf::Socket::Done)
	{
		std::cout << "[ERROR] Cannot connect" << std::endl;
	}
	else
	{
		std::cout << "[OK] Connected" << std::endl;
	}

	// socket TCP:
	if (socket.send(data.data(), data.size()) != sf::Socket::Done)
	{
		// erreur...
	}
}
