#include "ChangeWatcher.h"
#include <iostream>

ChangeWatcher::ChangeWatcher()
{
}

void ChangeWatcher::addNotifier(ChangeNotifier& notifier)
{
	m_notifiers.push_back(&notifier);
	
	//HANDLE hdl = FindFirstChangeNotification(notifier.getDirPath().c_str(), TRUE, notifier.getFilter());
	m_handles.push_back(notifier.getHandle());

	/*if (hdl == INVALID_HANDLE_VALUE)
	{
		std::cout << "[ERROR] FACK" << std::endl;
	}*/

	

	std::wcout << "Added notifier for: '" << notifier.getDirPath() << "'" << std::endl;
}

ChangeNotifier* ChangeWatcher::startWatching()
{
	int size = m_handles.size();
	int dwWaitStatus = WaitForMultipleObjects(size, m_handles.data(), FALSE, INFINITE);

	std::cout << "Something moved..." << std::endl;

	if (dwWaitStatus > -1 && dwWaitStatus < size)
	{
		ChangeNotifier* cn = m_notifiers[dwWaitStatus];
		cn->reset();
		return cn;
	}

	return nullptr;
}
