#include "MagicSynchronizer.h"

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
			//RefreshDirectory(cn2.getDirPath().c_str(), cn1.getDirPath().c_str());
		}
		else if (cn == &m_notifierB)
		{
			//RefreshDirectory(cn1.getDirPath().c_str(), cn2.getDirPath().c_str());
		}
	}
}

void MagicSynchronizer::synchronize()
{
}


