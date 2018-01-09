#include "FileTree.h"
#include <iostream>

FileTree::FileTree(const std::wstring& name, NodeType type) :
	m_type(type),
	m_name(name)
{

}

FileTree* FileTree::addSubNode(const std::wstring & name, NodeType type)
{
	FileTree* newFileTree = new FileTree(name, type);
	m_subNodes.push_back(newFileTree);

	return newFileTree;
}

void FileTree::print(const std::wstring& indent) const
{
	std::wcout << indent << m_name << std::endl;
	for (const FileTree* subNode : m_subNodes)
	{
		subNode->print(indent + std::wstring(L"   "));
	}
}
