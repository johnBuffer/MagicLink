#include "MagicSynchronizer.h"
#include "FileExplorer.h"
#include "utils.h"
#include <iostream>

MagicSynchronizer::MagicSynchronizer(const std::wstring & dirA, const std::wstring & dirB):
	m_notifierA(dirA),
	m_notifierB(dirB)
{
	m_watcher.addNotifier(m_notifierA);
	m_watcher.addNotifier(m_notifierB);
}

void MagicSynchronizer::startSync()
{
	while (TRUE)
	{
		// Wait for notification.
		ChangeNotifier* cn = m_watcher.startWatching();

		if (cn == &m_notifierA)
		{
			synchronize(&m_notifierB);
		}
		else if (cn == &m_notifierB)
		{
			synchronize(&m_notifierA);
		}
	}
}

void MagicSynchronizer::synchronize(ChangeNotifier* cn)
{
	ChangeNotifier* ref = cn == &m_notifierA ? &m_notifierB : &m_notifierA;

	FileExplorer feRef(ref->getDirPath());
	FileExplorer feSyn(cn->getDirPath());
	
	FileTreeDiff diff = feRef.getFileTree().getDiff(feSyn.getFileTree());

	std::cout << "------------------------------" << std::endl;
	std::wcout << L"\nFiles to add to " << cn->getDirPath() << std::endl;
	for (std::wstring& str : diff.filesToCreate)
	{
		std::wcout << L"    " << str << std::endl;
		//createFile(str);
	}

	std::cout << "\nDirs to add:" << std::endl;
	for (std::wstring& str : diff.dirsToCreate)
		std::wcout << L"    " << str << std::endl;

	std::wcout << L"\nFiles to delete in " << cn->getDirPath() << std::endl;
	for (std::wstring& str : diff.filesToDelete)
	{
		std::wcout << L"    " << str << std::endl;
		bool success = DeleteFile(str.c_str());
		if (!success)
			std::cout << "[ERROR] Cannot delete file" << std::endl;
	}
}


