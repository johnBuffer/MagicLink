#include "MagicServer.h"
#include "MagicSynchronizer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "utils.h"

MagicServer::MagicServer():
	m_port(8080),
	m_running(false)
{
}

void MagicServer::start()
{
	m_running = true;

	std::vector<std::thread> threads;
	for (auto it : m_synchronizers)
	{
		threads.emplace_back(&MagicSynchronizer::startSync, it.second, this);
	}

	std::thread t(&MagicServer::listenIncoming, this);
	t.join();
}

void MagicServer::stop()
{
	m_running = false;
}

void MagicServer::sendPing(MagicSynchronizer* sync)
{
}

void MagicServer::requestDirectoryList(const std::string& ip, int port)
{
}

void MagicServer::sendFile(DistantDirectory& dd, const std::wstring& filePath)
{
	std::vector<char> data;
	std::wstring name = filePath.substr(filePath.find_last_of('\\') + 1);
	std::string filename(name.begin(), name.end());

	data.push_back(char(5));
	
	// Add dest ID
	for (char c : dd.getID())
		data.push_back(c);

	data.push_back(char(filename.size()));
	
	// Add filename
	for (char c : filename)
		data.push_back(c);

	std::vector<char> fileData = importFile(filePath);
	// Add file
	for (char c : fileData)
		data.push_back(c);

	std::cout << "Sending file of size " << fileData.size() << std::endl;

	sendData(dd.getIp(), dd.getPort(), data.data(), data.size());
}

void MagicServer::sendFileTree(DistantDirectory& dd, const std::string& id, const std::string& tree)
{
	std::string data = char(2) + dd.getID() + id + tree;
	sendData(dd.getIp(), dd.getPort(), data.c_str(), data.size());
}

void MagicServer::sendFileRequest(DistantDirectory& dd, const std::string& id, const std::string& filename)
{
	std::string data = char(3) + dd.getID() + id + filename;
	sendData(dd.getIp(), dd.getPort(), data.c_str(), data.size());
}

void MagicServer::loadConfigFile(const std::string& filename)
{
	std::ifstream infile(filename);
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string dirPath, ip, id;
		int port;
		if (!(iss >> dirPath >> ip >> port >> id)) { break; }

		MagicSynchronizer* newMS = new MagicSynchronizer(toWstr(dirPath));
		newMS->addLinkedDirectory(id, ip, port);
		const std::string& hash = newMS->getID();
		m_synchronizers[hash] = newMS;

		std::cout << "[ADD] New synchronizer (" << hash << ") -> '" << newMS->getLocalDir() << "'" << std::endl;
	}
}

void MagicServer::registerSynchronizer(MagicSynchronizer* sync)
{
	const std::string& hash = sync->getID();
	m_synchronizers[hash] = sync;
	std::cout << "[ADD] New synchronizer (" << hash << ")" << std::endl;
}

void MagicServer::listExposedDirectories() const
{
	std::cout << "\nExposed directories" << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	for (const auto it : m_synchronizers)
	{
		std::cout << it.second->getLocalDir() << " -> " << it.second->getID() << std::endl;
	}
}

void MagicServer::setPort(int port)
{
	m_port = port;
}

void MagicServer::listenIncoming()
{
	if (m_listener.listen(m_port) != sf::Socket::Done)
	{
		std::cout << "[ERROR] Cannot bind port " << m_port << std::endl;
	}

	while (m_running)
	{
		sf::TcpSocket client;
		if (m_listener.accept(client) != sf::Socket::Done) { /* ERROR */ }

		const size_t MAX_DATA = 1000000;
		char data[MAX_DATA];
		std::size_t received;

		if (client.receive(data, MAX_DATA, received) != sf::Socket::Done) { /* ERROR */ }

		parseData(data, received);

		//client.disconnect();
	}
}

void MagicServer::parseData(char* data, size_t length)
{
	switch (data[0])
	{
	case 0:
	{
		// Ping
		break;
	}
	case 1:
	{
		// Registration
		break;
	}
	case 2:
	{
		// Change notification
		std::string id(&data[1], &data[65]);
		std::string distantID(&data[65], &data[65+64]);
		//std::cout << "[CHANGE] for directory '" << id << "'" << std::endl;
		std::string tree(&data[65 + 64], &data[length]);
		m_synchronizers[id]->notifyChange(distantID, tree);
		break;
	}
	case 3:
	{
		// File request
		std::string id(&data[1], &data[65]);
		std::cout << "[FREQUEST] for directory '" << id << "'" << std::endl;
		std::string distantID(&data[65], &data[65 + 64]);
		std::string filename(&data[65 + 64], &data[length]);
		m_synchronizers[id]->sendRequestedFile(distantID, filename);
		break;
	}
	case 4:
	{
		// Listing
		break;
	}
	case 5:
	{
		// File received
		std::string id(&data[1], 64);
		int size = data[65];
		std::string filename(&data[66], size);

		std::cout << "'" << filename << "' received (" << length - size - 66 << " bytes) total " << length << std::endl;
		m_synchronizers[id]->createReceivedFile(filename, &data[66+size], length - size - 66);
		break;
	}
	default:
	{
		// Error
		break;
	}
	}
}
