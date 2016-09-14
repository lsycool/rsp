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
	int m_nClassNumber;//类编号
	int m_nClassPixelNum;//每一个类的像元个数
	float m_fDistance;//距离类中心的距离
	float n_fMaxSD;//最大标准差
	float m_pStandardDifference;//类分解分离值和类均值间的最小距离
	int m_nNumOfMaxSD;
	float m_pCenter;//记录均值
	BYTE pX;
	BYTE pY;

	vector<BYTE*> m_FeatureClassData;//存储类中的样本点
};

//聚类中心距离
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
// ISODATA 对话框

class ISODATA : public CDialogEx
{
	DECLARE_DYNAMIC(ISODATA)

public:
	ISODATA(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ISODATA();
	vector<Sample*> m_sample;//存储每个类的象元
	vector<ClassMaxMin> m_ClassMaxMin;//每个类中的最大和最小象元值
	int m_nTotalClassNum; //实际计算的类数
	float m_fMinClassDis;
	int m_iLoopNum;//迭代次数
	float m_fMinPixelNum;//最小像元数
	int m_iClasses;//类数
	int m_iIniClasses;//初始类数
	float m_fMaxSD;//最大标准差
	int m_iMaxMergeClassNum;//最大类合并数
	vector<CenterDistant> m_UinClassDis;//记录类中心得距离

// 对话框数据
	enum { IDD = IDD_DIALOG_ISOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	void Classfying();//分类
	void DelFeatureClass(Sample*  pClass);//删除类
	void UniFeatureClass(Sample* pClass,Sample* pClassB);//合并类
	void DivFeatureClass(Sample* pClass);//分裂类
	void UinClassDisVec(void);//需要进行合并类两类之间的最小距离排序
	bool CalClassMaxMin(void);
	CEdit m_Classes;//期望类数
	CEdit m_IniClasses;//初始类数
	CEdit m_LoopNum;//迭代次数
	CEdit m_MinPixelNum;//最小像素值
	CEdit m_MaxSD;//最大标准差
	CEdit m_MinClassDis;//最小类间距离
	CEdit m_MaxMergeClassNum;//最大类合并数
	CProgressCtrl* m_Progress;

};
