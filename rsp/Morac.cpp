// Morac.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "Morac.h"
#include "afxdialogex.h"
#include "ProgressBar.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b));

CProgressBar* pro;
vector<FEATUREPOINT> RemoveReplicative(vector<FEATUREPOINT> v,int &i)//ȥ���ظ�����Ȥ��
{
	vector<FEATUREPOINT> ret;

	vector<FEATUREPOINT>::iterator iter = v.begin();

	ret.clear();

	ret.push_back(*iter);

	i=0;

	BOOL bEXIST(FALSE);

	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		bEXIST = false;

		for (vector<FEATUREPOINT>::iterator iter2 = ret.begin(); iter2 != ret.end(); ++iter2)
		{
			int a = iter->x;
			int aa = iter2->x;
			int b = iter->y;
			int bb = iter2->y;

			if (a == aa && b == bb)
			{
				bEXIST = TRUE;	//����
				break;
			}
		}

		if (bEXIST == FALSE)
		{
			ret.push_back(*iter);
			++i;
		}
	}
	return ret;
}


// CMorac �Ի���

IMPLEMENT_DYNAMIC(CMorac, CDialogEx)

CMorac::CMorac(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMorac::IDD, pParent)
{

	m_thre = 1000.0;
	m_win1 = 7.0;
	m_win2 = 5.0;
}

CMorac::~CMorac()
{
}

void CMorac::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THRE, m_thre);
	DDX_Text(pDX, IDC_EDIT_WIN1, m_win1);
	DDX_Text(pDX, IDC_EDIT_WIN2, m_win2);
}


BEGIN_MESSAGE_MAP(CMorac, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MOIN, &CMorac::OnBnClickedButtonMoin)
	ON_BN_CLICKED(IDC_BUTTON_MOUT, &CMorac::OnBnClickedButtonMout)
	ON_BN_CLICKED(IDC_BUTTON_CODAT, &CMorac::OnBnClickedButtonCodat)
	ON_BN_CLICKED(IDOK, &CMorac::OnBnClickedOk)
END_MESSAGE_MAP()


// CMorac ��Ϣ�������


//����Ӱ��
void CMorac::OnBnClickedButtonMoin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="���������ȡmoravecң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		M_in=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INMOC,M_in);
}

//������Ӱ��
void CMorac::OnBnClickedButtonMout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.BMP)|*.BMP|JPG�ļ�(*.JPG)|*.JPG;*.JPEG|tiff�ļ�(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="��������Ӱ��";
	if (file.DoModal()==IDOK)
	{
		M_out=file.GetPathName();
		switch(file.m_ofn.nFilterIndex)
		{
		case 2:
			{
				if (M_out.Right(4)!=".BMP")
				{
					M_out=M_out+".BMP";
				}
				break;
			}
		case 4:
			{
				if (M_out.Right(5)!=".TIFF")
				{
					M_out=M_out+".TIFF";
				}
				break;
			}

		}

	}
	SetDlgItemText(IDC_STATIC_OUTMOC,M_out);
}

//������Ȥ��
void CMorac::OnBnClickedButtonCodat()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|�ı�(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="��������Ȥ���ļ�";
	if (file.DoModal()==IDOK)
	{
		/*if (M_data.Right(4)!=".txt")
		{
			M_data=file.GetPathName()+".txt";
		}
		else*/
		M_data=file.GetPathName();	
	}
	SetDlgItemText(IDC_STATIC_OUTDATA,M_data);
}


void CMorac::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (m_thre<1||m_win1<1||m_win2<1)
	{
		AfxMessageBox("�����������0�����������룡");
		return;
	}

	GDALDataset* poDataset;//GDAL���ݼ�
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(M_in,GA_ReadOnly);

	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
		return;
	}

	int nImageSizeX=poDataset->GetRasterXSize();
	int nImageSizeY=poDataset->GetRasterYSize();

	BYTE *InData1 = new BYTE[nImageSizeX*nImageSizeY*3];
	BYTE *OutData1=new BYTE[nImageSizeX*nImageSizeY*3];
	poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,3, NULL,0,0,0 );
	/*GDALRasterBand* Band1;
	Band1=poDataset->GetRasterBand(1);
	Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Byte,0,0);*/

	BeginWaitCursor();

	//���ý�����
	pro=new CProgressBar;
	pro->Create(IDD_DIALOG_JDT);
	pro->ShowWindow(SW_SHOW);
	//����Ī��ά��
	CaculMoc(InData1,OutData1,nImageSizeX,nImageSizeY,m_win1,m_win2,m_thre,M_out,M_data);

	delete pro;

	int IDType=AfxMessageBox("�Ƿ������鿴���Ӱ����������ĵ���",MB_YESNO|MB_ICONINFORMATION);
	if (IDType==IDYES)
	{
		ShellExecute(NULL,"open",M_out,NULL,NULL,SW_SHOWNORMAL);
		ShellExecute(NULL,"open",M_data,NULL,NULL,SW_SHOWNORMAL);
	}
	

	EndWaitCursor();

	delete[] InData1;
	delete[] OutData1;
	GDALClose(poDataset);

	CDialogEx::OnOK();
}


// ����Ī��ά��
//����˵��;indata:����Ӱ��outdata�����Ӱ��ImageSizeX������Ӱ��ĳ���ImageSizeY������Ӱ��Ŀ�win1����Ȥֵ���ڴ�С��win2�������㴰�ڴ�С��thre����Ȥ����ֵ��file�������������ļ�����·��
void CMorac::CaculMoc(BYTE* Indata1, BYTE* outdata,int ImageSizeX,int ImageSizeY,int win1,int win2,double thre,CString file,CString TxtFile)
{
	int k=win1/2;
	int Count1=0;
	int Count3=0;

	float* pInterest = new float[ImageSizeX*ImageSizeY];
	ZeroMemory(pInterest, sizeof(float)*ImageSizeX*ImageSizeY);//����ֵ����(��memset������಻��ֻ������win32����)

	if(pro != NULL)  
	{  
		pro->ReSetProcess();  
		pro->SetMessage("���ڼ������Ȥ�㡣��");
		pro->SetStepCount(ImageSizeY*ImageSizeX); //���ý��������ܲ���
	}  

	//����Ӱ���һ������
	for (int i=0;i<ImageSizeX*ImageSizeY*3;i++)
	{
		outdata[i]=Indata1[i];
	}

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

		if (pro!= NULL)
		{
			bool bIsCancel = pro->StepIt();  
			if(!bIsCancel)  
			{
				delete[] pInterest;
				delete[] Indata;
				delete[] TempD1;
				delete[] TempD2;
				delete[] TempD3;
				return; 
			}
		}
	}
	//delete[] TempD1;//�ͷ��ڴ�
	//delete[] TempD2;
	//delete[] TempD3;


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
				++Count1;
			}
		}
	}
	
	int Count2(0);
	int k2 = m_win2 / 2;

	if (pro!=NULL)
	{
		pro->SetMessage("����ɸѡ�����㡣��");
		pro->SetStepCount((ImageSizeY - k2)*(ImageSizeX - k2)); //���ý��������ܲ���
	}

	vector<FEATUREPOINT> FeaturePoint;
	FEATUREPOINT tempFP;
	ZeroMemory(&tempFP, sizeof(FEATUREPOINT));
	
	float max2(0);
	int tempX(0), tempY(0);
	bool b(false);

	for(int i = k2; i < ImageSizeY - k2; i += m_win2)
	{
		for(int j = k2; j < ImageSizeX - k2; j += m_win2)
		{
			if (pro!= NULL)
			{
				bool bIsCancel = pro->StepIt();  
				if(!bIsCancel)  
				{
					delete[] pInterest;
					delete[] Indata;
					delete[] TempD1;
					delete[] TempD2;
					delete[] TempD3;
					return; 
				}
			}

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

				++Count2;	//��δ�޳��ظ���
			}
		}
	}

	vector<FEATUREPOINT> FeaturePoint2;
	FeaturePoint2 = RemoveReplicative(FeaturePoint,Count3);//ȥ���ظ����Ƶ�

	//������λͼ

	if (pro!=NULL)
	{
		pro->SetMessage("���ڱ�����Ӱ�񡣡�");
		pro->SetStepCount((int)FeaturePoint2.size()); //���ý��������ܲ���
	}

	vector<FEATUREPOINT>::iterator iter;

	for (iter = FeaturePoint2.begin(); iter != FeaturePoint2.end(); ++iter)
	{
		if (pro!= NULL)
		{
			bool bIsCancel = pro->StepIt();  
			if(!bIsCancel)  
			{
				delete[] pInterest;
				delete[] Indata;
				delete[] TempD1;
				delete[] TempD2;
				delete[] TempD3;
				return; 
			}
		}

		int i = iter->y;
		int j = iter->x;

		//��һ����
		outdata[i*ImageSizeX+j] = 255;

		outdata[i*ImageSizeX+j+1] = 255;
		outdata[i*ImageSizeX+j-1] = 255;
		outdata[(i+1)*ImageSizeX+j] = 255;
		outdata[(i-1)*ImageSizeX+j] = 255;

		outdata[i*ImageSizeX+j+2] = 255;
		outdata[i*ImageSizeX+j-2] = 255;
		outdata[(i+2)*ImageSizeX+j] = 255;
		outdata[(i-2)*ImageSizeX+j] = 255;


		//�ڶ�����
		outdata[ImageSizeX*ImageSizeY+i*ImageSizeX+j] = 0;

		outdata[ImageSizeX*ImageSizeY+i*ImageSizeX+j+1] = 0;
		outdata[ImageSizeX*ImageSizeY+i*ImageSizeX+j-1] = 0;
		outdata[ImageSizeX*ImageSizeY+(i+1)*ImageSizeX+j] = 0;
		outdata[ImageSizeX*ImageSizeY+(i-1)*ImageSizeX+j] = 0;

		outdata[ImageSizeX*ImageSizeY+i*ImageSizeX+j+2] = 0;
		outdata[ImageSizeX*ImageSizeY+i*ImageSizeX+j-2] = 0;
		outdata[ImageSizeX*ImageSizeY+(i+2)*ImageSizeX+j] = 0;
		outdata[ImageSizeX*ImageSizeY+(i-2)*ImageSizeX+j] = 0;

		//��������
		outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeX+j] = 0;

		outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeX+j+1] = 0;
		outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeX+j-1] = 0;
		outdata[ImageSizeX*ImageSizeY*2+(i+1)*ImageSizeX+j] = 0;
		outdata[ImageSizeX*ImageSizeY*2+(i-1)*ImageSizeX+j] = 0;

		outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeX+j+2] = 0;
		outdata[ImageSizeX*ImageSizeY*2+i*ImageSizeX+j-2] = 0;
		outdata[ImageSizeX*ImageSizeY*2+(i+2)*ImageSizeX+j] = 0;
		outdata[ImageSizeX*ImageSizeY*2+(i-2)*ImageSizeX+j] = 0;

	}
	SaveImage(outdata,ImageSizeX,ImageSizeY,M_out,"BMP");//����Ӱ��
	SaveToText(TxtFile,Count1,FeaturePoint2);//���������


	//�ͷ��ڴ�
	delete[] pInterest;
	delete[] Indata;
	delete[] TempD1;
	delete[] TempD2;
	delete[] TempD3;
}

// ������Ӱ��
void CMorac::SaveImage(BYTE* OutData, int ImageSizeX,int ImageSizeY,CString saveFile, CString ImageType)
{
//	GDALDataset* pD=NULL;//���ݼ����
//	GDALDatasetH  pDriver;//���ݼ�����
//	GDALRasterBandH hBand;
//
//	double adfGeoTransform[6]={444720,30,0,3751320,0,-30};
////	double* adfGeoTransform=NULL;
//	char* pszSRS_WKT=NULL;
////	poDataset->GetGeoTransform(adfGeoTransform);
////	const char* pszSRS_WKT=poDataset->GetProjectionRef();
//
//
//	pDriver=GetGDALDriverManager()->GetDriverByName(ImageType);
//	if (pDriver==NULL)
//	{
//		AfxMessageBox("�޷���������ָ�ʽ��");
//		return;
//	}
//	
//	TRY {
//
//		//��������
//		pD=(GDALDataset*)GDALCreate(pDriver,saveFile,ImageSizeX,ImageSizeY,3,GDT_Byte,NULL);
//		//��ӵ�����Ϣ
//		pD->SetGeoTransform(adfGeoTransform);
//		//���ͶӰ��Ϣ
//		pD->SetProjection(pszSRS_WKT);
//
//		hBand=GDALGetRasterBand(pD,1);
//
//		GDALRasterIO(hBand,GF_Write,0,0,ImageSizeX,ImageSizeY,OutData,ImageSizeX,ImageSizeY,GDT_Byte,0,0);
//	}
//	CATCH (CMemoryException, e)
//	{
//		AfxMessageBox("Ӱ��洢ʧ�ܣ�");
//		return;
//	}
//	END_CATCH
//
//	AfxMessageBox("Ӱ�񱣴�ɹ���");
//
//	GDALClose(pD);
//	GDALClose(pDriver);
	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(ImageType);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)M_out,//���·��
		ImageSizeX,ImageSizeY, //Ӱ�����ݿ��
		3, //���ȡ�Ĳ�����
		GDT_Byte, 	//��������������
		papszOptions );

	if(poDstDS->RasterIO(
		GF_Write,						//IO��ʽ
		0,0,							//Ӱ��������ʼλ��x,y
		ImageSizeX,ImageSizeY,				//Ӱ�����ݿ��
		OutData,						//ָ�򻺳�������ָ��
		ImageSizeX,ImageSizeY,				//���������ݿ��
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
	else
	{
		AfxMessageBox("��ɣ�");
	}

	GDALClose(poDstDS);

}


// //��������������
void CMorac::SaveToText(CString file,int Count1,vector<FEATUREPOINT> FeaturePoint2)
{
	FILE*  pTxtFile= fopen(file, "wt");

	if (!pTxtFile)
		return ;

	fprintf(pTxtFile, "**********\t�ĵ�˵����%s\n**********\t���������ˣ�%s\n**********\t��λ: %s\n"," Moravec����������ĵ�","������", "�й����ʴ�ѧ");
	fprintf(pTxtFile, "**********\t��ֵ: %d\n", (int)m_thre);
	fprintf(pTxtFile, "**********\t��Ȥ�㴰�ڴ�С: %d��%d\n**********\t�����㴰�ڴ�С: %d��%d\n", (int)m_win1, (int)m_win1, (int)m_win2, (int)m_win2);
	fprintf(pTxtFile, "**********\t��Ȥ����: %d\n", Count1);
	fprintf(pTxtFile, "**********\t��������: %d\n\n", FeaturePoint2.size());
	fprintf(pTxtFile, "����X\t����Y\t��Ȥֵ\n");

	TRY {
		for (vector<FEATUREPOINT>::iterator iter = FeaturePoint2.begin(); iter != FeaturePoint2.end(); ++ iter)
			fprintf(pTxtFile, "%d\t%d\t%.1f\n", iter->x, iter->y, iter->IV);
		AfxMessageBox("������洢��ɣ�");
	}
	CATCH (CMemoryException, e)
	{
		AfxMessageBox("�洢ʧ�ܣ�");
	}
	END_CATCH
	
	fclose(pTxtFile);
}



BOOL CMorac::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
