
// rsp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "gdiplusinit.h"

#include "SkinMagicLib.h"
#pragma comment(lib, "SkinMagicTrial.lib")
// CrspApp:
// �йش����ʵ�֣������ rsp.cpp
//

class CrspApp : public CWinApp
{
public:
	CrspApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	GdiplusStartupInput m_gdiplusstartupinput;
	ULONG_PTR m_gdiplusToken;
	int ExitInstance(void);
	afx_msg void OnUpdateFileMruFile1(CCmdUI *pCmdUI);
};

extern CrspApp theApp;