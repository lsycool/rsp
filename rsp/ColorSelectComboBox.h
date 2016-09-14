#if !defined(AFX_COLORSELECTCOMBOBOX_H__39CCB3EF_DBFA_11D3_A999_F16F4595342E__INCLUDED_)
#define AFX_COLORSELECTCOMBOBOX_H__39CCB3EF_DBFA_11D3_A999_F16F4595342E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorSelectComboBox.h : header file
//
#define COLOR_RECT_WIDTH  30
#define COLOR_RECT_BORDER 2
/////////////////////////////////////////////////////////////////////////////
// CColorSelectComboBox window

class CColorSelectComboBox : public CComboBox
{
// Construction
public:
	CColorSelectComboBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorSelectComboBox)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF GetSelColor();
	void InitBox();
	BOOL CanAddThisColor(COLORREF rgbColor);
	int AddString( LPCTSTR lpszItem);
	int AddColor(COLORREF rgbColor,LPCTSTR sComment);
	virtual ~CColorSelectComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorSelectComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSELECTCOMBOBOX_H__39CCB3EF_DBFA_11D3_A999_F16F4595342E__INCLUDED_)
