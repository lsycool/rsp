#pragma once


// CRGBXY 对话框

class CRGBXY : public CDialogEx
{
	DECLARE_DYNAMIC(CRGBXY)

public:
	CRGBXY(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRGBXY();

// 对话框数据
	enum { IDD = IDD_DIALOG_RGBXY };

	BYTE R;
	BYTE G;
	BYTE B;

	int x;
	int y;

	int x_geo;
	int y_geo;

	CStatic* cs;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void updateInfo(BYTE R, BYTE G, BYTE B, int x, int y, int xgeo, int ygeo);
};
