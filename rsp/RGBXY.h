#pragma once


// CRGBXY �Ի���

class CRGBXY : public CDialogEx
{
	DECLARE_DYNAMIC(CRGBXY)

public:
	CRGBXY(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRGBXY();

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void updateInfo(BYTE R, BYTE G, BYTE B, int x, int y, int xgeo, int ygeo);
};
