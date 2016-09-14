#pragma once

#include <vector>
using namespace std;

// CMorac �Ի���

//������
struct FEATUREPOINT
{
	int x;
	int y;		//����
	float IV;	//��Ȥֵ
};

class CMorac : public CDialogEx
{
	DECLARE_DYNAMIC(CMorac)

public:
	CMorac(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMorac();

// �Ի�������
	enum { IDD = IDD_DIALOG_PHOTOGRAMMETRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_thre;
	double m_win1;
	double m_win2;

	CString M_in;//����Ӱ��
	CString M_out;//���Ӱ��
	CString M_data;//�����Ȥ��
	afx_msg void OnBnClickedButtonMoin();
	afx_msg void OnBnClickedButtonMout();
	afx_msg void OnBnClickedButtonCodat();
	afx_msg void OnBnClickedOk();
	// ����Ī��ά��
	void CaculMoc(BYTE* Indata1, BYTE* outdata,int ImageSizeX,int ImageSizeY,int win1,int win2,double thre,CString file,CString TxtFile);
	// ������Ӱ��
	void SaveImage(BYTE* OutData,int ImageSizeX,int ImageSizeY, CString saveFile, CString ImageType);
	// //��������������
	void SaveToText(CString file,int Count1,vector<FEATUREPOINT> FeaturePoint2);
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();
};
