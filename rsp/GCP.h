#pragma once
//#include <string>
//using namespace std;
// GCP �Ի���
//class CProcessBase  
//{  
//	public:
//		CProcessBase()   
//		{  
//			m_dPosition = 0.0;  
//			m_iStepCount = 100;  
//			m_iCurStep = 0;  
//			m_bIsContinue = true;  
//		}  
//		virtual ~CProcessBase() {}  
//		virtual void SetMessage(const char* pszMsg) = 0;
//		virtual bool SetPosition(double dPosition) = 0;  
//		virtual bool StepIt() = 0;
//		virtual void SetStepCount(int iStepCount)  
//		{  
//			ReSetProcess();   
//			m_iStepCount = iStepCount;  
//		}  
//		string GetMessage()  
//		{  
//			return m_strMessage;  
//		}  
//		double GetPosition()  
//		{
//			return m_dPosition;  
//		}
//		void ReSetProcess()  
//			{  
//				m_dPosition = 0.0;  
//				m_iStepCount = 100;  
//				m_iCurStep = 0;  
//				m_bIsContinue = true;  
//			}  
//		/*! ������Ϣ */  
//		string m_strMessage;  
//		 /*! ����ֵ */  
//		double m_dPosition;       
//		/*! ���ȸ��� */  
//		int m_iStepCount;         
//		/*! ���ȵ�ǰ���� */  
//		int m_iCurStep;           
//		 /*! �Ƿ�ȡ����ֵΪfalseʱ��ʾ����ȡ�� */  
//		bool m_bIsContinue;       
//};
//class CProcessDlg :   
//	public CDialog,public CProcessBase  
//	{  
//
//		public:  
//		    /** 
//			   14.    * @brief ���캯�� 
//			   15.    */  
//		 CProcessDlg(CWnd *parent = 0);     
//		  
//			    /** 
//				   19.    * @brief �������� 
//				   20.    */  
//		 ~CProcessDlg();  
//		 
//			   /** 
//				   24.    * @brief ���ý�����Ϣ 
//				   25.    * @param pszMsg         ������Ϣ 
//				   26.    */  
//		 void SetMessage(const char* pszMsg);  
//		  
//			    /** 
//				   30.    * @brief ���ý���ֵ 
//				   31.    * @param dPosition      ����ֵ 
//				   32.    */  
//		bool SetPosition(double dPosition);  
//		 
//		    /** 
//				   36.    * @brief ������ǰ��һ�� 
//				   37.    */  
//		bool StepIt();  
//		void updateProgress(int);  
//};  
//

class GCP : public CDialogEx
{
	DECLARE_DYNAMIC(GCP)

public:
	GCP(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GCP();
	GDALDataset* poDataset;//GDALԭʼӰ�����ݼ�
	GDALDataset* poDataset2;//�ο�Ӱ��
	BYTE *m_databuff;//�洢���ص���Ϣ
	//������Ƶ�����
	CList<CPoint,CPoint&>m_BPointList;
	CList<CPoint,CPoint&>m_WPointList;
	//�����ļ��ĸ߶ȺͿ��
	WORD m_dataWidth;
	WORD m_dataHeight;
	//У����Ӱ��ķ�Χ
	float xmin,xmax,ymin,ymax;
	//һ�ζ���ʽ������ϵ��;x'=a0+a1*x+a2*y;y'=b0+b1*x+b2*y; 
	float a0,a1,a2,b0,b1,b2; 
	//Ӱ���ļ���Ϣ
//	DWORD m_imgewith;
//	DWORD m_imgeheight;
	WORD m_bandcount; 
	//�����ļ�����ʼλ��
	WORD m_dataXOff;
	WORD m_dataYOff;
	 //λͼ�ļ�ͷָ��
//	LPBITMAPINFO m_bitinfo;
	float* m_fPara;
	float* m_OutPara;
	//������ָ��
	CProgressCtrl* m_progress;
	//λͼ
	CBitmap MemBitmap;
	//ͼƬ�ռ�ָ��
	CStatic* pic1;
	CStatic* pic2;
	//���Ƶ�
	float m_basex;
	float m_basey;
	float m_warpx;
	float m_warpy;
	int m_count;
// �Ի�������
	enum { IDD = IDD_DIALOG_JZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl* m_GCPList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	bool ReadImage(CString file,GDALDataset* hDataset,CStatic* pic);
	int GetSmpStep(float size);
	BOOL CalForwardParater(void);
	BOOL CalBackwardParater(void);
	void CalScale(void);
	bool NearestSample(float * fPara , int* Range,int Width,int Hight,int Bands, BYTE* InData,BYTE* OutData);
	bool Bilinear(float* fPara, int* Range, int Width,int Hight,int Bands,BYTE* InData,BYTE* OutData);
	void Convolution(BYTE*buff,BYTE*pWDataBuff,int m_imgewith,int m_imgeheight,int bands);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	bool GetGCPFromFile(char* GcpPath, float* fAdjustCoord, float* fReferCoord);
	bool GetParaFromCoord(float* fAdjustCoord, float* fReferCoord, float* fPara);
	bool GetRange(int Width, int Hight, float* fPara, int* Range);
	afx_msg void OnBnClickedOk();
	bool CreatePyramids(CString pszFileName,GDALDataset* hDataset);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonAddgcp();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButtonAuto();
	afx_msg void OnBnClickedCancel();
};
