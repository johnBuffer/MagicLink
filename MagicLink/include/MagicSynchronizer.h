#pragma once

#include <string>

#include "ChangeWatcher.h"

class MagicSynchronizer
{
public:
	MagicSynchronizer(const std::wstring& dirA, const std::wstring& dirB);

	void startSync();

private:
	ChangeNotifier m_notifierA, m_notifierB;

	ChangeWatcher m_watcher;

	void synchronize(ChangeNotifier* cn);
};