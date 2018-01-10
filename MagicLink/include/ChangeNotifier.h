#pragma once

#include <string>
#include <windows.h>


class ChangeNotifier
{
public:
	ChangeNotifier(const std::wstring& dirPath);

	DWORD getFilter() const;
	const std::wstring& getDirPath() const;

	HANDLE getHandle();

	void reset();

private:
	std::wstring m_dirPath;
	DWORD m_filter;
	HANDLE m_handle;

};