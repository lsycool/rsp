// BP.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "BP.h"
#include "afxdialogex.h"
#include <math.h>
#include <stdlib.h>



#define  ABS(x)  (((x)>0.0)?(x):(-(x)))

//有参数的宏定义（执行速度较快）宏定义换行要用“\”来连接：快速复制
#define  fastcopy(to,from,len){\
	char *_to,*_from;\
	int _i,_l;\
	_to=(char*)(to);\
	_from=(char*)(from);\
	_l=(len);\
	for (_i=0;_i<_l;_i++) *_to++=*_from++;\
}

//返回0~1的双精度随机数
double drnd()
{
	return ((double)rand()/(double)BIGRND);
}

//返回-1~1之间的双精度随机数
double dpn1()
{
	return((drnd()*2.0)-1.0);
}

//作用函数，S型函数（有两种类型，目前只用对数正切类型--输入与输出成对数正切关系）
double squash(double x)
{
	return(1.0/1.0+exp(-x));//(exp==自然对数'e')
}

//申请一维双精度实数数组
//参数：n（数组的维数）
double *alloc_1d_dbl(int n)
{
	double *new1;
	new1=(double*)malloc((unsigned)(n*sizeof(double)));
	if (new1==NULL)
	{
//		printf("ALLIC_1D_DBL:不能分配双精度的数组\n");
		AfxMessageBox("不能分配双精度的数组");
		return(NULL);
	}
	return(new1);
}

//申请2维双精度实数数组
//参数：m（数组的行数） n（数组的列数）
double **alloc_2d_dbl(int m,int n)
{
	int i;
	double **new1;

	new1=(double**)malloc((unsigned)(m*sizeof(double*)));
	if(new1==NULL)
	{
//		printf("ALLOC_2D_DBL:不能分配双精度实数数组\n");
		AfxMessageBox("不能分配双精度的数组");
		return NULL;
	}

	for(i=0;i<m;i++)
	{
		new1[i]=alloc_1d_dbl(n);
	}

	return(new1);
}

//随机初始化权值
//参数：w（保存权值的二级指针）m（数组的行数）n（数组的列数）
void bpnn_randomize_weights(double**w,int m,int n)
{
	int i,j;
	for (i=0;i<=m;i++)
	{
		for (j=0;j<=n;j++)
		{
			w[i][j]=dpn1();//-1~1的双精度随机数
		}
	}
}

//0初始化权值
//参数：w（保存权值的二级指针）m（数组的行数）n（数组的列数）
void bpnn_zero_weights(double **w,int m,int n)
{
	int i,j;
	for (i=0;i<=m;i++)
	{
		for (j=0;j<=n;j++)
		{
			w[i][j]=0.0;		
		}
	}
}

//设置随机数种子
//参数：seed（随机数种子）
void bpnn_initialize(int seed)
{
//	printf("随机数种子发生器：%d\n",seed);
	srand(seed);
}

//创建BP网络
//参数：n_in(输入神经元个数)n_hidden(隐藏神经元个数)n_out(输出神经元个数)
BPNN *bpnn_internal_creat(int n_in,int n_hidden,int n_out)
{
	BPNN *newnet;

	newnet=(BPNN*)malloc(sizeof(BPNN));
	if (newnet==NULL)
	{
//		printf("BPNN_CREATE:不能分配神经元网络\n");
		AfxMessageBox("不能分配神经元网络");
		return(NULL);
	}
	newnet->input_n=n_in;//输入层的神经元
	newnet->hidden_n=n_hidden;//隐藏层的神经元
	newnet->output_n=n_out;//输出层的神经元
	newnet->input_units=alloc_1d_dbl(n_in+1);//分配内存
	newnet->hidden_units=alloc_1d_dbl(n_hidden+1);
	newnet->output_units=alloc_1d_dbl(n_out+1);

	newnet->hidden_delta=alloc_1d_dbl(n_hidden+1);//隐藏层的误差
	newnet->output_delta=alloc_1d_dbl(n_out+1);//输入层的误差
	newnet->target=alloc_1d_dbl(n_out+1);//目标向量

	newnet->input_weights=alloc_2d_dbl(n_in+1,n_hidden+1);//输入层到隐藏层的连接权
	newnet->hidden_weights=alloc_2d_dbl(n_hidden+1,n_out+1);//隐藏层到输出层的连接权

	newnet->input_prev_weights=alloc_2d_dbl(n_in+1,n_hidden+1);//首次输入层到隐藏层权值得改变（用于迭代）
	newnet->hidden_prev_weights=alloc_2d_dbl(n_hidden+1,n_out+1);//首次隐藏层到输出层权值得改变

	return(newnet);
}

//释放BP网络所占用的空间
//参数：net(需要释放的内存地址)
void bpnn_free(BPNN *net)
{
	int n1,n2,i;

	n1=net->input_n;
	n2=net->hidden_n;

	free((char*)net->input_units);//释放神经元
	free((char*)net->hidden_units);
	free((char*)net->output_units);

	free((char*)net->hidden_delta);//释放误差
	free((char*)net->output_delta);
	free((char*)net->target);//释放目标向量

	for(i=0;i<=n1;i++)
	{
		free((char*)net->input_weights[i]);
		free((char*)net->input_prev_weights[i]);
	}
	free((char*)net->input_weights);
	free((char*)net->input_prev_weights);

	for (i=0;i<=n2;i++)
	{
		free((char*)net->hidden_weights[i]);
		free((char*)net->hidden_prev_weights[i]);
	}
	free((char*)net->hidden_weights);
	free((char*)net->hidden_prev_weights);

	free((char*)net);
}

//创建一个BP网络，并初始化权值
//参数：n_in(输入层个数)n_out（输出层个数）n_hidden（隐藏层神经元个数）
BPNN *bpnn_creat(int n_in,int n_hidden,int n_out)
{
	BPNN *newnet;
	newnet=bpnn_internal_creat(n_in,n_hidden,n_out);
#ifdef INITZERO
	bpnn_zero_weights(newnet->input_weights,n_in,n_hidden);
#else
	bpnn_randomize_weights(newnet->input_weights,n_in,n_hidden);
#endif
	bpnn_randomize_weights(newnet->input_weights,n_hidden,n_out);
	bpnn_zero_weights(newnet->input_prev_weights,n_in,n_hidden);
	bpnn_zero_weights(newnet->input_prev_weights,n_hidden,n_out);

	return(newnet);
}

//计算从前一层到后一层的输出
//参数：l1(前一层的神经元)l2（后一层的神经元）conn（连接权值）n1（前一层神经元个数）n2（后一层神经元个数）
void bpnn_layerforward(double *l1,double *l2,double **conn,int n1,int n2)
{
	double sum;
	int j,k;
	//设置阈值
	l1[0]=1.0;
	//对于第二层的每个神经元
	for (j=1;j<=n2;j++)
	{
		//计算输入的加权总和
		sum=0.0;
		for (k=0;k<=n1;k++)
		{
			sum+=conn[k][j]*l1[k];
		}
		l2[j]=squash(sum);//作用函数（s函数）
	}
}

//输出误差
//参数：delta(误差)target（目标数组）output（实际输出数组）n1（神经元个数）err（误差综合）
void bpnn_output_error(double *delta,double *target,double *output,int nj,double *err)
{
	int j;
	double o,t,errsum;

	errsum=0.0;
	for (j=1;j<=nj;j++)
	{
		o=output[j];
		t=target[j];
		delta[j]=o*(1.0-o)*(t-o);//计算误差，输出结果与目标结果的差值
		errsum+=ABS(delta[j]);
	}
	*err=errsum;
}

//隐藏层误差
/*
参数：delta_h(隐藏层误差数组)nh（隐藏层神经元个数）delta_0(输出层误差数组)
no（输出层神经元个数）who（隐藏层到输出层的连接权值）hidden（隐藏层的神经元）err（总误差）
*/
void bpnn_hidden_error(double*delta_h,int nh,double *delta_o,int no,double **who,double *hidden,double *err)
{
	int j,k;
	double h,sum,errsum;

	errsum=0.0;
	for(j=1;j<=nh;j++)
	{
		h=hidden[j];
		sum=0.0;
		for(k=1;k<=no;k++)
		{
			sum+=delta_o[k]*who[j][k];
		}
		delta_h[j]=h*(1.0-h)*sum;
		errsum+=ABS(delta_h[j]);
	}
	*err=errsum;
}

//调整权值
//参数：delta(误差数组)ndelta（数组长度）w（新权值数组）oldw（旧权值数组）eta（学习速率）momentum（学习动量因子）
void bpnn_adjust_weights(double *delta,int ndelta,double *ly,int nly,double **w,double **oldw,double eta,double momentum)
{
	double new_dw;
	int k,j;
	ly[0]=1.0;
	for(j=1;j<ndelta;j++)
	{
		for(k=0;k<nly;k++)
		{
			new_dw=((eta*delta[j]*ly[k])+(momentum*oldw[k][j]));
			w[k][j]+=new_dw;
			oldw[k][j]=new_dw;
		}
	}
}

//进行前向运算
//参数：net（BP网络）
void bpnn_feedforward(BPNN *net)
{
	int in,hid,out;

	in=net->input_n;
	hid=net->hidden_n;
	out=net->output_n;
	//前向运算输入激活
	bpnn_layerforward(net->input_units,net->hidden_units,net->input_weights,in,hid);
	bpnn_layerforward(net->hidden_units,net->output_units,net->hidden_weights,hid,out);
}

//训练BP网络
//参数net（BP网络）eta（学习速率）momentum（学习动量因子）e0（输入层误差）eh（隐藏层误差）
void bpnn_train(BPNN *net,double eta,double momentum,double *eo,double *eh)
{
	int in,hid,out;
	double out_err,hid_err;

	in=net->input_n;//记录神经元个数
	hid=net->hidden_n;
	out=net->output_n;

	//前向输入激活
	bpnn_layerforward(net->input_units,net->hidden_units,net->input_weights,in,hid);//计算从前一层到后一层的输出
	bpnn_layerforward(net->hidden_units,net->output_units,net->hidden_weights,hid,out);

	//计算隐藏层和输入层的误差
	bpnn_output_error(net->output_delta,net->target,net->output_units,out,&out_err);
	bpnn_hidden_error(net->hidden_delta,hid,net->output_delta,out,net->hidden_weights,net->hidden_units,&hid_err);
	*eo=out_err;//输出误差结果
	*eh=hid_err;

	//调整输入层和隐藏层权值
	bpnn_adjust_weights(net->output_delta,out,net->hidden_units,hid,net->hidden_weights,net->hidden_prev_weights,eta,momentum);
	bpnn_adjust_weights(net->hidden_delta,hid,net->input_units,in,net->input_weights,net->input_prev_weights,eta,momentum);
}

//保存神经网络
//参数：net（待保存的网络）filename（文件名）
void bpnn_save(BPNN *net,char *filename)
{
	int n1,n2,n3,i,j,memcnt;
	double dvalue,**w;
	char *mem;
	FILE *fd;

	if ((fd=fopen(filename,"w"))=NULL)
	{
//		printf("BPNN_SAVE:不能创建'%s'\n",filename);
		AfxMessageBox("不能创建指定文件");
		return;
	}
	n1=net->input_n;
	n2=net->hidden_n;
	n3=net->output_n;
//	printf("正在保存%dx%dx%d network to '%s'\n",filename);
	AfxMessageBox("正在保存");
//	fflush(stdout);//使上面的输出马上输出到显示器上

	fwrite((char*)&n1,sizeof(int),1,fd);
	fwrite((char*)&n2,sizeof(int),1,fd);	
	fwrite((char*)&n3,sizeof(int),1,fd);

	memcnt=0;
	w=net->hidden_weights;
	mem=(char*)malloc((unsigned)((n1+1)*(n2+1)*sizeof(double)));
	for (i=0;i<=n1;i++)
	{
		for (j=0;j<=n2;j++)
		{
			dvalue=w[i][j];//保存权值
			fastcopy(&mem[memcnt],&dvalue,sizeof(double));
			memcnt+=sizeof(double);
		}
	}
	fwrite(mem,(n1+1)*(n2+1)*sizeof(double),1,fd);
	free(mem);

	memcnt=0;
	w=net->hidden_weights;
	mem=(char*)malloc((unsigned)((n2+1)*(n3+1)*sizeof(double)));
	for (i=0;i<=n2;i++)
	{
		for (j=0;j<=n3;j++)
		{
			dvalue=w[j][j];
			fastcopy(&mem[memcnt],&dvalue,sizeof(double));
			memcnt+=sizeof(double);
		}
	}
	fwrite(mem,(n2+1)*(n3+1)*sizeof(double),1,fd);
	free(mem);

	fclose(fd);
	return;
}

//从文件中读取BP网络
//参数：filename(输入的文件名)
//返回：BP网络结构
BPNN *bpnn_read(char *filename)
{
	char *mem;
	BPNN *new1;
	int n1,n2,n3,i,j,memcnt;
	FILE *fd;

	if ((fd=fopen(filename,"r"))==NULL)
	{
		return(NULL);
	}
//	printf("正在读取'%s'\n",filename);
//	fflush(stdout);

	fread((char*)&n1,sizeof(int),1,fd);
	fread((char*)&n2,sizeof(int),1,fd);
	fread((char*)&n3,sizeof(int),1,fd);

	new1=bpnn_internal_creat(n1,n2,n3);
//	MessageBox("'%s'包含了一个 %dx%dx%d 神经网络",filename,n1,n2,n3);
	AfxMessageBox("正在读取权值。。。");
//	fflush(stdout);

	memcnt=0;
	mem=(char*)malloc((unsigned)((n1+1)*(n2+1)*sizeof(double)));

	fread(mem,(n1+1)*(n2+1)*sizeof(double),1,fd);
	for (i=0;i<=n1;i++)
	{
		for (j=0;j<=n2;j++)
		{
			fastcopy(&(new1->input_weights[i][j]),&mem[memcnt],sizeof(double));
			memcnt+=sizeof(double);
		}
	}
	free(mem);

	AfxMessageBox("读取隐藏层权值。。。");
//	fflush(stdout);

	memcnt=0;
	mem=(char*)malloc((unsigned)((n2+1)*(n3+1)*sizeof(double)));

	fread(mem,(n2+1)*(n3+1)*sizeof(double),1,fd);
	for(i=0;i<=n2;i++)
	{
		for (j=0;j<=n3;j++)
		{
			fastcopy(&(new1->hidden_weights[i][j]),&mem[memcnt],sizeof(double));
			memcnt+=sizeof(double);
		}
	}
	free(mem);
	fclose(fd);

	AfxMessageBox("完成！");
//	fflush(stdout);

	bpnn_zero_weights(new1->input_prev_weights,n1,n2);
	bpnn_zero_weights(new1->hidden_prev_weights,n2,n3);

	return(new1);
}

// CBP 对话框

IMPLEMENT_DYNAMIC(CBP, CDialogEx)

CBP::CBP(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBP::IDD, pParent)
{

	m_rate = 0.0f;
	m_monument = 0.0f;
	m_diedai = 0.0f;

	nSImageSizeX=nSImageSizeY=0;
	nImageSizeX=nImageSizeY=0;
	m_RMS = 0.0;
}

CBP::~CBP()
{
}

void CBP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RATE, m_rate);
	DDX_Text(pDX, IDC_EDIT_monument, m_monument);
	DDX_Text(pDX, IDC_EDIT_DIED, m_diedai);
	DDX_Text(pDX, IDC_EDIT1_RMS, m_RMS);
}


BEGIN_MESSAGE_MAP(CBP, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBP::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CBP::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBP::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CBP::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_BPSAVE, &CBP::OnBnClickedButtonBpsave)
	ON_BN_CLICKED(IDC_BUTTON_BPREAD, &CBP::OnBnClickedButtonBpread)
	ON_BN_CLICKED(IDC_BUTTON_Train, &CBP::OnBnClickedButtonTrain)
END_MESSAGE_MAP()


// CBP 消息处理程序


void CBP::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	BeginWaitCursor();//开始等待光标

	InData3=new BYTE[nImageSizeX*nImageSizeY*3];

	double qmax=0;//存储最大那个类的概率
	int cnum=0;//记录象元所属的类

	for (int i=0;i<nImageSizeX*nImageSizeY;i++)
	{
		for (int j=0;j<6;j++)
		{
			net->input_units[j]=InData1[i];
			bpnn_feedforward(net);
			double p=net->output_units[j];
			if (qmax<p)
			{
				qmax=p;
				cnum=j;
			}
		}
		InData3[i]=(BYTE)(51*cnum);
		InData3[1*nImageSizeX*nImageSizeY+i]=(BYTE)(41*cnum);	
		InData3[2*nImageSizeX*nImageSizeY+i]=(BYTE)(31*cnum);
	}

	char* pszFormat;
	if(save_path1.Right(4)==".BMP")
	{
		pszFormat = "BMP";
	}	

	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	GDALDataset *poDstDS;
	char **papszOptions =poDriver->GetMetadata();
	poDstDS = poDriver->Create( (char*)(LPCTSTR)save_path1,//输出路径
		nImageSizeX,nImageSizeY, //影像数据宽高
		3, //存或取的波段数
		GDT_Byte, 	//缓冲区数据类型
		papszOptions );

	if(poDstDS->RasterIO(
		GF_Write,						//IO方式
		0,0,							//影像数据起始位置x,y
		nImageSizeX,nImageSizeY,				//影像数据宽高
		InData3,						//指向缓冲区数据指针
		nImageSizeX,nImageSizeY,				//缓冲区数据宽高
		GDT_Byte,						//缓冲区数据类型
		3,						//存或取的波段数
		NULL,							//存或取的波段列表
		0,								//缓冲区数据一个像素占用的字节数
		0,								//缓冲区一行数据所占用的字节数
		0)==CE_Failure )				//缓冲区数据存储方式 0为RRRRGGGGBBBB-bsp 1为RGBRGBRGB-bip	
	{
		AfxMessageBox("保存文件失败！");
		return;
	}
	else
	{
		AfxMessageBox("分类完成！");
	}

	EndWaitCursor();

	delete[] InData3;
	delete[] InData1;
	GDALClose(poDstDS);
	CDialogEx::OnOK();
}


void CBP::OnBnClickedButton1()//输入影像
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入待分类遥感影像";
	if (file.DoModal()==IDOK)
	{
		read_path=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INBP,read_path);

	GDALDataset* poDataset;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(read_path,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}
	BeginWaitCursor();
	GDALRasterBand* Band1;
	int Bands = poDataset->GetRasterCount();
	nImageSizeX=poDataset->GetRasterXSize();
	nImageSizeY=poDataset->GetRasterYSize();

	InData1 = new double[nImageSizeX*nImageSizeY];
	Band1=poDataset->GetRasterBand(1);
	Band1->RasterIO( GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Float32,0,0);
//	poDataset->RasterIO(GF_Read, 0, 0,nImageSizeX,nImageSizeY, InData1,nImageSizeX,nImageSizeY, GDT_Float32,Bands,NULL,0,0,0);
	GDALClose(poDataset);
	EndWaitCursor();
}


void CBP::OnBnClickedButton2()//输入样本影像
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.bmp)|*.bmp|JPG文件(*.jpg)|*.jpg;*.jpeg|tiff文件(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="请输入样本遥感影像";
	if (file.DoModal()==IDOK)
	{
		read_path=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INBP2,read_path);

	GDALDataset* poDataset;//GDAL数据集
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(read_path,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("打开文件失败！！！"));
		return;
	}

	BeginWaitCursor();
	GDALRasterBand* Band1;
//	int Bands = poDataset->GetRasterCount();
	nSImageSizeX=poDataset->GetRasterXSize();
	nSImageSizeY=poDataset->GetRasterYSize();

	InData2 = new double[nSImageSizeX*nSImageSizeY];
	Band1=poDataset->GetRasterBand(1);
	Band1->RasterIO( GF_Read, 0, 0,nSImageSizeX,nSImageSizeY, InData2,nSImageSizeX,nSImageSizeY, GDT_Float32,0,0);
//	poDataset->RasterIO( GF_Read, 0, 0,nSImageSizeX,nSImageSizeY, InData2,nSImageSizeX,nSImageSizeY, GDT_Float32,Bands,NULL,0,0,0);
	GDALClose(poDataset);
	EndWaitCursor();
}


void CBP::OnBnClickedButton3()//输出结果影像
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|位图文件(*.BMP)|*.BMP|JPG文件(*.JPG)|*.JPG;*.JPEG|tiff文件(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="请输入分类结果";
	if (file.DoModal()==IDOK)
	{
		save_path1=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_OUTBP,save_path1);
}


void CBP::OnBnClickedButtonBpsave()
{
	// TODO: 在此添加控件通知处理程序代码
	if (net->input_n==NULL&&net->hidden_n==NULL&&net->output_n==NULL)
	{
//		printf("输入样本为空，无法进行训练/n");
		AfxMessageBox("神经网络为空，无法保存！");
		return;
	}

	CFileDialog file(false, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"*.txt(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="请输入保存路径";
	if (file.DoModal()==IDOK)
	{
		save_path=file.GetPathName();
		bpnn_save(net, (LPSTR)(LPCTSTR)save_path);
	}
}


void CBP::OnBnClickedButtonBpread()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"*.txt(*.txt)|*.txt|*.*(*.*)|*.*||");
	file.m_ofn.lpstrTitle="请输入读取文件路径";
	if (file.DoModal()==IDOK)
	{ 
		read_path1=file.GetPathName();
		bpnn_read((LPSTR)(LPCTSTR)read_path1);
	}
}


BOOL CBP::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_rate=0.5;
	m_monument=0.95;
	m_diedai=3;
	m_RMS=0.2;
	UpdateData(FALSE);

	bpnn_initialize(1);
	net=bpnn_creat(6,5,5);

	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int m=0;
void CBP::OnBnClickedButtonTrain()
{
	// TODO: 在此添加控件通知处理程序代码
	//训练

	UpdateData(TRUE);

	double* eo=new double;
	double* eh=new double;

	double tar[5][5]={{1,0,0,0,0},{0,1,0,0,0},{0,0,1,0,0},{0,0,0,1,0},{0,0,0,0,1}};

	BeginWaitCursor();
	if (nSImageSizeY!=0&&nSImageSizeY!=0)
	{
		for (int z=0;z<m_diedai;z++)
		{
			for (int j=0;j<5;j++)
			{
				net->target[j]=tar[m][j];
			}
			for (int i=0;i<nSImageSizeX*nSImageSizeY;i++)
			{
				double obj=InData2[i];
				for (int j=0;j<6;j++)
				{
					net->input_units[j]=obj;
					bpnn_train(net,0.3,0.3,eo,eh);
				}	
			}
		}
	}

	if(*eo>m_RMS&&*eh>m_RMS) AfxMessageBox("误差太大请重新训练！");

	m=m+1;
	delete[] InData2;

	EndWaitCursor();
	AfxMessageBox("样本训练完毕");
}
