#include "FileExplorer.h"

#include <strsafe.h>

FileExplorer::FileExplorer(TCHAR* path) :
	m_path(path),
	m_fileTree(L"root", NodeType::NodeTypeDIR)
{
	m_fillFileTree();
}

const FileTree& FileExplorer::getFileTree() const
{
	return m_fileTree;
}

void FileExplorer::m_fillFileTree()
{
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	/*StringCchLength(argv[1], MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		_tprintf(TEXT("\nDirectory path is too long.\n"));
		return (-1);
	}

	_tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);*/

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	m_exploreDir(m_path, &m_fileTree);

	//FindClose(hFind);
}

void FileExplorer::m_exploreDir(TCHAR* dirPath, FileTree* tree)
{
	LARGE_INTEGER filesize;
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	TCHAR szDir[MAX_PATH];

	StringCchCopy(szDir, MAX_PATH, dirPath);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(szDir, &ffd);

	//_tprintf(TEXT("Exploring: %s\n"), szDir);

	if (INVALID_HANDLE_VALUE != hFind)
	{
		// List all the files in the directory with some info about them.
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (wcscmp(ffd.cFileName, L".") != 0 && wcscmp(ffd.cFileName, L".."))
				{
					//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
					TCHAR subDir[MAX_PATH];
					StringCchCopy(subDir, MAX_PATH, dirPath);
					StringCchCat(subDir, MAX_PATH, TEXT("\\"));
					StringCchCat(subDir, MAX_PATH, ffd.cFileName);

					FileTree* subTree = tree->addSubNode(std::wstring(ffd.cFileName), NodeType::NodeTypeDIR);
					m_exploreDir(subDir, subTree);
				}
			}
			else
			{
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;
				//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
				tree->addSubNode(std::wstring(ffd.cFileName), NodeType::NodeTypeFILE);
			}
		} while (FindNextFile(hFind, &ffd) != 0);
	}
}
