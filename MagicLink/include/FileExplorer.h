#pragma once
#include <windows.h>
#include <tchar.h>

#include "FileTree.h"

class FileExplorer
{
public:
	FileExplorer(TCHAR* path);

	const FileTree& getFileTree() const;

private:
	TCHAR* m_path;
	FileTree m_fileTree;

	void m_fillFileTree();
	void m_exploreDir(TCHAR* dirPath, FileTree* tree);
};