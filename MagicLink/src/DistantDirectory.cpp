#include "DistantDirectory.h"

DistantDirectory::DistantDirectory():
	m_ip(""),
	m_port(0),
	m_id("")
{
}

DistantDirectory::DistantDirectory(const std::string & ip, int port, const std::string& id):
	m_ip(ip),
	m_port(port),
	m_id(id)
{
}

const std::string& DistantDirectory::getIp() const
{
	return m_ip;
}

const std::string & DistantDirectory::getID() const
{
	return m_id;
}

int DistantDirectory::getPort() const
{
	return m_port;
}
