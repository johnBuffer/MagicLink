#include "FileExplorer.h"

#include <strsafe.h>

FileExplorer::FileExplorer(const std::wstring& path) :
	m_path(path),
	m_fileTree(L"", NodeType::NodeTypeDIR)
{
	size_t length = path.length();
	size_t lastPose = path.find_last_of('\\');

	m_fileTree.setName(path.substr(lastPose+1));
	m_fileTree.setPath(path.substr(0, lastPose));

	m_fillFileTree();
}

const FileTree& FileExplorer::getFileTree() const
{
	return m_fileTree;
}

void FileExplorer::m_fillFileTree()
{
	m_exploreDir(m_path, &m_fileTree);
}

void FileExplorer::m_exploreDir(const std::wstring& dirPath, FileTree* tree)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	std::wstring searchDirPath = dirPath + L"\\*";

	hFind = FindFirstFile(searchDirPath.c_str(), &ffd);

	if (INVALID_HANDLE_VALUE != hFind)
	{
		// List all the files in the directory with some info about them.
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::wstring filename(ffd.cFileName);
				if (filename.compare(L".") && filename.compare(L".."))
				{
					std::wstring subDirPath = dirPath + L"\\" + filename;

					FileTree* subTree = tree->addSubNode(std::wstring(ffd.cFileName), NodeType::NodeTypeDIR);
					m_exploreDir(subDirPath, subTree);
				}
			}
			else
			{
				FILETIME lwt = ffd.ftLastWriteTime;
				FileTree* subNode = tree->addSubNode(std::wstring(ffd.cFileName), NodeType::NodeTypeFILE);
				subNode->setWriteTime(ffd.ftLastWriteTime);
			}
		} while (FindNextFile(hFind, &ffd) != 0);
	}

	FindClose(hFind);
}
