#pragma once
#include <vector>
using namespace std;

// MinMax �Ի���
struct MaSample
{
	BYTE data;//ÿ������Ԫ��
	int mark;//�������
};

struct MaClass//��������
{
	vector<MaSample*>sample;//���е�Ԫ��
	BYTE data;//������
};

class MinMax : public CDialogEx
{
	DECLARE_DYNAMIC(MinMax)

public:
	MinMax(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MinMax();

// �Ի�������
	enum { IDD = IDD_DIALOG_MinMAX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CProgressCtrl* m_progress;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	MaClass* mclass;
	MaSample* msample;
//	float m_sida;
	float m_sida;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	int nImageSizeX;
	int nImageSizeY;
};
int LinearRegression(BYTE *data1,BYTE* data2,int rows, double *a, double *b/*, double *SquarePoor*/);