#pragma once
#include <vector>
using namespace std;


// CSimilar �Ի���
struct Sim_center
{
	vector<BYTE>data;//�洢��������
};

struct Sim_sample
{
	BYTE data;
	int mark;
};

class CSimilar : public CDialogEx
{
	DECLARE_DYNAMIC(CSimilar)

public:
	CSimilar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSimilar();

// �Ի�������
	enum { IDD = IDD_DIALOG_Similar };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_T;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CString Similar_IN;//�����ļ�·��
	CString Similar_OUT;//����ļ�·��

	Sim_center center;
	Sim_sample* sample;
	virtual BOOL OnInitDialog();
};
