#pragma once

#include <string>

class DistantDirectory
{
public:
	DistantDirectory();
	DistantDirectory(const std::string& ip, int port, const std::string& id);

	const std::string& getIp() const;
	const std::string& getID() const;
	int getPort() const;

private:
	std::string m_ip;
	std::string m_id;

	int m_port;
};