
// rsp.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "gdiplusinit.h"

#include "SkinMagicLib.h"
#pragma comment(lib, "SkinMagicTrial.lib")
// CrspApp:
// 有关此类的实现，请参阅 rsp.cpp
//

class CrspApp : public CWinApp
{
public:
	CrspApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	GdiplusStartupInput m_gdiplusstartupinput;
	ULONG_PTR m_gdiplusToken;
	int ExitInstance(void);
	afx_msg void OnUpdateFileMruFile1(CCmdUI *pCmdUI);
};

extern CrspApp theApp;