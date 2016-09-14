// LinearStepParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rsp.h"
#include "LinearStepParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LinearStepParamDlg dialog


LinearStepParamDlg::LinearStepParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LinearStepParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(LinearStepParamDlg)
	m_nX1 = 50;
	m_nX2 = 200;
	m_nY1 = 50;
	m_nY2 = 200;

	m_imgWidthOut=0;
	m_imgHeightOut=0;
	m_pImgDataOut=NULL;
	//}}AFX_DATA_INIT
}


void LinearStepParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LinearStepParamDlg)
	DDX_Text(pDX, IDC_EDIT_LinetransX1, m_nX1);
	DDX_Text(pDX, IDC_EDIT_LinetransX2, m_nX2);
	DDX_Text(pDX, IDC_EDIT_LinetransY1, m_nY1);
	DDX_Text(pDX, IDC_EDIT_LinetransY2, m_nY2);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LinearStepParamDlg, CDialog)
	//{{AFX_MSG_MAP(LinearStepParamDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_EN_KILLFOCUS(IDC_EDIT_LinetransX1, OnKillfocusEDITLinetransX1)
	ON_EN_KILLFOCUS(IDC_EDIT_LinetransX2, OnKillfocusEDITLinetransX2)
	ON_EN_KILLFOCUS(IDC_EDIT_LinetransY1, OnKillfocusEDITLinetransY1)
	ON_EN_KILLFOCUS(IDC_EDIT_LinetransY2, OnKillfocusEDITLinetransY2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LinearStepParamDlg message handlers



BOOL LinearStepParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_LINTRANS_SHOW);
	
	// 计算接受鼠标事件的有效区域
	pWnd->GetClientRect(m_rectMouse);
	pWnd->ClientToScreen(&m_rectMouse);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_rectMouse.top -= rect.top;
	m_rectMouse.left -= rect.left;
	
	// 设置接受鼠标事件的有效区域
	m_rectMouse.top += 25;
	m_rectMouse.left += 10;
	m_rectMouse.bottom = m_rectMouse.top + 255;
	m_rectMouse.right = m_rectMouse.left + 256;
	
	// 初始化拖动状态
	m_nIsDraging = 0;


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void LinearStepParamDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// 字符串
	CString str;
	
	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_LINTRANS_SHOW);
	
	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,330,305);
	
	// 创建画笔对象
	CPen* pPenRed = new CPen;
	
	// 红色画笔
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255,0,0));
	
	// 创建画笔对象
	CPen* pPenBlue = new CPen;
	
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0, 255));
	
	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// 绘制坐标轴
	pDC->MoveTo(10,10);
	
	// 垂直轴
	pDC->LineTo(10,280);
	
	// 水平轴
	pDC->LineTo(320,280);
	
	// 坐标端点
	str.Format("0");
	pDC->TextOut(10, 281, str);
	
	str.Format("255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// 绘制X轴箭头
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
	pDC->LineTo(315,285);
	
	// 绘制X轴箭头
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);	
	
	// 绘制坐标值
	str.Format("(%d, %d)", m_nX1, m_nY1);
	pDC->TextOut(m_nX1 + 10, 281 - m_nY1, str);

	str.Format("(%d, %d)", m_nX2, m_nY2);
	pDC->TextOut(m_nX2 + 10, 281 - m_nY2, str);

 
	// 绘制用户指定的变换直线
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_nX1 + 10, 280 - m_nY1);
	pDC->LineTo(m_nX2 + 10, 280 - m_nY2);
	pDC->LineTo(265, 25);
	
	// 绘制点边缘的小矩形
	CBrush  brush;
	brush.CreateSolidBrush(RGB(0,255,0));
	
	// 选中刷子
	CGdiObject* pOldBrush = pDC->SelectObject(&brush);
	
	// 绘制小矩形
	pDC->Rectangle(m_nX1 + 10 - 2, 280 - m_nY1 - 2, m_nX1 + 12, 280 - m_nY1 + 2);
	pDC->Rectangle(m_nX2 + 10 - 2, 280 - m_nY2 - 2, m_nX2 + 12, 280 - m_nY2 + 2);
	
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	// 绘制边缘
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;

	// Do not call CDialog::OnPaint() for painting messages
}

void LinearStepParamDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// 当用户单击鼠标左键开始拖动
	if(m_rectMouse.PtInRect(point))
	{
		CRect	rectTemp;
		
		// 计算点1临近区域
		rectTemp.left = m_rectMouse.left + m_nX1 - 2;
		rectTemp.right = m_rectMouse.left + m_nX1 + 2;
		rectTemp.top = 255 + m_rectMouse.top - m_nY1 - 2;
		rectTemp.bottom = 255 + m_rectMouse.top - m_nY1 + 2;
		
		// 判断用户是不是想拖动点1
		if (rectTemp.PtInRect(point))
		{
			
			// 设置拖动状态1，拖动点1
			m_nIsDraging = 1;
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		}
		else
		{
			// 计算点2临近区域
			rectTemp.left = m_rectMouse.left + m_nX2 - 2;
			rectTemp.right = m_rectMouse.left + m_nX2 + 2;
			rectTemp.top = 255 + m_rectMouse.top - m_nY2 - 2;
			rectTemp.bottom = 255 + m_rectMouse.top - m_nY2 + 2;
			
			// 判断用户是不是想拖动点2
			if (rectTemp.PtInRect(point))
			{
				
				// 设置拖动状态为2，拖动点2
				m_nIsDraging = 2;
				
				// 更改光标
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void LinearStepParamDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nIsDraging != 0)
	{
		// 重置拖动状态
		m_nIsDraging = 0;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void LinearStepParamDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// 判断当前光标是否在绘制区域
	if(m_rectMouse.PtInRect(point))
	{
		// 判断是否正在拖动
		if (m_nIsDraging != 0)
		{
			// 判断正在拖动点1还是点2
			if (m_nIsDraging == 1)
			{
				// 判断是否下限<上限
				if (point.x - m_rectMouse.left < m_nX2)
				{
					// 更改下限
					m_nX1 =  point.x - m_rectMouse.left;
				}
				else
				{
					// 下限拖过上限，设置为上限-1
					m_nX1 = m_nX2 - 1;
					
					// 重设鼠标位置
					point.x = m_rectMouse.left + m_nX2 - 1;
				}
				
				// 更改Y坐标
				m_nY1 = 255 + m_rectMouse.top - point.y;
			}
			else
			{
				// 正在拖动点2
				
				// 判断是否上限>下限
				if (point.x - m_rectMouse.left > m_nX1)
				{
					// 更改下限
					m_nX2 = point.x - m_rectMouse.left;
				}
				else
				{
					// 下限拖过上限，设置为下限＋1
					m_nX2 = m_nX1 + 1;
					
					// 重设鼠标位置
					point.x = m_rectMouse.left + m_nX1 + 1;
				}
				
				// 更改Y坐标
				m_nY2 = 255 + m_rectMouse.top - point.y;
			}
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			
			// 更新
			UpdateData(FALSE);
			
			// 重绘
			InvalidateRect(m_rectMouse, TRUE);
		}
		else
		{
			CRect	rectTemp1;
			CRect	rectTemp2;
			
			// 计算点1临近区域
			rectTemp1.left = m_rectMouse.left + m_nX1 - 2;
			rectTemp1.right = m_rectMouse.left + m_nX1 + 2;
			rectTemp1.top = 255 + m_rectMouse.top - m_nY1 - 2;
			rectTemp1.bottom = 255 + m_rectMouse.top - m_nY1 + 2;
			
			// 计算点2临近区域
			rectTemp2.left = m_rectMouse.left + m_nX2 - 2;
			rectTemp2.right = m_rectMouse.left + m_nX2 + 2;
			rectTemp2.top = 255 + m_rectMouse.top - m_nY2 - 2;
			rectTemp2.bottom = 255 + m_rectMouse.top - m_nY2 + 2;
			
			// 判断用户在点1或点2旁边
			if ((rectTemp1.PtInRect(point)) || (rectTemp2.PtInRect(point)))
			{
				// 更改光标
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void LinearStepParamDlg::OnOK() 
{
	// TODO: Add extra validation here
/**/
	// 判断是否下限超过上限
	if (m_nX1 > m_nX2)
	{
		// 互换
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	CDialog::OnOK();
}

void LinearStepParamDlg::OnKillfocusEDITLinetransX1() 
{
	// TODO: Add your control notification handler code here
	// 更新
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_nX1 > m_nX2)
	{
		// 互换
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 重绘
	InvalidateRect(m_rectMouse, TRUE);
}

void LinearStepParamDlg::OnKillfocusEDITLinetransX2() 
{
	// TODO: Add your control notification handler code here
	// 更新
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_nX1 > m_nX2)
	{
		// 互换
		int nTemp = m_nX1;
		m_nX1 = m_nX2;
		m_nX2 = nTemp;
		nTemp = m_nY1;
		m_nY1 = m_nY2;
		m_nY2 = nTemp;
		
		// 更新
		UpdateData(FALSE);
	}
}

void LinearStepParamDlg::OnKillfocusEDITLinetransY1() 
{
	// TODO: Add your control notification handler code here
	// 更新
	UpdateData(TRUE);
	
	// 重绘
	InvalidateRect(m_rectMouse, TRUE);
}

void LinearStepParamDlg::OnKillfocusEDITLinetransY2() 
{
	// TODO: Add your control notification handler code here
	// 更新
	UpdateData(TRUE);
	
	// 重绘
	InvalidateRect(m_rectMouse, TRUE);
}

/***********************************************************************
* 函数名称：  LinearStrech()
* 函数参数:   CPoint point1,CPoint point2  -分段线性的对应点
* 返回值:     无
* 说明：分段线性拉伸函数,给定两个分段点，对m_pImgData所指向缓冲区中的灰度
*      或彩色图像进行线性拉伸，为m_pImgDataOut申请内存，存放拉伸结果。
***********************************************************************/
void LinearStepParamDlg::LinearStrech(CPoint point1, CPoint point2, unsigned char *m_pImgData ,int BandCount, int m_imgWidth, int m_imgHeight)
{
	//释放旧的输出图像缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	//if(m_lpColorTableOut!=NULL){
	//	delete []m_lpColorTableOut;
	//	m_lpColorTableOut=NULL;
	//}

	//输出图像每像素位数与输入图像相同
	//m_nBitCountOut=m_nBitCount;

	//计算颜色表长度
	//m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//若有颜色表，输入图像的颜色表与输出图像颜色表相同
	//if(m_nColorTableLengthOut!=0){
	//	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
	//	memcpy(m_lpColorTableOut,m_lpColorTable,
	//		sizeof(RGBQUAD)*m_nColorTableLengthOut);
	//}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//输出图像每行像素所占的字节数
	//int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[BandCount*m_imgWidth*m_imgHeight];


	if(BandCount==1)//如果是灰度图像，则调用单通道数据线性拉伸函数
		LinearStrechForSnglChannel(m_pImgData, m_pImgDataOut,
		m_imgWidth, m_imgHeight,point1,point2);
	else{
		//若彩色图像，则把每个通道的数据进行分离，并线性拉伸，然后再合成为彩色图像

		//单通道数据每行像素所占的字节数
		//int lineByteGray=(m_imgWidth+3)/4*4;

		//临时缓冲区，存放单通道数据
		unsigned char *bufIn=new unsigned char[m_imgHeight*m_imgWidth];

		//临时缓冲区，单通道数据拉伸的结果
		unsigned char *bufOut=new unsigned char[m_imgHeight*m_imgWidth];
		//循环变量，图像坐标
		int i, j;

		//循环变量，遍历彩色图像的三个通道
		int k;

		for(k=0;k<BandCount;k++){
			//将每个通道数据提取出来，存入bufIn缓冲区
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					bufIn[i*m_imgWidth+j]=m_pImgData[k*m_imgWidth*m_imgHeight+i*m_imgWidth+j];
			}
			//对bufIn缓冲区中的数据进行拉伸，拉伸结果存入bufOut中
			LinearStrechForSnglChannel(bufIn,bufOut,m_imgWidth,m_imgHeight,point1,point2);

			//将单通道拉伸的结果存入输出图像m_pImgDataOut对应通道中
			for(i=0;i<m_imgHeight;i++){
				for(j=0;j<m_imgWidth;j++)
					m_pImgDataOut[k*m_imgWidth*m_imgHeight+i*m_imgWidth+j]=bufOut[i*m_imgWidth+j];
			}
		}

		//释放缓冲区
		delete []bufIn;
		delete []bufOut;
	}
}

/***********************************************************************
* 函数名称：  LinearStrechForSnglChannel()
* 函数参数:   unsigned char *pImgDataIn   -待拉伸的位图数据指针
*             unsigned char *pImgDataOut  -拉伸后位图数据指针
*             int imgWidth   -位图宽,像素为单位
*             int imgHeight  -位图高,像素为单位
*             CPoint point1,point2 -分段线性的两个转折点坐标
* 返回值:     无
* 说明：给定一个通道的数据，以及两个转折点坐标，做分段线性拉伸, 将结果
*     存入pImgDataOut所指向的缓冲区。
***********************************************************************/
void LinearStepParamDlg::LinearStrechForSnglChannel(unsigned char *pImgDataIn, 
	unsigned char *pImgDataOut, int imgWidth,int imgHeight,CPoint point1,CPoint point2)
{
	//循环变量，图像坐标
	int i,j;

	//每行像素的字节数，单通道图像
	//int lineByte=(imgWidth+3)/4*4;

	//(x1,y1)和(x2,y2)为两个分段点的坐标
	double x1=point1.x,y1=point1.y,x2=point2.x, y2=point2.y;

	//三个分段的直线斜率
	double slop1=y1/x1,slop2=(y2-y1)/(x2-x1),slop3=(255-y2)/(255-x2);

	//三个直线坐标的斜率
	double dis1=0,dis2=y1-slop2*x1,dis3=y2-slop3*x2;

	//映射表
	int Map[256];

	//根据三个分段，对输入图像每个灰度级计算映射表
	for(i=0;i<256;i++)	
	{
		if(i<x1)
			Map[i]=(int)(slop1*i+dis1+0.5);
		else if(i<x2)
			Map[i]=(int)(slop2*i+dis2+0.5);
		else
			Map[i]=(int)(slop3*i+dis3+0.5);
	}

	//临时变量
	int temp;

	//对每个像素，根据输入像素灰度级查找映射表，并为输出像素赋值
	for(i=0;i<imgHeight;i++)
	{
		for(j=0;j<imgWidth;j++)
		{
			//输入图像(i,j)像素的灰度级
			//temp=*(pImgDataIn+i*lineByte+j);
			temp=pImgDataIn[i*imgWidth+j];
			//根据映射表为输出图像赋值
			//*(pImgDataOut+i*lineByte+j)=Map[temp];
			pImgDataOut[i*imgWidth+j]=Map[temp];
		}
	}
}
