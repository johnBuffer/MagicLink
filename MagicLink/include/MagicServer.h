#pragma once
#include <SFML\Network.hpp>
#include <thread>
#include <unordered_map>
#include "DistantDirectory.h"

class MagicSynchronizer;

class MagicServer
{
public:
	MagicServer();

	void start();
	void stop();

	void sendPing(MagicSynchronizer* sync);
	void requestDirectoryList(const std::string& ip, int port);
	void sendFile(DistantDirectory& dd, const std::wstring& filePath);
	void sendFileTree(DistantDirectory& dd, const std::string& id, const std::string& tree);
	void sendRegistrationRequest();
	void sendDirectoryList();

	void sendFileRequest(DistantDirectory& dd, const std::string& id, const std::string& filename);

	void loadConfigFile(const std::string& filename);
	void registerSynchronizer(MagicSynchronizer* sync);


private:
	int m_port;
	sf::TcpListener m_listener;

	bool m_running;

	std::unordered_map<std::string, MagicSynchronizer*> m_synchronizers;

	void listenIncoming();
	void waitForOutput();

	void parseData(char* data, size_t length);
};