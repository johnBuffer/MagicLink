#pragma once

#include <map>
#include <string>
#include <windows.h>
#include <vector>

struct FileTreeDiff
{
	std::vector<std::wstring> filesToCreate, filesToDelete;
	std::vector<std::wstring> dirsToCreate, dirsToDelete;
};

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

	void setName(const std::wstring& name);
	void setPath(const std::wstring& path);
	void setWriteTime(FILETIME time);

	void print(const std::wstring& indent) const;
	void showDiff(const FileTree& tree) const;

	FileTreeDiff getDiff(const FileTree& tree) const;

private:
	NodeType m_type;

	std::wstring m_path;
	std::wstring m_name;
	
	FILETIME m_lastWriteTime;
	std::map<std::wstring, FileTree*> m_subNodes;

	void compare(const FileTree* tree1, const FileTree* tree2, FileTreeDiff* result) const;
	bool hasSubNode(const std::wstring& name) const;
};