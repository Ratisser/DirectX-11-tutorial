#ifndef _FILE_DLG_H_
#define _FILE_DLG_H_

#include <commdlg.h>

class FileDlg
{
public:
	FileDlg();
	~FileDlg();

	int OpenFile(HWND hWnd);
	int SaveFile(HWND hWnd);
	int GetFile(TCHAR* filaName);
	TCHAR* GetFileName();
	bool GetFileNameMBCS(char* pOut);

	inline static FileDlg *GetInstance() { static FileDlg singleton; return &singleton; }

private:
	OPENFILENAME* mOpenFileName;
	TCHAR mstrFileName[MAX_PATH];
	TCHAR mstrFilePath[MAX_PATH];
};

#endif