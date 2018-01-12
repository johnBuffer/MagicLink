#include "MagicSynchronizer.h"
#include "FileExplorer.h"
#include "utils.h"
#include "sha2.h"

#include <thread>
#include <iostream>

MagicSynchronizer::MagicSynchronizer(const std::wstring& localDir):
	m_notifier(localDir)
{
	m_watcher.addNotifier(m_notifier);
	picosha2::hash256_hex_string(localDir, m_id);
}

void MagicSynchronizer::addLinkedDirectory(const std::string& id, const std::string& ip, int port)
{
	m_linked[id] = DistantDirectory(ip, port, id);
}

void MagicSynchronizer::startSync(MagicServer* server)
{
	m_server = server;
	std::thread t(&MagicSynchronizer::synchronize, this);
	t.detach();
}

void MagicSynchronizer::notifyChange(const std::string& id, const std::string& tree)
{
	std::wcout << L"\n***** Change received for '" << m_notifier.getDirPath() << L"' *****" << std::endl;
	json jTree = json::parse(tree);

	FileTree thisFT = FileExplorer(m_notifier.getDirPath()).getFileTree();
	FileTree refFT(jTree);

	//thisFT.showDiff(refFT);
	FileTreeDiff diff = thisFT.getDiff(refFT);
	std::wstring dirPath = m_notifier.getDirPath();
	
	for (std::wstring& str : diff.filesToCreate)
	{
		std::wcout << L"    " << str << std::endl;

		m_server->sendFileRequest(m_linked[id], m_id, toStr(str));
	}

	std::cout << "\nDirs to add:" << std::endl;
	for (std::wstring& str : diff.dirsToCreate)
		std::wcout << L"    " << str << std::endl;

	for (std::wstring& str : diff.filesToDelete)
	{
		std::wstring fullPath = dirPath + L"\\" + str;
		bool success = DeleteFile(fullPath.c_str());
		if (!success)
			std::cout << "[ERROR] Cannot delete file" << toStr(fullPath) << std::endl;
	}
}

void MagicSynchronizer::sendRequestedFile(const std::string& id, const std::string& filename)
{
	std::wstring dirPath = m_notifier.getDirPath();
	std::wstring fullPath = dirPath + toWstr(filename);

	std::wcout << L"File to be sent: " << fullPath << std::endl;

	m_server->sendFile(m_linked[id], fullPath);
}

void MagicSynchronizer::setID(const std::string& id)
{
	m_id = id;
}

const std::string& MagicSynchronizer::getID() const
{
	return m_id;
}

std::string MagicSynchronizer::getLocalDir() const
{
	return toStr(m_notifier.getDirPath());
}

void MagicSynchronizer::fileRequest(const std::string& filename)
{
	std::cout << "File requested : " << filename << std::endl;
}

void MagicSynchronizer::createReceivedFile(const std::string& filename, char* data, size_t length)
{
	std::wstring dirPath = m_notifier.getDirPath();
	std::wstring fullPath = dirPath + L"\\" + toWstr(filename);

	std::wcout << L"File to be created: " << fullPath << std::endl;

	createFile(fullPath, data, length);
}

void MagicSynchronizer::synchronize()
{
	if (!m_server)
		return;

	std::wcout << L"Started monitoring for " << m_notifier.getDirPath()  << std::endl;

	while (true)
	{
		ChangeNotifier* cn = m_watcher.startWatching();

		//std::wcout << L"Change in " << m_notifier.getDirPath() << std::endl;
		
		for (auto it : m_linked)
		{
			std::string tree = FileExplorer(m_notifier.getDirPath()).getFileTree().toJson().dump();
			DistantDirectory& dd = it.second;
			m_server->sendFileTree(dd, m_id, tree);
		}
	}
}


