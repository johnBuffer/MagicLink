#pragma once
#include <SFML\Network.hpp>

class MagicServer
{
public:
	MagicServer();

	void start();

	void stop();

	void sendFile(const std::string& ip, int port, const std::wstring& filePath);

private:
	int m_port;
	sf::TcpListener m_listener;

	

};