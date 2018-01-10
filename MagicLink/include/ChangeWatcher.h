#pragma once

#include <vector>
#include "ChangeNotifier.h"

class ChangeWatcher
{
public:
	ChangeWatcher();

	void addNotifier(ChangeNotifier& notifier);

	ChangeNotifier* startWatching();

private:
	std::vector<ChangeNotifier*> m_notifiers;
	std::vector<HANDLE> m_handles;
};