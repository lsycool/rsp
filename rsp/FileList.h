#pragma once
using namespace std;

// CFileList �Ի���

class CFileList : public CDialogEx
{
	DECLARE_DYNAMIC(CFileList)

public:
	CFileList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileList();

	CString filename;//��¼�ļ���
	CString projection;//ͶӰ
	int bandcount;//������
	float pixel;//�ֱ���
	BOOL isAdded;//�Ƿ��Ѿ����
	vector<CString> pathname;//�洢�ļ���
	CMenu* pMenu;
// �Ի�������
	enum { IDD = IDD_DIALOG_FILELIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// //��ʾ��Ϣ
	void setMessage(CString filepath,CString projection,CString cordin,CString datum,int bandcount,int sizex,int sizey,double gt1=0,double gt2=0,double gt3=0,double gt4=0,double gt5=0,double gt6=0);
	CTreeCtrl m_TreeFile;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAddfile();
	afx_msg void OnBnClickedButtonDelfile();
	CMFCMenuButton m_menubutton;
	afx_msg void OnBnClickedMfcmenubuttonWdowdisp();
	afx_msg void OnBnClickedButtonMg();
	afx_msg void OnBnClickedCancel();
};
