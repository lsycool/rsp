#pragma once
//#include <string>
//using namespace std;
// GCP 对话框
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
//		/*! 进度信息 */  
//		string m_strMessage;  
//		 /*! 进度值 */  
//		double m_dPosition;       
//		/*! 进度个数 */  
//		int m_iStepCount;         
//		/*! 进度当前个数 */  
//		int m_iCurStep;           
//		 /*! 是否取消，值为false时表示计算取消 */  
//		bool m_bIsContinue;       
//};
//class CProcessDlg :   
//	public CDialog,public CProcessBase  
//	{  
//
//		public:  
//		    /** 
//			   14.    * @brief 构造函数 
//			   15.    */  
//		 CProcessDlg(CWnd *parent = 0);     
//		  
//			    /** 
//				   19.    * @brief 析构函数 
//				   20.    */  
//		 ~CProcessDlg();  
//		 
//			   /** 
//				   24.    * @brief 设置进度信息 
//				   25.    * @param pszMsg         进度信息 
//				   26.    */  
//		 void SetMessage(const char* pszMsg);  
//		  
//			    /** 
//				   30.    * @brief 设置进度值 
//				   31.    * @param dPosition      进度值 
//				   32.    */  
//		bool SetPosition(double dPosition);  
//		 
//		    /** 
//				   36.    * @brief 进度条前进一步 
//				   37.    */  
//		bool StepIt();  
//		void updateProgress(int);  
//};  
//

class GCP : public CDialogEx
{
	DECLARE_DYNAMIC(GCP)

public:
	GCP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GCP();
	GDALDataset* poDataset;//GDAL原始影像数据集
	GDALDataset* poDataset2;//参考影像
	BYTE *m_databuff;//存储像素点信息
	//保存控制点坐标
	CList<CPoint,CPoint&>m_BPointList;
	CList<CPoint,CPoint&>m_WPointList;
	//数据文件的高度和宽度
	WORD m_dataWidth;
	WORD m_dataHeight;
	//校正后影像的范围
	float xmin,xmax,ymin,ymax;
	//一次多项式的六个系数;x'=a0+a1*x+a2*y;y'=b0+b1*x+b2*y; 
	float a0,a1,a2,b0,b1,b2; 
	//影像文件信息
//	DWORD m_imgewith;
//	DWORD m_imgeheight;
	WORD m_bandcount; 
	//数据文件的起始位置
	WORD m_dataXOff;
	WORD m_dataYOff;
	 //位图文件头指针
//	LPBITMAPINFO m_bitinfo;
	float* m_fPara;
	float* m_OutPara;
	//进度条指针
	CProgressCtrl* m_progress;
	//位图
	CBitmap MemBitmap;
	//图片空间指针
	CStatic* pic1;
	CStatic* pic2;
	//控制点
	float m_basex;
	float m_basey;
	float m_warpx;
	float m_warpy;
	int m_count;
// 对话框数据
	enum { IDD = IDD_DIALOG_JZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
