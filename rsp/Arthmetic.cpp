// Arthmetic.cpp : 实现文件
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
extern CProgressBar* pro1;//进度条

CString BiaoZhi=" ";
CString filename=" ";
double moban[9];
int fenge=0;
bool isOpened=false;
//记录裁剪区范围
CPoint BeginSub;
CPoint EndSub;

struct Modal//模式
{
	int x;
	int y;
	int mark;
};

// CArthmetic 对话框

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


// 位势函数分类法
void CArthmetic::WeiShiClass(double a[][2],double b[][2],int a1,int b1,double Afa=0)//a为w1类，b为w2类;a1,b1为模式个数
{
	//double k[100];//积累位势函数
	//int m=0;//判错计数
	//k[0]=0;//初始化积累位势函数
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

	//样本结构
	int n=6,m=30,d=3;
	struct sample {
		int x[3];
		int cl;
	};
	//势函数标识与索引结构
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
		AfxMessageBox("只支持txt格式数据！");
		return;
	}

	CFile file(FILE_NAME,CFile::modeRead);
	CArchive ar(&file, CArchive::load) ;

	int count=0;
	FILE *fp;
	int ch,ch2;
	int curr;

	//打开样本文件
	if((fp=fopen(FILE_NAME,"rb"))==NULL)
	{
		AfxMessageBox("无法打开文件！");
		return;
	}
	//打印样本
	i=0;
	while(i<n)
	{
		for(int iii=0;iii<4;++iii)
		{
			ar>>ch>>ch2;
			//个位与十位
			if(ch2==' ') 
				curr=ch;
			else 
				curr=(ch)*10+ch2;

			if(iii==3) 
				s[i].cl=curr;//类别
			else 
				s[i].x[iii]=curr;//x1,x2,x3
		}
		++i;
		++count;
	}
	file.Close();
	//构造位势函数
	while(tag==1)
	{	
		tag=0;
		for(k=0;k<n;k++)
		{
			if(r==-1)
			{
				r++;
				ftbl[r].symbol=1;//标示
				ftbl[r].index=0;//索引号
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
					g+=ftbl[i].symbol*exp(-temp);//计算位势函数
				}
				if((g>0 && s[k].cl==1)||(g<0&&s[k].cl==2))continue;
				else
				{
					r++;
					ftbl[r].index=k;//记录当前的数据的下标
					tag=1;
					if(g>0 && s[k].cl==2)//根据算出来的位势函数值与所属类别判断
						ftbl[r].symbol=-1;//分类错误
					else if(g<0&&s[k].cl==1)
						ftbl[r].symbol=1;//分类正确
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
// 感知器算法
void GanZhiQi(Modal* a,int num,int* result)
{
	int w[100][2];
	w[0][0]=1;//初始化增广权矢量
	w[0][1]=1;
	int jishu=0;

loop:	for (int i=0;i<num;i++)
		{
			int result=w[i][0]*a[i].x+w[i][1]*a[i].y+w[i][2]*a[i].mark;//计算判别函数值
			if (a[i].mark==1&&result<=0)//属于w1类
			{
				w[jishu][0]=w[jishu][0]+a[i].x;
				w[jishu][1]=w[jishu][1]+a[i].y;
				jishu++;
			}
			else if (a[i].mark==0&&result>=0)//属于w2类
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
		//返回计算结果
		result[0]=w[jishu][0];
		result[1]=w[jishu][1];
}

void CArthmetic::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="打开新影像";
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
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(flag, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="保存影像";
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
	// TODO: 在此添加控件通知处理程序代码
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


		ReadImage(DATA2,nImageSizeX,nImageSizeY,file_out);//读取单波段影像
		if (poDataset==NULL)
		{
			return;
		}
		ChongCaiYang(DATA,nImageSizeX,nImageSizeY,file_in);//读取多光谱影像

		if (poDataset==NULL)
		{
			return;
		}

		OUTDATA=new double[nImageSizeX*nImageSizeY*3];
		RGBtoHIS(DATA,OUTDATA);//获取多光谱IHS分量

		float MaxHd=0,MinHd=10000000;
		float MaxIn=0,MinIn=1000000;
		float Median;
		for (int i=0;i<nImageSizeX*nImageSizeY;i++)
		{
			if (DATA2[i]>MaxHd)
			{
				MaxHd=DATA2[i];//获取灰度影像最大值
			}
			else if (DATA2[i]<MinHd)
			{
				MinHd=DATA2[i];//获取灰度影像最小值
			}

			if (OUTDATA[i]>MaxIn)
			{
				MaxIn=OUTDATA[i];//获取I分量最大值
			}
			else if (OUTDATA[i]<MinIn)
			{
				MinIn=OUTDATA[i];//获取I分量最小值
			}
		}

		for (int i=0;i<nImageSizeX*nImageSizeY;i++)
		{
			OUTDATA[i]=(double)((DATA2[i]-MinHd)*(MaxIn-MinIn)/(MaxHd-MinHd)+MinIn);//全色波段代替I分量
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
	if (BiaoZhi=="FFT")//离散傅里叶
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
		double * img = ValueToImg(outdata,3,nImageSizeX,nImageSizeY);//二维转化成一维
		SaveImage(img,nImageSizeX,nImageSizeY,3,file_out,"HFA");
		cfiles->setMessage(file_out,"UTM","WGS-84","",3,nImageSizeX,nImageSizeY);

		delete pro1;
		delete[] outdata;
		delete[] DATA;
	}
	if (BiaoZhi=="FFTN")//离散傅里叶逆变换
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
	if (BiaoZhi=="QFFT")//计算快速傅里叶
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
		double * img = ValueToImg(outdata,3,nImageSizeX,nImageSizeY);//二维转化成一维
		SaveImage(img,nImageSizeX,nImageSizeY,3,file_out,"HFA");
		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",3,nImageSizeX,nImageSizeY);
		}

		delete pro1;
		delete[] outdata;
		delete[] DATA;
	}
	if (BiaoZhi=="QFFTN")//计算快速傅里叶逆
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
			AfxMessageBox("输入影像至少需要两个波段！");
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
			TeZhengVector[i] = new double[nBand];//存储特征向量
		}
		int nrot;
		double *d =new double[nBand];
		Eigs(Xiefangcha, nBand, d, TeZhengVector,  nrot);//计算特征向量
		double* Paixu = new double [nBand];
		for(int i = 0 ; i<nBand;i++)
		{
			Paixu[i] = Xiefangcha[i][i];//存储特征值
		}
		//冒泡排序法从大到小排序
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
					shuzu[i] = j;//记录下标
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
				Min[Bandi] = min(Min[Bandi],Results[Bandi][i]);//获取波段最小值
				Max[Bandi] = max(Max[Bandi],Results[Bandi][i]);//获取波段最大值

			}	
		}

		pro1->ReSetProcess();  
		pro1->SetStepCount(nBand*nImageSizeX*nImageSizeY); //设置进度条的总步长
		pro1->SetMessage("正在计算PCA变换结果..");

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
		ReadImage(DATA2,nImageSizeX1,nImageSizeY1,file_out);//读取全色
		if (poDataset==NULL)
		{
			return;
		}
		GDALClose(poDataset);

		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);//读取多光谱
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
		ReadImage(DATA2,nImageSizeX1,nImageSizeY1,file_out);//读取全色
		if (poDataset==NULL)
		{
			return;
		}
		GDALClose(poDataset);

		ReadImage(DATA,nImageSizeX,nImageSizeY,file_in);//读取多光谱
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
		//阈值
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
			AfxMessageBox("请先打开视图选取裁剪区..");
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

	//计算坡度
	if (BiaoZhi=="Slope")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",1);
	
		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
		Slope.processRaster(pro1,"正在计算坡度。。");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
	}
	//计算坡向
	if (BiaoZhi=="Aspect")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",2);

		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
		Slope.processRaster(pro1,"正在计算坡向。。");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
	}
	//地形耐用指数（TPI）
	if (BiaoZhi=="TPI")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",3);

		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
		Slope.processRaster(pro1,"正在计算地形耐用指数。。");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
	}
	//山体阴影
	if (BiaoZhi=="HillShade")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",4);

		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);

		Slope.setLightAzimuth(300);
		Slope.setLightAngle(40);
		Slope.processRaster(pro1,"正在计算山体阴影。。");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
	}
	//地形位置指数
	if (BiaoZhi=="TRI")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",5);

		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
		Slope.processRaster(pro1,"正在计算粗糙度。。");

		if (cfiles->GetSafeHwnd()!=NULL)
		{
			cfiles->setMessage(file_out,"UTM","WGS-84","",1,nImageSizeX,nImageSizeY);
		}
		delete pro1;  
	}
	//粗糙度
	if (BiaoZhi=="Roughness")
	{
		TerrainAnalysis Slope(file_in,file_out,"GTiff",6);

		pro1=new CProgressBar;
		pro1->Create(IDD_DIALOG_JDT);
		pro1->ShowWindow(SW_SHOW);
		Slope.processRaster(pro1,"正在计算粗糙度。。");

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

	DWORD dwStyles=GetWindowLong(m_TreeFile.m_hWnd,GWL_STYLE);//获取树控制原风格
	dwStyles|=TVS_EDITLABELS|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT;
	SetWindowLong(m_TreeFile.m_hWnd,GWL_STYLE,dwStyles);//设置风格
	ShowImageInfo();
	// TODO:  在此添加额外的初始化
	if (BiaoZhi=="ProTran")
	{
		CButton* btn=(CButton*)GetDlgItem(IDC_BUTTON1);
		btn->SetWindowText("参考影像：");
		btn=(CButton*)GetDlgItem(IDC_BUTTON2);
		btn->SetWindowText("原始影像：");
		flag=true;
	}
	else if (BiaoZhi=="IHSMERGE"||BiaoZhi=="UnweightingMerge"||BiaoZhi=="WeightingMerge")
	{
		CButton* btn=(CButton*)GetDlgItem(IDC_BUTTON1);
		btn->SetWindowText("多光谱影像：");
		btn=(CButton*)GetDlgItem(IDC_BUTTON2);
		btn->SetWindowText("全色影像：");
		flag=true;
	}
	else if (BiaoZhi=="OnSubsetimg")
	{
		file_in=cfiles->pathname.at(0);
		SetDlgItemText(IDC_STATIC_AR1,file_in);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
//显示影像信息
void CArthmetic::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	GDALDataset* poDataset1;//GDAL数据集
	GDALRasterBand* band;
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	HTREEITEM hSel=m_TreeFile.GetSelectedItem();//取得选择项句柄
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
		AfxMessageBox("初始化有问题！");
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
	a.Format("文件路径：%s\r\n",name);
	m_edit.SetSel(0,len);
	m_edit.ReplaceSel(a);
	a.Format("文件大小：%d*%d\r\n\n",x,y);
	m_edit.SetSel(len,2*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	a.Format("象元最小值：%f\r\n\n",*c);
	m_edit.SetSel(2*len,3*len);
	m_edit.ReplaceSel(a);
	a.Format("象元最大值：%f\r\n\n",*d);
	m_edit.SetSel(3*len,4*len);
	m_edit.ReplaceSel(a);

	if (poDataset1->GetGeoTransform(adfGeoTransform)==CE_None)
	{
		a.Format("象元分辨率：%fm*%fm\r\n\n",abs(adfGeoTransform[1]),abs(adfGeoTransform[5]));
		m_edit.SetSel(4*len,5*len);
		m_edit.ReplaceSel(a);
		m_edit.ReplaceSel("\n");
	}

	a.Format("波段均值：%f\r\n\n",*e);
	m_edit.SetSel(5*len,6*len);
	m_edit.ReplaceSel(a);
	a.Format("波段方差：%f\r\n\n",*f);
	m_edit.SetSel(6*len,7*len);
	m_edit.ReplaceSel(a);
	m_edit.SetSel(7*len,8*len);
	m_edit.ReplaceSel("\n");

	a.Format("投影信息：%s\r\n\n",projection);
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	a=GDALGetDataTypeName(band->GetRasterDataType());
	a="存储格式："+a;
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	a=GDALGetColorInterpretationName(band->GetColorInterpretation());
	a="波段类型:"+a;
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel("\n");

	if (poDataset1->GetGeoTransform(adfGeoTransform)==CE_None)
	{
		a.Format("左上角点坐标：x=%f,y=%f",adfGeoTransform[0],adfGeoTransform[3]);
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
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}
	GDALRasterBand* band=poDataset->GetRasterBand(1);

	pro1=new CProgressBar;
	pro1->Create(IDD_DIALOG_JDT);
	pro1->ShowWindow(SW_SHOW);
	if(pro1 != NULL)  
	{  
		pro1->ReSetProcess();  
		pro1->SetMessage("正在读取影像。。");
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
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}
	GDALRasterBand* band=poDataset->GetRasterBand(1);


	pro1=new CProgressBar;
	pro1->Create(IDD_DIALOG_JDT);
	pro1->ShowWindow(SW_SHOW);

	pro1->ReSetProcess();  
	pro1->SetMessage("正在读取影像。。");

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
	poDstDS = poDriver->Create( (char*)(LPCTSTR)OutPath,//输出路径
		nImageSizeX,nImageSizeY, //影像数据宽高
		bandcount, //存或取的波段数
		GDT_Byte, 	//缓冲区数据类型
		papszOptions );

	poDstDS->SetGeoTransform(adfGeoTransform);
	poDstDS->SetProjection(pszSRS_WKT);

	if(poDstDS->RasterIO(
		GF_Write,						//IO方式
		0,0,							//影像数据起始位置x,y
		nImageSizeX,nImageSizeY,				//影像数据宽高
		Outdata,						//指向缓冲区数据指针
		nImageSizeX,nImageSizeY,				//缓冲区数据宽高
		GDT_Byte,						//缓冲区数据类型
		bandcount,						//存或取的波段数
		NULL,							//存或取的波段列表
		0,								//缓冲区数据一个像素占用的字节数
		0,								//缓冲区一行数据所占用的字节数
		0)==CE_Failure )				//缓冲区数据存储方式 0为RRRRGGGGBBBB-bsp 1为RGBRGBRGB-bip	
	{
		AfxMessageBox("保存文件失败！");
		return;
	}
	pro1->SetMessage("正在保存影像。。");

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
	poDstDS = poDriver->Create( (char*)(LPCTSTR)OutPath,//输出路径
		nImageSizeX,nImageSizeY, //影像数据宽高
		bandcount, //存或取的波段数
		GDT_Float64, 	//缓冲区数据类型
		papszOptions );

	poDstDS->SetGeoTransform(adfGeoTransform);
	poDstDS->SetProjection(pszSRS_WKT);

	if(poDstDS->RasterIO(
		GF_Write,						//IO方式
		0,0,							//影像数据起始位置x,y
		nImageSizeX,nImageSizeY,				//影像数据宽高
		Outdata,						//指向缓冲区数据指针
		nImageSizeX,nImageSizeY,				//缓冲区数据宽高
		GDT_Float64,						//缓冲区数据类型
		bandcount,						//存或取的波段数
		NULL,							//存或取的波段列表
		0,								//缓冲区数据一个像素占用的字节数
		0,								//缓冲区一行数据所占用的字节数
		0)==CE_Failure )				//缓冲区数据存储方式 0为RRRRGGGGBBBB-bsp 1为RGBRGBRGB-bip	
	{
		AfxMessageBox("保存文件失败！");
		return;
	}
	pro1->SetMessage("正在保存影像。。");

	GDALClose(poDataset);
	GDALClose(poDstDS);
}

void CArthmetic::RGBtoHIS(BYTE* DATA,double* OUTDATA)
{
		
	pro1->ReSetProcess();  
	pro1->SetStepCount(nImageSizeX*nImageSizeY); //设置进度条的总步长
	pro1->SetMessage("正在计算结果请等待。。");

	for (int i=0;i<nImageSizeX*nImageSizeY;i++)
	{
		//OUTDATA[i]=(DATA[i]+DATA[nImageSizeX*nImageSizeY+i]+DATA[nImageSizeX*nImageSizeY*2+i])/3;//I分量
		//if ((DATA[i]>DATA[nImageSizeX*nImageSizeY*2+i]&&DATA[nImageSizeX*nImageSizeY*2+i]<=DATA[nImageSizeX*nImageSizeY+i]))
		//{
		//	OUTDATA[nImageSizeX*nImageSizeY+i]=(DATA[nImageSizeX*nImageSizeY+i]-DATA[nImageSizeX*nImageSizeY*2+i])/(3*(OUTDATA[i]-DATA[nImageSizeX*nImageSizeY*2+i]));//H分量;
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

		OUTDATA[i]=(DATA[i]+DATA[nImageSizeX*nImageSizeY+i]+DATA[nImageSizeX*nImageSizeY*2+i]);//I分量
		OUTDATA[nImageSizeX*nImageSizeY+i]=(DATA[nImageSizeX*nImageSizeY+i]-DATA[nImageSizeX*nImageSizeY*2+i])/(OUTDATA[i]-3*DATA[nImageSizeX*nImageSizeY*2+i]);//H分量;
		OUTDATA[nImageSizeX*nImageSizeY*2+i]=(1-3*(DATA[nImageSizeX*nImageSizeY*2+i]/OUTDATA[i]));//s=(1-b)/i
		pro1->StepIt();
	}
}
//显示影像信息
void CArthmetic::ShowImageInfo(void)
{
	for (int i=0;i<cfiles->pathname.size();i++)
	{
		GDALDataset* poDataset1;//GDAL数据集
		GDALAllRegister();
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
		poDataset1=(GDALDataset*)GDALOpen(cfiles->pathname.at(i),GA_ReadOnly);
		if(poDataset1==NULL)
		{
			AfxMessageBox("初始化有问题！");
			return;
		}
		int bandcount=poDataset1->GetRasterCount();

		HTREEITEM hSel=TVI_ROOT;//m_TreeFile.GetRootItem();//取得根项句柄
		static int nAddNo=10;//编号大于10为打开数据
		TV_INSERTSTRUCT TCItem;//定义插入项数据结构

		TCItem.hParent=hSel; //设置父项句柄
		TCItem.hInsertAfter=TVI_LAST;//在最后增加
		TCItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;//设屏蔽
		TCItem.item.pszText=(LPSTR)(LPCSTR)cfiles->pathname.at(i);
		TCItem.item.lParam=nAddNo+10;//索引号增加
		TCItem.item.iImage=0;//正常图标
		TCItem.item.iSelectedImage=1;//选中时图标
		HTREEITEM hInsert=m_TreeFile.InsertItem(&TCItem);//增加

		for (int j=0;j<bandcount;j++)
		{
			CString bnd;
			bnd.Format("波段%d",j);

			TCItem.hParent=hInsert;
			TCItem.item.pszText=(LPSTR)(LPCSTR)bnd;
			TCItem.item.lParam=nAddNo+i;//子项序号
			m_TreeFile.InsertItem(&TCItem);
			m_TreeFile.Expand(hSel,TVE_EXPAND);
		}
		GDALClose(poDataset1);
	}
	//影像信息
	HTREEITEM hSel=m_TreeFile.GetRootItem();//取得选择项句柄
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
		AfxMessageBox("初始化有问题！");
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
	a.Format("文件路径：%s\r\n",name);
	m_edit.SetSel(0,len);
	m_edit.ReplaceSel(a);
	a.Format("文件大小：%d*%d\r\n\n",x,y);
	m_edit.SetSel(len,2*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	a.Format("象元最小值：%f\r\n\n",*c);
	m_edit.SetSel(2*len,3*len);
	m_edit.ReplaceSel(a);
	a.Format("象元最大值：%f\r\n\n",*d);
	m_edit.SetSel(3*len,4*len);
	m_edit.ReplaceSel(a);

	if (poDataset1->GetGeoTransform(adfGeoTransform)==CE_None)
	{
		a.Format("象元分辨率：%fm*%fm\r\n\n",adfGeoTransform[1],adfGeoTransform[5]);
		m_edit.SetSel(4*len,5*len);
		m_edit.ReplaceSel(a);
		m_edit.ReplaceSel("\n");
	}
	a.Format("波段均值：%f\r\n\n",*e);
	m_edit.SetSel(5*len,6*len);
	m_edit.ReplaceSel(a);
	a.Format("波段方差：%f\r\n\n",*f);
	m_edit.SetSel(6*len,7*len);
	m_edit.ReplaceSel(a);
	m_edit.SetSel(7*len,8*len);
	m_edit.ReplaceSel("\n");

	a.Format("投影信息：%s\r\n\n",projection);
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);

	a=GDALGetDataTypeName(band->GetRasterDataType());
	a="存储格式："+a;
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	a=GDALGetColorInterpretationName(band->GetColorInterpretation());
	a="波段类型: "+a;
	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel(a);
	m_edit.ReplaceSel("\n");

	m_edit.SetSel(8*len,9*len);
	m_edit.ReplaceSel("\n");

	if (poDataset1->GetGeoTransform(adfGeoTransform)==CE_None)
	{
		a.Format("左上角点坐标：x=%f,y=%f",abs(adfGeoTransform[0]),abs(adfGeoTransform[3]));
		m_edit.SetSel(8*len,9*len);
		m_edit.ReplaceSel(a);
		m_edit.ReplaceSel("\n");
	}

	GDALClose(poDataset1);

}

void CArthmetic::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CArthmetic::IHStoRGB(double* DATA1,double* OUTDATA)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(nImageSizeX*nImageSizeY); //设置进度条的总步长
	pro1->SetMessage("正在计算结果请等待。。");

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
// 二维快速傅里叶
void CArthmetic::ImageFFt2D(BYTE* In_img,int width, int height,double** Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height); //设置进度条的总步长

	ComplexNumber* m_pFFTBuf=new ComplexNumber[width*height];
	memset(m_pFFTBuf, 0, sizeof(ComplexNumber)*width*height);


	//图像数据变成复数类型存入m_pFFTBuf
	for(int i=0;i<width*height;i++)
	{
		m_pFFTBuf[i].real=In_img[i];
		m_pFFTBuf[i].imag=0;
	}

	//申请ComplexNumber结构体数组,长度为height
	ComplexNumber *array=new ComplexNumber[height];

	pro1->SetMessage("正在进行纵向傅里叶变换。。");
	//先纵向一维快速傅立叶变换
	for(int u=0;u<width;u++)
	{
		for(int v=0;v<height;v++)
		{
			array[v].real=m_pFFTBuf[v*width+u].real;
			array[v].imag=m_pFFTBuf[v*width+u].imag;
		}
		ImageFFt1D(array, height);//计算纵向傅里叶
		for(int v=0;v<height;v++)//存储结果
		{
			m_pFFTBuf[v*width+u].real=array[v].real;
			m_pFFTBuf[v*width+u].imag=array[v].imag;
			pro1->StepIt();
		}
	}
	delete []array;
	//再横向一维快速傅立叶变换
	for(int v=0;v<height;v++)
	{
		ImageFFt1D(m_pFFTBuf+v*width, width);
	}

	pro1->SetMessage("正在存储频谱图。。");
	pro1->ReSetProcess();
	//将频谱图以图像形式存入imgBufOut
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

			Out_img[0][i*width+j]=m_pFFTBuf[i0*width+j0].real;//存储实部
			Out_img[1][i*width+j]=m_pFFTBuf[i0*width+j0].imag;//存储虚部

			//用于显示波段
			if(t>255)
				Out_img[2][i*width+j]=255;//存储实部
			else 
				Out_img[2][i*width+j]=t;

			pro1->StepIt();
		}
	}
	
}
//一维傅里叶
void CArthmetic::ImageFFt1D(ComplexNumber *arrayBuf, int n)
{
	//申请临时复数缓冲区buf1，长度为n
	ComplexNumber *buf1=new ComplexNumber[n];

	//将arrayBuf拷贝进buf1
	memcpy(buf1,arrayBuf,sizeof(ComplexNumber)*n);

	//申请临时复数缓冲区buf2，长度为n
	ComplexNumber *buf2=new ComplexNumber[n];

	//将arrayBuf数组元素基2抽取并重新排列
	//若0、1、2、3、4、5、6、7八点序列对调后变作0、4、2、6、1、5、3、7
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


	//采用蝶型算法进行快速傅立叶变换
	//buf1是第r级的输入，buf2存放第r级的输出
	float c,s;
	for(int r=1;pow(2.0f,r)<=n;r++)
	{
		t1=pow(2.0f,r);
		for(int k=0;k<n/t1;k++)
		{
			for(int i=t1/2;i<t1;i++)
			{
				//加权因子
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

		//第r级的输出存入buf1,作为下一级的输入数据
		memcpy(buf1,buf2,sizeof(ComplexNumber)*n);
	}
	//傅立叶变换的结果存入arrayBuf
	memcpy(arrayBuf,buf2,sizeof(ComplexNumber)*n);

	//释放缓冲区
	delete[]buf2;
	delete[]buf1;
}
//一维傅里叶反变换
void CArthmetic::ImageRevFFt1D(ComplexNumber *arrayBuf, int n)
{
	//变量
	int i;

	//对数组arrayBuf求共轭
	for(i=0;i<n;i++)
		arrayBuf[i].imag=-arrayBuf[i].imag;

	//一维快速傅立叶变换
	ImageFFt1D(arrayBuf, n);

	//时域数据求共轭，并除以N
	for(i=0;i<n;i++)
	{
		arrayBuf[i].real=arrayBuf[i].real/n;
		arrayBuf[i].imag=-arrayBuf[i].imag/n;
	}

}
// 二维快速傅里叶反变换
void CArthmetic::ImageRevFFt2D(double* In_img, int width, int height,BYTE* Out_img)
{
	//若傅立叶变换缓冲区为空,则失败返回
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height); //设置进度条的总步长

	ComplexNumber* m_pFFTBuf=new ComplexNumber[width*height];
	memset(m_pFFTBuf, 0, sizeof(ComplexNumber)*width*height);

	//图像数据变成复数类型存入m_pFFTBuf
	for(int i=0;i<width*height;i++)
	{
		m_pFFTBuf[i].real=In_img[i];
		m_pFFTBuf[i].imag=In_img[width*height+i];
	}

	if(!m_pFFTBuf)
		return;

	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height); //设置进度条的总步长
	//变量
	int i, u, v;

	//先纵向傅立叶反变换
	pro1->SetMessage("纵向傅立叶反变换..");
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

	//再横向傅立叶反变换
	for(v=0;v<height;v++)
	{
		ImageRevFFt1D(m_pFFTBuf+v*width, width);
	}

	//求m_pFFTBuf的幅度谱,恢复图像存入imgBufOut
	pro1->SetMessage("回复影像中..");
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
//二维转一维
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
//二维转一维
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
//一维转二维
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
//二维转一维
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
// 离散傅里叶
void CArthmetic::FFT(BYTE* indata,int width,int hight,double** outdata)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*hight*width*hight); //设置进度条的总步长
	pro1->SetMessage("正在进行离散傅立叶变换..");

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
// 离散傅里叶逆
void CArthmetic::FTTN(double* In_img, int width, int height,BYTE* Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height*width*height); //设置进度条的总步长
	pro1->SetMessage("正在进行离散傅立叶反变换..");

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
					m_pFFTBuf[i*width+j].real=In_img[i*width+j];//获取傅里叶影像的实部和虚部
					m_pFFTBuf[i*width+j].imag=In_img[width*height+i*width+j];

					double Angle = (double(i1*i)/double(height)+double(j1*j)/double(width))*(2.0*PI);
					m_pFFTBuf1.real=cos(Angle);
					m_pFFTBuf1.imag=sin(Angle);
					Real+=m_pFFTBuf[i*width+j].real*m_pFFTBuf1.real-m_pFFTBuf[i*width+j].imag*m_pFFTBuf1.imag;
					Image+=m_pFFTBuf[i*width+j].real*m_pFFTBuf1.imag+m_pFFTBuf[i*width+j].imag*m_pFFTBuf1.real;

					pro1->StepIt();
				}
			}
			Out_img[width*i1+j1]=sqrt(double(Real)*double(Real)-double(Image)*double(Image));//存储反变换影像结果
		}
	}
}
// 计算影像协方差
double** CArthmetic::XieFangCha(BYTE* value,int nBand,int lie,int hang)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(nBand*nBand*hang*lie); //设置进度条的总步长
	pro1->SetMessage("正在计算波段间协方差..");
	double **Xie= new double *[nBand];//用来存储不同波段间的协方差
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
				Ave[Bandi] +=value[Bandi*hang*lie+i*lie+j];//求解波段的像素和

			}
			Ave[Bandi] = Ave[Bandi]/(hang*lie);//求解波段的均值
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

				Xie[i][j] = Xie[i][j]/(hang*lie);//求解ij两波段的协方差值
		}

		for(int i = 0;i<nBand;i++)
			for(int j = i ; j<nBand;j++)
			{
				Xie[j][i] = Xie[i][j];//补齐空缺的值
			}

	return Xie;
}
/********************************************************************************
* 求实对称矩阵的特征值及特征向量的雅格比法
* 利用雅格比(Jacobi)方法求实对称矩阵的全部特征值及特征向量
* 返回值小于0表示超过迭代jt次仍未达到精度要求
* 返回值大于0表示正常返回
* a-长度为n*n的数组，存放实对称矩阵，返回时对角线存放n个特征值
* n-矩阵的阶数
* v-长度为n*n的数组，返回特征向量(按列存储)
* eps-控制精度要求
* jt-整型变量，控制最大迭代次数
*********************************************************************************/
int CArthmetic::Eigs(double **a, int n, double *d, double **v, int& nrot)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(n*n); //设置进度条的总步长
	pro1->SetMessage("正在计算特征向量..");

	
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
//矩阵乘法的函数
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
// 投影变换
void CArthmetic::ProTran(CString ReferenceFileName,CString OriFIleName)
{
	GDALDatasetH  hSrcDS, hDstDS;

	// 打开输入输出图像
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	hSrcDS = GDALOpen(ReferenceFileName, GA_ReadOnly );
	hDstDS = GDALOpen(OriFIleName, GA_Update );


	poDataset = (GDALDataset *) GDALOpen(OriFIleName, GA_ReadOnly );
	if( poDataset == NULL )
	{
		AfxMessageBox("文件打开失败!!!");

	}
	int nBand=poDataset->GetRasterCount();     //获得波段数
	
	// 建立变换选项
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
	pro1->SetMessage("正在进行投影变换..");
	psWarpOptions->pfnProgress = ALGTermProgress; 

	   // 创建重投影变换函数
   psWarpOptions->pTransformerArg =
        GDALCreateGenImgProjTransformer( hSrcDS,
                                         GDALGetProjectionRef(hSrcDS),
                                        hDstDS,
                                         GDALGetProjectionRef(hDstDS),
                                         FALSE,0.0, 1 );
   psWarpOptions->pfnTransformer = GDALGenImgProjTransform;
   psWarpOptions->eWorkingDataType=GDT_Byte;

      // 初始化并且执行变换操作
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
//高通滤波
void CArthmetic::Hightong(BYTE *PImg,int m_cols,int m_rows)
{
	//定义模版
	int *temp=new int[9];
	temp[0]=0,temp[1]=-1,temp[2]=0;
	temp[3]=-1,temp[4]=5;temp[5]=-1;
	temp[6]=0,temp[7]=-1,temp[8]=0;


	//使用模版进行高通滤波
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
//空间域不加权影像融合
void CArthmetic::Unweighting(BYTE *MImg,BYTE *PImg,int m_cols,int m_rows)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(m_cols*m_rows); //设置进度条的总步长
	pro1->SetMessage("正在进行融合操作..");

	Hightong(PImg,m_cols,m_rows);//对高分辨率全色影像进行高通滤波

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
//空间域加权影像融合
void CArthmetic::Weighting(BYTE *MImg,BYTE *PImg,int m_cols,int m_rows) 
{
	// TODO: Add your command handler code here
	double *m_r=new double[3];//相关系数

	float m_gyaver=0;
	//计算灰度图像的像素平均值
	for (int i=0;i<m_rows*m_cols;i++)
	{
		ASSERT(PImg[i]>=0&&PImg[i]<=255);
		m_gyaver+=PImg[i];
	}
	m_gyaver=m_gyaver/(m_rows*m_cols);

	//计算彩色图像各个波段的像素平均值
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

	//计算相关系数
	float *m_der=new float[3];//分子部分
	float *m_num1=new float[3];//分母第一部分
	float *m_num2=new float[3];//分母第二部分

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

	//形成新的图像

	for (int k=0;k<m_cols*m_rows;k++)
	{
		for (int i=0;i<3;i++)
		{
			BYTE index=(BYTE)(((1-m_r[i])*PImg[k]+(1+m_r[i])*MImg[k*3+i])/2);
			MImg[k*3+i]=index;
		}
	}
}
//添加高斯噪声
void CArthmetic::AddGaussNoise(BYTE* In_img, int width,int height,int nband,BYTE* Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height*nband); //设置进度条的总步长
	pro1->SetMessage("正在产生高斯噪声..");
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
//添加椒盐噪声
void CArthmetic::AddPepperSaltNoise(BYTE* In_img, int width,int height,int nband,BYTE* Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height*nband); //设置进度条的总步长
	pro1->SetMessage("正在产生椒盐噪声..");
	for (int k=0;k<nband;k++)
	{
		for (int i=0;i<width*height;i=i+3)
		{
			int num=rand();
			if (num>32000) 
			{
				In_img[k*width*height+i]=(rand())%255;        //处理每一个像素的RGB值
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
//卷积运算
void CArthmetic::Convolution(BYTE *PImg,double* temp,int m_cols,int m_rows,int nBand)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(m_cols*m_rows*nBand); //设置进度条的总步长
	pro1->SetMessage("正在进行卷积运算..");
	//使用模版进行高通滤波
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
// Robert算法
void CArthmetic::Robert(BYTE* In_img, int width,int height,int nband,BYTE* Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height*nband); //设置进度条的总步长
	pro1->SetMessage("正在进行Robert运算..");

	for (int k=0;k<nband;k++)
	{
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<width;j++)
			{
				//x方向梯度
				int x=(k*height*width+In_img[k*height*width+(i+1)*width+j+1]-In_img[k*height*width+i*width+j]);

				//y方向梯度
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
// Sobel算法
void CArthmetic::Sobel(BYTE *PImg,BYTE* Out_img,int m_cols,int m_rows,int nBand,int chidu)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(m_cols*m_rows*nBand); //设置进度条的总步长
	pro1->SetMessage("正在进行Sobel运算..");

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
// Prewitt算子
void CArthmetic::Prewitt(BYTE *PImg,BYTE* Out_img,int m_cols,int m_rows,int nBand,int chidu)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(m_cols*m_rows*nBand); //设置进度条的总步长
	pro1->SetMessage("正在进行Prewitt运算..");

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
//边界追踪
void CArthmetic::OnEdgeTrace(BYTE* In_img, int width,int height,BYTE* Out_img)
{
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height); //设置进度条的总步长
	pro1->SetMessage("正在进行边界追踪..");
	//顺时针定义中心像素点的8邻域坐标，第一列为x方向的偏移,第二列为y方向的偏移
	int direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};

	//边界起始点,待处理的当前点,当前点的邻域点
	CPoint startP, currentP, neighborP;

	//是否当前点与起始边界点重合的标志变量
	int findStartPoint=0;

	//判断是否是二值图像
	for(int j=0; j<width*height;j++)
	{
		if(In_img[j]!=0&&In_img[j]!=255)
		{
			AfxMessageBox("请输入二值图像！");
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
		//已经找到起始边界点
		if(findStartPoint)
			break;
	}

	//边界跟踪
	//从初始点开始跟踪
	currentP.x=startP.x;
	currentP.y=startP.y;

	//邻域点是否边界点标志变量
	int isContourP;

	//开始方向
	int startDirect=0;

	//0表示还没有返回最初的边界起始点
	findStartPoint=0;
	while(findStartPoint==0)
	{
		isContourP=false;
		while(isContourP==false)
		{
			pro1->StepIt();

			neighborP.x=currentP.x+direction[startDirect][0];
			neighborP.y=currentP.y+direction[startDirect][1];

			//搜索到邻域点
			if(In_img[neighborP.y*width+neighborP.x]==0)
			{
				isContourP=true;
				currentP.x=neighborP.x;
				currentP.y=neighborP.y;

				if(currentP.x==startP.x&&currentP.y==startP.y)
					findStartPoint=true;//回到边界起始点了

				Out_img[neighborP.y*width+neighborP.x]=0;

				//扫描方向逆时针旋转90度
				startDirect-=2;
				if(startDirect<0)
					startDirect+=8;
			}
			else
			{
				//扫描方向顺时针旋转45度
				startDirect++;
				if(startDirect==8)
					startDirect=0;
			}
		}
	}
}
//区域增长
void CArthmetic::RegionGrow(BYTE* In_img, int width,int height,int nBand,BYTE* Out_img,CPoint SeedPos, int thresh)
{
	//二维数组direction代表中心像素点8邻域坐标与该点在x和y方向上的偏移,
	//其中第一列为x方向的偏移,第二列为y方向的偏移
	pro1->ReSetProcess();  
	pro1->SetStepCount(width*height); //设置进度条的总步长
	pro1->SetMessage("正在进行区域增长..");

	if (nBand==3)
	{
		for (int i=0;i<nImageSizeX*nImageSizeY;i++)
		{
			In_img[i]=In_img[nImageSizeX*nImageSizeY*2+i]*0.144+In_img[nImageSizeX*nImageSizeY+i]*0.587+In_img[i]*0.299;//RGB转灰度
		}
	}

	//将输出图像初始化置255
	memset(Out_img,255,width*height);

	int direction[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

	//栈申请，此处假定进栈的像素最多为图像总像素数
	CPoint *stack=new CPoint[width*height];

	//栈顶指针
	int top;

	//当前正处理的点和弹出的点
	CPoint currentPoint, popPoint;

	//循环变量，遍历array数组的第一维下标
	int k;

	//标记变量
	int label;

	//临时变量
	int temp1, temp2;

	//记录种子像素的灰度值
	temp1=In_img[SeedPos.y*width+SeedPos.x];

	//将给定种子点置标记0,入栈
	Out_img[SeedPos.y*width+SeedPos.x]=0;
	top=0;
	stack[top].x=SeedPos.x;
	stack[top].y=SeedPos.y;

	//堆栈
	while(top>-1)
	{
		pro1->StepIt();
		//弹出栈顶元素,该元素已经生长过
		popPoint.x=stack[top].x;
		popPoint.y=stack[top].y;
		top--;

		//考察弹出像素周围是否有没有生长的像素
		for(k=0;k<8;k++)
		{

			//待考察的邻域点
			currentPoint.x=popPoint.x+direction[k][0];
			currentPoint.y=popPoint.y+direction[k][1];

			//如果待考察的点不在图像内，则跳过
			if(currentPoint.x<0||currentPoint.x>width-1||
				currentPoint.y<0||currentPoint.y>height-1)
				continue;

			//该点标号
			label=Out_img[currentPoint.y*width+currentPoint.x];

			//弹出的点周围有尚没生长的点
			if(label==255)
			{
				temp2=In_img[currentPoint.y*width+currentPoint.x];
				//如果当前被考察的像素灰度值与种子点灰度值之差小于给定的阈值,
				//则认为相似,将其进栈处理
				if(abs(temp1-temp2)<thresh)
				{
					//给该点置生长标记0
					Out_img[currentPoint.y*width+currentPoint.x]=0;
					top++;
					stack[top].x=currentPoint.x;
					stack[top].y=currentPoint.y;
				}
			}
		}

	}
	//清除缓冲区
	delete []stack;
}
//像素坐标转换为地理坐标
void Trans(double * Transform,double x,double y,double &l,double & p)
{

	l  = Transform[0]+Transform[1]*y+Transform[2]*x;
	p = Transform[3]+Transform[4]*y+Transform[5]*x;

}
//计算地理坐标到像素坐标的变换公式
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
//由地理坐标反算像素坐标
void Tran2(double * Transform,double l,double p,double & x,double &y)
{
	CPoint point ;
	x = Transform[0]+Transform[1]*p+Transform[2]*l;
	y = Transform[3]+Transform[4]*p+Transform[5]*l;

}
//计算影像分辨率
void caculFBL(int width,int heigth,double l1,double p1,double l2,double p2,double& FBLx,double& FBLy)
{
	double geoWidth=l2-l1;
	double geoHeigth=p2-p1;

	FBLx=abs(geoWidth/(double)width);
	FBLy=abs(geoHeigth/(double)heigth);
}
//影像裁剪
int CArthmetic::ImgSubset(CPoint begin,CPoint end,BYTE* In_img,BYTE* Out_img,CString OutPath,CString ImageType)
{
	pro1->ReSetProcess();  
	pro1->SetMessage("正在进行影像裁剪..");
	pro1->SetStepCount(100);
	pro1->SetPosition(0);

	m_Gcps=new GDAL_GCP[2];
	double l1,p1,l2,p2;
	double * Transform=new double[6];
	poDataset->GetGeoTransform(Transform);
	Trans(Transform,(double)begin.x,(double)begin.y,l1,p1);
	//记录左上角点的地理和像素坐标
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
	poDstDS = poDriver->Create( (char*)(LPCTSTR)OutPath,//输出路径
		nImageSizeX,nImageSizeY, //影像数据宽高
		bandcount, //存或取的波段数
		GDT_Byte, 	//缓冲区数据类型
		papszOptions );

	poDstDS->SetGeoTransform(Transform);
	poDstDS->SetProjection(pszSRS_WKT);

	if(poDstDS->RasterIO(
		GF_Write,						//IO方式
		0,0,							//影像数据起始位置x,y
		nImageSizeX,nImageSizeY,				//影像数据宽高
		Out_img,						//指向缓冲区数据指针
		nImageSizeX,nImageSizeY,				//缓冲区数据宽高
		GDT_Byte,						//缓冲区数据类型
		bandcount,						//存或取的波段数
		NULL,							//存或取的波段列表
		0,								//缓冲区数据一个像素占用的字节数
		0,								//缓冲区一行数据所占用的字节数
		0)==CE_Failure )				//缓冲区数据存储方式 0为RRRRGGGGBBBB-bsp 1为RGBRGBRGB-bip	
	{
		AfxMessageBox("保存文件失败！");
		return 0;
	}
	pro1->SetMessage("正在保存影像。。");
	pro1->SetPosition(100);

	GDALClose(poDataset);
	GDALClose(poDstDS);
	delete[] m_Gcps;

	return 1;
}
// 重采样
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
		AfxMessageBox(_T("打开文件失败！！！"));
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
	pro1->SetMessage("正在重采样影像。。");

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
* 函数名称： MatrixSampleRow()
* 参数： unsigned char *matrixInput 待采样矩阵数组指针
*        unsigned char *matrixOutputOdd 奇数行采样生成的矩阵
*        unsigned char *matrixOutputEven 偶数行采样生成的矩阵
*        int heightOutput 输出矩阵高度
*        int widthOutput 输出矩阵宽度
*        int widthInput 输入矩阵宽度
* 返回值： void
* 说明： 对输入矩阵进行行抽点采样
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
* 函数名称： MatrixSub()
* 参数： unsigned char *matrixA 待求差矩阵A数组指针
*        unsigned char *matrixB 待求差矩阵B数组指针
*        int height 高度
*        int width 宽度
*        unsigned char *result 差矩阵数组指针
* 返回值： void
* 说明： 将输入的两个矩阵A和B求差
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
 * 函数名称： MatrixSampleLine()
 * 参数： unsigned char *matrixInput 待采样矩阵数组指针
 *        unsigned char *matrixOutputOdd 奇数列采样生成的矩阵
 *        unsigned char *matrixOutputEven 偶数列采样生成的矩阵
 *        int heightOutput 输出矩阵高度
 *        int widthOutput 输出矩阵宽度
 *        int widthInput 输入矩阵宽度
 * 返回值：void
 * 说明： 对输入矩阵进行列抽点采样
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
 * 函数名称： MatrixSub()
 * 参数： unsigned char *matrixA 待求和矩阵A数组指针
 *        unsigned char *matrixB 待求和矩阵B数组指针
 *        int height 高度
 *        int width 宽度
 *        unsigned char *result 和矩阵数组指针
 * 返回值：void
 * 说明： 将输入的两个矩阵A和B求和
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
* 函数名称：
* MatrixRegionCopy()
* 参数： unsigned char *matrixRegion 源矩阵数组指针
*        int heightRegion 复制区域的高度
*        int widthRegion 复制区域的宽度
*        unsigned char *matrixDest 目的矩阵数组指针
*        int heightDest 目的矩阵区域的高度
*        int widthDest 目的矩阵区域的宽度
* 返回值：void
* 说明： 将源矩阵的指定区域数据复制到目的矩阵的指定区域
**********************************************************************/
void CArthmetic::MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,unsigned char *matrixDest, int heightDest, int widthDest)
{
	//计算区域高度和宽度
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
* 函数名称：WavlDecmposeTrans()
* 参数: unsigned char *inputData 输入矩阵数组指针
*       int inputHeight 输入矩阵高度
*       int inputWidth 输入矩阵宽度
*       int lineByte 矩阵行字节数
*       int scale 分解尺度
*       unsigned char *outputData 输出矩阵数组指针
* 返回值：void
* 说明：进行一遍小波分解
************************************************************************/
void CArthmetic::WavlDecmposeTrans(unsigned char *inputData, int inputHeight, int inputWidth,int OutImageSizeX,int OutImageSizeY,int scale, unsigned char *outputData)
{
	unsigned char *buff = new unsigned char[OutImageSizeX*OutImageSizeY];

	int imageHeight = inputHeight;
	int imageWidth = inputWidth;

	unsigned char *temp1 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	/**/
	//对图像矩阵进行行采样
	MatrixSampleRow(inputData, temp1, temp2, (imageHeight/2), imageWidth, OutImageSizeX);
	//行差分，得到高频与低频数据
	MatrixSub(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//对低频数据进行列采样
	MatrixSampleLine(temp2, temp01, temp00, (imageHeight/2), (imageWidth/2), imageWidth);
	//列差分得到LL和LH
	MatrixSub(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//对高频数据进行列采样
	MatrixSampleLine(temp1, temp11, temp10, (imageHeight/2), (imageWidth/2), imageWidth);
	//列差分，得到HL和HH
	MatrixSub(temp11, temp10, (imageHeight/2), (imageWidth/2), temp11);

	MatrixRegionCopy(temp01, (imageHeight/2), (imageWidth/2),
		buff+OutImageSizeX*(int)(imageHeight/2)+(int)(imageWidth/2), imageHeight, OutImageSizeX);
	MatrixRegionCopy(temp10, (imageHeight/2), (imageWidth/2), buff, imageHeight, OutImageSizeX);
	MatrixRegionCopy(temp11, (imageHeight/2), (imageWidth/2), buff+(int)(imageWidth/2), imageHeight, OutImageSizeX);
	//释放空间
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;

	scale--;
	//继续对LL进行递归分解
	if (scale > 0)
	{
		WavlDecmposeTrans(temp00,imageHeight/2, imageWidth/2, imageWidth/2,OutImageSizeY,scale,temp00);
	}
	MatrixRegionCopy(temp00, (imageHeight/2), (imageWidth/2), buff+OutImageSizeX*(int)(imageHeight/2),
		imageHeight, OutImageSizeX);
	delete[] temp00;

	//把最后的结果拷贝到m_pImgDataOut
	memcpy(outputData, buff,  OutImageSizeX * inputHeight);
	delete[] buff;
}
/**********************************************************************
* 函数名称： SimpleWaveletTrans()
* 参数：     int scale 分解尺度，默认为1
* 返回值：   void
* 说明：     对图像数据进行给定尺度的小波分解
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
//分水岭分割
/*===================================================================
函数名          ：    Watershed 

功能            ：    用标记-分水岭算法对输入图像进行分割 

算法实现        ：    无 

输入参数说明    ：    OriginalImage --输入图像（灰度图，0~255） 
                      SeedImage   --标记图像（二值图，0-非标记，1-标记）
                      LabelImage   --输出图像（1-第一个分割区域，2-第二个分割区域，...） 
                      row       --图像行数 
                      col       --图像列数 
返回值说明      ：    无     

====================================================================*/ 
void CArthmetic::Watershed(unsigned char **OriginalImage,char** SeedImage, BYTE **LabelImage, int row, int col)
{
	// using namespace std;

	//标记区域标识号，从1开始
	int Num=0;
	int i,j;

	//保存每个队列种子个数的数组
	vector<int*> SeedCounts;
	//临时种子队列
	queue<POINT> quetem;
	//保存所有标记区域种子队列的数组，里面放的是种子队列的指针
	vector<queue<POINT>*> vque;

	int* array;
	//指向种子队列的指针
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

	//预处理,提取区分每个标记区域，并初始化每个标记的种子队列
	//种子是指标记区域边缘的点，他们可以在水位上升时向外淹没（或者说生长）
	//pan's words：我的理解是梯度值较小的象素点，或者是极小灰度值的点。
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			//如果找到一个标记区域
			if(SeedImage[i][j]==1)
			{
				//区域的标识号加一
				Num++;
				//分配数组并初始化为零，表示可有256个灰阶
				array=new int[256];
				ZeroMemory(array,256*sizeof(int));
				//种子个数数组进vector，每次扫描则生成一个数组，并用区域标识号来做第一维。灰度级做第二维。
				//表示某个盆地区域中某灰阶所对应的点的数目。
				SeedCounts.push_back(array);
				//分配本标记号的优先队列，256个种子队列，
				//表示对应一个灰阶有一个队列，并且每个队列可以存储一个集合的点信息
				pque=new queue<POINT>[256];
				//加入到队列数组中，对应的是本标记号Num的
				vque.push_back(pque);
				//当前点放入本标记区域的临时种子队列中
				temp.x=i;
				temp.y=j;
				quetem.push(temp);
				//当前点标记为已处理
				LabelImage[i][j]=Num;
				SeedImage[i][j]=127;//表示已经处理过

				//让临时种子队列中的种子进行生长直到所有的种子都生长完毕
				//生长完毕后的队列信息保存在vque中，包括区域号和灰阶，对应点数存储在seedcounts中
				while(!quetem.empty())
				{
					up=down=right=left=FALSE;
					upleft=upright=downleft=downright=FALSE;
					//队列中取出一个种子
					temp=quetem.front();
					m=temp.x;
					n=temp.y;
					quetem.pop();
					//注意到127对扫描过程的影响，影响下面的比较，但是不影响while语句中的扫描

					if(m>0)
					{
						//上方若为可生长点则加为新种子
						if(SeedImage[m-1][n]==1)
						{
							temp.x=m-1;
							temp.y=n;
							quetem.push(temp);//如果这样的话，那么这些标记过的区域将再次在while循环中被扫描到，不会，因为值是127
							//新种子点标记为已淹没区域，而且是当前区域，并记录区域号到labelImage
							LabelImage[m-1][n]=Num;
							SeedImage[m-1][n]=127;
						}
						else//否则上方为不可生长
						{
							up=TRUE;
						}
					}
					if(m>0&&n>0)
					{
						if(SeedImage[m-1][n-1]==1)//左上方若为可生长点则加为新种子
						{
							temp.x=m-1;
							temp.y=n-1;
							quetem.push(temp);
							//新种子点标记为已淹没区域，即下一个循环中以127来标识不再扫描，而且是当前区域
							LabelImage[m-1][n-1]=Num;
							SeedImage[m-1][n-1]=127;
						}
						else//否则左上方为不可生长
						{
							upleft=TRUE;
						}
					}

					if(m<row-1)
					{
						if(SeedImage[m+1][n]==1)//下方若为可生长点则加为新种子
						{
							temp.x=m+1;
							temp.y=n;
							quetem.push(temp);
							//新种子点标记为已淹没区域，而且是当前区域
							LabelImage[m+1][n]=Num;
							SeedImage[m+1][n]=127;
						}
						else//否则下方为不可生长
						{
							down=TRUE;
						}
					}
					if(m<(row-1)&&n<(col-1))
					{
						if(SeedImage[m+1][n+1]==1)//下方若为可生长点则加为新种子
						{
							temp.x=m+1;
							temp.y=n+1;
							quetem.push(temp);
							//新种子点标记为已淹没区域，而且是当前区域
							LabelImage[m+1][n+1]=Num;
							SeedImage[m+1][n+1]=127;
						}
						else//否则下方为不可生长
						{
							downright=TRUE;
						}
					}

					if(n<col-1)
					{
						if(SeedImage[m][n+1]==1)//右方若为可生长点则加为新种子
						{
							temp.x=m;
							temp.y=n+1;
							quetem.push(temp);
							//新种子点标记为已淹没区域，而且是当前区域
							LabelImage[m][n+1]=Num;
							SeedImage[m][n+1]=127;
						}
						else//否则右方为不可生长
						{
							right=TRUE;
						}
					}
					if(m>0&&n<(col-1))
					{
						if(SeedImage[m-1][n+1]==1)//右上方若为可生长点则加为新种子
						{
							temp.x=m-1;
							temp.y=n+1;
							quetem.push(temp);
							//新种子点标记为已淹没区域，而且是当前区域
							LabelImage[m-1][n+1]=Num;
							SeedImage[m-1][n+1]=127;
						}
						else//否则右上方为不可生长
						{
							upright=TRUE;
						}
					}

					if(n>0)
					{
						if(SeedImage[m][n-1]==1)//左方若为可生长点则加为新种子
						{
							temp.x=m;
							temp.y=n-1;
							quetem.push(temp);
							//新种子点标记为已淹没区域
							LabelImage[m][n-1]=Num;
							SeedImage[m][n-1]=127;
						}
						else//否则左方为不可生长
						{
							left=TRUE;
						}
					}
					if(m<(row-1)&&n>0)
					{
						if(SeedImage[m+1][n-1]==1)//左下方若为可生长点则加为新种子
						{
							temp.x=m+1;
							temp.y=n-1;
							quetem.push(temp);
							//新种子点标记为已淹没区域
							LabelImage[m+1][n-1]=Num;
							SeedImage[m+1][n-1]=127;
						}
						else//否则左方为不可生长
						{
							downleft=TRUE;
						}
					}

					//上下左右只要有一点不可生长，那么本点为初始种子队列中的一个
					//这里可否生长是由seedimage中的值来决定的。
					if(up||down||right||left||
						upleft||downleft||upright||downright)
					{
						temp.x=m;
						temp.y=n;
						//下面这个矢量数组：第一维是标记号；第二维是该图像点的灰度级
						//m,n点对应的是while循环里面扫描的像素点。
						//Num是当前的区域号
						//这样这个二维信息就表示了，某个区域中对应某个灰度级对应的成员点的集合与个数
						//分别由下面两个量来表达
						vque[Num-1][OriginalImage[m][n]].push(temp);//这两句中我把Num-1改成了Num...pan's codes...
						SeedCounts[Num-1][OriginalImage[m][n]]++;
					}     
				}//while结束，扫描到quetem为空而止。也就是对应所有的节点都得到不可生长为止（或者是周围的点要么不可生长，要么已生长）
			}//if结束
			// if(Num==5)
			// return;
		}
	}
	//在上述过程中，如果标记的点为0则表示，没有扫描到的点，或者表明不是输入的种子点
	//这里相当于是找seedimage传过来的初始区域的分水岭界线的所有的点；并且用标号记录每个区域，同时集水盆的边缘点进入队列。
	//上面是找集水盆的程序。同时也是连通区域。
	/*************************************/
	//test 这里测试一下剩下的非水盆地的点数。
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
	bool actives;//在某一水位处，所有标记的种子生长完的标志
	int WaterLevel;

	//淹没过程开始，水位从零开始上升，水位对应灰度级，采用四连通法
	for(WaterLevel=0;WaterLevel<180;WaterLevel++)//第二维。。。
	{
		actives=true;
		while(actives)
		{
			actives=false;
			//依次处理每个标记号所对应的区域，且这个标记号对应的区域的点的个数在SeedCounts里面
			for(i=0;i<Num;i++)//第一维。。。
			{
				if(!vque[i][WaterLevel].empty())//对应的分水岭不为空集,i表示区域号，waterlevel表示灰阶
				{
					actives=true;
					while(SeedCounts[i][WaterLevel]>0)
					{
						SeedCounts[i][WaterLevel]--;//取出一个点，个数少一
						temp=vque[i][WaterLevel].front();//取出该区域的一个点，清空这个边缘点，表示当前
						//灰度级该像素已经处理掉了。
						vque[i][WaterLevel].pop();
						m = temp.x;
						n = temp.y;//当前种子的坐标
						if(m>0)
						{
							if(!LabelImage[m-1][n])//上方若未处理，表示没有标号，应该在输入前已经作过初始化为0
								//本函数中在开头也作过初始化
							{
								temp.x=m-1;
								temp.y=n;
								LabelImage[m-1][n]=i+1;//上方点标记为已淹没区域
								//注意到这个标记是与扫描点的区域号相同，一定在这个标号所属的区域吗？是的
								//这样在下一轮至少会扫描到这个点，确保不遗漏，但是下一轮的处理会使它合理
								//归类吗？问题还有这样标记并没有一定将它加入到种子队列。也就是说它
								//只是被淹没而不能向上淹没。只有满足下述可生长条件才行。
								if(OriginalImage[m-1][n]<=WaterLevel)//上方若为可生长点则加入当前队列，当前高度的队列
								{
									vque[i][WaterLevel].push(temp);
								}
								else//否则加入OriginalImage[m-1][n]对应的灰度级的队列，为什么？
								{
									vque[i][OriginalImage[m-1][n]].push(temp);
									SeedCounts[i][OriginalImage[m-1][n]]++;
								}
							}
						}

						if(m<row-1)
						{
							if(!LabelImage[m+1][n])//下方若未处理
							{
								temp.x=m+1;
								temp.y=n;
								LabelImage[m+1][n]=i+1;//下方点标记为已淹没区域

								if(OriginalImage[m+1][n]<=WaterLevel)//下方若为可生长点则加入当前队列
								{
									vque[i][WaterLevel].push(temp);
								}
								else//否则加入OriginalImage[m+1][n]级队列
								{
									vque[i][OriginalImage[m+1][n]].push(temp);
									SeedCounts[i][OriginalImage[m+1][n]]++;
								}
							}
						}

						if(n<col-1)
						{
							if(!LabelImage[m][n+1])//右边若未处理
							{
								temp.x=m;
								temp.y=n+1;
								LabelImage[m][n+1]=i+1;//右边点标记为已淹没区域

								if(OriginalImage[m][n+1]<=WaterLevel)//右边若为可生长点则加入当前队列
								{
									vque[i][WaterLevel].push(temp);
								}
								else//否则加入OriginalImage[m][n+1]级队列
								{
									vque[i][OriginalImage[m][n+1]].push(temp);
									SeedCounts[i][OriginalImage[m][n+1]]++;
								}
							}
						}

						if(n>0)
						{
							if(!LabelImage[m][n-1])//左边若未处理
							{
								temp.x=m;
								temp.y=n-1;
								LabelImage[m][n-1]=i+1;//左边点标记为已淹没区域

								if(OriginalImage[m][n-1]<=WaterLevel)//左边若为可生长点则加入当前队列
								{
									vque[i][WaterLevel].push(temp);
								}
								else//否则加入OriginalImage[m][n-1]级队列
								{
									vque[i][OriginalImage[m][n-1]].push(temp);
									SeedCounts[i][OriginalImage[m][n-1]]++;
								}
							}
						}
					}//while循环结束
					SeedCounts[i][WaterLevel]=vque[i][WaterLevel].size();
				}//if结束
			}//for循环结束
		}//while循环结束
	}//for循环结束

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
	str.Format("new region num:%d",nonzeronum);//这表示的是可扩张的点
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

//地理坐标转换为栅格坐标
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
//栅格坐标转换为地理坐标
bool ImageRowCol2Projection(double *adfGeoTransform, int iCol, int iRow, double &dProjX, double &dProjY)
{
	//adfGeoTransform[6]  数组adfGeoTransform保存的是仿射变换中的一些参数，分别含义见下
	//adfGeoTransform[0]  左上角x坐标 
	//adfGeoTransform[1]  东西方向分辨率
	//adfGeoTransform[2]  旋转角度, 0表示图像 "北方朝上"
	//adfGeoTransform[3]  左上角y坐标 
	//adfGeoTransform[4]  旋转角度, 0表示图像 "北方朝上"
	//adfGeoTransform[5]  南北方向分辨率

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