// ColorSelectComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "ColorSelectComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorSelectComboBox

CColorSelectComboBox::CColorSelectComboBox()
{
}

CColorSelectComboBox::~CColorSelectComboBox()
{
}


BEGIN_MESSAGE_MAP(CColorSelectComboBox, CComboBox)
	//{{AFX_MSG_MAP(CColorSelectComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorSelectComboBox message handlers

void CColorSelectComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
}

void CColorSelectComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	if(lpDrawItemStruct->CtlType!=ODT_COMBOBOX)
		return;
	if(lpDrawItemStruct->itemID <0)
		return;
	COLORREF	itemColor=GetItemData(lpDrawItemStruct->itemID);//项目的颜色
	COLORREF	textColor=::GetSysColor( COLOR_WINDOWTEXT );//文本的颜色
	COLORREF	backColor=::GetSysColor(COLOR_WINDOW);//项目背景色
	CString		itemString;//存储项目字符串
	//得到项目的包围矩形
	CRect	itemRect=lpDrawItemStruct->rcItem;
	CDC	*pDC=CDC::FromHandle (lpDrawItemStruct->hDC );
	if( lpDrawItemStruct->itemState & ODS_FOCUS )
	{
		//如果项目具有输入焦点,则设置背景色为用系统高亮色(通常为蓝色)
		backColor=::GetSysColor(COLOR_HIGHLIGHT);
		textColor= 0x00FFFFFF & ~( textColor );
	}
	if(  lpDrawItemStruct->itemState & ODS_DISABLED )
	{
		//如果控件被禁止,则设置文本色和项目颜色为禁止色
		textColor = ::GetSysColor( COLOR_INACTIVECAPTIONTEXT );
		itemColor=textColor;
	}
	//先绘制背景
	pDC->FillRect (&itemRect,&CBrush(backColor));
	if( lpDrawItemStruct->itemState & ODS_FOCUS )
	{
		//如果项目具有输入焦点,则还要画出一个焦点框
		pDC->DrawFocusRect( &itemRect);
	}
	//设置颜色区域
	CRect	colorRect;
	colorRect.left=itemRect.left+COLOR_RECT_BORDER;
	colorRect.top=itemRect.top+COLOR_RECT_BORDER;
	colorRect.right=colorRect.left+COLOR_RECT_WIDTH;
	colorRect.bottom=itemRect.bottom - COLOR_RECT_BORDER;
	//画出颜色区域
	CBrush	brush(itemColor);
	CBrush	*oldbrush=pDC->SelectObject (&brush);
	pDC->Rectangle (&colorRect);
	pDC->SelectObject (oldbrush);
	//得到并画出文字
	GetLBText(lpDrawItemStruct->itemID, itemString );
	itemRect.OffsetRect (2*COLOR_RECT_BORDER+COLOR_RECT_WIDTH+5,0);
	pDC->SetBkMode(TRANSPARENT);//设置文字输出模式为透明背景
	pDC->SetTextColor (textColor);
	DrawText(lpDrawItemStruct->hDC, itemString, -1, &itemRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

int CColorSelectComboBox::AddColor(COLORREF rgbColor, LPCTSTR sComment)
{
	if(!CanAddThisColor(rgbColor))
		return -1;
	int nItem=AddString (sComment);
	if(nItem >=0)
		SetItemData (nItem,rgbColor);
	return	nItem;
}

int CColorSelectComboBox::AddString(LPCTSTR lpszItem)
{
	return ((CComboBox*)this)->AddString(lpszItem);
}

BOOL CColorSelectComboBox::CanAddThisColor(COLORREF rgbColor)
{
	int ntotal=GetCount( );
	for(int i=0;i<ntotal;i++)
	{
		if(GetItemData(i)==rgbColor)
			return	FALSE;
	}
	return TRUE;
}

void CColorSelectComboBox::InitBox()
{
	AddColor (RGB(255,0,0),"Red");
	AddColor (RGB(0,255,0),"Green");
	AddColor (RGB(0,0,255),"Blue");
	AddColor (RGB(255,255,0),"Yellow");
	AddColor (RGB(255,0,255),"Magenta");
	AddColor (RGB(0,255,255),"Cyan");
	AddColor (RGB(128,0,0),"Dark Red");
	AddColor (RGB(0,128,0),"Dark Green");
	AddColor (RGB(0,0,128),"Dark Blue");
	AddColor (RGB(128,128,0),"Dark Yellow");
	AddColor (RGB(128,0,128),"Dark Magenta");
	AddColor (RGB(0,128,128),"Dark Cyan");
	AddColor (RGB(255,255,255),"White");
	AddColor (RGB(0,0,0),"Black");
	SetCurSel (0);
}

COLORREF CColorSelectComboBox::GetSelColor()
{
	int nSelectedItem=this->GetCurSel ();
	return GetItemData(nSelectedItem);
}
