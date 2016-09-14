// cDlgMorphErosion.cpp : implementation file
//

#include "stdafx.h"
#include "rsp.h"
#include "cDlgMorphErosion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cDlgMorphErosion dialog


cDlgMorphErosion::cDlgMorphErosion(CWnd* pParent /*=NULL*/)
	: CDialog(cDlgMorphErosion::IDD, pParent)
{
	//{{AFX_DATA_INIT(cDlgMorphErosion)
	m_nMode = -1;
	m_nStructure1 = -1;
	m_nStructure2 = -1;
	m_nStructure3 = -1;
	m_nStructure4 = -1;
	m_nStructure5 = -1;
	m_nStructure6 = -1;
	m_nStructure7 = -1;
	m_nStructure8 = -1;
	m_nStructure9 = -1;
	//}}AFX_DATA_INIT
}


void cDlgMorphErosion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cDlgMorphErosion)
	DDX_Control(pDX, IDC_Structure9, m_Control9);
	DDX_Control(pDX, IDC_Structure8, m_Control8);
	DDX_Control(pDX, IDC_Structure7, m_Control7);
	DDX_Control(pDX, IDC_Structure6, m_Control6);
	DDX_Control(pDX, IDC_Structure5, m_Control5);
	DDX_Control(pDX, IDC_Structure4, m_Control4);
	DDX_Control(pDX, IDC_Structure3, m_Control3);
	DDX_Control(pDX, IDC_Structure2, m_Control2);
	DDX_Control(pDX, IDC_Structure1, m_Control1);
	DDX_Radio(pDX, IDC_Hori, m_nMode);
	DDX_Radio(pDX, IDC_Structure1, m_nStructure1);
	DDX_Radio(pDX, IDC_Structure2, m_nStructure2);
	DDX_Radio(pDX, IDC_Structure3, m_nStructure3);
	DDX_Radio(pDX, IDC_Structure4, m_nStructure4);
	DDX_Radio(pDX, IDC_Structure5, m_nStructure5);
	DDX_Radio(pDX, IDC_Structure6, m_nStructure6);
	DDX_Radio(pDX, IDC_Structure7, m_nStructure7);
	DDX_Radio(pDX, IDC_Structure8, m_nStructure8);
	DDX_Radio(pDX, IDC_Structure9, m_nStructure9);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cDlgMorphErosion, CDialog)
	//{{AFX_MSG_MAP(cDlgMorphErosion)
	ON_BN_CLICKED(IDC_custom, Oncustom)
	ON_BN_CLICKED(IDC_Vert, OnVert)
	ON_BN_CLICKED(IDC_Hori, OnHori)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cDlgMorphErosion message handlers

void cDlgMorphErosion::Oncustom() 
{
	m_Control1.EnableWindow(TRUE);
	m_Control2.EnableWindow(TRUE);
	m_Control3.EnableWindow(TRUE);
	m_Control4.EnableWindow(TRUE);
	m_Control5.EnableWindow(TRUE);
	m_Control6.EnableWindow(TRUE);
	m_Control7.EnableWindow(TRUE);
	m_Control8.EnableWindow(TRUE);
	m_Control9.EnableWindow(TRUE);
	
}

void cDlgMorphErosion::OnVert() 
{
	m_Control1.EnableWindow(FALSE);
	m_Control2.EnableWindow(FALSE);
	m_Control3.EnableWindow(FALSE);
	m_Control4.EnableWindow(FALSE);
	m_Control5.EnableWindow(FALSE);
	m_Control6.EnableWindow(FALSE);
	m_Control7.EnableWindow(FALSE);
	m_Control8.EnableWindow(FALSE);
	m_Control9.EnableWindow(FALSE);
	
}

void cDlgMorphErosion::OnHori() 
{
	m_Control1.EnableWindow(FALSE);
	m_Control2.EnableWindow(FALSE);
	m_Control3.EnableWindow(FALSE);
	m_Control4.EnableWindow(FALSE);
	m_Control5.EnableWindow(FALSE);
	m_Control6.EnableWindow(FALSE);
	m_Control7.EnableWindow(FALSE);
	m_Control8.EnableWindow(FALSE);
	m_Control9.EnableWindow(FALSE);
	
}

/*************************************************************************
 *
 * �������ƣ�
 *   ErosiontionDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int   nMode		- ��ʴ��ʽ��0��ʾˮƽ����1��ʾ��ֱ����2��ʾ�Զ���ṹԪ�ء�
 *	 int   structure[3][3]
						- �Զ����3��3�ṹԪ�ء�
 *
 * ����ֵ:
 *   BOOL               - ��ʴ�ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ����и�ʴ���㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 * 
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

BOOL cDlgMorphErosion::ErosionDIB(BYTE* lpDIBBits, LONG lWidth, LONG lHeight, int nMode , int structure[3][3])
{
	
	// ָ��Դͼ���ָ��
	//LPSTR	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	//LPSTR	lpDst;
	
	// ָ�򻺴�DIBͼ���ָ��
	//LPSTR	lpNewDIBBits;
	//HLOCAL	hNewDIBBits;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//����ֵ
	unsigned char pixel;

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	//hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	//if (hNewDIBBits == NULL)
	//{
	//	// �����ڴ�ʧ��
	//	return FALSE;
	//}
	
	// �����ڴ�
	//lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	unsigned char* lpDst = new unsigned char[lWidth*lHeight];
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	if(nMode == 0)
	{
		//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				//lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				//lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = lpDIBBits[lWidth*j+i];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;
				
				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
				lpDst[lWidth*j+i] = (unsigned char)0;

				//���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
				for (n = 0;n < 3;n++ )
				{
					//pixel = *(lpSrc+n-1);
					pixel = lpDIBBits[lWidth*j+i+n-1];
					if (pixel == 255 )
					{
						//*lpDst = (unsigned char)255;
						lpDst[lWidth*j+i] = (unsigned char)255;
						break;
					}
				}
				
			}
		}

	}
	else if(nMode == 1)
	{
		//ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				//lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				//lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = lpDIBBits[lWidth*j+i];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
				//*lpDst = (unsigned char)0;
				lpDst[lWidth*j+i] = (unsigned char)0;
				//���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
				for (n = 0;n < 3;n++ )
				{
					//pixel = *(lpSrc+(n-1)*lWidth);
					pixel = lpDIBBits[lWidth*j+i+(n-1)*lWidth];
					if (pixel == 255 )
					{
						//*lpDst = (unsigned char)255;
						lpDst[lWidth*j+i] = (unsigned char)255;
						break;
					}
				}
				
			}
		}

	}
	else
	{
		//ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
				//�����ϱߺ����±ߵ���������
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				//lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				//lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				//pixel = (unsigned char)*lpSrc;
				pixel = lpDIBBits[lWidth*j+i];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
				//*lpDst = (unsigned char)0;
				lpDst[lWidth*j+i] = (unsigned char)0;
				//���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ�����Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
				//ע����DIBͼ�������������µ��õ�
				for (m = 0;m < 3;m++ )
				{
					for (n = 0;n < 3;n++)
					{
						if( structure[m][n] == -1)
							continue;
						//pixel = *(lpSrc + ((2-m)-1)*lWidth + (n-1));
						pixel = lpDIBBits[lWidth*j+i+((2-m)-1)*lWidth + (n-1)];
						if (pixel == 255 )
						{	
							//*lpDst = (unsigned char)255;
							lpDst[lWidth*j+i]=(unsigned char)255;
							break;
						}
					}
				}
				
			}
		}

	}
	// ���Ƹ�ʴ���ͼ��
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	memcpy(lpDIBBits, lpDst, lWidth * lHeight);
	// �ͷ��ڴ�
	//LocalUnlock(hNewDIBBits);
	//LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DilationDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int   nMode		- ���ͷ�ʽ��0��ʾˮƽ����1��ʾ��ֱ����2��ʾ�Զ���ṹԪ�ء�
 *	 int   structure[3][3]
						- �Զ����3��3�ṹԪ�ء�
 *
 * ����ֵ:
 *   BOOL               - ���ͳɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ������������㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 * 
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/


BOOL WINAPI DilationDIB(BYTE* lpDIBBits, LONG lWidth, LONG lHeight, int nMode , int structure[3][3])
{
	
	// ָ��Դͼ���ָ��
	//LPSTR	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	//LPSTR	lpDst;
	
	// ָ�򻺴�DIBͼ���ָ��
	//LPSTR	lpNewDIBBits;
	//HLOCAL	hNewDIBBits;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//����ֵ
	unsigned char pixel;

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	//hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	//if (hNewDIBBits == NULL)
	//{
	//	// �����ڴ�ʧ��
	//	return FALSE;
	//}
	
	// �����ڴ�
	//lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	//lpDst = (char *)lpNewDIBBits;
	//memset(lpDst, (BYTE)255, lWidth * lHeight);
	unsigned char* lpDst = new unsigned char[lWidth*lHeight];
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	if(nMode == 0)
	{
		//ʹ��ˮƽ����ĽṹԪ�ؽ�������
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
	/*			lpSrc = (char *)lpDIBBits + lWidth * j + i;*/

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				//lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)lpDIBBits[lWidth * j + i];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;
				
				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
				lpDst[lWidth * j + i] = (unsigned char)255;

				//Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
				for (n = 0;n < 3;n++ )
				{
					//pixel = *(lpSrc+n-1);
					pixel = lpDIBBits[lWidth * j + i+n-1];
					if (pixel == 0 )
					{
						lpDst[lWidth * j + i] = (unsigned char)0;
						break;
					}
				}
				
			}
		}

	}
	else if(nMode == 1)
	{
		//ʹ�ô�ֱ����ĽṹԪ�ؽ�������
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				//lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				//lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)lpDIBBits[ lWidth * j + i];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
				lpDst[ lWidth * j + i] = (unsigned char)255;

				//Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
				for (n = 0;n < 3;n++ )
				{
					pixel = lpDIBBits[ lWidth * j + i+(n-1)*lWidth ];
					if (pixel == 0 )
					{
						lpDst[ lWidth * j + i] = (unsigned char)0;
						break;
					}
				}
				
			}
		}

	}
	else
	{
		//ʹ���Զ���ĽṹԪ�ؽ�������
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
				//�����ϱߺ����±ߵ���������
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				//lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				//lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)lpDIBBits[ lWidth * j + i ];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
				lpDst[ lWidth * j + i ] = (unsigned char)255;

				//ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ����ֻҪ��һ���Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
				//ע����DIBͼ�������������µ��õ�
				for (m = 0;m < 3;m++ )
				{
					for (n = 0;n < 3;n++)
					{
						if( structure[m][n] == -1)
							continue;
						pixel = lpDIBBits[ lWidth * j + i  + ((2-m)-1)*lWidth + (n-1)];
						if (pixel == 0 )
						{	
							lpDst[ lWidth * j + i ] = (unsigned char)0;
							break;
						}
					}
				}
				
			}
		}

	}
	// �������ͺ��ͼ��
	memcpy(lpDIBBits, lpDst, lWidth * lHeight);

	// �ͷ��ڴ�
	//LocalUnlock(hNewDIBBits);
	//LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   OpenDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int   nMode		- �����㷽ʽ��0��ʾˮƽ����1��ʾ��ֱ����2��ʾ�Զ���ṹԪ�ء�
 *	 int   structure[3][3]
						- �Զ����3��3�ṹԪ�ء�
 *
 * ����ֵ:
 *   BOOL               - ������ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ����п����㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 * 
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

BOOL WINAPI OpenDIB(BYTE* lpDIBBits, LONG lWidth, LONG lHeight, int nMode , int structure[3][3])
{
	
	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	BYTE*	lpDst;
	
	// ָ�򻺴�DIBͼ���ָ��
	BYTE*	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//����ֵ
	unsigned char pixel;

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (BYTE* )LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (BYTE *)lpNewDIBBits;
	//unsigned char* lpDst = new unsigned char[lWidth*lHeight];
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	if(nMode == 0)
	{
		//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (BYTE*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (BYTE*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;
				
				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
				*lpDst = (unsigned char)0;

				//���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+n-1);
					if (pixel == 255 )
					{
						*lpDst = (unsigned char)255;
						break;
					}
				}
				
			}
		}

	}
	else if(nMode == 1)
	{
		//ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (BYTE*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (BYTE*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
				*lpDst = (unsigned char)0;

				//���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+(n-1)*lWidth);
					if (pixel == 255 )
					{
						*lpDst = (unsigned char)255;
						break;
					}
				}
				
			}
		}

	}
	else
	{
		//ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
				//�����ϱߺ����±ߵ���������
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (BYTE*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (BYTE*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
				*lpDst = (unsigned char)0;

				//���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ�����Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
				//ע����DIBͼ�������������µ��õ�
				for (m = 0;m < 3;m++ )
				{
					for (n = 0;n < 3;n++)
					{
						if( structure[m][n] == -1)
							continue;
						pixel = *(lpSrc + ((2-m)-1)*lWidth + (n-1));
						if (pixel == 255 )
						{	
							*lpDst = (unsigned char)255;
							break;
						}
					}
				}
				
			}
		}

	}
	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	// ���³�ʼ���·�����ڴ棬�趨��ʼֵΪ255
	//lpDst = (char *)lpNewDIBBits;
	//memset(lpDst, (BYTE)255, lWidth * lHeight);
	if(nMode == 0)
	{
		//ʹ��ˮƽ����ĽṹԪ�ؽ�������
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (BYTE*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (BYTE*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;
				
				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
				*lpDst = (unsigned char)255;

				//Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+n-1);
					if (pixel == 0 )
					{
						*lpDst = (unsigned char)0;
						break;
					}
				}
				
			}
		}

	}
	else if(nMode == 1)
	{
		//ʹ�ô�ֱ����ĽṹԪ�ؽ�������
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (BYTE*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (BYTE*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
				*lpDst = (unsigned char)255;

				//Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
				for (n = 0;n < 3;n++ )
				{
					pixel = *(lpSrc+(n-1)*lWidth);
					if (pixel == 0 )
					{
						*lpDst = (unsigned char)0;
						break;
					}
				}
				
			}
		}

	}
	else
	{
		//ʹ���Զ���ĽṹԪ�ؽ�������
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
				//�����ϱߺ����±ߵ���������
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (BYTE*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (BYTE*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
				*lpDst = (unsigned char)255;

				//ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ����ֻҪ��һ���Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
				//ע����DIBͼ�������������µ��õ�
				for (m = 0;m < 3;m++ )
				{
					for (n = 0;n < 3;n++)
					{
						if( structure[m][n] == -1)
							continue;
						pixel = *(lpSrc + ((2-m)-1)*lWidth + (n-1));
						if (pixel == 0 )
						{	
							*lpDst = (unsigned char)0;
							break;
						}
					}
				}
				
			}
		}

	}
	// �������ͺ��ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);


	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   CloseDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int   nMode		- �����㷽ʽ��0��ʾˮƽ����1��ʾ��ֱ����2��ʾ�Զ���ṹԪ�ء�
 *	 int   structure[3][3]
						- �Զ����3��3�ṹԪ�ء�
 *
 * ����ֵ:
 *   BOOL               - ������ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ����п����㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 * 
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

/*************************************************************************
 *
 * �������ƣ�
 *   ThinDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *   BOOL               - ������ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ�����ϸ�����㡣
 * 
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

BOOL WINAPI CloseDIB(BYTE* lpDIBBits, LONG lWidth, LONG lHeight, int nMode , int structure[3][3])
{
		if (DilationDIB(lpDIBBits, lWidth, lHeight, nMode , structure))
		{

			if (ErosionDIB(lpDIBBits, lWidth, lHeight, nMode , structure))
			{
				// ����
				return TRUE;

			}
		}
		return FALSE;
	
	// ����
	return TRUE;
}

//Zhang���п���ϸ���㷨
BOOL WINAPI ThiningDIB(BYTE* lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	BYTE*	lpDst;
	
	// ָ�򻺴�DIBͼ���ָ��
	BYTE*	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//����
	BOOL bModified;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//�ĸ�����
	BOOL bCondition1;
	BOOL bCondition2;
	BOOL bCondition3;
	BOOL bCondition4;

	//������
	unsigned char nCount;
	
	//����ֵ
	unsigned char pixel;

	//5��5������������ֵ
	unsigned char neighbour[5][5];

	for (int c=0;c<lHeight;c++)
	{
		for (int v=0;v<lWidth;v++)
		{
			if(lpDIBBits[c*lWidth+v] != 255 && lpDIBBits[c*lWidth+v] != 0)
				return false;
		}
	}

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (BYTE* )LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (BYTE*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	bModified=TRUE;

	while(bModified)
	{

		bModified = FALSE;
		// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
		lpDst = (BYTE*)lpNewDIBBits;
		memset(lpDst, (BYTE)255, lWidth * lHeight);

		for(j = 2; j <lHeight-2; j++)
		{
			for(i = 2;i <lWidth-2; i++)
			{

				bCondition1 = FALSE;
				bCondition2 = FALSE;
				bCondition3 = FALSE;
				bCondition4 = FALSE;

				//����ʹ��5��5�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ�������Χ�ļ��кͼ�������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (BYTE*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (BYTE*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && *lpSrc != 0)
					//return FALSE;
					continue;
				//���Դͼ���е�ǰ��Ϊ��ɫ��������
				else if(pixel == 255)
					continue;

				//��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
				for (m = 0;m < 5;m++ )
				{
					for (n = 0;n < 5;n++)
					{
						neighbour[m][n] =(255 - (unsigned char)*(lpSrc + ((4 - m) - 2)*lWidth + n - 2 )) / 255;
					}
				}
//				neighbour[][]
				//����ж�������
				//�ж�2<=NZ(P1)<=6
				nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
						+ neighbour[2][1] + neighbour[2][3] + \
						+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if ( nCount >= 2 && nCount <=6)
					bCondition1 = TRUE;

				//�ж�Z0(P1)=1
				nCount = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if (nCount == 1)
					bCondition2 = TRUE;

				//�ж�P2*P4*P8=0 or Z0(p2)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[2][3] == 0)
					bCondition3 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount++;
					if (nCount != 1)
						bCondition3 = TRUE;
				}

				//�ж�P2*P4*P6=0 or Z0(p4)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[3][2] == 0)
					bCondition4 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (nCount != 1)
						bCondition4 = TRUE;
				}
				if(bCondition1 && bCondition2 && bCondition3 && bCondition4)
				{
					*lpDst = (unsigned char)255;
					bModified = TRUE;
				}
				else
				{
					*lpDst = (unsigned char)0;
				}
			}
		}
			// ���Ƹ�ʴ���ͼ��
			memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	}
	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}

BOOL WINAPI ErosionDIB(BYTE* lpDIBBits, LONG lWidth, LONG lHeight, int nMode , int structure[3][3])
{
	
	// ָ��Դͼ���ָ��
	//LPSTR	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	//LPSTR	lpDst;
	
	// ָ�򻺴�DIBͼ���ָ��
	//LPSTR	lpNewDIBBits;
	//HLOCAL	hNewDIBBits;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//����ֵ
	unsigned char pixel;

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	//hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	//if (hNewDIBBits == NULL)
	//{
	//	// �����ڴ�ʧ��
	//	return FALSE;
	//}
	
	// �����ڴ�
	//lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	unsigned char* lpDst = new unsigned char[lWidth*lHeight];
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	if(nMode == 0)
	{
		//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
		for(j = 0; j <lHeight; j++)
		{
			for(i = 1;i <lWidth-1; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				//lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				//lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = lpDIBBits[lWidth*j+i];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;
				
				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
				lpDst[lWidth*j+i] = (unsigned char)0;

				//���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
				for (n = 0;n < 3;n++ )
				{
					//pixel = *(lpSrc+n-1);
					pixel = lpDIBBits[lWidth*j+i+n-1];
					if (pixel == 255 )
					{
						//*lpDst = (unsigned char)255;
						lpDst[lWidth*j+i] = (unsigned char)255;
						break;
					}
				}
				
			}
		}

	}
	else if(nMode == 1)
	{
		//ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				//lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				//lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = lpDIBBits[lWidth*j+i];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
				//*lpDst = (unsigned char)0;
				lpDst[lWidth*j+i] = (unsigned char)0;
				//���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
				for (n = 0;n < 3;n++ )
				{
					//pixel = *(lpSrc+(n-1)*lWidth);
					pixel = lpDIBBits[lWidth*j+i+(n-1)*lWidth];
					if (pixel == 255 )
					{
						//*lpDst = (unsigned char)255;
						lpDst[lWidth*j+i] = (unsigned char)255;
						break;
					}
				}
				
			}
		}

	}
	else
	{
		//ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
		for(j = 1; j <lHeight-1; j++)
		{
			for(i = 0;i <lWidth; i++)
			{
				//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
				//�����ϱߺ����±ߵ���������
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				//lpSrc = (char *)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				//lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				//pixel = (unsigned char)*lpSrc;
				pixel = lpDIBBits[lWidth*j+i];

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && pixel != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
				//*lpDst = (unsigned char)0;
				lpDst[lWidth*j+i] = (unsigned char)0;
				//���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ�����Ǻ�ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
				//ע����DIBͼ�������������µ��õ�
				for (m = 0;m < 3;m++ )
				{
					for (n = 0;n < 3;n++)
					{
						if( structure[m][n] == -1)
							continue;
						//pixel = *(lpSrc + ((2-m)-1)*lWidth + (n-1));
						pixel = lpDIBBits[lWidth*j+i+((2-m)-1)*lWidth + (n-1)];
						if (pixel == 255 )
						{	
							//*lpDst = (unsigned char)255;
							lpDst[lWidth*j+i]=(unsigned char)255;
							break;
						}
					}
				}
				
			}
		}

	}
	// ���Ƹ�ʴ���ͼ��
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	memcpy(lpDIBBits, lpDst, lWidth * lHeight);
	// �ͷ��ڴ�
	//LocalUnlock(hNewDIBBits);
	//LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}

/***********************************************************************
* �������ƣ�
* HitAndMiss()
*
*����������
*  unsigned char *imgBufIn   -���任������ͼ��
*  unsigned char *imgBufOut   -���ͼ��
*  int imgWidth   -ͼ���
*  int imgHeight   -ͼ���
*  struct ElementPair hitMissMask  -���л����нṹԪ�ض�
*
*����ֵ��
*   ��
*
*˵�������л����б任,0������,255����Ŀ��
***********************************************************************/
BOOL WINAPI HitAndMiss(unsigned char *imgBufIn, int imgWidth, int imgHeight)						 
{
	ElementPair hitMissMask[8];
	//����8������Ļ��л����б任�ṹԪ�ض�
	for(int i=0;i<8;i++)
	{
		for(int k=0;k<3;k++)
		{
			for(int l=0;l<3;l++)
			{
				hitMissMask[i].hitElement[k*3+l]=0;
				hitMissMask[i].missElement[k*3+l]=0;
			}
		}
	}
	hitMissMask[0].hitElement[0]=1;
	hitMissMask[0].hitElement[1]=1;
	hitMissMask[0].hitElement[2]=1;
	hitMissMask[0].hitElement[4]=1;
	hitMissMask[0].missElement[6]=1;
	hitMissMask[0].missElement[7]=1;
	hitMissMask[0].missElement[8]=1;

	hitMissMask[1].hitElement[6]=1;
	hitMissMask[1].hitElement[7]=1;
	hitMissMask[1].hitElement[8]=1;
	hitMissMask[1].hitElement[4]=1;
	hitMissMask[1].missElement[0]=1;
	hitMissMask[1].missElement[1]=1;
	hitMissMask[1].missElement[2]=1;

	hitMissMask[2].hitElement[2]=1;
	hitMissMask[2].hitElement[5]=1;
	hitMissMask[2].hitElement[8]=1;
	hitMissMask[2].hitElement[4]=1;
	hitMissMask[2].missElement[0]=1;
	hitMissMask[2].missElement[3]=1;
	hitMissMask[2].missElement[6]=1;

	hitMissMask[3].hitElement[0]=1;
	hitMissMask[3].hitElement[3]=1;
	hitMissMask[3].hitElement[6]=1;
	hitMissMask[3].hitElement[4]=1;
	hitMissMask[3].missElement[2]=1;
	hitMissMask[3].missElement[5]=1;
	hitMissMask[3].missElement[8]=1;


	hitMissMask[4].hitElement[0]=1;
	hitMissMask[4].hitElement[1]=1;
	hitMissMask[4].hitElement[3]=1;
	hitMissMask[4].hitElement[4]=1;
	hitMissMask[4].missElement[5]=1;
	hitMissMask[4].missElement[7]=1;
	hitMissMask[4].missElement[8]=1;

	hitMissMask[5].hitElement[5]=1;
	hitMissMask[5].hitElement[7]=1;
	hitMissMask[5].hitElement[8]=1;
	hitMissMask[5].hitElement[4]=1;
	hitMissMask[5].missElement[0]=1;
	hitMissMask[5].missElement[1]=1;
	hitMissMask[5].missElement[3]=1;


	hitMissMask[6].hitElement[1]=1;
	hitMissMask[6].hitElement[2]=1;
	hitMissMask[6].hitElement[5]=1;
	hitMissMask[6].hitElement[4]=1;
	hitMissMask[6].missElement[3]=1;
	hitMissMask[6].missElement[6]=1;
	hitMissMask[6].missElement[7]=1;

	hitMissMask[7].hitElement[3]=1;
	hitMissMask[7].hitElement[6]=1;
	hitMissMask[7].hitElement[7]=1;
	hitMissMask[7].hitElement[4]=1;
	hitMissMask[7].missElement[1]=1;
	hitMissMask[7].missElement[2]=1;
	hitMissMask[7].missElement[5]=1;

	for (int c=0;c<imgHeight;c++)
	{
		for (int v=0;v<imgWidth;v++)
		{
			if(imgBufIn[c*imgWidth+v] != 255 && imgBufIn[c*imgWidth+v] != 0)
				return false;
		}
	}

	//ͼ��ÿ��������ռ�ֽ���
	int lineByte=(imgWidth+3)/4*4;

	//���ͼ����0
	unsigned char *imgBufOut = new unsigned char[imgWidth*imgHeight];
	memset(imgBufOut, 0, lineByte*imgHeight);

	for(int h=0;h<8;h++)
	{
		//ѭ������
		int i, j;

		//��־������1��ʾ�ṹԪ�ض���Ч��0��ʾ��Ч
		int validateFlag=1;

		//���ṹԪ�ض��Ƿ񽻼�Ϊ��,��������Ϊ����Ϊ��Ч�ṹԪ�ض�,�㷨���˳�
		for(i=0; i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if(hitMissMask[h].hitElement[i*3+j]&&hitMissMask[h].missElement[i*3+j])
				{
					validateFlag=0;
					break;
				}
			}
		}

		//�Ƿ��ṹԪ�ض�,����
		if(validateFlag==0)
			return false;

		//ѭ������
		int k,l;

		//���б�־�����ͻ����б�־����
		int hitFlag, missFlag;

		for(i=1; i<imgHeight-1; i++)
		{
			for(j=1;j<imgWidth-1; j++)
			{
				hitFlag=1;
				missFlag=1;
				for(k=-1;k<=1;k++)
				{
					for(l=-1;l<=1;l++)
					{
						//������нṹԪ�ص�ǰλ��Ϊ1
						if(hitMissMask[h].hitElement[(k+1)*3+l+1]==1)
						{
							//�ж�ͼ���Ӧ���Ƿ�Ϊ0,�����,��û�л���ͼ��ǰ��
							if(!*(imgBufIn+(i+k)*lineByte+j+l))
								hitFlag=0;
						}

						//��������нṹԪ�ص�ǰλ��Ϊ1
						if(hitMissMask[h].missElement[(k+1)*3+l+1]==1)
						{
							//�ж�ͼ���Ӧ���Ƿ�Ϊ0,�����,��û�л���ͼ��ǰ��
							if(*(imgBufIn+(i+k)*lineByte+j+l))
								missFlag=0;
						}
					}
				}

				//�������255
				if(hitFlag&&missFlag)
					*(imgBufOut+i*lineByte+j)=255;
			}
		}
	}
	memcpy(imgBufIn, imgBufOut, imgWidth * imgHeight);
	delete []imgBufOut;
	return true;
}

