// ImageMatch.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "ImageMatch.h"
#include "afxdialogex.h"


// CImageMatch �Ի���

IMPLEMENT_DYNAMIC(CImageMatch, CDialogEx)

CImageMatch::CImageMatch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageMatch::IDD, pParent)
{

	m_threL = 1000;
	m_threR = 1000;
	m_winL1 = 7;
	m_winL2 = 5;
	m_winR1 = 7;
	m_winR2 = 5;
	m_scare = 11;
	m_cort = 0.9;
}

CImageMatch::~CImageMatch()
{
}

void CImageMatch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MATCHTHRE2, m_threL);
	DDX_Text(pDX, IDC_EDIT_MATCHTHRE3, m_threR);
	DDX_Text(pDX, IDC_EDIT_MATCHWIN3, m_winL1);
	DDX_Text(pDX, IDC_EDIT_MATCHWIN4, m_winL2);
	DDX_Text(pDX, IDC_EDIT_MATCHWIN5, m_winR1);
	DDX_Text(pDX, IDC_EDIT_MATCHWIN6, m_winR2);
	DDX_Text(pDX, IDC_EDIT_SCARE, m_scare);
	DDX_Text(pDX, IDC_EDIT2_MATCHCORT, m_cort);
}


BEGIN_MESSAGE_MAP(CImageMatch, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MOIN, &CImageMatch::OnBnClickedButtonMoin)
	ON_BN_CLICKED(IDC_BUTTON_MOIN2, &CImageMatch::OnBnClickedButtonMoin2)
	ON_BN_CLICKED(IDC_BUTTON_MOUT, &CImageMatch::OnBnClickedButtonMout)
	ON_BN_CLICKED(IDC_BUTTON_MOUT2, &CImageMatch::OnBnClickedButtonMout2)
	ON_BN_CLICKED(IDC_BUTTON_CODAT, &CImageMatch::OnBnClickedButtonCodat)
	ON_BN_CLICKED(IDOK, &CImageMatch::OnBnClickedOk)
END_MESSAGE_MAP()


// CImageMatch ��Ϣ�������

//������Ӱ��
void CImageMatch::OnBnClickedButtonMoin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="���������ȡmoravecң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		MH_inL=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INMATCHL,MH_inL);

}


void CImageMatch::OnBnClickedButtonMoin2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="���������ȡmoravecң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		MH_inR=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INMATCHR,MH_inR);

}

//�����Ӱ��
void CImageMatch::OnBnClickedButtonMout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.BMP)|*.BMP|JPG�ļ�(*.JPG)|*.JPG;*.JPEG|tiff�ļ�(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="��������Ӱ��";
	if (file.DoModal()==IDOK)
	{
		MH_outL=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_OUTMATCHL,MH_outL);
}


void CImageMatch::OnBnClickedButtonMout2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.BMP)|*.BMP|JPG�ļ�(*.JPG)|*.JPG;*.JPEG|tiff�ļ�(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="��������Ӱ��";
	if (file.DoModal()==IDOK)
	{
		MH_outR=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_OUTMATCHR,MH_outR);
}

//���ƥ���
void CImageMatch::OnBnClickedButtonCodat()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|�ı�(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="��������Ȥ���ļ�";
	if (file.DoModal()==IDOK)
	{
		MH_data=file.GetPathName();	
	}
	SetDlgItemText(IDC_STATIC_OUTMATCHDATA,MH_data);
}



//����˵����Data�����Ӱ�����ݣ�nImageSizeX������Ӱ���ȣ�nImageSizeY������Ӱ�񳤶ȣ�FilePath����Ӱ���·����
void CImageMatch::ReadImage(BYTE* &Data,int &nImageSizeX,int &nImageSizeY,CString FilePath)
{
	GDALDataset* poDataset;//GDAL���ݼ�
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(FilePath,GA_ReadOnly);

	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
		return;
	}

	nImageSizeX=poDataset->GetRasterXSize();
	nImageSizeY=poDataset->GetRasterYSize();

	Data = new BYTE[nImageSizeX*nImageSizeY*3];
	poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, Data,nImageSizeX,nImageSizeY, GDT_Byte,3, NULL,0,0,0 );
	GDALClose(poDataset);
}


// �洢Ӱ��
//����˵����Indata��Ҫ�����Ӱ��nImageSizeX��Ӱ���ȣ�nImageSizeY��Ӱ�񳤶ȣ�OutPath�����·����ImageType���洢��ʽ
void CImageMatch::SaveImage(BYTE* Outdata,int nImageSizeX,int nImageSizeY,CString OutPath, CString ImageType)
{
	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(ImageType);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)OutPath,//���·��
		nImageSizeX,nImageSizeY, //Ӱ�����ݿ��
		3, //���ȡ�Ĳ�����
		GDT_Byte, 	//��������������
		papszOptions );

	if(poDstDS->RasterIO(
		GF_Write,						//IO��ʽ
		0,0,							//Ӱ��������ʼλ��x,y
		nImageSizeX,nImageSizeY,				//Ӱ�����ݿ��
		Outdata,						//ָ�򻺳�������ָ��
		nImageSizeX,nImageSizeY,				//���������ݿ��
		GDT_Byte,						//��������������
		3,						//���ȡ�Ĳ�����
		NULL,							//���ȡ�Ĳ����б�
		0,								//����������һ������ռ�õ��ֽ���
		0,								//������һ��������ռ�õ��ֽ���
		0)==CE_Failure )				//���������ݴ洢��ʽ 0ΪRRRRGGGGBBBB-bsp 1ΪRGBRGBRGB-bip	
	{
		AfxMessageBox("�����ļ�ʧ�ܣ�");
		return;
	}
	GDALClose(poDstDS);
}


// �洢ƥ���
void CImageMatch::SaveToTxt(vector<MATCHEDPOINTS> MatchedPoints,float thre,int Wsize,CString OutFile)
{
	FILE*  pTxtFile= fopen(OutFile, "wt");

	if (!pTxtFile)
		return ;

	fprintf(pTxtFile, "**********\t�ĵ�˵����%s\n**********\t���������ˣ�%s\n**********\t��λ: %s\n"," ���ϵ��ƥ��������ĵ�","������", "�й����ʴ�ѧ");
	fprintf(pTxtFile, "**********\t���ϵ����ֵ: %f\n", thre);
	fprintf(pTxtFile, "**********\tƥ�䴰�ڴ�С: %d\n",Wsize);
	fprintf(pTxtFile, "**********\tƥ�����: %d\n\n",MatchedPoints.size());
	fprintf(pTxtFile, "X_L\tY_L\tX_R\tY_R\t���ϵ��\n");

	vector<MATCHEDPOINTS>::iterator iter;

	for (iter = MatchedPoints.begin(); iter != MatchedPoints.end(); ++iter)
		fprintf(pTxtFile, "%d\t%d\t%d\t%d\t%f\n",
		iter->x1, iter->y1, iter->x2, iter->y2, iter->coefficient);

	fclose(pTxtFile);

}


// //����Ī��ά������
void CImageMatch::Movarec(BYTE* Indata1,vector<FEATUREPOINT> &FeaturePoint,int ImageSizeX,int ImageSizeY,int m_win1,int m_win2,double m_thre)
{
	int k=m_win1/2;
	//int Count1=0;//��ѡ�����
	//int Count3=0;

	float* pInterest = new float[ImageSizeX*ImageSizeY];
	ZeroMemory(pInterest, sizeof(float)*ImageSizeX*ImageSizeY);//����ֵ����(��memset������಻��ֻ������win32����)

	BYTE* Indata=new BYTE[ImageSizeX*ImageSizeY];
	ZeroMemory(Indata,sizeof(BYTE)*ImageSizeX*ImageSizeY);

	BYTE *TempD1 = new BYTE[ImageSizeX*ImageSizeY];
	BYTE *TempD2 = new BYTE[ImageSizeX*ImageSizeY];
	BYTE *TempD3 = new BYTE[ImageSizeX*ImageSizeY];
	for (int i=0;i<ImageSizeX*ImageSizeY;i++)//�Ը������ν��м�Ȩ���
	{
		TempD1[i]=Indata1[i]*0.299;
		TempD2[i]=Indata1[ImageSizeX*ImageSizeY+i]*0.587;
		TempD3[i]=Indata1[ImageSizeX*ImageSizeY*2+i]*0.144;
		Indata[i]=TempD1[i]+TempD2[i]+TempD3[i];//�洢��Ȩ��͵Ľ��
	}

	for (int i = k; i < ImageSizeY - k; ++i)
	{
		for (int j = k; j < ImageSizeX - k; ++j)
		{
			double IV=0.0,v1=0.0,v2 =0.0,v3=0.0,v4=0.0;//�����ĸ������ƽ����
			for (int m = 0; m < m_win1-1; ++m)
			{
				v1 += (Indata[(i-k+m)*ImageSizeX + (j)] - Indata[(i-k+1+m)*ImageSizeX + (j)])*
					(Indata[(i-k+m)*ImageSizeX + (j)] - Indata[(i-k+1+m)*ImageSizeX + (j)]);//��ֱ����

				v2 += (Indata[(i-k+m)*ImageSizeX + (j-k+m)] - Indata[(i-k+1+m)*ImageSizeX + (j-k+m+1)])*
					(Indata[(i-k+m)*ImageSizeX + (j-k+m)] - Indata[(i-k+1+m)*ImageSizeX + (j-k+m+1)]);//��������

				v3 += (Indata[(i)*ImageSizeX + (j-k+m)] - Indata[(i)*ImageSizeX + (j-k+1+m)])*
					(Indata[(i)*ImageSizeX + (j-k+m)] - Indata[(i)*ImageSizeX + (j-k+1+m)]);//ˮƽ����

				v4 += (Indata[(i-k+m)*ImageSizeX + (j+k-m)] - Indata[(i-k+m+1)*ImageSizeX + (j+k-m-1)])*
					(Indata[(i-k+m)*ImageSizeX + (j+k-m)] - Indata[(i-k+m+1)*ImageSizeX + (j+k-m-1)]);//��������
			}
			IV = min(v1, v2);
			IV = min(IV, v3);
			IV = min(IV, v4);

			if (IV > m_thre)
			{
				//��Ϊ��ѡ��
				pInterest[i*ImageSizeX+j] = IV;
			}
		}
	}


	int k2 = m_win2 / 2;
	float max2(0);
	int tempX(0), tempY(0);
	bool b(false);

	FEATUREPOINT tempFP;
	ZeroMemory(&tempFP, sizeof(FEATUREPOINT));

	for(int i = k2; i < ImageSizeY - k2; i += m_win2)
	{
		for(int j = k2; j < ImageSizeX - k2; j += m_win2)
		{
			max2 = 0;
			b = false;
			for (int m = i-k2; m < i+k2+1; ++m)
			{
				for (int n = j-k2; n<j+k2+1; ++n)
				{
					if(pInterest[m*ImageSizeX + n]>max2)
					{
						max2 = pInterest[m*ImageSizeX + n];//��¼���㴰���е����ֵ
						tempY = m;//��¼���ֵ��������
						tempX = n;
						b = true;//�ҵ������ֵ
					}
				}
			}
			if (b)
			{
				tempFP.IV = pInterest[tempY*ImageSizeX+tempX];
				tempFP.x = tempX;
				tempFP.y = tempY;
				FeaturePoint.push_back(tempFP);//����������
			}
		}
	}



	//ȥ���ظ����Ƶ�
	vector<FEATUREPOINT> ret;
	vector<FEATUREPOINT>::iterator iter = FeaturePoint.begin();

	ret.clear();
	ret.push_back(*iter);

	BOOL b1(FALSE);

	for (iter = FeaturePoint.begin(); iter != FeaturePoint.end(); ++iter)
	{
		b1 = false;

		for (vector<FEATUREPOINT>::iterator iter2 = ret.begin(); iter2 != ret.end(); ++iter2)
		{
			if (iter->x == iter2->x && iter->y == iter2->y)
			{
				b1 = TRUE;	//����
				break;
			}
		}

		if (b1 == FALSE)
		{
			ret.push_back(*iter);
		}
	}
	FeaturePoint=ret;

	//������λͼ
	//vector<FEATUREPOINT>::iterator iter;
	//for (iter = FeaturePoint2.begin(); iter != FeaturePoint2.end(); ++iter)
	//{
	//	int i = iter->y;
	//	int j = iter->x;

	//	//��һ����
	//	outdata[i*ImageSizeY+j] = 255;

	//	outdata[i*ImageSizeY+j+1] = 255;
	//	outdata[i*ImageSizeY+j-1] = 255;
	//	outdata[(i+1)*ImageSizeY+j] = 255;
	//	outdata[(i-1)*ImageSizeY+j] = 255;

	//	outdata[i*ImageSizeY+j+2] = 255;
	//	outdata[i*ImageSizeY+j-2] = 255;
	//	outdata[(i+2)*ImageSizeY+j] = 255;
	//	outdata[(i-2)*ImageSizeY+j] = 255;


	//	//�ڶ�����
	//	outdata[ImageSizeX*ImageSizeY+i*ImageSizeY+j] = 0;

	//	outdata[ImageSizeX*ImageSizeY+i*ImageSizeY+j+1] = 0;
	//	outdata[ImageSizeX*ImageSizeY+i*ImageSizeY+j-1] = 0;
	//	outdata[ImageSizeX*ImageSizeY+(i+1)*ImageSizeY+j] = 0;
	//	outdata[ImageSizeX*ImageSizeY+(i-1)*ImageSizeY+j] = 0;

	//	outdata[ImageSizeX*ImageSizeY+i*ImageSizeY+j+2] = 0;
	//	outdata[ImageSizeX*ImageSizeY+i*ImageSizeY+j-2] = 0;
	//	outdata[ImageSizeX*ImageSizeY+(i+2)*ImageSizeY+j] = 0;
	//	outdata[ImageSizeX*ImageSizeY+(i-2)*ImageSizeY+j] = 0;

	//	//��������
	//	outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeY+j] = 0;

	//	outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeY+j+1] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeY+j-1] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+(i+1)*ImageSizeY+j] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+(i-1)*ImageSizeY+j] = 0;

	//	outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeY+j+2] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeY+j-2] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+(i+2)*ImageSizeY+j] = 0;
	//	outdata[ImageSizeX*ImageSizeY*2+(i-2)*ImageSizeY+j] = 0;

		//�ͷ��ڴ�
	delete[] pInterest;
	delete[] Indata;
	delete[] TempD1;
	delete[] TempD2;
	delete[] TempD3;

}


void CImageMatch::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (MH_inL==""||MH_inR==""||MH_outL==""||MH_outR==""||MH_data=="")
	{
		AfxMessageBox("���������ļ�����Ϊ�գ�");
		return;
	}


	//BYTE* ImageL=new BYTE[5000*5000*3];//�洢��Ӱ��
	//ZeroMemory(ImageL,sizeof(BYTE)*5000*5000*3);
	//BYTE* ImageR=new BYTE[5000*5000*3];//�洢��Ӱ��
	//ZeroMemory(ImageR,sizeof(BYTE)*5000*5000*3);
	BYTE* ImageL;
	BYTE* ImageR;

	int nImageSizeX_L;//��Ӱ�񳤿�;
	int nImageSizeY_L;
	int nImageSizeX_R;//��Ӱ�񳤿�
	int nImageSizeY_R;
	
	AfxMessageBox("����ʱ��ϳ������ĵȴ�������");

	BeginWaitCursor();

	ReadImage(ImageL,nImageSizeX_L,nImageSizeY_L,MH_inL);//��ȡ��Ӱ��
	ReadImage(ImageR,nImageSizeX_R,nImageSizeY_R,MH_inR);//��ȡ��Ӱ��

	//��ʼ��������Ӱ��������
	vector<FEATUREPOINT> FeaturePointLeft;
	vector<FEATUREPOINT> FeaturePointRight;
	Movarec(ImageL,FeaturePointLeft,nImageSizeX_L,nImageSizeY_L,m_winL1,m_winL2,m_threL);
	Movarec(ImageR,FeaturePointRight,nImageSizeX_R,nImageSizeY_R,m_winR1,m_winR2,m_threR);

	//�����������ϣ��洢��vector��
	//����vector, ����Ƭ����Ѱ���ϵ�����Ĵ���
	vector<FEATUREPOINT>::iterator iter1;
	vector<FEATUREPOINT>::iterator iter2;

	int k = m_scare/2;
	int i(0), j(0), m(0), n(0);
	//BYTE* pTemplate = new BYTE[m_scare*m_scare];	//�洢ģ�崰��
	//ZeroMemory(pTemplate, sizeof(BYTE)*m_scare*m_scare);

	//BYTE* pTarget = new BYTE[m_scare*m_scare];		//�洢��������
	//ZeroMemory(pTarget, sizeof(BYTE)*m_scare*m_scare);

	vector<MATCHEDPOINTS> MatchedPoints;//��ȡ�����������ϵ���ĵ�
	float MaxR(0);
	float R(0);
	double A(0), B(0), C(0), D(0), E(0);	//�������ϵ�����м���

	int lMaxWidth;
	int lMaxHeight;
	int Count3(0), Count4(0);

	for (iter1 = FeaturePointLeft.begin(); iter1 != FeaturePointLeft.end(); ++iter1)
	{
		//Count2++;
		//����õ��Ӧ��������

		B = 0.0;
		D = 0.0;

		MaxR = 0.0;
		R = 0.0;

		if (iter1->x < k || iter1->y < k || iter1->x > nImageSizeY_L - k || iter1->y > nImageSizeX_L - k)
		{
			Count3++;
			continue;
		}

		//�������B,D
		for (i = 0;i < m_scare;++i)
		{
			for(j = 0;j < m_scare;++j)
			{
				//B = С�����������������ػҶ�ֵ���
				//D = ..........................��ƽ�������
				B += ImageL[(i+iter1->y)*nImageSizeX_L + (j+iter1->x)];
				D += ImageL[(i+iter1->y)*nImageSizeX_L + (j+iter1->x)] * ImageL[(i+iter1->y)*nImageSizeX_L + (j+iter1->x)];
			}
		}
		Count4 = 0;

		//���С���ڼ������ϵ��R
		for (iter2 = FeaturePointRight.begin(); iter2 != FeaturePointRight.end(); ++iter2)
		{
			if (iter2->x < k || iter2->y < k || iter2->x > nImageSizeY_R - k || iter2->y > nImageSizeX_R - k)
			{
				Count4++;
				continue;
			}

			A = 0.0;
			C = 0.0;
			E = 0.0;

			j = iter2->x;	//i,jΪ���ĵ�
			i = iter2->y;

			j -= k;			//i,jΪ����
			i -= k;

			//�����centerX��centerY������Ƭĳ��С���ڵ�����
			int centerX = j + k;
			int centerY = i + k;

			//��Ӱ�������������Ӱ����������������ƥ��
			//��A,C,E
			for (m = 0; m < m_scare; ++m)
			{
				for(n = 0; n < m_scare ; ++n)
				{
					//C = ��Ӱ��С�����������ػҶ�ֵ���
					//E = ........................  ƽ�������
					//A = ��Ӧλ�����ػҶ�ֵ�˻�֮��
					C += ImageR[(i+m)*nImageSizeX_R + (j+n)];
					E += ImageR[(i+m)*nImageSizeX_R+(j+n)]*ImageR[(i+m)*nImageSizeX_R+(j+n)];
					A += ImageR[(i+m)*nImageSizeX_R+(j+n)] *ImageL[(m+iter1->y)*nImageSizeX_L + (n+iter1->x)];
				}
			}

			double temp1 = D - (B*B)/(m_scare*m_scare);//B = ��������Ϊ����С���������ػҶ�ֵ֮��
			double temp2 = E - (C*C)/(m_scare*m_scare);//D =.................................��ƽ�������

			if (temp1 != 0 && temp2 != 0)
			{
				R = (A - B*C/(m_scare*m_scare)) / sqrt(temp1*temp2);//�������ϵ��
			}
			else
				R = 0;


			//�ҳ��������ϵ��
			if (R > MaxR)
			{
				MaxR = R;
				lMaxWidth = centerX;
				lMaxHeight = centerY;
			}
		}

		int Distance=(iter1->x-lMaxWidth)*(iter1->x-lMaxWidth)+(iter1->y-lMaxHeight)*(iter1->y-lMaxHeight);
		//�����ϵ��������ֵ������������Ϊģ�崰�ڵı�ѡ�����
		if (MaxR >= m_cort&&Distance<=900)
		{
			MATCHEDPOINTS mp;
			mp.coefficient = MaxR;
			mp.x1 = iter1->x;
			mp.y1 = iter1->y;
			mp.x2 = lMaxWidth;
			mp.y2 = lMaxHeight;

			MatchedPoints.push_back(mp);
		}
	}

	TRY {
		DrawCross(ImageL,ImageR, MatchedPoints,nImageSizeX_L,nImageSizeX_R,nImageSizeY_L,nImageSizeY_R);
		SaveImage(ImageL,nImageSizeX_L,nImageSizeY_L,MH_outL,"BMP");
		SaveImage(ImageR,nImageSizeX_R,nImageSizeY_R,MH_outR,"BMP");
		SaveToTxt(MatchedPoints,m_cort,m_scare,MH_data);
		int IDType=AfxMessageBox("ƥ����ɣ����Ƿ�������ƥ����Ӱ���ƥ�����ĵ���",MB_YESNO|MB_ICONINFORMATION);
		//�򿪱�����
		if (IDType==IDYES)
		{
			ShellExecute(NULL,"open",MH_outL,NULL,NULL,SW_SHOWNORMAL);
			ShellExecute(NULL,"open",MH_outR,NULL,NULL,SW_SHOWNORMAL);
			ShellExecute(NULL,"open",MH_data,NULL,NULL,SW_SHOWNORMAL);
		}
		
	}
	CATCH (CMemoryException, e)
	{
		AfxMessageBox("�����ļ�ʧ�ܣ�");
		return;
	}
	END_CATCH

	EndWaitCursor();

	delete[] ImageL;
	delete[] ImageR;
	CDialogEx::OnOK();
}

//Ӱ��ʮ��
void CImageMatch::DrawCross(BYTE* pBits1, BYTE* pBits2, vector<MATCHEDPOINTS> MatchedPoints,int _width1, int _width2, int _height1, int _height2)
{
	vector<MATCHEDPOINTS>::iterator iter;

	int i(0), j(0);

	//for (i = 0; i < byteCount1; ++i)
	//{
	//	if (pBits1[i] == 255)
	//		pBits1[i] = 254;
	//}

	//for (j = 0; j < byteCount2; ++j)
	//{
	//	if (pBits2[j] == 255)
	//		pBits2[j] = 254;
	//}

	for (iter = MatchedPoints.begin(); iter != MatchedPoints.end(); ++iter)
	{
		i = iter->y1;
		j = iter->x1;

		//һ����
		pBits1[i*_width1+j] = 255;

		pBits1[i*_width1+j+1] = 255;
		pBits1[i*_width1+j-1] = 255;
		pBits1[(i+1)*_width1+j] = 255;
		pBits1[(i-1)*_width1+j] = 255;

		pBits1[i*_width1+j+2] = 255;
		pBits1[i*_width1+j-2] = 255;
		pBits1[(i+2)*_width1+j] = 255;
		pBits1[(i-2)*_width1+j] = 255;
		//������
		pBits1[_width1*_height1+i*_width1+j] = 0;

		pBits1[_width1*_height1+i*_width1+j+1] = 0;
		pBits1[_width1*_height1+i*_width1+j-1] = 0;
		pBits1[_width1*_height1+(i+1)*_width1+j] = 0;
		pBits1[_width1*_height1+(i-1)*_width1+j] = 0;

		pBits1[_width1*_height1+i*_width1+j+2] = 0;
		pBits1[_width1*_height1+i*_width1+j-2] = 0;
		pBits1[_width1*_height1+(i+2)*_width1+j] = 0;
		pBits1[_width1*_height1+(i-2)*_width1+j] = 0;
		//������
		pBits1[_width1*_height1*2+i*_width1+j] = 0;

		pBits1[_width1*_height1*2+i*_width1+j+1] = 0;
		pBits1[_width1*_height1*2+i*_width1+j-1] = 0;
		pBits1[_width1*_height1*2+(i+1)*_width1+j] = 0;
		pBits1[_width1*_height1*2+(i-1)*_width1+j] = 0;

		pBits1[_width1*_height1*2+i*_width1+j+2] = 0;
		pBits1[_width1*_height1*2+i*_width1+j-2] = 0;
		pBits1[_width1*_height1*2+(i+2)*_width1+j] = 0;
		pBits1[_width1*_height1*2+(i-2)*_width1+j] = 0;

		i = iter->y2;
		j = iter->x2;

		//һ����
		pBits2[i*_width2+j] = 255;	

		pBits2[i*_width2+j+1] = 255;
		pBits2[i*_width2+j-1] = 255;
		pBits2[(i+1)*_width2+j] = 255;
		pBits2[(i-1)*_width2+j] = 255;

		pBits2[i*_width2+j+2] = 255;
		pBits2[i*_width2+j-2] = 255;
		pBits2[(i+2)*_width2+j] = 255;
		pBits2[(i-2)*_width2+j] = 255;
		//������
		pBits2[_width2*_height2+i*_width2+j] = 0;	

		pBits2[_width2*_height2+i*_width2+j+1] = 0;
		pBits2[_width2*_height2+i*_width2+j-1] = 0;
		pBits2[_width2*_height2+(i+1)*_width2+j] = 0;
		pBits2[_width2*_height2+(i-1)*_width2+j] = 0;

		pBits2[_width2*_height2+i*_width2+j+2] = 0;
		pBits2[_width2*_height2+i*_width2+j-2] = 0;
		pBits2[_width2*_height2+(i+2)*_width2+j] = 0;
		pBits2[_width2*_height2+(i-2)*_width2+j] = 0;
		//������
		pBits2[_width2*_height2*2+i*_width2+j] = 0;	

		pBits2[_width2*_height2*2+i*_width2+j+1] = 0;
		pBits2[_width2*_height2*2+i*_width2+j-1] = 0;
		pBits2[_width2*_height2*2+(i+1)*_width2+j] = 0;
		pBits2[_width2*_height2*2+(i-1)*_width2+j] = 0;

		pBits2[_width2*_height2*2+i*_width2+j+2] = 0;
		pBits2[_width2*_height2*2+i*_width2+j-2] = 0;
		pBits2[_width2*_height2*2+(i+2)*_width2+j] = 0;
		pBits2[_width2*_height2*2+(i-2)*_width2+j] = 0;
	}
}


BOOL CImageMatch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
