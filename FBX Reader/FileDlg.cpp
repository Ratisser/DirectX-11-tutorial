#include "framework.h"
#include "FileDlg.h"


FileDlg::FileDlg()
{
	mOpenFileName = new OPENFILENAME;
	ZeroMemory(mstrFilePath, sizeof(mstrFilePath));
	ZeroMemory(mstrFileName, sizeof(mstrFileName));
}


FileDlg::~FileDlg()
{
	delete mOpenFileName;
}

int FileDlg::OpenFile(HWND hWnd)
{
	ZeroMemory(mOpenFileName, sizeof(*mOpenFileName));
	mOpenFileName->lStructSize = sizeof(*mOpenFileName);
	mOpenFileName->hwndOwner = hWnd;
	//m_ofn->lpstrTitle = _T("파일 열기");
	mOpenFileName->lpstrTitle = _T("파일 열기"); 
	mOpenFileName->lpstrFilter = _T("모든 파일(*.*)\0*.*\0\0");
	mOpenFileName->lpstrFile = mstrFilePath;
	mOpenFileName->lpstrFileTitle = mstrFileName;
	mOpenFileName->nMaxFile = MAX_PATH;
	mOpenFileName->nMaxFileTitle = MAX_PATH;


	if (GetOpenFileName(mOpenFileName) != NULL)
	{
		MessageBox(hWnd, mOpenFileName->lpstrFileTitle, _T("파일을 열었다."), MB_OK);
		return 1;
	}
	MessageBox(hWnd, mstrFilePath, _T("파일을 열지 않았다."), MB_OK);
	return -1;
}

int FileDlg::SaveFile(HWND hWnd)
{
	ZeroMemory(mOpenFileName, sizeof(*mOpenFileName));
	mOpenFileName->lStructSize = sizeof(*mOpenFileName);
	mOpenFileName->hwndOwner = hWnd;
	mOpenFileName->lpstrTitle = _T("파일 열기");
	mOpenFileName->lpstrFilter = _T("모든 파일(*.*)\0*.*\0\0");
	mOpenFileName->lpstrFile = mstrFilePath;
	mOpenFileName->lpstrFileTitle = mstrFileName;
	mOpenFileName->nMaxFile = MAX_PATH;
	mOpenFileName->nMaxFileTitle = MAX_PATH;
	mOpenFileName->lpstrDefExt = _T("txt");

	if (GetSaveFileName(mOpenFileName) != NULL)
	{
		MessageBox(hWnd, mstrFilePath, _T("파일을 저장했다."), MB_OK);
		return mOpenFileName->nFilterIndex;
	}
	MessageBox(hWnd, mstrFilePath, _T("파일을 저장하지 않았다."), MB_OK);
	return -1;
}

int FileDlg::GetFile(TCHAR* filePath) {
	if (*mstrFilePath != '\0') {
		memcpy(filePath, mstrFilePath, MAX_PATH);
		return 1;
	}
	return -1;
}

TCHAR* FileDlg::GetFileName() {
	if (*mstrFilePath != '\0') {
		return mstrFilePath;
	}
	return NULL;
}

bool FileDlg::GetFileNameMBCS(char* pOut)
{
	if (*mstrFilePath != '\0') {
		wcstombs(pOut, mstrFilePath, MAX_PATH);
		return true;
	}
	return false;
}

