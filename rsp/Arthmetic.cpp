// Arthmetic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "Arthmetic.h"
#include "afxdialogex.h"
#include <queue>
#include "FileList.h"
#include "ProgressBar.h"
#include "CustomInputDialog.h"
#include "gdalwarper.h"
#include "Convolution.h"
#include "TerrainAnalysis.h"

#define  PI 3.141592653
extern CFileList* cfiles;
extern CProgressBar* pro1;//������

CString BiaoZhi=" ";
CString filename=" ";
double moban[9];
int fenge=0;
bool isOpened=false;
//��¼�ü�����Χ
CPoint BeginSub;
CPoint EndSub;

struct Modal//ģʽ
{
	int x;
	int y;
	int mark;
};

// CArthmetic �Ի���

IMPLEMENT_DYNAMIC(CArthmetic, CDialogEx)

CArthmetic::CArthmetic(CWnd* pParent /*=NULL*/)
	: CDialogEx(CArthmetic::IDD, pParent)
	, file_in(_T(""))
	, file_out(_T(""))
{
	flag=false;
}

CArthmetic::~CArthmetic()
{
}

void CArthmetic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeFile);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}

BEGIN_MESSAGE_MAP(CArthmetic, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CArthmetic::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CArthmetic::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CArthmetic::OnBnClickedOk)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CArthmetic::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDCANCEL, &CArthmetic::OnBnClickedCancel)
END_MESSAGE_MAP()


// λ�ƺ������෨
void CArthmetic::WeiShiClass(double a[][2],double b[][2],int a1,int b1,double Afa=0)//aΪw1�࣬bΪw2��;a1,b1Ϊģʽ����
{
	//double k[100];//����λ�ƺ���
	//int m=0;//�д����
	//k[0]=0;//��ʼ������λ�ƺ���
	//bool q1=true;
	//k[1]=1/(1+Afa*(()))

	//for (int j=2;j<a1;j++)
	//{
	//	if (q1)
	//	{
	//		k[j]=(1+4*a[j-1][0]*a[0][0]+4*a[j-1][1]*a[0][1]+16*a[j-1][0]*a[j-1][1]*a[0][0]*a[0][1]);
	//		continue;
	//	}
	//	else
	//	{
	//		q1=false;
	//		k[j+1]=(1+4*a[j-1][0]*a[0][0]+4*a[j-1][1]*a[0][1]+16*a[j-1][0]*a[j-1][1]*a[0][0]*a[0][1])+(1+4*a[j-1][0]*a[q1-1][0]+4*a[j-1][1]*a[q1-1][1]+16*a[j-1][0]*a[j-1][1]*a[q1-1][0]*a[q1-1][1]);
	//	}
	//}

	//�����ṹ
	int n=6,m=30,d=3;
	struct sample {
		int x[3];
		int cl;
	};
	//�ƺ�����ʶ�������ṹ
	struct func {
		int symbol;
		int index;
	};

	struct shi{
		int sub[3];
	};
	struct shi lei[2];
	struct func ftbl[30];
	int k,r=-1,tag=1,i,j;
	double g=0,temp;
	struct sample s[10];//={{1,2,5,1},{1,1,2,1},{3,3,6,1},{5,6,10,2},{7,6,11,2},{8,7,12,2}};
	CString FILE_NAME = file_in;

	if (FILE_NAME.Find("txt")==-1)
	{
		AfxMessageBox("ֻ֧��txt��ʽ���ݣ�");
		return;
	}

	CFile file(FILE_NAME,CFile::modeRead);
	CArchive ar(&file, CArchive::load) ;

	int count=0;
	FILE *fp;
	int ch,ch2;
	int curr;

	//�������ļ�
	if((fp=fopen(FILE_NAME,"rb"))==NULL)
	{
		AfxMessageBox("�޷����ļ���");
		return;
	}
	//��ӡ����
	i=0;
	while(i<n)
	{
		for(int iii=0;iii<4;++iii)
		{
			ar>>ch>>ch2;
			//��λ��ʮλ
			if(ch2==' ') 
				curr=ch;
			else 
				curr=(ch)*10+ch2;

			if(iii==3) 
				s[i].cl=curr;//���
			else 
				s[i].x[iii]=curr;//x1,x2,x3
		}
		++i;
		++count;
	}
	file.Close();
	//����λ�ƺ���
	while(tag==1)
	{	
		tag=0;
		for(k=0;k<n;k++)
		{
			if(r==-1)
			{
				r++;
				ftbl[r].symbol=1;//��ʾ
				ftbl[r].index=0;//������
				continue;
			}
			else
			{
				g=0;
				for(i=0;i<=r;i++)
				{
					temp=0;
					for(j=0;j<d;j++)
						temp+=(s[k].x[j]-s[ftbl[i].index].x[j])*(s[k].x[j]-s[ftbl[i].index].x[j]);
					g+=ftbl[i].symbol*exp(-temp);//����λ�ƺ���
				}
				if((g>0 && s[k].cl==1)||(g<0&&s[k].cl==2))continue;
				else
				{
					r++;
					ftbl[r].index=k;//��¼��ǰ�����ݵ��±�
					tag=1;
					if(g>0 && s[k].cl==2)//�����������λ�ƺ���ֵ����������ж�
						ftbl[r].symbol=-1;//�������
					else if(g<0&&s[k].cl==1)
						ftbl[r].symbol=1;//������ȷ
				}
			}
		}
	}
	for(i=0;i<=r;i++)
	{

		if(s[ftbl[i].index].x[0]>0)
		{
			lei[i].sub[0]=s[ftbl[i].index].x[0];
		}
		else if(s[ftbl[i].index].x[0]<0)
		{
			lei[i].sub[0]=s[ftbl[i].index].x[0];
		}

		if(s[ftbl[i].index].x[1]>0)
		{
			lei[i].sub[1]=s[ftbl[i].index].x[1];
		}
		else if(s[ftbl[i].index].x[1]<0)
		{
			lei[i].sub[1]=s[ftbl[i].index].x[1];
		}
		if(s[ftbl[i].index].x[2]>0)
		{
			lei[i].sub[2]=s[ftbl[i].index].x[2];
		}
		else if(s[ftbl[i].index].x[2]<0)
		{
			lei[i].sub[2]=s[ftbl[i].index].x[2];
		}
	}
}
// ��֪���㷨
void GanZhiQi(Modal* a,int num,int* result)
{
	int w[100][2];
	w[0][0]=1;//��ʼ������Ȩʸ��
	w[0][1]=1;
	int jishu=0;

loop:	for (int i=0;i<num;i++)
		{
			int result=w[i][0]*a[i].x+w[i][1]*a[i].y+w[i][2]*a[i].mark;//�����б���ֵ
			if (a[i].mark==1&&result<=0)//����w1��
			{
				w[jishu][0]=w[jishu][0]+a[i].x;
				w[jishu][1]=w[jishu][1]+a[i].y;
				jishu++;
			}
			else if (a[i].mark==0&&result>=0)//����w2��
			{
				w[jishu][0]=w[jishu][0]-a[i].x;
				w[jishu][1]=w[jishu][1]-a[i].y;
				jishu++;
			}
			else
			{
				jishu++;
				continue;
			}
		}

		for (int n=num;n>0;n--)
		{
			if ((w[jishu-n][0]!=w[jishu-n+1][0])||(w[jishu-n][1]!=w[jishu-n+1][1]))
			{
				goto loop;
			}
		}
		//���ؼ�����
		result[0]=w[jishu][0];
		result[1]=w[jishu][1];
}

void CArthmetic::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="����Ӱ��";
	if (file.DoModal()==IDOK)
	{
		file_in=file.GetPathName();
	}
	else
	{
		return;
	}
	BOOL isADD=FALSE;
	for (int i=0;i<cfiles->pathname.size();i++)
	{
		if (cfiles->pathname.at(i)==file_in)
		{
			isADD=TRUE;
		}
	}
	if (isADD==FALSE)
	{
		cfiles->pathname.push_back(file_in);
		ShowImageInfo();
	}

	SetDlgItemText(IDC_STATIC_AR1,file_in);
}

void CArthmetic::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(flag, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="����Ӱ��";
	if (file.DoModal()==IDOK)
	{
		file_out=file.GetPathName();
	}
	else
	{
		return;
	}
	SetDlgItemText(IDC_STATIC_AR2,file_out);

}

void CArthmetic::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (BiaoZhi=="IHS")
	{
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		if (poDataset==NULL)
		{
			return;
		}
		OUTDATA=new double[nImageSizeX*nImageSizeY*3];
		RGBtoHIS(DATA,OUTDATA);
		SaveImage(OUTDATA,nImageSizeX,nImageSizeY,3,file_out,"HFA");
		delete[] DATA;
		delete[] OUTDATA;
		delete pro1;
	}
	if (BiaoZhi=="IHSF")
	{
		ReadImage(DATA1,nImageSizeX,nImageSizeY,file_in);
		if (poDataset==NULL)
		{
			return;
		}
		OUTDATA=new double[nImageSizeX*nImageSizeY*3];
		IHStoRGB(DATA1,OUTDATA);
		SaveImage(OUTDATA,nImageSizeX,nImageSizeY,3,file_out,"HFA");
		delete[] DATA1;
		delete[] OUTDATA;
		delete pro1;
	}
	if (BiaoZhi=="IHSMERGE")
	{

		CString output="";
		BYTE* DATA2;

		CCustomInputDialog DiaInput;
		DiaInput.DoModal();
		if (filename!="")
		{
			output=filename;
		}


		ReadImage(DATA2,nImageSizeX,nImageSizeY,file_out);//��ȡ������Ӱ��
		if (poDataset==NULL)
		{
			return;
		}
		ChongCaiYang(DATA,nImageSizeX,nImageSizeY,file_in);//��ȡ�����Ӱ��

		if (poDataset==NULL)
		{
			return;
		}

		OUTDATA=new double[nImageSizeX*nImageSizeY*3];
		RGBtoHIS(DATA,OUTDATA);//��ȡ�����IHS����

		float MaxHd=0,MinHd=10000000;
		float MaxIn=0,MinIn=1000000;
		float Median;
		for (int i=0;i<nImageSizeX*nImageSizeY;i++)
		{
			if (DATA2[i]>MaxHd)
			{
				MaxHd=DATA2[i];//��ȡ�Ҷ�Ӱ�����ֵ
			}
			else if (DATA2[i]<MinHd)
			{
				MinHd=DATA2[i];//��ȡ�Ҷ�Ӱ����Сֵ
			}

			if (OUTDATA[i]>MaxIn)
			{
				MaxIn=OUTDATA[i];//��ȡI�������ֵ
			}
			else if (OUTDATA[i]<MinIn)
			{
				MinIn=OUTDATA[i];//��ȡI������Сֵ
			}
		}

		for (int i=0;i<nImageSizeX*nImageSizeY;i++)
		{
			OUTDATA[i]=(double)((DATA2[i]-MinHd)*(MaxIn-MinIn)/(MaxHd-MinHd)+MinIn);//ȫɫ���δ���I����
		}
		double* outdata1=new double[nImageSizeX*nImageSizeY*3];
		IHStoRGB(OUTDATA,outdata1);
		SaveImage(outdata1,nImageSizeX,nImageSizeY,3,output,"HFA");

		delete pro1;
		delete[] outdata1;
		delete[] OUTDATA;
		delete[] DATA2;
		delete[] DATA;
	}
	if (BiaoZhi=="FFT")//��ɢ����Ҷ
	{
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		if (poDataset==NULL)
		{
			return;
		}
		for (int i=0;i<nImageSizeX*nImageSizeY;i++)
		{
			DATA[i]=(DATA[i]*0.299+DATA[nImageSizeX*nImageSizeY+i]*0.587+DATA[nImageSizeX*nImageSizeY*2+i]*0.144);
		}
		double** outdata=new double*[3];
		for (int i=0;i<3;i++)
		{
			outdata[i]=new double[nImageSizeX*nImageSizeY];
		}
		FFT(DATA,nImageSizeX,nImageSizeY,outdata);
		double * img = ValueToImg(outdata,3,nImageSizeX,nImageSizeY);//��άת����һά
		SaveImage(img,nImageSizeX,nImageSizeY,3,file_out,"HFA");
		cfiles->setMessage(file_out,"UTM","WGS-84","",3,nImageSizeX,nImageSizeY);

		delete pro1;
		delete[] outdata;
		delete[] DATA;
	}
	if (BiaoZhi=="FFTN")//��ɢ����Ҷ��任
	{
		ReadImage(DATA1,nImageSizeX,nImageSizeY,file_in);
		if (poDataset==NULL)
		{
			return;
		}
		OUTDATA1=new BYTE[nImageSizeX*nImageSizeY];
		FTTN(DATA1,nImageSizeX,nImageSizeY,OUTDATA1);
		SaveImage(OUTDATA1,nImageSizeX,nImageSizeY,1,file_out,"HFA");
		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;
		delete[] OUTDATA1;
		delete[] DATA1;
	}
	if (BiaoZhi=="QFFT")//������ٸ���Ҷ
	{
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		if (poDataset==NULL)
		{
			return;
		}
		for (int i=0;i<nImageSizeX*nImageSizeY;i++)
		{
			DATA[i]=(DATA[i]*0.299+DATA[nImageSizeX*nImageSizeY+i]*0.587+DATA[nImageSizeX*nImageSizeY*2+i]*0.144);
		}
		double** outdata=new double*[3];
		for (int i=0;i<3;i++)
		{
			outdata[i]=new double[nImageSizeX*nImageSizeY];
		}
		ImageFFt2D(DATA,nImageSizeX,nImageSizeY,outdata);
		double * img = ValueToImg(outdata,3,nImageSizeX,nImageSizeY);//��άת����һά
		SaveImage(img,nImageSizeX,nImageSizeY,3,file_out,"HFA");
		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",3,nImageSizeX,nImageSizeY);
		}

		delete pro1;
		delete[] outdata;
		delete[] DATA;
	}
	if (BiaoZhi=="QFFTN")//������ٸ���Ҷ��
	{
		double* data1;
		ReadImage(data1,nImageSizeX,nImageSizeY,file_in);
		if (poDataset==NULL)
		{
			return;
		}
		BYTE* outdata=new BYTE[nImageSizeX*nImageSizeY];
		ImageRevFFt2D(data1,nImageSizeX,nImageSizeY,outdata);
		SaveImage(outdata,nImageSizeX,nImageSizeY,1,file_out,"HFA");
		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}

		delete pro1;
		delete[] outdata;
		delete[] data1;
	}
	if (BiaoZhi=="KL")
	{
		int nBand=0;
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		if (poDataset==NULL)
		{
			return;
		}
		else if (poDataset->GetRasterCount()<2)
		{
			AfxMessageBox("����Ӱ��������Ҫ�������Σ�");
			return;
		}
		else
		{
			if (poDataset->GetRasterCount()==2)
			{
				nBand=2;
			}
			else
			{
				nBand=3;
			}
		}
		double **Xiefangcha = XieFangCha(DATA,nBand,nImageSizeX,nImageSizeY);
		double **TeZhengVector = new double *[nBand];
		for(int i = 0 ; i<nBand;i++)
		{
			TeZhengVector[i] = new double[nBand];//�洢��������
		}
		int nrot;
		double *d =new double[nBand];
		Eigs(Xiefangcha, nBand, d, TeZhengVector,  nrot);//������������
		double* Paixu = new double [nBand];
		for(int i = 0 ; i<nBand;i++)
		{
			Paixu[i] = Xiefangcha[i][i];//�洢����ֵ
		}
		//ð�����򷨴Ӵ�С����
		for(int j = nBand;j>0;j--)
			for(int i = 0 ; i<j-1;i++)
			{
				if(Paixu[i+1]>Paixu[i])
				{
					double temppai = Paixu[i];
					Paixu[i] = Paixu[i+1];
					Paixu[i+1] =temppai;
				}
			}
		int *shuzu =new int[nBand];
		for(int i = 0 ; i < nBand;i++)
			for(int j = 0 ; j<nBand;j++)
			{
				if(Xiefangcha[j][j] ==Paixu[i])
				{
					shuzu[i] = j;//��¼�±�
				}
			}
			
		delete []d;d=NULL;
		double **result = new double *[nBand];
		for(int i = 0 ; i<nBand;i++)
		{
			result[i] = new double [nImageSizeX*nImageSizeY];
		}

		for(int Bandi = 0 ; Bandi<nBand;Bandi++)
		{
			int temp = 0;
			for(int i =0 ; i<nImageSizeY;i++)
				for(int j = 0 ; j<nImageSizeX;j++)
				{
					result[Bandi][temp++] = DATA[Bandi*nImageSizeX*nImageSizeY+i*nImageSizeX+j];
				}
		}
		double **Results =matrix_mutiply(TeZhengVector, result, nBand, nBand, nBand,nImageSizeX*nImageSizeY);
		for(int i = 0 ; i<nBand;i++)
		{
			delete []TeZhengVector[i] ;TeZhengVector[i]=NULL;

			delete []result[i];result[i] =NULL;
		}
		delete []TeZhengVector;TeZhengVector=NULL;

		delete []result;result = 0 ;

		double *Min = new double[nBand];

		double *Max = new double[nBand];

		for(int Bandi = 0 ; Bandi<nBand;Bandi++)
		{
			Min[Bandi] =9999 ;

			Max[Bandi] = -9999 ;
			for(int i =0 ; i<nImageSizeX*nImageSizeY;i++)
			{
				Min[Bandi] = min(Min[Bandi],Results[Bandi][i]);//��ȡ������Сֵ
				Max[Bandi] = max(Max[Bandi],Results[Bandi][i]);//��ȡ�������ֵ

			}	
		}

		pro1->ReSetProcess();  
		pro1->SetStepCount(nBand*nImageSizeX*nImageSizeY); //���ý��������ܲ���
		pro1->SetMessage("���ڼ���PCA�任���..");

		for(int Bandi = 0 ; Bandi<nBand;Bandi++)
		{
			for(int i =0 ; i<nImageSizeY;i++)
				for(int j = 0 ; j<nImageSizeX;j++)
				{
					pro1->StepIt();
					DATA[Bandi*nImageSizeX*nImageSizeY+i*nImageSizeX+j] = 255*(Results[shuzu[Bandi]][i*nImageSizeX+j]-Min[Bandi])/(Max[Bandi]-Min[Bandi]);
				}
		}
		for(int i = 0 ; i<nBand;i++)
		{

			delete []Results[i];Results[i] =NULL;
		}
		delete []Results;Results=NULL;
		delete []Min;Min=NULL;delete []Max;Max=NULL;
		delete []shuzu;shuzu =NULL;
		SaveImage(DATA,nImageSizeX,nImageSizeY,nBand,file_out,"HFA");
		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",nBand,nImageSizeX,nImageSizeY);
		}
		delete[] DATA;
		delete pro1;
	}
	if (BiaoZhi=="ProTran")
	{
		ProTran(file_in,file_out);
		delete pro1;
	}
	if (BiaoZhi=="UnweightingMerge")
	{
		CCustomInputDialog DiaInput;
		DiaInput.DoModal();

		CString output="";
		if (filename!="")
		{
			output=filename;
		}
		else
		{
			return;
		}

		BYTE* DATA2;
		int nImageSizeX1,nImageSizeY1;
		ReadImage(DATA2,nImageSizeX1,nImageSizeY1,file_out);//��ȡȫɫ
		if (poDataset==NULL)
		{
			return;
		}
		GDALClose(poDataset);

		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);//��ȡ�����
		if (poDataset==NULL)
		{
			return;
		}
		if (nImageSizeX>nImageSizeX1)
		{
			nImageSizeX=nImageSizeX1;
		}
		if (nImageSizeY>nImageSizeY1)
		{
			nImageSizeY=nImageSizeY1;
		}

		Unweighting(DATA,DATA2,nImageSizeX,nImageSizeY);
		SaveImage(DATA,nImageSizeX,nImageSizeY,3,output,"HFA");
	
		delete pro1;
		delete[] DATA;
		delete[] DATA2;
	}
	if (BiaoZhi=="WeightingMerge")
	{
		CCustomInputDialog DiaInput;
		DiaInput.DoModal();

		CString output="";
		if (filename!="")
		{
			output=filename;
		}

		BYTE* DATA2;
		int nImageSizeX1,nImageSizeY1;
		ReadImage(DATA2,nImageSizeX1,nImageSizeY1,file_out);//��ȡȫɫ
		if (poDataset==NULL)
		{
			return;
		}
		GDALClose(poDataset);

		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);//��ȡ�����
		if (poDataset==NULL)
		{
			return;
		}
		if (nImageSizeX>nImageSizeX1)
		{
			nImageSizeX=nImageSizeX1;
		}
		if (nImageSizeY>nImageSizeY1)
		{
			nImageSizeY=nImageSizeY1;
		}

		Weighting(DATA,DATA2,nImageSizeX,nImageSizeY);
		SaveImage(DATA,nImageSizeX,nImageSizeY,3,output,"HFA");

		delete pro1;
		delete[] DATA2;
		delete[] DATA;
	}
	if (BiaoZhi=="GAOSI")
	{
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		if (poDataset==NULL)
		{
			return;
		}
		int nBand=poDataset->GetRasterCount();
		OUTDATA1=new BYTE[nImageSizeX*nImageSizeY*nBand];
		AddGaussNoise(DATA,nImageSizeX,nImageSizeY,nBand,OUTDATA1);
		SaveImage(OUTDATA1,nImageSizeX,nImageSizeY,nBand,file_out,"HFA");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",nBand,nImageSizeX,nImageSizeY);
		}

		delete pro1;
		delete[] DATA;
		delete[] OUTDATA1;
	}
	if (BiaoZhi=="SaltNoise")
	{
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		if (poDataset==NULL)
		{
			return;
		}
		int nBand=poDataset->GetRasterCount();
		OUTDATA1=new BYTE[nImageSizeX*nImageSizeY*nBand];
		AddPepperSaltNoise(DATA,nImageSizeX,nImageSizeY,nBand,OUTDATA1);
		SaveImage(OUTDATA1,nImageSizeX,nImageSizeY,nBand,file_out,"HFA");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",nBand,nImageSizeX,nImageSizeY);
		}

		delete pro1;
		delete[] DATA;
		delete[] OUTDATA1;
	}
	if (BiaoZhi=="Convolution")
	{
		CConvolution cov;
		if(cov.DoModal()==IDOK)
		{
			ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
			if (poDataset==NULL)
			{
				return;
			}
			Convolution(DATA,moban,nImageSizeX,nImageSizeY,poDataset->GetRasterCount());
			SaveImage(DATA,nImageSizeX,nImageSizeY,poDataset->GetRasterCount(),file_out,"HFA");
			if (cfiles->GetSafeHwnd()!=NULL)
			{
				cfiles->setMessage(file_out,"UTM","WGS-84","",poDataset->GetRasterCount(),nImageSizeX,nImageSizeY);
			}

			delete pro1;  
			delete[] DATA;
		}
	}
	if (BiaoZhi=="Robert")
	{
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		OUTDATA1=new BYTE[nImageSizeX*nImageSizeY*poDataset->GetRasterCount()];
		Robert(DATA,nImageSizeX,nImageSizeY,poDataset->GetRasterCount(),OUTDATA1);
		SaveImage(OUTDATA1,nImageSizeX,nImageSizeY,poDataset->GetRasterCount(),file_out,"HFA");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",poDataset->GetRasterCount(),nImageSizeX,nImageSizeY);
		}

		delete pro1;  
		delete[] DATA;
		delete[] OUTDATA1;
	}
	if (BiaoZhi=="Sobel")
	{
		CCustomInputDialog DiaInput;
		DiaInput.DoModal();
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		OUTDATA1=new BYTE[nImageSizeX*nImageSizeY*poDataset->GetRasterCount()];
		Sobel(DATA,OUTDATA1,nImageSizeX,nImageSizeY,poDataset->GetRasterCount(),fenge);
		SaveImage(OUTDATA1,nImageSizeX,nImageSizeY,poDataset->GetRasterCount(),file_out,"HFA");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",poDataset->GetRasterCount(),nImageSizeX,nImageSizeY);
		}

		delete pro1;  
		delete[] DATA;
		delete[] OUTDATA1;
	}
	if (BiaoZhi=="Prewitt")
	{
		CCustomInputDialog DiaInput;
		DiaInput.DoModal();
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		OUTDATA1=new BYTE[nImageSizeX*nImageSizeY*poDataset->GetRasterCount()];
		Prewitt(DATA,OUTDATA1,nImageSizeX,nImageSizeY,poDataset->GetRasterCount(),fenge);
		SaveImage(OUTDATA1,nImageSizeX,nImageSizeY,poDataset->GetRasterCount(),file_out,"HFA");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",poDataset->GetRasterCount(),nImageSizeX,nImageSizeY);
		}

		delete pro1;  
		delete[] DATA;
		delete[] OUTDATA1;

	}
	if (BiaoZhi=="Laplacian")
	{
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		Laplacian(DATA,nImageSizeX,nImageSizeY,poDataset->GetRasterCount());
		SaveImage(DATA,nImageSizeX,nImageSizeY,poDataset->GetRasterCount(),file_out,"HFA");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",poDataset->GetRasterCount(),nImageSizeX,nImageSizeY);
		}

		delete pro1;  
		delete[] DATA;	
	}
	if (BiaoZhi=="Edgetrace")
	{
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		OUTDATA1=new BYTE[nImageSizeX*nImageSizeY];
		OnEdgeTrace(DATA,nImageSizeX,nImageSizeY,OUTDATA1);
		SaveImage(OUTDATA1,nImageSizeX,nImageSizeY,poDataset->GetRasterCount(),file_out,"HFA");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",poDataset->GetRasterCount(),nImageSizeX,nImageSizeY);
		}

		delete pro1;  
		delete[] DATA;
		delete[] OUTDATA1;
	}

	if (BiaoZhi=="RegionGrow")
	{
		CCustomInputDialog DiaInput;
		DiaInput.DoModal();

		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		CPoint Pos(nImageSizeX/2,nImageSizeY/2);
		//��ֵ
		int nThresh=fenge;

		OUTDATA1=new BYTE[nImageSizeX*nImageSizeY];
		RegionGrow(DATA,nImageSizeX,nImageSizeY,poDataset->GetRasterCount(),OUTDATA1,Pos,nThresh);
		SaveImage(OUTDATA1,nImageSizeX,nImageSizeY,1,file_out,"HFA");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}

		delete pro1;  
		delete[] DATA;
		delete[] OUTDATA1;
	}
	if (BiaoZhi=="OnSubsetimg")
	{
		if (cfiles->pathname.capacity()==0)
		{
			AfxMessageBox("���ȴ���ͼѡȡ�ü���..");
			return;
		}
		file_in=cfiles->pathname.at(0);
//		SetDlgItemText(IDC_STATIC_AR1,file_in);
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		OUTDATA1=new BYTE[nImageSizeX*nImageSizeY*poDataset->GetRasterCount()];
		ImgSubset(BeginSub,EndSub,DATA,OUTDATA1,file_out,"HFA");
		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",3,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
		delete[] DATA;
		delete[] OUTDATA1;

	}

	//�����¶�
	if (BiaoZhi=="Slope")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",1);
	
		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
		Slope.processRaster(pro1,"���ڼ����¶ȡ���");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
	}
	//��������
	if (BiaoZhi=="Aspect")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",2);

		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
		Slope.processRaster(pro1,"���ڼ������򡣡�");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
	}
	//��������ָ����TPI��
	if (BiaoZhi=="TPI")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",3);

		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
		Slope.processRaster(pro1,"���ڼ����������ָ������");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
	}
	//ɽ����Ӱ
	if (BiaoZhi=="HillShade")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",4);

		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);

		Slope.setLightAzimuth(300);
		Slope.setLightAngle(40);
		Slope.processRaster(pro1,"���ڼ���ɽ����Ӱ����");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
	}
	//����λ��ָ��
	if (BiaoZhi=="TRI")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",5);

		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
		Slope.processRaster(pro1,"���ڼ���ֲڶȡ���");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
	}
	//�ֲڶ�
	if (BiaoZhi=="Roughness")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",6);

		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
		Slope.processRaster(pro1,"���ڼ���ֲڶȡ���");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
	}
	if (BiaoZhi=="Watershed")
	{
		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);
		BYTE** OrgImg = OneToTwo(DATA,nImageSizeX,nImageSizeY);

		BYTE** LableImg = new BYTE*[nImageSizeX];
		for (int i=0;i<nImageSizeX;i++)
		{
			LableImg[i] = new BYTE[nImageSizeY];
		}

		char** SeedImg = new char*[nImageSizeX];
		for (int i=0;i<nImageSizeX;i++)
		{
			SeedImg[i] = new char[nImageSizeY];
		}
		for (int i=0;i<nImageSizeY;i++)
		{
			for (int j=0;j<nImageSizeX;j++)
			{
				SeedImg[i][j] = 1;
			}
		}

		Watershed(OrgImg,SeedImg,LableImg,nImageSizeX,nImageSizeY);
		BYTE* OutImg = TwoToOne(LableImg,nImageSizeX,nImageSizeY);
		SaveImage(OutImg,nImageSizeX,nImageSizeY,poDataset->GetRasterCount(),file_out,"HFA");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",poDataset->GetRasterCount(),nImageSizeX,nImageSizeY);
		}

		delete pro1;  
		delete[] DATA;	
		delete[] OutImg;
		for(int i=0;i<nImageSizeX;i++)
		{
			delete [nImageSizeY]OrgImg[i];
			OrgImg[i]=NULL;
			delete [nImageSizeY]LableImg[i];
			LableImg[i]=NULL;
		}
		delete [nImageSizeX]OrgImg;
		delete [nImageSizeX]LableImg;
	}
	if (BiaoZhi==" ")
	{
		return;
	}
	CDialogEx::OnOK();
}

BOOL CArthmetic::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD dwStyles=GetWindowLong(m_TreeFile.m_hWnd,GWL_STYLE);//��ȡ������ԭ���
	dwStyles|=TVS_EDITLABELS|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT;
	SetWindowLong(m_TreeFile.m_hWnd,GWL_STYLE,dwStyles);//���÷��
	ShowImageInfo();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (BiaoZhi=="ProTran")
	{
		CButton* btn=(CButton*)GetDlgItem(IDC_BUTTON1);
		btn->SetWindowText("�ο�Ӱ��");
		btn=(CButton*)GetDlgItem(IDC_BUTTON2);
		btn->SetWindowText("ԭʼӰ��");
		flag=true;
	}
	else if (BiaoZhi=="IHSMERGE"||BiaoZhi=="UnweightingMerge"||BiaoZhi=="WeightingMerge")
	{
		CButton* btn=(CButton*)GetDlgItem(IDC_BUTTON1);
		btn->SetWindowText("�����Ӱ��");
		btn=(CButton*)GetDlgItem(IDC_BUTTON2);
		btn->SetWindowText("ȫɫӰ��");
		flag=true;
	}
	else if (BiaoZhi=="OnSubsetimg")
	{
		file_in=cfiles->pathname.at(0);
		SetDlgItemText(IDC_STATIC_AR1,file_in);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
//��ʾӰ����Ϣ
void CArthmetic::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GDALDataset* poDataset1;//GDAL���ݼ�
	GDALRasterBand* band;
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	HTREEITEM hSel=m_TreeFile.GetSelectedItem();//ȡ��ѡ������
	if (hSel==NULL)
	{
		return;
	}
	CString name=m_TreeFile.GetItemText(hSel);;
	if (name.Find(":")<0)
	{
		return;
	}
	poDataset1=(GDALDataset*)GDALOpen(name,GA_ReadOnly);
	if(poDataset1==NULL)
	{
		AfxMessageBox("��ʼ�������⣡");
		return ;
	}

	poDataset=poDataset1;
	isOpened=true;
	SetDlgItemText(IDC_STATIC_AR1,name);
	file_in=name;

	band=poDataset1->GetRasterBand(1);
	m_edit.SetWindowText(" ");

	int m=1,b=1,x=0,y=0;
	double *c,*d,*e,*f,adfGeoTransform[6];
	c=new double;
	d=new double;
	e=new double;
	f=new double;

	band->GetStatistics(m,b,c,d,e,f);
	x=poDataset1->GetRasterXSize();
	y=poDataset1->GetRasterYSize();

	CString proj=poDataset1->GetProjectionRef();
	CString projection;
	int a1=proj.Find("PROJCS[");
	int a2;
	if (!proj.IsEmpty())
	{
		if (a1>=0)
		{
			a2=proj.Find(",",a1);
			projection=proj.Mid(a1+7,a2-(a1+7));
		}
		else
		{
			projection="geographic Lat/Lot";
		}
	}

	CString a;
	CRect rect;
	m_edit.GetClientRect(rect);
	int len=rect.Width();
	a.Format("�ļ�·����%s\r\n",name);
	m_edit.SetSel(0,len);
	m_edit.ReplaceSel(a);
	a.Format("�ļ���С��%d*%d\r\n\n",x,y);
	m_edit.SetSel(len,2*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	a.Format("��Ԫ��Сֵ��%f\r\n\n",*c);
	m_edit.SetSel(2*len,3*len);
	m_edit.ReplaceSel(a);
	a.Format("��Ԫ���ֵ��%f\r\n\n",*d);
	m_edit.SetSel(3*len,4*len);
	m_edit.ReplaceSel(a);

	if (poDataset1->GetGeoTransform(adfGeoTransform)==CE_None)
	{
		a.Format("��Ԫ�ֱ��ʣ�%fm*%fm\r\n\n",abs(adfGeoTransform[1]),abs(adfGeoTransform[5]));
		m_edit.SetSel(4*len,5*len);
		m_edit.ReplaceSel(a);
		m_edit.ReplaceSel("\n");
	}

	a.Format("���ξ�ֵ��%f\r\n\n",*e);
	m_edit.SetSel(5*len,6*len);
	m_edit.ReplaceSel(a);
	a.Format("���η��%f\r\n\n",*f);
	m_edit.SetSel(6*len,7*len);
	m_edit.ReplaceSel(a);
	m_edit.SetSel(7*len,8*len);
	m_edit.ReplaceSel("\n");

	a.Format("ͶӰ��Ϣ��%s\r\n\n",projection);
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	a=GDALGetDataTypeName(band->GetRasterDataType());
	a="�洢��ʽ��"+a;
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	a=GDALGetColorInterpretationName(band->GetColorInterpretation());
	a="��������:"+a;
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel("\n");

	if (poDataset1->GetGeoTransform(adfGeoTransform)==CE_None)
	{
		a.Format("���Ͻǵ����꣺x=%f,y=%f",adfGeoTransform[0],adfGeoTransform[3]);
		m_edit.SetSel(8*len,9*len);
		m_edit.ReplaceSel(a);
		m_edit.ReplaceSel("\n");
		m_edit.ReplaceSel("\n");
	}

//	GDALClose(poDataset1);

	*pResult = 0;
}

void CArthmetic::ReadImage(double* &Data,int &nImageSizeX,int &nImageSizeY,CString FilePath)
{
	
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(FilePath,GA_ReadOnly);

	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
		return;
	}
	GDALRasterBand* band=poDataset->GetRasterBand(1);

	pro1=new CProgressBar;
	pro1->Create(IDD_DIALOG_JDT);
	pro1->ShowWindow(SW_SHOW);
	if(pro1 != NULL)  
	{  
		pro1->ReSetProcess();  
		pro1->SetMessage("���ڶ�ȡӰ�񡣡�");
	}

	nImageSizeX=poDataset->GetRasterXSize();
	nImageSizeY=poDataset->GetRasterYSize();

	GDALDataType a=band->GetRasterDataType();
	if (poDataset->GetRasterCount()==1)
	{
		Data = new double[nImageSizeX*nImageSizeY];
		poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, Data,nImageSizeX,nImageSizeY,a,1, NULL,0,0,0 );
	}
	else
	{
		Data = new double[nImageSizeX*nImageSizeY*3];
		poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, Data,nImageSizeX,nImageSizeY,a,3, NULL,0,0,0 );
	}
}

void CArthmetic::ReadImage(BYTE* &Data,int &nImageSizeX,int &nImageSizeY,CString FilePath)
{

	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	if (isOpened==false)
	{
		poDataset=(GDALDataset*)GDALOpen(FilePath,GA_ReadOnly);
	}
	
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
		return;
	}
	GDALRasterBand* band=poDataset->GetRasterBand(1);


	pro1=new CProgressBar;
	pro1->Create(IDD_DIALOG_JDT);
	pro1->ShowWindow(SW_SHOW);

	pro1->ReSetProcess();  
	pro1->SetMessage("���ڶ�ȡӰ�񡣡�");

	nImageSizeX=poDataset->GetRasterXSize();
	nImageSizeY=poDataset->GetRasterYSize();
	GDALDataType a=band->GetRasterDataType();

	if (poDataset->GetRasterCount()==1)
	{
		Data = new BYTE[nImageSizeX*nImageSizeY];
		poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, Data,nImageSizeX,nImageSizeY,a,1, NULL,0,0,0 );
	}
	else
	{
		Data = new BYTE[nImageSizeX*nImageSizeY*3];
		poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, Data,nImageSizeX,nImageSizeY,a,3, NULL,0,0,0 );
	}
}

void CArthmetic::SaveImage(BYTE* Outdata,int nImageSizeX,int nImageSizeY,int bandcount,CString OutPath, CString ImageType)
{
	double adfGeoTransform[6];
	poDataset->GetGeoTransform(adfGeoTransform);
	const char* pszSRS_WKT=poDataset->GetProjectionRef();

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(ImageType);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)OutPath,//���·��
		nImageSizeX,nImageSizeY, //Ӱ�����ݿ��
		bandcount, //���ȡ�Ĳ�����
		GDT_Byte, 	//��������������
		papszOptions );

	poDstDS->SetGeoTransform(adfGeoTransform);
	poDstDS->SetProjection(pszSRS_WKT);

	if(poDstDS->RasterIO(
		GF_Write,						//IO��ʽ
		0,0,							//Ӱ��������ʼλ��x,y
		nImageSizeX,nImageSizeY,				//Ӱ�����ݿ��
		Outdata,						//ָ�򻺳�������ָ��
		nImageSizeX,nImageSizeY,				//���������ݿ��
		GDT_Byte,						//��������������
		bandcount,						//���ȡ�Ĳ�����
		NULL,							//���ȡ�Ĳ����б�
		0,								//����������һ������ռ�õ��ֽ���
		0,								//������һ��������ռ�õ��ֽ���
		0)==CE_Failure )				//���������ݴ洢��ʽ 0ΪRRRRGGGGBBBB-bsp 1ΪRGBRGBRGB-bip	
	{
		AfxMessageBox("�����ļ�ʧ�ܣ�");
		return;
	}
	pro1->SetMessage("���ڱ���Ӱ�񡣡�");

	GDALClose(poDataset);
	GDALClose(poDstDS);
}

void CArthmetic::SaveImage(double* Outdata,int nImageSizeX,int nImageSizeY,int bandcount,CString OutPath, CString ImageType)
{
	double adfGeoTransform[6];
	poDataset->GetGeoTransform(adfGeoTransform);
	const char* pszSRS_WKT=poDataset->GetProjectionRef();

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(ImageType);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)OutPath,//���·��
		nImageSizeX,nImageSizeY, //Ӱ�����ݿ��
		bandcount, //���ȡ�Ĳ�����
		GDT_Float64, 	//��������������
		papszOptions );

	poDstDS->SetGeoTransform(adfGeoTransform);
	poDstDS->SetProjection(pszSRS_WKT);

	if(poDstDS->RasterIO(
		GF_Write,						//IO��ʽ
		0,0,							//Ӱ��������ʼλ��x,y
		nImageSizeX,nImageSizeY,				//Ӱ�����ݿ��
		Outdata,						//ָ�򻺳�������ָ��
		nImageSizeX,nImageSizeY,				//���������ݿ��
		GDT_Float64,						//��������������
		bandcount,						//���ȡ�Ĳ�����
		NULL,							//���ȡ�Ĳ����б�
		0,								//����������һ������ռ�õ��ֽ���
		0,								//������һ��������ռ�õ��ֽ���
		0)==CE_Failure )				//���������ݴ洢��ʽ 0ΪRRRRGGGGBBBB-bsp 1ΪRGBRGBRGB-bip	
	{
		AfxMessageBox("�����ļ�ʧ�ܣ�");
		return;
	}
	pro1->SetMessage("���ڱ���Ӱ�񡣡�");

	GDALClose(poDataset);
	GDALClose(poDstDS);
}

void CArthmetic::RGBtoHIS(BYTE* DATA,double* OUTDATA)
{
		
	pro1->ReSetProcess();  
	pro1->SetStepCount(nImageSizeX*nImageSizeY); //���ý��������ܲ���
	pro1->SetMessage("���ڼ�������ȴ�����");

	for (int i=0;i<nImageSizeX*nImageSizeY;i++)
	{
		//OUTDATA[i]=(DATA[i]+DATA[nImageSizeX*nImageSizeY+i]+DATA[nImageSizeX*nImageSizeY*2+i])/3;//I����
		//if ((DATA[i]>DATA[nImageSizeX*nImageSizeY*2+i]&&DATA[nImageSizeX*nImageSizeY*2+i]<=DATA[nImageSizeX*nImageSizeY+i]))
		//{
		//	OUTDATA[nImageSizeX*nImageSizeY+i]=(DATA[nImageSizeX*nImageSizeY+i]-DATA[nImageSizeX*nImageSizeY*2+i])/(3*(OUTDATA[i]-DATA[nImageSizeX*nImageSizeY*2+i]));//H����;
		//	OUTDATA[nImageSizeX*nImageSizeY*2+i]=(1-3*DATA[nImageSizeX*nImageSizeY*2+i])/(OUTDATA[i]*3);//s=(1-b)/i
		//}
		//else if ((DATA[nImageSizeX*nImageSizeY+i]>DATA[i])&&(DATA[i]<=DATA[nImageSizeX*nImageSizeY*2+i]))
		//{
		//	OUTDATA[nImageSizeX*nImageSizeY+i]=(DATA[nImageSizeX*nImageSizeY*2+i]-DATA[i])/(3*(OUTDATA[i]-DATA[nImageSizeX*nImageSizeY*2+i]))+1;
		//	OUTDATA[nImageSizeX*nImageSizeY*2+i]=(1-3*DATA[i])/OUTDATA[i];//s=1-r/i				
		//}
		//else if ((DATA[nImageSizeX*nImageSizeY+i]<DATA[nImageSizeX*nImageSizeY*2+i]&&DATA[nImageSizeX*nImageSizeY+i]<=DATA[i]))
		//{
		//	OUTDATA[nImageSizeX*nImageSizeY+i]=(DATA[i]-DATA[nImageSizeX*nImageSizeY+i])/(3*(OUTDATA[i]-DATA[nImageSizeX*nImageSizeY+i]))+2;
		//	OUTDATA[nImageSizeX*nImageSizeY*2+i]=1-DATA[nImageSizeX*nImageSizeY+i]/OUTDATA[i];//s=1-g/i
		//}

		OUTDATA[i]=(DATA[i]+DATA[nImageSizeX*nImageSizeY+i]+DATA[nImageSizeX*nImageSizeY*2+i]);//I����
		OUTDATA[nImageSizeX*nImageSizeY+i]=(DATA[nImageSizeX*nImageSizeY+i]-DATA[nImageSizeX*nImageSizeY*2+i])/(OUTDATA[i]-3*DATA[nImageSizeX*nImageSizeY*2+i]);//H����;
		OUTDATA[nImageSizeX*nImageSizeY*2+i]=(1-3*(DATA[nImageSizeX*nImageSizeY*2+i]/OUTDATA[i]));//s=(1-b)/i
		pro1->StepIt();
	}
}
//��ʾӰ����Ϣ
void CArthmetic::ShowImageInfo(void)
{
	for (int i=0;i<cfiles->pathname.size();i++)
	{
		GDALDataset* poDataset1;//GDAL���ݼ�
		GDALAllRegister();
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
		poDataset1=(GDALDataset*)GDALOpen(cfiles->pathname.at(i),GA_ReadOnly);
		if(poDataset1==NULL)
		{
			AfxMessageBox("��ʼ�������⣡");
			return;
		}
		int bandcount=poDataset1->GetRasterCount();

		HTREEITEM hSel=TVI_ROOT;//m_TreeFile.GetRootItem();//ȡ�ø�����
		static int nAddNo=10;//��Ŵ���10Ϊ������
		TV_INSERTSTRUCT TCItem;//������������ݽṹ

		TCItem.hParent=hSel; //���ø�����
		TCItem.hInsertAfter=TVI_LAST;//���������
		TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//������
		TCItem.item.pszText=(LPSTR)(LPCSTR)cfiles->pathname.at(i);
		TCItem.item.lParam=nAddNo+10;//����������
		TCItem.item.iImage=0;//����ͼ��
		TCItem.item.iSelectedImage=1;//ѡ��ʱͼ��
		HTREEITEM hInsert=m_TreeFile.InsertItem(&TCItem);//����

		for (int j=0;j<bandcount;j++)
		{
			CString bnd;
			bnd.Format("����%d",j);

			TCItem.hParent=hInsert;
			TCItem.item.pszText=(LPSTR)(LPCSTR)bnd;
			TCItem.item.lParam=nAddNo+i;//�������
			m_TreeFile.InsertItem(&TCItem);
			m_TreeFile.Expand(hSel,TVE_EXPAND);
		}
		GDALClose(poDataset1);
	}
	//Ӱ����Ϣ
	HTREEITEM hSel=m_TreeFile.GetRootItem();//ȡ��ѡ������
	if (hSel==NULL)
	{
		return ;
	}
	CString name=m_TreeFile.GetItemText(hSel);;
	if (name.Find(":")<0)
	{
		return ;
	}
	GDALDataset* poDataset1=(GDALDataset*)GDALOpen(name,GA_ReadOnly);
	if(poDataset1==NULL)
	{
		AfxMessageBox("��ʼ�������⣡");
		return ;
	}
	GDALRasterBand* band=poDataset1->GetRasterBand(1);
	m_edit.SetWindowText(" ");

	int m=1,b=1,x=0,y=0;
	double *c,*d,*e,*f,adfGeoTransform[6];
	c=new double;
	d=new double;
	e=new double;
	f=new double;
	band->GetStatistics(m,b,c,d,e,f);
	x=poDataset1->GetRasterXSize();
	y=poDataset1->GetRasterYSize();

	CString proj=poDataset1->GetProjectionRef();
	CString projection;
	int a1=proj.Find("PROJCS[");
	int a2;
	if (!proj.IsEmpty())
	{
		if (a1>=0)
		{
			a2=proj.Find(",",a1);
			projection=proj.Mid(a1+7,a2-(a1+7));
		}
		else
		{
			projection="geographic Lat/Lot";
		}
	}

	CString a;
	CRect rect;
	m_edit.GetClientRect(rect);
	int len=rect.Width();
	a.Format("�ļ�·����%s\r\n",name);
	m_edit.SetSel(0,len);
	m_edit.ReplaceSel(a);
	a.Format("�ļ���С��%d*%d\r\n\n",x,y);
	m_edit.SetSel(len,2*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	a.Format("��Ԫ��Сֵ��%f\r\n\n",*c);
	m_edit.SetSel(2*len,3*len);
	m_edit.ReplaceSel(a);
	a.Format("��Ԫ���ֵ��%f\r\n\n",*d);
	m_edit.SetSel(3*len,4*len);
	m_edit.ReplaceSel(a);

	if (poDataset1->GetGeoTransform(adfGeoTransform)==CE_None)
	{
		a.Format("��Ԫ�ֱ��ʣ�%fm*%fm\r\n\n",adfGeoTransform[1],adfGeoTransform[5]);
		m_edit.SetSel(4*len,5*len);
		m_edit.ReplaceSel(a);
		m_edit.ReplaceSel("\n");
	}
	a.Format("���ξ�ֵ��%f\r\n\n",*e);
	m_edit.SetSel(5*len,6*len);
	m_edit.ReplaceSel(a);
	a.Format("���η��%f\r\n\n",*f);
	m_edit.SetSel(6*len,7*len);
	m_edit.ReplaceSel(a);
	m_edit.SetSel(7*len,8*len);
	m_edit.ReplaceSel("\n");

	a.Format("ͶӰ��Ϣ��%s\r\n\n",projection);
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);

	a=GDALGetDataTypeName(band->GetRasterDataType());
	a="�洢��ʽ��"+a;
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	a=GDALGetColorInterpretationName(band->GetColorInterpretation());
	a="��������: "+a;
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel("\n");

	if (poDataset1->GetGeoTransform(adfGeoTransform)==CE_None)
	{
		a.Format("���Ͻǵ����꣺x=%f,y=%f",abs(adfGeoTransform[0]),abs(adfGeoTransform[3]));
		m_edit.SetSel(8*len,9*len);
		m_edit.ReplaceSel(a);
		m_edit.ReplaceSel("\n");
	}

	GDALClose(poDataset1);

}

void CArthmetic::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

void CArthmetic::IHStoRGB(double* DATA1,double* OUTDATA)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(nImageSizeX*nImageSizeY); //���ý��������ܲ���
	pro1->SetMessage("���ڼ�������ȴ�����");

	for (int i=0;i<nImageSizeX*nImageSizeY;i++)
	{
		//if (0<=DATA1[nImageSizeX*nImageSizeY+i]&&DATA1[nImageSizeX*nImageSizeY+i]<1)
		//{
		//	OUTDATA[i]=DATA1[i]*(1+2*DATA1[nImageSizeX*nImageSizeY*2+i]-3*DATA1[nImageSizeX*nImageSizeY*2+i]*DATA1[nImageSizeX*nImageSizeY+i]);//R
		//	OUTDATA[nImageSizeX*nImageSizeY+i]=DATA1[i]*(1-DATA1[nImageSizeX*nImageSizeY*2+i]+3*DATA1[nImageSizeX*nImageSizeY*2+i]*DATA1[nImageSizeX*nImageSizeY+i]);//G
		//	OUTDATA[nImageSizeX*nImageSizeY*2+i]=DATA1[i]*(1-DATA1[nImageSizeX*nImageSizeY*2+i]);//B
		//}
		//else if (1<=DATA1[nImageSizeX*nImageSizeY+i]&&DATA1[nImageSizeX*nImageSizeY+i]<2)
		//{
		//	OUTDATA[i]=DATA1[i]*(1-DATA1[nImageSizeX*nImageSizeY*2+i]);//R
		//	OUTDATA[nImageSizeX*nImageSizeY+i]=DATA1[i]*(1+5*DATA1[nImageSizeX*nImageSizeY*2+i]-3*DATA1[nImageSizeX*nImageSizeY*2+i]*DATA1[nImageSizeX*nImageSizeY+i]);//G
		//	OUTDATA[nImageSizeX*nImageSizeY*2+i]=DATA1[i]*(1-4*DATA1[nImageSizeX*nImageSizeY*2+i]+3*DATA1[nImageSizeX*nImageSizeY*2+i]*DATA1[nImageSizeX*nImageSizeY+i]);//B
		//}
		//else if (2<=DATA1[nImageSizeX*nImageSizeY+i]&&DATA1[nImageSizeX*nImageSizeY+i]<3)
		//{
		//	OUTDATA[i]=DATA1[i]*(1-7*DATA1[nImageSizeX*nImageSizeY*2+i]+3*DATA1[nImageSizeX*nImageSizeY*2+i]*DATA1[nImageSizeX*nImageSizeY+i]);//R
		//	OUTDATA[nImageSizeX*nImageSizeY+i]=DATA1[i]*(1-DATA1[nImageSizeX*nImageSizeY*2+i]);//G
		//	OUTDATA[nImageSizeX*nImageSizeY*2+i]=DATA1[i]*(1+8*DATA1[nImageSizeX*nImageSizeY*2+i]-3*DATA1[nImageSizeX*nImageSizeY*2+i]*DATA1[nImageSizeX*nImageSizeY+i]);//B
		//}

		OUTDATA[i]=(1+2*DATA1[nImageSizeX*nImageSizeY*2+i]-3*DATA1[nImageSizeX*nImageSizeY*2+i]*DATA1[nImageSizeX*nImageSizeY+i])*(DATA1[i]+DATA1[nImageSizeX*nImageSizeY+i]+DATA1[nImageSizeX*nImageSizeY*2+i])/3.0f;//R
		OUTDATA[nImageSizeX*nImageSizeY+i]=(1-DATA1[nImageSizeX*nImageSizeY*2+i]+3*DATA1[nImageSizeX*nImageSizeY*2+i]*DATA1[nImageSizeX*nImageSizeY+i])*(DATA1[i]+DATA1[nImageSizeX*nImageSizeY+i]+DATA1[nImageSizeX*nImageSizeY*2+i])/3.0f;//G
		OUTDATA[nImageSizeX*nImageSizeY*2+i]=(1-DATA1[nImageSizeX*nImageSizeY*2+i])*(DATA1[i]+DATA1[nImageSizeX*nImageSizeY+i]+DATA1[nImageSizeX*nImageSizeY*2+i])/3.0f;//B
		pro1->StepIt();
	}
}
// ��ά���ٸ���Ҷ
void CArthmetic::ImageFFt2D(BYTE* In_img,int width, int height,double** Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height); //���ý��������ܲ���

	ComplexNumber* m_pFFTBuf=new ComplexNumber[width*height];
	memset(m_pFFTBuf, 0, sizeof(ComplexNumber)*width*height);


	//ͼ�����ݱ�ɸ������ʹ���m_pFFTBuf
	for(int i=0;i<width*height;i++)
	{
		m_pFFTBuf[i].real=In_img[i];
		m_pFFTBuf[i].imag=0;
	}

	//����ComplexNumber�ṹ������,����Ϊheight
	ComplexNumber *array=new ComplexNumber[height];

	pro1->SetMessage("���ڽ���������Ҷ�任����");
	//������һά���ٸ���Ҷ�任
	for(int u=0;u<width;u++)
	{
		for(int v=0;v<height;v++)
		{
			array[v].real=m_pFFTBuf[v*width+u].real;
			array[v].imag=m_pFFTBuf[v*width+u].imag;
		}
		ImageFFt1D(array, height);//����������Ҷ
		for(int v=0;v<height;v++)//�洢���
		{
			m_pFFTBuf[v*width+u].real=array[v].real;
			m_pFFTBuf[v*width+u].imag=array[v].imag;
			pro1->StepIt();
		}
	}
	delete []array;
	//�ٺ���һά���ٸ���Ҷ�任
	for(int v=0;v<height;v++)
	{
		ImageFFt1D(m_pFFTBuf+v*width, width);
	}

	pro1->SetMessage("���ڴ洢Ƶ��ͼ����");
	pro1->ReSetProcess();
	//��Ƶ��ͼ��ͼ����ʽ����imgBufOut
	float t;
	int i0,j0;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(i<height/2)
				i0=i+height/2;
			else
				i0=i-height/2;
			if(j<width/2)
				j0=j+width/2;
			else
				j0=j-width/2;

			t=sqrt((float)(m_pFFTBuf[i0*width+j0].real*m_pFFTBuf[i0*width+j0].real+m_pFFTBuf[i0*width+j0].imag*m_pFFTBuf[i0*width+j0].imag));
			t=t/500;

			Out_img[0][i*width+j]=m_pFFTBuf[i0*width+j0].real;//�洢ʵ��
			Out_img[1][i*width+j]=m_pFFTBuf[i0*width+j0].imag;//�洢�鲿

			//������ʾ����
			if(t>255)
				Out_img[2][i*width+j]=255;//�洢ʵ��
			else 
				Out_img[2][i*width+j]=t;

			pro1->StepIt();
		}
	}
	
}
//һά����Ҷ
void CArthmetic::ImageFFt1D(ComplexNumber *arrayBuf, int n)
{
	//������ʱ����������buf1������Ϊn
	ComplexNumber *buf1=new ComplexNumber[n];

	//��arrayBuf������buf1
	memcpy(buf1,arrayBuf,sizeof(ComplexNumber)*n);

	//������ʱ����������buf2������Ϊn
	ComplexNumber *buf2=new ComplexNumber[n];

	//��arrayBuf����Ԫ�ػ�2��ȡ����������
	//��0��1��2��3��4��5��6��7�˵����жԵ������0��4��2��6��1��5��3��7
	int t1,t2;
	for(int r=1;pow(2.0f,r)<n;r++){
		t1=pow(2.0f,r);
		t2=pow(2.0f,r-1);
		for(int k=0;k<t1;k++)
		{
			for(int i=0;i<n/t1;i++)
			{
				buf2[k*n/t1+i].real=buf1[k/2*n/t2+i*2+k%2].real;
				buf2[k*n/t1+i].imag=buf1[k/2*n/t2+i*2+k%2].imag;
			}
		}
		memcpy(buf1,buf2,sizeof(ComplexNumber)*n);
	}


	//���õ����㷨���п��ٸ���Ҷ�任
	//buf1�ǵ�r�������룬buf2��ŵ�r�������
	float c,s;
	for(int r=1;pow(2.0f,r)<=n;r++)
	{
		t1=pow(2.0f,r);
		for(int k=0;k<n/t1;k++)
		{
			for(int i=t1/2;i<t1;i++)
			{
				//��Ȩ����
				c=cos(-2*PI*(i-t1/2)/t1);
				s=sin(-2*PI*(i-t1/2)/t1);
				buf1[k*t1+i].real= buf2[k*t1+i].real*c - buf2[k*t1+i].imag*s;
				buf1[k*t1+i].imag=buf2[k*t1+i].imag*c+buf2[k*t1+i].real*s;
			}
		}
		for(int k=0; k<n/t1; k++)
		{
			for(int i=0;i<t1/2;i++)
			{
				buf2[k*t1+i].real= buf1[k*t1+i].real+buf1[k*t1+i+t1/2].real;
				buf2[k*t1+i].imag= buf1[k*t1+i].imag+buf1[k*t1+i+t1/2].imag;
			}
			for(int i=t1/2;i<t1;i++)
			{
				buf2[k*t1+i].real= buf1[k*t1+i-t1/2].real-buf1[k*t1+i].real;
				buf2[k*t1+i].imag= buf1[k*t1+i-t1/2].imag-buf1[k*t1+i].imag;
			}
		}

		//��r�����������buf1,��Ϊ��һ������������
		memcpy(buf1,buf2,sizeof(ComplexNumber)*n);
	}
	//����Ҷ�任�Ľ������arrayBuf
	memcpy(arrayBuf,buf2,sizeof(ComplexNumber)*n);

	//�ͷŻ�����
	delete[]buf2;
	delete[]buf1;
}
//һά����Ҷ���任
void CArthmetic::ImageRevFFt1D(ComplexNumber *arrayBuf, int n)
{
	//����
	int i;

	//������arrayBuf����
	for(i=0;i<n;i++)
		arrayBuf[i].imag=-arrayBuf[i].imag;

	//һά���ٸ���Ҷ�任
	ImageFFt1D(arrayBuf, n);

	//ʱ���������������N
	for(i=0;i<n;i++)
	{
		arrayBuf[i].real=arrayBuf[i].real/n;
		arrayBuf[i].imag=-arrayBuf[i].imag/n;
	}

}
// ��ά���ٸ���Ҷ���任
void CArthmetic::ImageRevFFt2D(double* In_img, int width, int height,BYTE* Out_img)
{
	//������Ҷ�任������Ϊ��,��ʧ�ܷ���
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height); //���ý��������ܲ���

	ComplexNumber* m_pFFTBuf=new ComplexNumber[width*height];
	memset(m_pFFTBuf, 0, sizeof(ComplexNumber)*width*height);

	//ͼ�����ݱ�ɸ������ʹ���m_pFFTBuf
	for(int i=0;i<width*height;i++)
	{
		m_pFFTBuf[i].real=In_img[i];
		m_pFFTBuf[i].imag=In_img[width*height+i];
	}

	if(!m_pFFTBuf)
		return;

	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height); //���ý��������ܲ���
	//����
	int i, u, v;

	//��������Ҷ���任
	pro1->SetMessage("������Ҷ���任..");
	ComplexNumber *array=new ComplexNumber[height];
	for(u=0;u<width;u++)
	{
		for(v=0;v<height;v++)
		{
			array[v].real=m_pFFTBuf[v*width+u].real;
			array[v].imag=m_pFFTBuf[v*width+u].imag;
		}
		ImageRevFFt1D(array, height);
		for(v=0;v<height;v++)
		{
			m_pFFTBuf[v*width+u].real=array[v].real;
			m_pFFTBuf[v*width+u].imag=array[v].imag;
			pro1->StepIt();
		}
	}
	delete []array;

	//�ٺ�����Ҷ���任
	for(v=0;v<height;v++)
	{
		ImageRevFFt1D(m_pFFTBuf+v*width, width);
	}

	//��m_pFFTBuf�ķ�����,�ָ�ͼ�����imgBufOut
	pro1->SetMessage("�ظ�Ӱ����..");
	pro1->ReSetProcess();
	float t;
	for(i=0;i<width*height;i++)
	{
		t=sqrt(m_pFFTBuf[i].real*m_pFFTBuf[i].real
			+m_pFFTBuf[i].imag*m_pFFTBuf[i].imag);
		if(t>255)
			Out_img[i]=255;
		else Out_img[i]=t;

		pro1->StepIt();
	}
}
//��άתһά
double* CArthmetic::ValueToImg(double **value , int nBand, int hang, int lie)
{
	double *img = new double[nBand*hang*lie];
	int temp = 0;
	for(int Bandi =0 ; Bandi <nBand;Bandi++)
	{
		for(int i = 0 ; i <lie;i++)
		{
			for(int j =0 ; j<hang;j++)
			{
				/*if(value[Bandi][hang*lie*j+i]<0||value[Bandi][hang*lie*j+i]>255)
				img[temp++] = 0;
				else*/
				img[temp] = value[Bandi][hang*i+j];
				temp++;
			}
		}
	}
	return img;

}
//��άתһά
BYTE* CArthmetic::ValueToImg(BYTE **value , int nBand, int hang, int lie)
{
	BYTE *img = new BYTE[nBand*hang*lie];
	int temp = 0;
	for(int Bandi =0 ; Bandi <nBand;Bandi++)
	{
		for(int i = 0 ; i <lie;i++)
		{
			for(int j =0 ; j<hang;j++)
			{
				img[temp++] = value[Bandi][hang*i+j];
			}
		}
	}
	return img;
}
//һάת��ά
BYTE** CArthmetic::OneToTwo(BYTE *value , int hang, int lie)
{
	BYTE **img = new BYTE*[hang];
	for(int i=0;i<hang;i++)
	{   
		img[i] = new BYTE[lie]; 
	} 
	int temp = 0;
	for(int i = 0 ; i <lie;i++)
	{
		for(int j =0 ; j<hang;j++)
		{
			img[i][j] = value[hang*i+j];
		}
	}
	return img;
}
//��άתһά
BYTE* CArthmetic::TwoToOne(BYTE **value , int hang, int lie)
{
	BYTE *img = new BYTE[hang*lie];
	for(int i = 0 ; i <lie;i++)
	{
		for(int j =0 ; j<hang;j++)
		{
			img[hang*i+j]=value[i][j];
		}
	}
	return img;
}
// ��ɢ����Ҷ
void CArthmetic::FFT(BYTE* indata,int width,int hight,double** outdata)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*hight*width*hight); //���ý��������ܲ���
	pro1->SetMessage("���ڽ�����ɢ����Ҷ�任..");

	for (int i1=0;i1<hight;i1++)
	{
		for (int j1=0;j1<width;j1++)
		{
			double Real = 0;
			double Image = 0;
			for(int i = 0 ; i <hight;i++)
			{
				for(int j = 0 ; j <width;j++)
				{
					double Angle = (double((i1-hight/2)*i)/double(hight)+double((j1-width/2)*j)/double(width))*(-2.0*PI);
					Real +=indata[width*i+j]*cos(Angle);
					Image +=indata[width*i+j]*sin(Angle);
					pro1->StepIt();
				}
			}
			outdata[0][width*i1+j1]=Real/hight/width;
			outdata[1][width*i1+j1]=Image/hight/width;
			outdata[2][width*i1+j1]=sqrt(double(Real)*double(Real)+double(Image)*double(Image));
		}
	}
}
// ��ɢ����Ҷ��
void CArthmetic::FTTN(double* In_img, int width, int height,BYTE* Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height*width*height); //���ý��������ܲ���
	pro1->SetMessage("���ڽ�����ɢ����Ҷ���任..");

	for (int i1=0;i1<height;i1++)
	{
		for (int j1=0;j1<width;j1++)
		{
			double Real =0;
			double Image = 0;
			ComplexNumber* m_pFFTBuf=new ComplexNumber[width*height];
			memset(m_pFFTBuf, 0, sizeof(ComplexNumber)*width*height);
			ComplexNumber m_pFFTBuf1;

			for(int i = 0 ; i<height;i++)
			{
				for(int j = 0 ; j<width;j++)
				{
					m_pFFTBuf[i*width+j].real=In_img[i*width+j];//��ȡ����ҶӰ���ʵ�����鲿
					m_pFFTBuf[i*width+j].imag=In_img[width*height+i*width+j];

					double Angle = (double(i1*i)/double(height)+double(j1*j)/double(width))*(2.0*PI);
					m_pFFTBuf1.real=cos(Angle);
					m_pFFTBuf1.imag=sin(Angle);
					Real+=m_pFFTBuf[i*width+j].real*m_pFFTBuf1.real-m_pFFTBuf[i*width+j].imag*m_pFFTBuf1.imag;
					Image+=m_pFFTBuf[i*width+j].real*m_pFFTBuf1.imag+m_pFFTBuf[i*width+j].imag*m_pFFTBuf1.real;

					pro1->StepIt();
				}
			}
			Out_img[width*i1+j1]=sqrt(double(Real)*double(Real)-double(Image)*double(Image));//�洢���任Ӱ����
		}
	}
}
// ����Ӱ��Э����
double** CArthmetic::XieFangCha(BYTE* value,int nBand,int lie,int hang)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(nBand*nBand*hang*lie); //���ý��������ܲ���
	pro1->SetMessage("���ڼ��㲨�μ�Э����..");
	double **Xie= new double *[nBand];//�����洢��ͬ���μ��Э����
	for(int  tempi = 0 ; tempi < nBand ; tempi++)
	{
		Xie[tempi] =new double [nBand];
		for(int i = 0 ;i < nBand;i++ )
		{
			Xie[tempi][i] = 0;
		}
	}
	double *Ave=new double[nBand];

	for(int Bandi = 0 ;Bandi <nBand;Bandi++)
	{
		Ave[Bandi] = 0 ;
		for(int i = 0 ; i <hang ; i++)
			for(int j = 0 ; j <lie ;j++)
			{
				Ave[Bandi] +=value[Bandi*hang*lie+i*lie+j];//��Ⲩ�ε����غ�

			}
			Ave[Bandi] = Ave[Bandi]/(hang*lie);//��Ⲩ�εľ�ֵ
	}
	for(int i = 0 ; i <nBand;i++)
		for(int j = i ; j <nBand;j++)
		{
			for(int I = 0 ; I<hang;I++)
				for(int J = 0 ; J<lie;J++)
				{
					Xie[i][j] +=(value[i*hang*lie+I*lie+J]-Ave[i])*(value[j*hang*lie+I*lie+J]-Ave[j]);
					pro1->StepIt();
				}

				Xie[i][j] = Xie[i][j]/(hang*lie);//���ij�����ε�Э����ֵ
		}

		for(int i = 0;i<nBand;i++)
			for(int j = i ; j<nBand;j++)
			{
				Xie[j][i] = Xie[i][j];//�����ȱ��ֵ
			}

	return Xie;
}
/********************************************************************************
* ��ʵ�Գƾ��������ֵ�������������Ÿ�ȷ�
* �����Ÿ��(Jacobi)������ʵ�Գƾ����ȫ������ֵ����������
* ����ֵС��0��ʾ��������jt����δ�ﵽ����Ҫ��
* ����ֵ����0��ʾ��������
* a-����Ϊn*n�����飬���ʵ�Գƾ��󣬷���ʱ�Խ��ߴ��n������ֵ
* n-����Ľ���
* v-����Ϊn*n�����飬������������(���д洢)
* eps-���ƾ���Ҫ��
* jt-���ͱ�������������������
*********************************************************************************/
int CArthmetic::Eigs(double **a, int n, double *d, double **v, int& nrot)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(n*n); //���ý��������ܲ���
	pro1->SetMessage("���ڼ�����������..");

	
	double b[100], z[100], g, s, c, h, tau, sss, ddd, t, tresh, sm, theta;
	int ip, iq, i, j;
	for (ip=0; ip<n; ip++)
	{
		for (iq=0; iq<n; iq++)
		{
			v[ip][iq]=0.0;
		}
		v[ip][ip]=1.0;
	}
	for (ip=0; ip<n; ip++)
	{
		b[ip]=a[ip][ip];
		d[ip]=b[ip];
		z[ip]=0.0;
	}
	nrot=0;
	for (i=0; i<50; i++)
	{
		sm=0.0;
		for (ip=0; ip<n-1; ip++)
		{
			for (iq=ip+1; iq<n; iq++)
			{
				sm=sm+fabs(a[ip][iq]);
			}
		}
		if (sm==0.0)
		{
			return 0;
		}
		if (i<3)
		{
			tresh=0.2*sm/double(n*n);
		}
		else
		{
			tresh=0.0;
		}
		for (ip=0; ip<n-1; ip++)
		{
			for (iq=ip+1; iq<n; iq++)
			{
				pro1->StepIt();

				g=100.0*fabs(a[ip][iq]);
				sss=fabs(d[ip])+g;
				ddd=fabs(d[iq])+g;
				if ((i>3)&&(sss==fabs(d[ip]))&&(ddd==fabs(d[iq])))
				{
					a[ip][iq]=0.0;
				}
				else
				{
					if (fabs(a[ip][iq])>tresh)
					{
						h=d[iq]-d[ip];
						if ((fabs(h)+g)==fabs(h))
						{
							t=a[ip][iq]/h;
						}
						else
						{
							theta=0.5*h/a[ip][iq];
							t=1.0/(fabs(theta)+sqrt(1.0+pow(theta, 2)));
							if (theta<0.0)
								t=-t;
						}
						c=1.0/sqrt(1.0+t*t);
						s=t*c;
						tau=s/(1.0+c);
						h=t*a[ip][iq];
						z[ip]=z[ip]-h;
						z[iq]=z[iq]+h;
						d[ip]=d[ip]-h;
						d[iq]=d[iq]+h;
						a[ip][iq]=0.0;
						for (j=0; j<=ip-1; j++)
						{
							g=a[j][ip];
							h=a[j][iq];
							a[j][ip]=g-s*(h+g*tau);
							a[j][iq]=h+s*(g-h*tau);
						}
						for (j=ip+1; j<=iq-1; j++)
						{
							g=a[ip][j];
							h=a[j][iq];
							a[ip][j]=g-s*(h+g*tau);
							a[j][iq]=h+s*(g-h*tau);
						}
						for (j=iq+1; j<n; j++)
						{
							g=a[ip][j];
							h=a[iq][j];
							a[ip][j]=g-s*(h+g*tau);
							a[iq][j]=h+s*(g-h*tau);
						}
						for (j=0; j<n; j++)
						{
							g=v[j][ip];
							h=v[j][iq];
							v[j][ip]=g-s*(h+g*tau);
							v[j][iq]=h+s*(g-h*tau);
						}
						nrot=nrot+1;
					}
				}
			}
		}
		for (ip=0; ip<n; ip++)
		{
			b[ip]=b[ip]+z[ip];
			d[ip]=b[ip];
			z[ip]=0.0;
		}
	}
	return 1;
}
//����˷��ĺ���
double ** CArthmetic::matrix_mutiply(double **Matrix_1, double **Matrix_2, int Row1, int Column1, int Row2, int Column2)
{ 
	double **Result_Matrix = new double *[Row1];
	for(int i = 0 ; i <Row1;i++)
	{
		Result_Matrix[i] = new double[Column2];
		for(int j = 0 ; j <Column2;j++)
			Result_Matrix[i][j] = 0;

	}
	for(int k=0; k<Row1; k++) 
	{  
		for(int i=0; i<Column2; i++)  

		{  
			for(int j=0; j<Column1; j++)   
			{   
				Result_Matrix[k][i]+=Matrix_1[k][j]*Matrix_2[j][i]; 
			} 
		}
	} 
	return Result_Matrix;

}
// ͶӰ�任
void CArthmetic::ProTran(CString ReferenceFileName,CString OriFIleName)
{
	GDALDatasetH  hSrcDS, hDstDS;

	// ���������ͼ��
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	hSrcDS = GDALOpen(ReferenceFileName, GA_ReadOnly );
	hDstDS = GDALOpen(OriFIleName, GA_Update );


	poDataset = (GDALDataset *) GDALOpen(OriFIleName, GA_ReadOnly );
	if( poDataset == NULL )
	{
		AfxMessageBox("�ļ���ʧ��!!!");

	}
	int nBand=poDataset->GetRasterCount();     //��ò�����
	
	// �����任ѡ��
	GDALWarpOptions* psWarpOptions = GDALCreateWarpOptions();

	psWarpOptions->hSrcDS =hSrcDS;
	psWarpOptions->hDstDS =hDstDS;

	psWarpOptions->nBandCount = nBand;
	psWarpOptions->panSrcBands =(int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );
	for(int i =0 ; i<nBand;i++)
		psWarpOptions->panSrcBands[i] = i+1;
	psWarpOptions->panDstBands =
		(int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );
	for(int i = 0 ; i<nBand;i++)
		psWarpOptions->panDstBands[i] = i+1;
	
	pro1->ReSetProcess();
	pro1->SetMessage("���ڽ���ͶӰ�任..");
	psWarpOptions->pfnProgress = ALGTermProgress; 

	   // ������ͶӰ�任����
   psWarpOptions->pTransformerArg =
        GDALCreateGenImgProjTransformer( hSrcDS,
                                         GDALGetProjectionRef(hSrcDS),
                                        hDstDS,
                                         GDALGetProjectionRef(hDstDS),
                                         FALSE,0.0, 1 );
   psWarpOptions->pfnTransformer = GDALGenImgProjTransform;
   psWarpOptions->eWorkingDataType=GDT_Byte;

      // ��ʼ������ִ�б任����
    GDALWarpOperation oOperation;
 
   oOperation.Initialize(psWarpOptions );
   oOperation.ChunkAndWarpImage( 0, 0,
                                  GDALGetRasterXSize( hDstDS),
                                  GDALGetRasterYSize( hDstDS) );
 
    GDALDestroyGenImgProjTransformer(psWarpOptions->pTransformerArg );
   GDALDestroyWarpOptions( psWarpOptions );
 
    GDALClose( hDstDS );
    GDALClose( hSrcDS );
	
}
//��ͨ�˲�
void CArthmetic::Hightong(BYTE *PImg,int m_cols,int m_rows)
{
	//����ģ��
	int *temp=new int[9];
	temp[0]=0,temp[1]=-1,temp[2]=0;
	temp[3]=-1,temp[4]=5;temp[5]=-1;
	temp[6]=0,temp[7]=-1,temp[8]=0;


	//ʹ��ģ����и�ͨ�˲�
	for (int i=1;i<m_rows-1;i++)
	{
		for (int j=1;j<m_cols-1;j++)
		{
			int Median=temp[0]*PImg[(i-1)*m_cols+j-1]+temp[1]*PImg[(i-1)*m_cols+j]
			+temp[2]*PImg[(i-1)*m_cols+j+1]+temp[3]*PImg[i*m_cols+j-1]+temp[4]*PImg[i*m_cols+j]
			+temp[5]*PImg[i*m_cols+j+1]+temp[6]*PImg[(i+1)*m_cols+j-1]+temp[7]*PImg[(i+1)*m_cols+j]
			+temp[8]*PImg[(i+1)*m_cols+j+1];

			PImg[i*m_cols+j]=max(0,min(Median,255));

		}
	}
	delete[] temp;
}
//�ռ��򲻼�ȨӰ���ں�
void CArthmetic::Unweighting(BYTE *MImg,BYTE *PImg,int m_cols,int m_rows)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(m_cols*m_rows); //���ý��������ܲ���
	pro1->SetMessage("���ڽ����ںϲ���..");

	Hightong(PImg,m_cols,m_rows);//�Ը߷ֱ���ȫɫӰ����и�ͨ�˲�

	for (int i=0;i<m_rows;i++)
	{
		for (int j=0;j<m_cols;j++)
		{
			int B=MImg[m_rows*m_cols*2+i*m_cols+j]+PImg[i*m_cols+j];
			B=min(255,max(0,B));
			MImg[m_rows*m_cols*2+i*m_cols+j]=B;

			int G=MImg[m_rows*m_cols*1+i*m_cols+j]+PImg[i*m_cols+j];
			G=min(255,max(0,G));
			MImg[m_rows*m_cols*1+i*m_cols+j]=G;

			int R=MImg[i*m_cols+j]+PImg[i*m_cols+j];
			R=min(255,max(0,R));
			MImg[i*m_cols+j]=R;

			pro1->StepIt();
		}
	}
}
//�ռ����ȨӰ���ں�
void CArthmetic::Weighting(BYTE *MImg,BYTE *PImg,int m_cols,int m_rows) 
{
	// TODO: Add your command handler code here
	double *m_r=new double[3];//���ϵ��

	float m_gyaver=0;
	//����Ҷ�ͼ�������ƽ��ֵ
	for (int i=0;i<m_rows*m_cols;i++)
	{
		ASSERT(PImg[i]>=0&&PImg[i]<=255);
		m_gyaver+=PImg[i];
	}
	m_gyaver=m_gyaver/(m_rows*m_cols);

	//�����ɫͼ��������ε�����ƽ��ֵ
	float *m_grbaver=new float[3];
	m_grbaver[0]=0;
	m_grbaver[1]=0;
	m_grbaver[2]=0;

	for (int k=0;k<3;k++)
	{
		for (int i=0;i<m_rows*m_cols;i++)
		{
			ASSERT(MImg[i*3+k]>=0&&MImg[i*3+k]<=255);
			m_grbaver[k]+=MImg[i*3+k];
		}
	}


	for (int i=0;i<3;i++)
	{
		m_grbaver[i]/=(m_cols*m_rows);
	}

	//�������ϵ��
	float *m_der=new float[3];//���Ӳ���
	float *m_num1=new float[3];//��ĸ��һ����
	float *m_num2=new float[3];//��ĸ�ڶ�����

	for (int i=0;i<3;i++)
	{
		m_der[i]=0;
		m_num1[i]=0;
		m_num2[i]=0;
	}

	for (int k=0;k<3;k++)
	{
		for (int i=0;i<m_rows*m_cols;i++)
		{
			m_num1[k]+=(PImg[i]-m_gyaver)*(PImg[i]-m_gyaver);
			m_der[k]+=(PImg[i]-m_gyaver)*(MImg[i*3+k]-m_grbaver[k]);
			m_num2[k]+=(MImg[i*3+k]-m_grbaver[k])*(MImg[i*3+k]-m_grbaver[k]);
		}

	}

	for (int i=0;i<3;i++)
	{
		m_r[i]=fabs(m_der[i]/sqrt(m_num2[i]*m_num1[i]));
	}

	//�γ��µ�ͼ��

	for (int k=0;k<m_cols*m_rows;k++)
	{
		for (int i=0;i<3;i++)
		{
			BYTE index=(BYTE)(((1-m_r[i])*PImg[k]+(1+m_r[i])*MImg[k*3+i])/2);
			MImg[k*3+i]=index;
		}
	}
}
//��Ӹ�˹����
void CArthmetic::AddGaussNoise(BYTE* In_img, int width,int height,int nband,BYTE* Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height*nband); //���ý��������ܲ���
	pro1->SetMessage("���ڲ�����˹����..");
	for (int k=0;k<nband;k++)
	{
		for(int i=0;i<width*height;i++)
		{
			LONG Temp=rand();
			Temp = In_img[width*height*k+i]*224/256+Temp/512;
			In_img[width*height*k+i] = Temp >= 255 ? 255 : Temp;
		}
	}

	for(int k=0;k<nband;k++)
	{
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{
				Out_img[width*height*k+width*i+j]=In_img[width*height*k+width*i+j];
				pro1->StepIt();
			}
		}
	}
}
//��ӽ�������
void CArthmetic::AddPepperSaltNoise(BYTE* In_img, int width,int height,int nband,BYTE* Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height*nband); //���ý��������ܲ���
	pro1->SetMessage("���ڲ�����������..");
	for (int k=0;k<nband;k++)
	{
		for (int i=0;i<width*height;i=i+3)
		{
			int num=rand();
			if (num>32000) 
			{
				In_img[k*width*height+i]=(rand())%255;        //����ÿһ�����ص�RGBֵ
			}
			if (num<200) 
			{
				In_img[k*width*height+i]=(rand())%255;
			}

			Out_img[width*height*k+i]=In_img[width*height*k+i];
			pro1->StepIt();
		}
	}
}
//�������
void CArthmetic::Convolution(BYTE *PImg,double* temp,int m_cols,int m_rows,int nBand)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(m_cols*m_rows*nBand); //���ý��������ܲ���
	pro1->SetMessage("���ڽ��о������..");
	//ʹ��ģ����и�ͨ�˲�
	for (int k=0;k<nBand;k++)
	{
		for (int i=1;i<m_rows-1;i++)
		{
			for (int j=1;j<m_cols-1;j++)
			{
				int Median=temp[0]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j-1]+temp[1]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j]
				+temp[2]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j+1]+temp[3]*PImg[k*m_rows*m_cols+i*m_cols+j-1]+temp[4]*PImg[k*m_rows*m_cols+i*m_cols+j]
				+temp[5]*PImg[k*m_rows*m_cols+i*m_cols+j+1]+temp[6]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j-1]+temp[7]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j]
				+temp[8]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j+1];

				PImg[k*m_rows*m_cols+i*m_cols+j]=abs(Median/9);
				pro1->StepIt();
			}
		}
	}
}
// Robert�㷨
void CArthmetic::Robert(BYTE* In_img, int width,int height,int nband,BYTE* Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height*nband); //���ý��������ܲ���
	pro1->SetMessage("���ڽ���Robert����..");

	for (int k=0;k<nband;k++)
	{
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{
				//x�����ݶ�
				int x=(k*height*width+In_img[k*height*width+(i+1)*width+j+1]-In_img[k*height*width+i*width+j]);

				//y�����ݶ�
				int y=In_img[k*height*width+i*width+j+1]-In_img[k*height*width+(i+1)*width+j];

				int t=sqrt((float)(x*x+y*y))+0.5;
				if(t>255)
					t=255;
				Out_img[k*height*width+i*width+j]=t;
				pro1->StepIt();
			}
		}
	}
}
// Sobel�㷨
void CArthmetic::Sobel(BYTE *PImg,BYTE* Out_img,int m_cols,int m_rows,int nBand,int chidu)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(m_cols*m_rows*nBand); //���ý��������ܲ���
	pro1->SetMessage("���ڽ���Sobel����..");

	double temp[9];
	temp[0] = 1;
	temp[1] = 2;
	temp[2] = 1;
	temp[3] = 0;
	temp[4] = 0;
	temp[5] = 0;
	temp[6] = -1;
	temp[7] = -2;
	temp[8] = -1;

	double temp1[9];
	temp1[0] = -1;
	temp1[1] = 0;
	temp1[2] = 1;
	temp1[3] = -2;
	temp1[4] = 0;
	temp1[5] = 2;
	temp1[6] = -1;
	temp1[7] = 0;
	temp1[8] = 1;
	
	for (int k=0;k<nBand;k++)
	{
		for (int i=1;i<m_rows-1;i++)
		{
			for (int j=1;j<m_cols-1;j++)
			{
				int Median=temp[0]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j-1]+temp[1]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j]
				+temp[2]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j+1]+temp[3]*PImg[k*m_rows*m_cols+i*m_cols+j-1]+temp[4]*PImg[k*m_rows*m_cols+i*m_cols+j]
				+temp[5]*PImg[k*m_rows*m_cols+i*m_cols+j+1]+temp[6]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j-1]+temp[7]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j]
				+temp[8]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j+1];

				int a=abs(Median/9);

				Median=temp1[0]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j-1]+temp1[1]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j]
				+temp1[2]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j+1]+temp1[3]*PImg[k*m_rows*m_cols+i*m_cols+j-1]+temp1[4]*PImg[k*m_rows*m_cols+i*m_cols+j]
				+temp1[5]*PImg[k*m_rows*m_cols+i*m_cols+j+1]+temp1[6]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j-1]+temp1[7]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j]
				+temp1[8]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j+1];
				int b=abs(Median/9);

				a=abs(max(a,b));
				if (a>chidu)
				{
					Out_img[k*m_rows*m_cols+i*m_cols+j]=255;
				}
				else
				{
					Out_img[k*m_rows*m_cols+i*m_cols+j]=0;
				}

				pro1->StepIt();
			}
		}
	}
}
// Prewitt����
void CArthmetic::Prewitt(BYTE *PImg,BYTE* Out_img,int m_cols,int m_rows,int nBand,int chidu)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(m_cols*m_rows*nBand); //���ý��������ܲ���
	pro1->SetMessage("���ڽ���Prewitt����..");

	double temp[9];
	temp[0] = -1;
	temp[1] = 0;
	temp[2] = 1;
	temp[3] = -1;
	temp[4] = 0;
	temp[5] = 1;
	temp[6] = -1;
	temp[7] = 0;
	temp[8] = 1;

	double temp1[9];
	temp1[0] = 1;
	temp1[1] = 1;
	temp1[2] = 1;
	temp1[3] = 0;
	temp1[4] = 0;
	temp1[5] = 0;
	temp1[6] = -1;
	temp1[7] = -1;
	temp1[8] = -1;

	for (int k=0;k<nBand;k++)
	{
		for (int i=1;i<m_rows-1;i++)
		{
			for (int j=1;j<m_cols-1;j++)
			{
				int Median=temp[0]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j-1]+temp[1]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j]
				+temp[2]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j+1]+temp[3]*PImg[k*m_rows*m_cols+i*m_cols+j-1]+temp[4]*PImg[k*m_rows*m_cols+i*m_cols+j]
				+temp[5]*PImg[k*m_rows*m_cols+i*m_cols+j+1]+temp[6]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j-1]+temp[7]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j]
				+temp[8]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j+1];

				int a=abs(Median/9);

				Median=temp1[0]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j-1]+temp1[1]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j]
				+temp1[2]*PImg[k*m_rows*m_cols+(i-1)*m_cols+j+1]+temp1[3]*PImg[k*m_rows*m_cols+i*m_cols+j-1]+temp1[4]*PImg[k*m_rows*m_cols+i*m_cols+j]
				+temp1[5]*PImg[k*m_rows*m_cols+i*m_cols+j+1]+temp1[6]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j-1]+temp1[7]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j]
				+temp1[8]*PImg[k*m_rows*m_cols+(i+1)*m_cols+j+1];
				int b=abs(Median/9);

				a=abs(max(a,b));
				if (a>chidu)
				{
					Out_img[k*m_rows*m_cols+i*m_cols+j]=255;
				}
				else
				{
					Out_img[k*m_rows*m_cols+i*m_cols+j]=0;
				}

				pro1->StepIt();
			}
		}
	}

}
//Laplacian
void CArthmetic::Laplacian(BYTE *PImg,int m_cols,int m_rows,int nBand)
{
	double temp[9];
	temp[0] = 0;
	temp[1] = -1;
	temp[2] = 0;
	temp[3] = -1;
	temp[4] = 4;
	temp[5] = -1;
	temp[6] = 0;
	temp[7] = -1;
	temp[8] = 0;
	Convolution(PImg,temp,m_cols,m_rows,nBand);
}
//�߽�׷��
void CArthmetic::OnEdgeTrace(BYTE* In_img, int width,int height,BYTE* Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height); //���ý��������ܲ���
	pro1->SetMessage("���ڽ��б߽�׷��..");
	//˳ʱ�붨���������ص��8�������꣬��һ��Ϊx�����ƫ��,�ڶ���Ϊy�����ƫ��
	int direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};

	//�߽���ʼ��,������ĵ�ǰ��,��ǰ��������
	CPoint startP, currentP, neighborP;

	//�Ƿ�ǰ������ʼ�߽���غϵı�־����
	int findStartPoint=0;

	//�ж��Ƿ��Ƕ�ֵͼ��
	for(int j=0; j<width*height;j++)
	{
		if(In_img[j]!=0&&In_img[j]!=255)
		{
			AfxMessageBox("�������ֵͼ��");
			return;
		}
	}

	for(int j=0; j<height;j++)
	{
		for (int i=0;i<width;i++)
		{
			if (In_img[j*width+i]==255)
			{
				Out_img[j*width+i]=255;
			}
			else if(In_img[j*width+i]==0)
			{
				startP.x=i;
				startP.y=j;
				findStartPoint=1;
				break;
			}
		}
		//�Ѿ��ҵ���ʼ�߽��
		if(findStartPoint)
			break;
	}

	//�߽����
	//�ӳ�ʼ�㿪ʼ����
	currentP.x=startP.x;
	currentP.y=startP.y;

	//������Ƿ�߽���־����
	int isContourP;

	//��ʼ����
	int startDirect=0;

	//0��ʾ��û�з�������ı߽���ʼ��
	findStartPoint=0;
	while(findStartPoint==0)
	{
		isContourP=false;
		while(isContourP==false)
		{
			pro1->StepIt();

			neighborP.x=currentP.x+direction[startDirect][0];
			neighborP.y=currentP.y+direction[startDirect][1];

			//�����������
			if(In_img[neighborP.y*width+neighborP.x]==0)
			{
				isContourP=true;
				currentP.x=neighborP.x;
				currentP.y=neighborP.y;

				if(currentP.x==startP.x&&currentP.y==startP.y)
					findStartPoint=true;//�ص��߽���ʼ����

				Out_img[neighborP.y*width+neighborP.x]=0;

				//ɨ�跽����ʱ����ת90��
				startDirect-=2;
				if(startDirect<0)
					startDirect+=8;
			}
			else
			{
				//ɨ�跽��˳ʱ����ת45��
				startDirect++;
				if(startDirect==8)
					startDirect=0;
			}
		}
	}
}
//��������
void CArthmetic::RegionGrow(BYTE* In_img, int width,int height,int nBand,BYTE* Out_img,CPoint SeedPos, int thresh)
{
	//��ά����direction�����������ص�8����������õ���x��y�����ϵ�ƫ��,
	//���е�һ��Ϊx�����ƫ��,�ڶ���Ϊy�����ƫ��
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height); //���ý��������ܲ���
	pro1->SetMessage("���ڽ�����������..");

	if (nBand==3)
	{
		for (int i=0;i<nImageSizeX*nImageSizeY;i++)
		{
			In_img[i]=In_img[nImageSizeX*nImageSizeY*2+i]*0.144+In_img[nImageSizeX*nImageSizeY+i]*0.587+In_img[i]*0.299;//RGBת�Ҷ�
		}
	}

	//�����ͼ���ʼ����255
	memset(Out_img,255,width*height);

	int direction[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

	//ջ���룬�˴��ٶ���ջ���������Ϊͼ����������
	CPoint *stack=new CPoint[width*height];

	//ջ��ָ��
	int top;

	//��ǰ������ĵ�͵����ĵ�
	CPoint currentPoint, popPoint;

	//ѭ������������array����ĵ�һά�±�
	int k;

	//��Ǳ���
	int label;

	//��ʱ����
	int temp1, temp2;

	//��¼�������صĻҶ�ֵ
	temp1=In_img[SeedPos.y*width+SeedPos.x];

	//���������ӵ��ñ��0,��ջ
	Out_img[SeedPos.y*width+SeedPos.x]=0;
	top=0;
	stack[top].x=SeedPos.x;
	stack[top].y=SeedPos.y;

	//��ջ
	while(top>-1)
	{
		pro1->StepIt();
		//����ջ��Ԫ��,��Ԫ���Ѿ�������
		popPoint.x=stack[top].x;
		popPoint.y=stack[top].y;
		top--;

		//���쵯��������Χ�Ƿ���û������������
		for(k=0;k<8;k++)
		{

			//������������
			currentPoint.x=popPoint.x+direction[k][0];
			currentPoint.y=popPoint.y+direction[k][1];

			//���������ĵ㲻��ͼ���ڣ�������
			if(currentPoint.x<0||currentPoint.x>width-1||
				currentPoint.y<0||currentPoint.y>height-1)
				continue;

			//�õ���
			label=Out_img[currentPoint.y*width+currentPoint.x];

			//�����ĵ���Χ����û�����ĵ�
			if(label==255)
			{
				temp2=In_img[currentPoint.y*width+currentPoint.x];
				//�����ǰ����������ػҶ�ֵ�����ӵ�Ҷ�ֵ֮��С�ڸ�������ֵ,
				//����Ϊ����,�����ջ����
				if(abs(temp1-temp2)<thresh)
				{
					//���õ����������0
					Out_img[currentPoint.y*width+currentPoint.x]=0;
					top++;
					stack[top].x=currentPoint.x;
					stack[top].y=currentPoint.y;
				}
			}
		}

	}
	//���������
	delete []stack;
}
//��������ת��Ϊ��������
void Trans(double * Transform,double x,double y,double &l,double & p)
{

	l  = Transform[0]+Transform[1]*y+Transform[2]*x;
	p = Transform[3]+Transform[4]*y+Transform[5]*x;

}
//����������굽��������ı任��ʽ
double * FanBianhuan(double * t)
{
	double * a =new double[6];
	double b = t[1]*t[5]-t[2]*t[4];

	a[0] = (t[2]*t[3]-t[0]*t[5])/b;
	a[1] = -t[2]/b;
	a[2] = (t[5])/b;
	a[3] = (t[0]*t[4]-t[1]*t[3])/b;
	a[4] = (t[1])/b;
	a[5] = -t[4]/b;
	return a;
}
//�ɵ������귴����������
void Tran2(double * Transform,double l,double p,double & x,double &y)
{
	CPoint point ;
	x = Transform[0]+Transform[1]*p+Transform[2]*l;
	y = Transform[3]+Transform[4]*p+Transform[5]*l;

}
//����Ӱ��ֱ���
void caculFBL(int width,int heigth,double l1,double p1,double l2,double p2,double& FBLx,double& FBLy)
{
	double geoWidth=l2-l1;
	double geoHeigth=p2-p1;

	FBLx=abs(geoWidth/(double)width);
	FBLy=abs(geoHeigth/(double)heigth);
}
//Ӱ��ü�
int CArthmetic::ImgSubset(CPoint begin,CPoint end,BYTE* In_img,BYTE* Out_img,CString OutPath,CString ImageType)
{
	pro1->ReSetProcess();  
	pro1->SetMessage("���ڽ���Ӱ��ü�..");
	pro1->SetStepCount(100);
	pro1->SetPosition(0);

	m_Gcps=new GDAL_GCP[2];
	double l1,p1,l2,p2;
	double * Transform=new double[6];
	poDataset->GetGeoTransform(Transform);
	Trans(Transform,(double)begin.x,(double)begin.y,l1,p1);
	//��¼���Ͻǵ�ĵ������������
	m_Gcps[0].dfGCPPixel=(double)begin.x;
	m_Gcps[0].dfGCPLine=(double)begin.y;
	m_Gcps[0].dfGCPX=l1;
	m_Gcps[0].dfGCPY=p1;
	Trans(Transform,(double)begin.x,(double)begin.y,l2,p2);
	m_Gcps[1].dfGCPPixel=(double)end.x;
	m_Gcps[1].dfGCPLine=(double)end.y;
	m_Gcps[1].dfGCPX=l2;
	m_Gcps[1].dfGCPY=p2;

	Transform[0]=l1;
	Transform[3]=p1;

	int x=poDataset->GetRasterXSize();
	int y=poDataset->GetRasterYSize();

	nImageSizeX=m_Gcps[1].dfGCPPixel-m_Gcps[0].dfGCPPixel;
	nImageSizeY=m_Gcps[1].dfGCPLine-m_Gcps[0].dfGCPLine;
	int bandcount=poDataset->GetRasterCount();

	for(int k=0;k<bandcount;k++)
	{
		for (int i=0;i<nImageSizeY;i++)
		{
			for (int j=0;j<nImageSizeX;j++)
			{
				Out_img[k*nImageSizeX*nImageSizeY+i*nImageSizeX+j]=In_img[k*x*y+((int)m_Gcps[0].dfGCPLine+i-1)*x+(int)m_Gcps[0].dfGCPPixel+j-1];
			}
		}
	}

	const char* pszSRS_WKT=poDataset->GetProjectionRef();

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(ImageType);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)OutPath,//���·��
		nImageSizeX,nImageSizeY, //Ӱ�����ݿ��
		bandcount, //���ȡ�Ĳ�����
		GDT_Byte, 	//��������������
		papszOptions );

	poDstDS->SetGeoTransform(Transform);
	poDstDS->SetProjection(pszSRS_WKT);

	if(poDstDS->RasterIO(
		GF_Write,						//IO��ʽ
		0,0,							//Ӱ��������ʼλ��x,y
		nImageSizeX,nImageSizeY,				//Ӱ�����ݿ��
		Out_img,						//ָ�򻺳�������ָ��
		nImageSizeX,nImageSizeY,				//���������ݿ��
		GDT_Byte,						//��������������
		bandcount,						//���ȡ�Ĳ�����
		NULL,							//���ȡ�Ĳ����б�
		0,								//����������һ������ռ�õ��ֽ���
		0,								//������һ��������ռ�õ��ֽ���
		0)==CE_Failure )				//���������ݴ洢��ʽ 0ΪRRRRGGGGBBBB-bsp 1ΪRGBRGBRGB-bip	
	{
		AfxMessageBox("�����ļ�ʧ�ܣ�");
		return 0;
	}
	pro1->SetMessage("���ڱ���Ӱ�񡣡�");
	pro1->SetPosition(100);

	GDALClose(poDataset);
	GDALClose(poDstDS);
	delete[] m_Gcps;

	return 1;
}
// �ز���
void CArthmetic::ChongCaiYang(BYTE* &Data,int nImageSizeX,int nImageSizeY,CString FilePath)
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	if (isOpened==false)
	{
		poDataset=(GDALDataset*)GDALOpen(FilePath,GA_ReadOnly);
	}

	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
		return;
	}
	GDALRasterBand* band=poDataset->GetRasterBand(1);

	if (pro1==NULL)
	{
		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
	}

	pro1->ReSetProcess();  
	pro1->SetMessage("�����ز���Ӱ�񡣡�");

	int nImageSizeX1=poDataset->GetRasterXSize();
	int nImageSizeY1=poDataset->GetRasterYSize();
	GDALDataType a=band->GetRasterDataType();

	if (poDataset->GetRasterCount()==1)
	{
		Data = new BYTE[nImageSizeX*nImageSizeY];
		poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX1,nImageSizeY1, Data,nImageSizeX,nImageSizeY,a,1, NULL,0,0,0 );
	}
	else
	{
		Data = new BYTE[nImageSizeX*nImageSizeY*3];
		poDataset->RasterIO( GF_Read, 0, 0,nImageSizeX1,nImageSizeY1, Data,nImageSizeX,nImageSizeY,a,3, NULL,0,0,0 );
	}

}
/**********************************************************************
* �������ƣ� MatrixSampleRow()
* ������ unsigned char *matrixInput ��������������ָ��
*        unsigned char *matrixOutputOdd �����в������ɵľ���
*        unsigned char *matrixOutputEven ż���в������ɵľ���
*        int heightOutput �������߶�
*        int widthOutput ���������
*        int widthInput ���������
* ����ֵ�� void
* ˵���� �������������г�����
 **********************************************************************/
void CArthmetic::MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd, unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput)
{
	int h, w;
	for (h=0; h<heightOutput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			matrixOutputEven[h * widthOutput + w] = matrixInput[h * 2 * widthInput + w];
			matrixOutputOdd[h * widthOutput + w] = matrixInput[(h * 2 + 1) * widthInput + w];
		}
	}
}
/**********************************************************************
* �������ƣ� MatrixSub()
* ������ unsigned char *matrixA ��������A����ָ��
*        unsigned char *matrixB ��������B����ָ��
*        int height �߶�
*        int width ���
*        unsigned char *result ���������ָ��
* ����ֵ�� void
* ˵���� ���������������A��B���
**********************************************************************/
void CArthmetic::MatrixSub(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result)
{
	unsigned char temp;
	int h, w;
	for (h=0; h<height; h++)
	{
		for (w=0; w<width; w++)
		{
			temp = matrixA[h * width + w] - matrixB[h * width + w] + 128;
			result[h * width + w] = temp;
		}
	}
}
/**********************************************************************
 * �������ƣ� MatrixSampleLine()
 * ������ unsigned char *matrixInput ��������������ָ��
 *        unsigned char *matrixOutputOdd �����в������ɵľ���
 *        unsigned char *matrixOutputEven ż���в������ɵľ���
 *        int heightOutput �������߶�
 *        int widthOutput ���������
 *        int widthInput ���������
 * ����ֵ��void
 * ˵���� �������������г�����
 **********************************************************************/
void CArthmetic::MatrixSampleLine(unsigned char *matrixInput, unsigned char *matrixOutputOdd,unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput)
{
	int h, w;
	for (h=0; h<heightOutput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			matrixOutputEven[h * widthOutput + w] = matrixInput[h * widthInput + w * 2];
			matrixOutputOdd[h * widthOutput + w] = matrixInput[h * widthInput + w * 2 + 1];
		}
	}
}
/**********************************************************************
 * �������ƣ� MatrixSub()
 * ������ unsigned char *matrixA ����;���A����ָ��
 *        unsigned char *matrixB ����;���B����ָ��
 *        int height �߶�
 *        int width ���
 *        unsigned char *result �;�������ָ��
 * ����ֵ��void
 * ˵���� ���������������A��B���
 **********************************************************************/
void CArthmetic::MatrixPlus(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result)
{
	unsigned char temp;
	int h, w;
	for (h=0; h<height; h++)
	{
		for (w=0; w<width; w++)
		{
			temp = matrixA[h * width + w] + matrixB[h * width + w] - 128;
			result[h * width + w] = temp;
		}
	}
}
/**********************************************************************
* �������ƣ�
* MatrixRegionCopy()
* ������ unsigned char *matrixRegion Դ��������ָ��
*        int heightRegion ��������ĸ߶�
*        int widthRegion ��������Ŀ��
*        unsigned char *matrixDest Ŀ�ľ�������ָ��
*        int heightDest Ŀ�ľ�������ĸ߶�
*        int widthDest Ŀ�ľ�������Ŀ��
* ����ֵ��void
* ˵���� ��Դ�����ָ���������ݸ��Ƶ�Ŀ�ľ����ָ������
**********************************************************************/
void CArthmetic::MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,unsigned char *matrixDest, int heightDest, int widthDest)
{
	//��������߶ȺͿ��
	int heightMin = heightRegion>heightDest ? heightDest : heightRegion;
	int widthMin = widthRegion>widthDest ? widthDest : widthRegion;
	int h, w;
	for (h=0; h<heightMin; h++)
	{
		for (w=0; w<widthMin; w++)
		{
			matrixDest[h * widthDest + w] = matrixRegion[h * widthRegion + w];
		}
	}
}
/**********************************************************************
* �������ƣ�WavlDecmposeTrans()
* ����: unsigned char *inputData �����������ָ��
*       int inputHeight �������߶�
*       int inputWidth ���������
*       int lineByte �������ֽ���
*       int scale �ֽ�߶�
*       unsigned char *outputData �����������ָ��
* ����ֵ��void
* ˵��������һ��С���ֽ�
************************************************************************/
void CArthmetic::WavlDecmposeTrans(unsigned char *inputData, int inputHeight, int inputWidth,int OutImageSizeX,int OutImageSizeY,int scale, unsigned char *outputData)
{
	unsigned char *buff = new unsigned char[OutImageSizeX*OutImageSizeY];

	int imageHeight = inputHeight;
	int imageWidth = inputWidth;

	unsigned char *temp1 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	/**/
	//��ͼ���������в���
	MatrixSampleRow(inputData, temp1, temp2, (imageHeight/2), imageWidth, OutImageSizeX);
	//�в�֣��õ���Ƶ���Ƶ����
	MatrixSub(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//�Ե�Ƶ���ݽ����в���
	MatrixSampleLine(temp2, temp01, temp00, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�ֵõ�LL��LH
	MatrixSub(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//�Ը�Ƶ���ݽ����в���
	MatrixSampleLine(temp1, temp11, temp10, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�֣��õ�HL��HH
	MatrixSub(temp11, temp10, (imageHeight/2), (imageWidth/2), temp11);

	MatrixRegionCopy(temp01, (imageHeight/2), (imageWidth/2),
		buff+OutImageSizeX*(int)(imageHeight/2)+(int)(imageWidth/2), imageHeight, OutImageSizeX);
	MatrixRegionCopy(temp10, (imageHeight/2), (imageWidth/2), buff, imageHeight, OutImageSizeX);
	MatrixRegionCopy(temp11, (imageHeight/2), (imageWidth/2), buff+(int)(imageWidth/2), imageHeight, OutImageSizeX);
	//�ͷſռ�
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;

	scale--;
	//������LL���еݹ�ֽ�
	if (scale > 0)
	{
		WavlDecmposeTrans(temp00,imageHeight/2, imageWidth/2, imageWidth/2,OutImageSizeY,scale,temp00);
	}
	MatrixRegionCopy(temp00, (imageHeight/2), (imageWidth/2), buff+OutImageSizeX*(int)(imageHeight/2),
		imageHeight, OutImageSizeX);
	delete[] temp00;

	//�����Ľ��������m_pImgDataOut
	memcpy(outputData, buff,  OutImageSizeX * inputHeight);
	delete[] buff;
}
/**********************************************************************
* �������ƣ� SimpleWaveletTrans()
* ������     int scale �ֽ�߶ȣ�Ĭ��Ϊ1
* ����ֵ��   void
* ˵����     ��ͼ�����ݽ��и����߶ȵ�С���ֽ�
**********************************************************************/
void CArthmetic::BasicWaveletTrans(BYTE* m_pImgData,BYTE* m_pImgDataOut,int nImageSizeX,int nImageSizeY,int scale=1)
{
	if (scale<1 || scale>5)
	{
		return;
	}
	if(m_pImgDataOut!=NULL){
		delete[] m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	m_pImgDataOut=new unsigned char[nImageSizeX*nImageSizeY];
	WavlDecmposeTrans(m_pImgData, nImageSizeX,nImageSizeY,nImageSizeX,nImageSizeY,scale,m_pImgDataOut);
}
//��ˮ��ָ�
/*===================================================================
������          ��    Watershed 

����            ��    �ñ��-��ˮ���㷨������ͼ����зָ� 

�㷨ʵ��        ��    �� 

�������˵��    ��    OriginalImage --����ͼ�񣨻Ҷ�ͼ��0~255�� 
                      SeedImage   --���ͼ�񣨶�ֵͼ��0-�Ǳ�ǣ�1-��ǣ�
                      LabelImage   --���ͼ��1-��һ���ָ�����2-�ڶ����ָ�����...�� 
                      row       --ͼ������ 
                      col       --ͼ������ 
����ֵ˵��      ��    ��     

====================================================================*/ 
void CArthmetic::Watershed(unsigned char **OriginalImage,char** SeedImage, BYTE **LabelImage, int row, int col)
{
	// using namespace std;

	//��������ʶ�ţ���1��ʼ
	int Num=0;
	int i,j;

	//����ÿ���������Ӹ���������
	vector<int*> SeedCounts;
	//��ʱ���Ӷ���
	queue<POINT> quetem;
	//�������б���������Ӷ��е����飬����ŵ������Ӷ��е�ָ��
	vector<queue<POINT>*> vque;

	int* array;
	//ָ�����Ӷ��е�ָ��
	queue<POINT> *pque;
	POINT temp;

	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
			LabelImage[i][j]=0;
	}
	//char** SeedImage = new char*[row];
	//for (int i=0;i<row;i++)
	//{
	//	SeedImage[i] = new char[col]; 
	//}
	int m,n,k=0;
	BOOL up,down,right,left,upleft,upright,downleft,downright;//8 directions...

	//Ԥ����,��ȡ����ÿ��������򣬲���ʼ��ÿ����ǵ����Ӷ���
	//������ָ��������Ե�ĵ㣬���ǿ�����ˮλ����ʱ������û������˵������
	//pan's words���ҵ�������ݶ�ֵ��С�����ص㣬�����Ǽ�С�Ҷ�ֵ�ĵ㡣
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			//����ҵ�һ���������
			if(SeedImage[i][j]==1)
			{
				//����ı�ʶ�ż�һ
				Num++;
				//�������鲢��ʼ��Ϊ�㣬��ʾ����256���ҽ�
				array=new int[256];
				ZeroMemory(array,256*sizeof(int));
				//���Ӹ��������vector��ÿ��ɨ��������һ�����飬���������ʶ��������һά���Ҷȼ����ڶ�ά��
				//��ʾĳ�����������ĳ�ҽ�����Ӧ�ĵ����Ŀ��
				SeedCounts.push_back(array);
				//���䱾��Ǻŵ����ȶ��У�256�����Ӷ��У�
				//��ʾ��Ӧһ���ҽ���һ�����У�����ÿ�����п��Դ洢һ�����ϵĵ���Ϣ
				pque=new queue<POINT>[256];
				//���뵽���������У���Ӧ���Ǳ���Ǻ�Num��
				vque.push_back(pque);
				//��ǰ����뱾����������ʱ���Ӷ�����
				temp.x=i;
				temp.y=j;
				quetem.push(temp);
				//��ǰ����Ϊ�Ѵ���
				LabelImage[i][j]=Num;
				SeedImage[i][j]=127;//��ʾ�Ѿ������

				//����ʱ���Ӷ����е����ӽ�������ֱ�����е����Ӷ��������
				//������Ϻ�Ķ�����Ϣ������vque�У���������źͻҽף���Ӧ�����洢��seedcounts��
				while(!quetem.empty())
				{
					up=down=right=left=FALSE;
					upleft=upright=downleft=downright=FALSE;
					//������ȡ��һ������
					temp=quetem.front();
					m=temp.x;
					n=temp.y;
					quetem.pop();
					//ע�⵽127��ɨ����̵�Ӱ�죬Ӱ������ıȽϣ����ǲ�Ӱ��while����е�ɨ��

					if(m>0)
					{
						//�Ϸ���Ϊ�����������Ϊ������
						if(SeedImage[m-1][n]==1)
						{
							temp.x=m-1;
							temp.y=n;
							quetem.push(temp);//��������Ļ�����ô��Щ��ǹ��������ٴ���whileѭ���б�ɨ�赽�����ᣬ��Ϊֵ��127
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ���򣬲���¼����ŵ�labelImage
							LabelImage[m-1][n]=Num;
							SeedImage[m-1][n]=127;
						}
						else//�����Ϸ�Ϊ��������
						{
							up=TRUE;
						}
					}
					if(m>0&&n>0)
					{
						if(SeedImage[m-1][n-1]==1)//���Ϸ���Ϊ�����������Ϊ������
						{
							temp.x=m-1;
							temp.y=n-1;
							quetem.push(temp);
							//�����ӵ���Ϊ����û���򣬼���һ��ѭ������127����ʶ����ɨ�裬�����ǵ�ǰ����
							LabelImage[m-1][n-1]=Num;
							SeedImage[m-1][n-1]=127;
						}
						else//�������Ϸ�Ϊ��������
						{
							upleft=TRUE;
						}
					}

					if(m<row-1)
					{
						if(SeedImage[m+1][n]==1)//�·���Ϊ�����������Ϊ������
						{
							temp.x=m+1;
							temp.y=n;
							quetem.push(temp);
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����
							LabelImage[m+1][n]=Num;
							SeedImage[m+1][n]=127;
						}
						else//�����·�Ϊ��������
						{
							down=TRUE;
						}
					}
					if(m<(row-1)&&n<(col-1))
					{
						if(SeedImage[m+1][n+1]==1)//�·���Ϊ�����������Ϊ������
						{
							temp.x=m+1;
							temp.y=n+1;
							quetem.push(temp);
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����
							LabelImage[m+1][n+1]=Num;
							SeedImage[m+1][n+1]=127;
						}
						else//�����·�Ϊ��������
						{
							downright=TRUE;
						}
					}

					if(n<col-1)
					{
						if(SeedImage[m][n+1]==1)//�ҷ���Ϊ�����������Ϊ������
						{
							temp.x=m;
							temp.y=n+1;
							quetem.push(temp);
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����
							LabelImage[m][n+1]=Num;
							SeedImage[m][n+1]=127;
						}
						else//�����ҷ�Ϊ��������
						{
							right=TRUE;
						}
					}
					if(m>0&&n<(col-1))
					{
						if(SeedImage[m-1][n+1]==1)//���Ϸ���Ϊ�����������Ϊ������
						{
							temp.x=m-1;
							temp.y=n+1;
							quetem.push(temp);
							//�����ӵ���Ϊ����û���򣬶����ǵ�ǰ����
							LabelImage[m-1][n+1]=Num;
							SeedImage[m-1][n+1]=127;
						}
						else//�������Ϸ�Ϊ��������
						{
							upright=TRUE;
						}
					}

					if(n>0)
					{
						if(SeedImage[m][n-1]==1)//����Ϊ�����������Ϊ������
						{
							temp.x=m;
							temp.y=n-1;
							quetem.push(temp);
							//�����ӵ���Ϊ����û����
							LabelImage[m][n-1]=Num;
							SeedImage[m][n-1]=127;
						}
						else//������Ϊ��������
						{
							left=TRUE;
						}
					}
					if(m<(row-1)&&n>0)
					{
						if(SeedImage[m+1][n-1]==1)//���·���Ϊ�����������Ϊ������
						{
							temp.x=m+1;
							temp.y=n-1;
							quetem.push(temp);
							//�����ӵ���Ϊ����û����
							LabelImage[m+1][n-1]=Num;
							SeedImage[m+1][n-1]=127;
						}
						else//������Ϊ��������
						{
							downleft=TRUE;
						}
					}

					//��������ֻҪ��һ�㲻����������ô����Ϊ��ʼ���Ӷ����е�һ��
					//����ɷ���������seedimage�е�ֵ�������ġ�
					if(up||down||right||left||
						upleft||downleft||upright||downright)
					{
						temp.x=m;
						temp.y=n;
						//�������ʸ�����飺��һά�Ǳ�Ǻţ��ڶ�ά�Ǹ�ͼ���ĻҶȼ�
						//m,n���Ӧ����whileѭ������ɨ������ص㡣
						//Num�ǵ�ǰ�������
						//���������ά��Ϣ�ͱ�ʾ�ˣ�ĳ�������ж�Ӧĳ���Ҷȼ���Ӧ�ĳ�Ա��ļ��������
						//�ֱ������������������
						vque[Num-1][OriginalImage[m][n]].push(temp);//���������Ұ�Num-1�ĳ���Num...pan's codes...
						SeedCounts[Num-1][OriginalImage[m][n]]++;
					}     
				}//while������ɨ�赽quetemΪ�ն�ֹ��Ҳ���Ƕ�Ӧ���еĽڵ㶼�õ���������Ϊֹ����������Χ�ĵ�Ҫô����������Ҫô��������
			}//if����
			// if(Num==5)
			// return;
		}
	}
	//�����������У������ǵĵ�Ϊ0���ʾ��û��ɨ�赽�ĵ㣬���߱���������������ӵ�
	//�����൱������seedimage�������ĳ�ʼ����ķ�ˮ����ߵ����еĵ㣻�����ñ�ż�¼ÿ������ͬʱ��ˮ��ı�Ե�������С�
	//�������Ҽ�ˮ��ĳ���ͬʱҲ����ͨ����
	/*************************************/
	//test �������һ��ʣ�µķ�ˮ��صĵ�����
	int seednum;
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			if(SeedImage[i][j]==0)
				seednum++;
		}
	}

	CString str;
	str.Format("pre region num:%d",Num);
	AfxMessageBox(str);
	/*************************************/
	bool actives;//��ĳһˮλ�������б�ǵ�����������ı�־
	int WaterLevel;

	//��û���̿�ʼ��ˮλ���㿪ʼ������ˮλ��Ӧ�Ҷȼ�����������ͨ��
	for(WaterLevel=0;WaterLevel<180;WaterLevel++)//�ڶ�ά������
	{
		actives=true;
		while(actives)
		{
			actives=false;
			//���δ���ÿ����Ǻ�����Ӧ�������������ǺŶ�Ӧ������ĵ�ĸ�����SeedCounts����
			for(i=0;i<Num;i++)//��һά������
			{
				if(!vque[i][WaterLevel].empty())//��Ӧ�ķ�ˮ�벻Ϊ�ռ�,i��ʾ����ţ�waterlevel��ʾ�ҽ�
				{
					actives=true;
					while(SeedCounts[i][WaterLevel]>0)
					{
						SeedCounts[i][WaterLevel]--;//ȡ��һ���㣬������һ
						temp=vque[i][WaterLevel].front();//ȡ���������һ���㣬��������Ե�㣬��ʾ��ǰ
						//�Ҷȼ��������Ѿ�������ˡ�
						vque[i][WaterLevel].pop();
						m = temp.x;
						n = temp.y;//��ǰ���ӵ�����
						if(m>0)
						{
							if(!LabelImage[m-1][n])//�Ϸ���δ������ʾû�б�ţ�Ӧ��������ǰ�Ѿ�������ʼ��Ϊ0
								//���������ڿ�ͷҲ������ʼ��
							{
								temp.x=m-1;
								temp.y=n;
								LabelImage[m-1][n]=i+1;//�Ϸ�����Ϊ����û����
								//ע�⵽����������ɨ�����������ͬ��һ�����������������������ǵ�
								//��������һ�����ٻ�ɨ�赽����㣬ȷ������©��������һ�ֵĴ����ʹ������
								//���������⻹��������ǲ�û��һ���������뵽���Ӷ��С�Ҳ����˵��
								//ֻ�Ǳ���û������������û��ֻ�����������������������С�
								if(OriginalImage[m-1][n]<=WaterLevel)//�Ϸ���Ϊ������������뵱ǰ���У���ǰ�߶ȵĶ���
								{
									vque[i][WaterLevel].push(temp);
								}
								else//�������OriginalImage[m-1][n]��Ӧ�ĻҶȼ��Ķ��У�Ϊʲô��
								{
									vque[i][OriginalImage[m-1][n]].push(temp);
									SeedCounts[i][OriginalImage[m-1][n]]++;
								}
							}
						}

						if(m<row-1)
						{
							if(!LabelImage[m+1][n])//�·���δ����
							{
								temp.x=m+1;
								temp.y=n;
								LabelImage[m+1][n]=i+1;//�·�����Ϊ����û����

								if(OriginalImage[m+1][n]<=WaterLevel)//�·���Ϊ������������뵱ǰ����
								{
									vque[i][WaterLevel].push(temp);
								}
								else//�������OriginalImage[m+1][n]������
								{
									vque[i][OriginalImage[m+1][n]].push(temp);
									SeedCounts[i][OriginalImage[m+1][n]]++;
								}
							}
						}

						if(n<col-1)
						{
							if(!LabelImage[m][n+1])//�ұ���δ����
							{
								temp.x=m;
								temp.y=n+1;
								LabelImage[m][n+1]=i+1;//�ұߵ���Ϊ����û����

								if(OriginalImage[m][n+1]<=WaterLevel)//�ұ���Ϊ������������뵱ǰ����
								{
									vque[i][WaterLevel].push(temp);
								}
								else//�������OriginalImage[m][n+1]������
								{
									vque[i][OriginalImage[m][n+1]].push(temp);
									SeedCounts[i][OriginalImage[m][n+1]]++;
								}
							}
						}

						if(n>0)
						{
							if(!LabelImage[m][n-1])//�����δ����
							{
								temp.x=m;
								temp.y=n-1;
								LabelImage[m][n-1]=i+1;//��ߵ���Ϊ����û����

								if(OriginalImage[m][n-1]<=WaterLevel)//�����Ϊ������������뵱ǰ����
								{
									vque[i][WaterLevel].push(temp);
								}
								else//�������OriginalImage[m][n-1]������
								{
									vque[i][OriginalImage[m][n-1]].push(temp);
									SeedCounts[i][OriginalImage[m][n-1]]++;
								}
							}
						}
					}//whileѭ������
					SeedCounts[i][WaterLevel]=vque[i][WaterLevel].size();
				}//if����
			}//forѭ������
		}//whileѭ������
	}//forѭ������

	/**********************************
	//test whether the origional segmentation num is changed...
	int nonzeronum=Num;
	for(m=0;m<Num;m++)
	{
	for(i=0;i<row;i++)
	{
	for(j=0;j<col;j++)
	{
	if(LabelImage[i][j]==m)
	break;
	}
	if(LabelImage[i][j]==m)
	break;
	}
	if(j==col-1 && i==row-1 && LabelImage[i][j]!=m)
	nonzeronum--;
	}
	str.Format("new region num:%d",nonzeronum);//���ʾ���ǿ����ŵĵ�
	AfxMessageBox(str);
	/*********************************/ 
	while(!vque.empty())
	{
		pque=vque.back();
		delete[] pque;
		vque.pop_back();
	}
	while(!SeedCounts.empty())
	{
		array=SeedCounts.back();
		delete[] array;
		SeedCounts.pop_back();
	}
}

//��������ת��Ϊդ������
bool Projection2ImageRowCol(double *adfGeoTransform, double dProjX, double dProjY, int &iCol, int &iRow)
{
	try
	{
		double dTemp = adfGeoTransform[1]*adfGeoTransform[5] - adfGeoTransform[2]*adfGeoTransform[4];
		double dCol = 0.0, dRow = 0.0;
		dCol = (adfGeoTransform[5]*(dProjX - adfGeoTransform[0]) - 
			adfGeoTransform[2]*(dProjY - adfGeoTransform[3])) / dTemp + 0.5;
		dRow = (adfGeoTransform[1]*(dProjY - adfGeoTransform[3]) - 
			adfGeoTransform[4]*(dProjX - adfGeoTransform[0])) / dTemp + 0.5;

		iCol = static_cast<int>(dCol);
		iRow = static_cast<int>(dRow);
		return true;
	}
	catch(...)
	{
		return false;
	}
}
//դ������ת��Ϊ��������
bool ImageRowCol2Projection(double *adfGeoTransform, int iCol, int iRow, double &dProjX, double &dProjY)
{
	//adfGeoTransform[6]  ����adfGeoTransform������Ƿ���任�е�һЩ�������ֱ������
	//adfGeoTransform[0]  ���Ͻ�x���� 
	//adfGeoTransform[1]  ��������ֱ���
	//adfGeoTransform[2]  ��ת�Ƕ�, 0��ʾͼ�� "��������"
	//adfGeoTransform[3]  ���Ͻ�y���� 
	//adfGeoTransform[4]  ��ת�Ƕ�, 0��ʾͼ�� "��������"
	//adfGeoTransform[5]  �ϱ�����ֱ���

	try
	{
		dProjX = adfGeoTransform[0] + adfGeoTransform[1] * iCol + adfGeoTransform[2] * iRow;
		dProjY = adfGeoTransform[3] + adfGeoTransform[4] * iCol + adfGeoTransform[5] * iRow;
		return true;
	}
	catch(...)
	{
		return false;
	}
}