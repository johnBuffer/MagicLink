#pragma once

#include <vector>
#include <string>

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

	void print(const std::wstring& indent) const;

private:
	NodeType m_type;
	std::wstring m_name;
	std::vector<FileTree*> m_subNodes;
};