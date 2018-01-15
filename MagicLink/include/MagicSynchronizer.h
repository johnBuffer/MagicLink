#pragma once

#include <string>
#include <unordered_map>

#include "DistantDirectory.h"
#include "ChangeWatcher.h"
#include "MagicServer.h"

class MagicSynchronizer
{
public:
	MagicSynchronizer(const std::wstring& localDir);

	void addLinkedDirectory(const std::string& id, const std::string& ip, int port);
	void startSync(MagicServer* server);

	void notifyChange(const std::string& id, const std::string& tree);
	void sendRequestedFile(const std::string& id, const std::string& filename);
	void setID(const std::string& id);

	const std::string& getID() const;
	std::string getLocalDir() const;

	void fileRequest(const std::string& filename);

	void createReceivedFile(const std::string& filename, char* data, size_t length);

private:
	std::string m_id;

	bool m_busy;

	ChangeNotifier m_notifier;
	ChangeWatcher m_watcher;

	std::unordered_map<std::string, DistantDirectory> m_linked;

	int m_remaining_actions;

	MagicServer* m_server;

	void synchronize();
};