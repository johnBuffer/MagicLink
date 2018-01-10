#pragma once

#include <map>
#include <string>
#include <windows.h>

enum NodeType 
{
	NodeTypeFILE,
	NodeTypeDIR
};

class FileTree
{
public:
	FileTree(const std::wstring& name, NodeType type);

	FileTree* addSubNode(const std::wstring& name, NodeType type);

	void setWriteTime(FILETIME time);

	void print(const std::wstring& indent) const;
	void showDiff(const FileTree& tree) const;

private:
	NodeType m_type;
	std::wstring m_name;
	FILETIME m_lastWriteTime;
	std::map<std::wstring, FileTree*> m_subNodes;

	void compare(const FileTree* tree1, const FileTree* tree2) const;
	bool hasSubNode(const std::wstring& name) const;
};