#include "ChangeNotifier.h"
#include <iostream>

ChangeNotifier::ChangeNotifier(const std::wstring & dirPath):
	m_dirPath(dirPath)
{
	m_filter = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_DIR_NAME;
	m_handle = FindFirstChangeNotification(m_dirPath.c_str(), TRUE, m_filter);

	if (m_handle == INVALID_HANDLE_VALUE)
	{
		std::cout << "[ERROR] FindFirstChangeNotification function failed" << std::endl;
		ExitProcess(GetLastError());
	}
}

DWORD ChangeNotifier::getFilter() const
{
	return m_filter;
}

const std::wstring& ChangeNotifier::getDirPath() const
{
	return m_dirPath;
}

HANDLE ChangeNotifier::getHandle()
{
	return m_handle;
}

void ChangeNotifier::reset()
{
	if (!FindNextChangeNotification(m_handle))
	{
		std::cout << "[ERROR] FindNextChangeNotification function failed" << std::endl;
		ExitProcess(GetLastError());
	}
}
