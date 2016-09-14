#pragma once
extern int nBandCount;
extern int nImageSizeX;
extern int nImageSizeY;
extern BYTE ColorsTable[12][256][4];
extern int flag1;
extern BYTE JunHen[256];
extern int nPosCon;
extern int EZ;
#include <vector>;
using namespace std;
// ImageShow 对话框


class ImageShow : public CDialogEx
{
	DECLARE_DYNAMIC(ImageShow)

public:
	ImageShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ImageShow();
	BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_IMAGE_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CBrush m_brush;
	DECLARE_MESSAGE_MAP()
public:
	CBitmap MemBitmap;
	bool ShowImage(int,int,int,CDC*);
	afx_msg void OnPaint();
	afx_msg void OnIdok();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateImage();
	afx_msg void OnElarge();
	afx_msg void OnNarrow();
	afx_msg void OnMobile();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void SuoXiao(/*CPoint point,*/ CDC* pdc1);
	afx_msg void OnRecovery();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRGBtoGray();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();

	BYTE*TempLock1;//数据缓冲区
	BYTE*TempLock2;
	BYTE*TempLock3;

	double adfGeoTransform[6];//影像投影变换参数

	COLORREF rgbColor;
	vector<CPoint>Roi;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
void setBitMapInfo(BITMAPINFO* pMapinfo,int nXsize,int nYsize);