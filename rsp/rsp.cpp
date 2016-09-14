
// rsp.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "rsp.h"
#include "rspDlg.h"
#include "Begin.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CrspApp

BEGIN_MESSAGE_MAP(CrspApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, &CrspApp::OnUpdateFileMruFile1)
END_MESSAGE_MAP()


// CrspApp ����

CrspApp::CrspApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CrspApp ����

CrspApp theApp;


// CrspApp ��ʼ��

BOOL CrspApp::InitInstance()
{
	GdiplusStartup(&m_gdiplusToken,&m_gdiplusstartupinput,NULL);
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(3); 

	////����Ƥ��
	VERIFY(InitSkinMagicLib(AfxGetInstanceHandle(),"MonitorDir",NULL,NULL));
	LoadSkinFromResource(AfxGetInstanceHandle(), (LPCTSTR)IDR_SKINMAGIC1, "SKINMAGIC");

	Begin* beginAPP=new Begin;
	beginAPP->Create(IDD_DIALOG_BEGIN);

	CrspDlg dlg;
	m_pMainWnd = &dlg;

	INT_PTR nResponse=dlg.DoModal();
	
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	

	
	
	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;

}



int CrspApp::ExitInstance(void)
{
	GdiplusShutdown(m_gdiplusToken);
	ExitSkinMagicLib(); 
	return CWinApp::ExitInstance();
}


void CrspApp::OnUpdateFileMruFile1(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (pCmdUI->m_pSubMenu!=NULL)
	{
		return;
	}
}
