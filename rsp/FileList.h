#pragma once
using namespace std;

// CFileList 对话框

class CFileList : public CDialogEx
{
	DECLARE_DYNAMIC(CFileList)

public:
	CFileList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileList();

	CString filename;//记录文件名
	CString projection;//投影
	int bandcount;//波段数
	float pixel;//分辨率
	BOOL isAdded;//是否已经添加
	vector<CString> pathname;//存储文件名
	CMenu* pMenu;
// 对话框数据
	enum { IDD = IDD_DIALOG_FILELIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// //显示信息
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
