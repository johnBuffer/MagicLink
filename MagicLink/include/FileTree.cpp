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
	m_subNodes[name] = (newFileTree);

	return newFileTree;
}

void FileTree::setWriteTime(FILETIME time)
{
	m_lastWriteTime = time;
}

void FileTree::print(const std::wstring& indent) const
{
	std::wcout << indent << m_name << std::endl;
	for (const auto elem : m_subNodes)
	{
		elem.second->print(indent + std::wstring(L"   "));
	}
}

void FileTree::showDiff(const FileTree& tree) const
{
	compare(this, &tree);
}

void FileTree::compare(const FileTree* tree1, const FileTree* tree2) const
{
	for (const auto elem : tree1->m_subNodes)
	{
		const std::wstring& name = elem.first;
		const FileTree* subNode = elem.second;

		if (tree2->hasSubNode(name))
		{
			const FileTree* subNode2 = tree2->m_subNodes.at(name);
			if (subNode->m_lastWriteTime.dwHighDateTime != subNode2->m_lastWriteTime.dwHighDateTime || subNode->m_lastWriteTime.dwLowDateTime != subNode2->m_lastWriteTime.dwLowDateTime)
			{
				std::wcout << L"[*] Modified '" << name << L"'" << std::endl;
			}

			compare(subNode, subNode2);
		}
		else
		{
			std::wcout << L"[-] Deleted node '" << name << L"'" << std::endl;
		}
	}

	for (const auto elem : tree2->m_subNodes)
	{
		const std::wstring& name = elem.first;
		const FileTree* subNode = elem.second;

		if (!tree1->hasSubNode(name))
		{
			std::wcout << L"[+] New node '" << name << L"'" << std::endl;
		}
	}
}

bool FileTree::hasSubNode(const std::wstring & name) const
{
	return (m_subNodes.find(name) != m_subNodes.end());
}

