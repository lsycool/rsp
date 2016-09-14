#pragma once
#include "afxwin.h"
#include <vector>
using namespace std;

class Sample
{
public:
	Sample();
	virtual ~Sample(){};
	float *GetClassCenter();
	inline float GetAverageDistance(){return m_fDistance;}
	void CalculateParameters();
	friend class ISODATA;
private:

public:
	int m_nClassNumber;//����
	int m_nClassPixelNum;//ÿһ�������Ԫ����
	float m_fDistance;//���������ĵľ���
	float n_fMaxSD;//����׼��
	float m_pStandardDifference;//��ֽ����ֵ�����ֵ�����С����
	int m_nNumOfMaxSD;
	float m_pCenter;//��¼��ֵ
	BYTE pX;
	BYTE pY;

	vector<BYTE*> m_FeatureClassData;//�洢���е�������
};

//�������ľ���
struct CenterDistant
{
	float fDis;
	int nNumClassA;
	int nNumClassB;
};

struct ClassMaxMin
{
	int ClassID;
	BYTE Max;
	BYTE Min;
};
// ISODATA �Ի���

class ISODATA : public CDialogEx
{
	DECLARE_DYNAMIC(ISODATA)

public:
	ISODATA(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ISODATA();
	vector<Sample*> m_sample;//�洢ÿ�������Ԫ
	vector<ClassMaxMin> m_ClassMaxMin;//ÿ�����е�������С��Ԫֵ
	int m_nTotalClassNum; //ʵ�ʼ��������
	float m_fMinClassDis;
	int m_iLoopNum;//��������
	float m_fMinPixelNum;//��С��Ԫ��
	int m_iClasses;//����
	int m_iIniClasses;//��ʼ����
	float m_fMaxSD;//����׼��
	int m_iMaxMergeClassNum;//�����ϲ���
	vector<CenterDistant> m_UinClassDis;//��¼�����ĵþ���

// �Ի�������
	enum { IDD = IDD_DIALOG_ISOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	void Classfying();//����
	void DelFeatureClass(Sample*  pClass);//ɾ����
	void UniFeatureClass(Sample* pClass,Sample* pClassB);//�ϲ���
	void DivFeatureClass(Sample* pClass);//������
	void UinClassDisVec(void);//��Ҫ���кϲ�������֮�����С��������
	bool CalClassMaxMin(void);
	CEdit m_Classes;//��������
	CEdit m_IniClasses;//��ʼ����
	CEdit m_LoopNum;//��������
	CEdit m_MinPixelNum;//��С����ֵ
	CEdit m_MaxSD;//����׼��
	CEdit m_MinClassDis;//��С������
	CEdit m_MaxMergeClassNum;//�����ϲ���
	CProgressCtrl* m_Progress;

};
