// BP.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "BP.h"
#include "afxdialogex.h"
#include <math.h>
#include <stdlib.h>



#define  ABS(x)  (((x)>0.0)?(x):(-(x)))

//�в����ĺ궨�壨ִ���ٶȽϿ죩�궨�廻��Ҫ�á�\�������ӣ����ٸ���
#define  fastcopy(to,from,len){\
	char *_to,*_from;\
	int _i,_l;\
	_to=(char*)(to);\
	_from=(char*)(from);\
	_l=(len);\
	for (_i=0;_i<_l;_i++) *_to++=*_from++;\
}

//����0~1��˫���������
double drnd()
{
	return ((double)rand()/(double)BIGRND);
}

//����-1~1֮���˫���������
double dpn1()
{
	return((drnd()*2.0)-1.0);
}

//���ú�����S�ͺ��������������ͣ�Ŀǰֻ�ö�����������--����������ɶ������й�ϵ��
double squash(double x)
{
	return(1.0/1.0+exp(-x));//(exp==��Ȼ����'e')
}

//����һά˫����ʵ������
//������n�������ά����
double *alloc_1d_dbl(int n)
{
	double *new1;
	new1=(double*)malloc((unsigned)(n*sizeof(double)));
	if (new1==NULL)
	{
//		printf("ALLIC_1D_DBL:���ܷ���˫���ȵ�����\n");
		AfxMessageBox("���ܷ���˫���ȵ�����");
		return(NULL);
	}
	return(new1);
}

//����2ά˫����ʵ������
//������m������������� n�������������
double **alloc_2d_dbl(int m,int n)
{
	int i;
	double **new1;

	new1=(double**)malloc((unsigned)(m*sizeof(double*)));
	if(new1==NULL)
	{
//		printf("ALLOC_2D_DBL:���ܷ���˫����ʵ������\n");
		AfxMessageBox("���ܷ���˫���ȵ�����");
		return NULL;
	}

	for(i=0;i<m;i++)
	{
		new1[i]=alloc_1d_dbl(n);
	}

	return(new1);
}

//�����ʼ��Ȩֵ
//������w������Ȩֵ�Ķ���ָ�룩m�������������n�������������
void bpnn_randomize_weights(double**w,int m,int n)
{
	int i,j;
	for (i=0;i<=m;i++)
	{
		for (j=0;j<=n;j++)
		{
			w[i][j]=dpn1();//-1~1��˫���������
		}
	}
}

//0��ʼ��Ȩֵ
//������w������Ȩֵ�Ķ���ָ�룩m�������������n�������������
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

//�������������
//������seed����������ӣ�
void bpnn_initialize(int seed)
{
//	printf("��������ӷ�������%d\n",seed);
	srand(seed);
}

//����BP����
//������n_in(������Ԫ����)n_hidden(������Ԫ����)n_out(�����Ԫ����)
BPNN *bpnn_internal_creat(int n_in,int n_hidden,int n_out)
{
	BPNN *newnet;

	newnet=(BPNN*)malloc(sizeof(BPNN));
	if (newnet==NULL)
	{
//		printf("BPNN_CREATE:���ܷ�����Ԫ����\n");
		AfxMessageBox("���ܷ�����Ԫ����");
		return(NULL);
	}
	newnet->input_n=n_in;//��������Ԫ
	newnet->hidden_n=n_hidden;//���ز����Ԫ
	newnet->output_n=n_out;//��������Ԫ
	newnet->input_units=alloc_1d_dbl(n_in+1);//�����ڴ�
	newnet->hidden_units=alloc_1d_dbl(n_hidden+1);
	newnet->output_units=alloc_1d_dbl(n_out+1);

	newnet->hidden_delta=alloc_1d_dbl(n_hidden+1);//���ز�����
	newnet->output_delta=alloc_1d_dbl(n_out+1);//���������
	newnet->target=alloc_1d_dbl(n_out+1);//Ŀ������

	newnet->input_weights=alloc_2d_dbl(n_in+1,n_hidden+1);//����㵽���ز������Ȩ
	newnet->hidden_weights=alloc_2d_dbl(n_hidden+1,n_out+1);//���ز㵽����������Ȩ

	newnet->input_prev_weights=alloc_2d_dbl(n_in+1,n_hidden+1);//�״�����㵽���ز�Ȩֵ�øı䣨���ڵ�����
	newnet->hidden_prev_weights=alloc_2d_dbl(n_hidden+1,n_out+1);//�״����ز㵽�����Ȩֵ�øı�

	return(newnet);
}

//�ͷ�BP������ռ�õĿռ�
//������net(��Ҫ�ͷŵ��ڴ��ַ)
void bpnn_free(BPNN *net)
{
	int n1,n2,i;

	n1=net->input_n;
	n2=net->hidden_n;

	free((char*)net->input_units);//�ͷ���Ԫ
	free((char*)net->hidden_units);
	free((char*)net->output_units);

	free((char*)net->hidden_delta);//�ͷ����
	free((char*)net->output_delta);
	free((char*)net->target);//�ͷ�Ŀ������

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

//����һ��BP���磬����ʼ��Ȩֵ
//������n_in(��������)n_out������������n_hidden�����ز���Ԫ������
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

//�����ǰһ�㵽��һ������
//������l1(ǰһ�����Ԫ)l2����һ�����Ԫ��conn������Ȩֵ��n1��ǰһ����Ԫ������n2����һ����Ԫ������
void bpnn_layerforward(double *l1,double *l2,double **conn,int n1,int n2)
{
	double sum;
	int j,k;
	//������ֵ
	l1[0]=1.0;
	//���ڵڶ����ÿ����Ԫ
	for (j=1;j<=n2;j++)
	{
		//��������ļ�Ȩ�ܺ�
		sum=0.0;
		for (k=0;k<=n1;k++)
		{
			sum+=conn[k][j]*l1[k];
		}
		l2[j]=squash(sum);//���ú�����s������
	}
}

//������
//������delta(���)target��Ŀ�����飩output��ʵ��������飩n1����Ԫ������err������ۺϣ�
void bpnn_output_error(double *delta,double *target,double *output,int nj,double *err)
{
	int j;
	double o,t,errsum;

	errsum=0.0;
	for (j=1;j<=nj;j++)
	{
		o=output[j];
		t=target[j];
		delta[j]=o*(1.0-o)*(t-o);//��������������Ŀ�����Ĳ�ֵ
		errsum+=ABS(delta[j]);
	}
	*err=errsum;
}

//���ز����
/*
������delta_h(���ز��������)nh�����ز���Ԫ������delta_0(������������)
no���������Ԫ������who�����ز㵽����������Ȩֵ��hidden�����ز����Ԫ��err������
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

//����Ȩֵ
//������delta(�������)ndelta�����鳤�ȣ�w����Ȩֵ���飩oldw����Ȩֵ���飩eta��ѧϰ���ʣ�momentum��ѧϰ�������ӣ�
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

//����ǰ������
//������net��BP���磩
void bpnn_feedforward(BPNN *net)
{
	int in,hid,out;

	in=net->input_n;
	hid=net->hidden_n;
	out=net->output_n;
	//ǰ���������뼤��
	bpnn_layerforward(net->input_units,net->hidden_units,net->input_weights,in,hid);
	bpnn_layerforward(net->hidden_units,net->output_units,net->hidden_weights,hid,out);
}

//ѵ��BP����
//����net��BP���磩eta��ѧϰ���ʣ�momentum��ѧϰ�������ӣ�e0���������eh�����ز���
void bpnn_train(BPNN *net,double eta,double momentum,double *eo,double *eh)
{
	int in,hid,out;
	double out_err,hid_err;

	in=net->input_n;//��¼��Ԫ����
	hid=net->hidden_n;
	out=net->output_n;

	//ǰ�����뼤��
	bpnn_layerforward(net->input_units,net->hidden_units,net->input_weights,in,hid);//�����ǰһ�㵽��һ������
	bpnn_layerforward(net->hidden_units,net->output_units,net->hidden_weights,hid,out);

	//�������ز�����������
	bpnn_output_error(net->output_delta,net->target,net->output_units,out,&out_err);
	bpnn_hidden_error(net->hidden_delta,hid,net->output_delta,out,net->hidden_weights,net->hidden_units,&hid_err);
	*eo=out_err;//��������
	*eh=hid_err;

	//�������������ز�Ȩֵ
	bpnn_adjust_weights(net->output_delta,out,net->hidden_units,hid,net->hidden_weights,net->hidden_prev_weights,eta,momentum);
	bpnn_adjust_weights(net->hidden_delta,hid,net->input_units,in,net->input_weights,net->input_prev_weights,eta,momentum);
}

//����������
//������net������������磩filename���ļ�����
void bpnn_save(BPNN *net,char *filename)
{
	int n1,n2,n3,i,j,memcnt;
	double dvalue,**w;
	char *mem;
	FILE *fd;

	if ((fd=fopen(filename,"w"))=NULL)
	{
//		printf("BPNN_SAVE:���ܴ���'%s'\n",filename);
		AfxMessageBox("���ܴ���ָ���ļ�");
		return;
	}
	n1=net->input_n;
	n2=net->hidden_n;
	n3=net->output_n;
//	printf("���ڱ���%dx%dx%d network to '%s'\n",filename);
	AfxMessageBox("���ڱ���");
//	fflush(stdout);//ʹ�������������������ʾ����

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
			dvalue=w[i][j];//����Ȩֵ
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

//���ļ��ж�ȡBP����
//������filename(������ļ���)
//���أ�BP����ṹ
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
//	printf("���ڶ�ȡ'%s'\n",filename);
//	fflush(stdout);

	fread((char*)&n1,sizeof(int),1,fd);
	fread((char*)&n2,sizeof(int),1,fd);
	fread((char*)&n3,sizeof(int),1,fd);

	new1=bpnn_internal_creat(n1,n2,n3);
//	MessageBox("'%s'������һ�� %dx%dx%d ������",filename,n1,n2,n3);
	AfxMessageBox("���ڶ�ȡȨֵ������");
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

	AfxMessageBox("��ȡ���ز�Ȩֵ������");
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

	AfxMessageBox("��ɣ�");
//	fflush(stdout);

	bpnn_zero_weights(new1->input_prev_weights,n1,n2);
	bpnn_zero_weights(new1->hidden_prev_weights,n2,n3);

	return(new1);
}

// CBP �Ի���

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


// CBP ��Ϣ�������


void CBP::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	BeginWaitCursor();//��ʼ�ȴ����

	InData3=new BYTE[nImageSizeX*nImageSizeY*3];

	double qmax=0;//�洢����Ǹ���ĸ���
	int cnum=0;//��¼��Ԫ��������

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
	poDstDS = poDriver->Create( (char*)(LPCTSTR)save_path1,//���·��
		nImageSizeX,nImageSizeY, //Ӱ�����ݿ��
		3, //���ȡ�Ĳ�����
		GDT_Byte, 	//��������������
		papszOptions );

	if(poDstDS->RasterIO(
		GF_Write,						//IO��ʽ
		0,0,							//Ӱ��������ʼλ��x,y
		nImageSizeX,nImageSizeY,				//Ӱ�����ݿ��
		InData3,						//ָ�򻺳�������ָ��
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
	else
	{
		AfxMessageBox("������ɣ�");
	}

	EndWaitCursor();

	delete[] InData3;
	delete[] InData1;
	GDALClose(poDstDS);
	CDialogEx::OnOK();
}


void CBP::OnBnClickedButton1()//����Ӱ��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="�����������ң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		read_path=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INBP,read_path);

	GDALDataset* poDataset;//GDAL���ݼ�
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(read_path,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
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


void CBP::OnBnClickedButton2()//��������Ӱ��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.bmp)|*.bmp|JPG�ļ�(*.jpg)|*.jpg;*.jpeg|tiff�ļ�(*.tiff)|*.tiff;*tif|img(*.img)|*.img|hdr(*.hdr)|*.hdr||");
	file.m_ofn.lpstrTitle="����������ң��Ӱ��";
	if (file.DoModal()==IDOK)
	{
		read_path=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_INBP2,read_path);

	GDALDataset* poDataset;//GDAL���ݼ�
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	poDataset=(GDALDataset*)GDALOpen(read_path,GA_ReadOnly);
	if(poDataset==NULL)
	{
		AfxMessageBox(_T("���ļ�ʧ�ܣ�����"));
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


void CBP::OnBnClickedButton3()//������Ӱ��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "BMP", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|λͼ�ļ�(*.BMP)|*.BMP|JPG�ļ�(*.JPG)|*.JPG;*.JPEG|tiff�ļ�(*.TIFF)|*.TIFF;*.TIF|img(*.IMG)|*.IMG|hdr(*.HDR)|*.HDR||");
	file.m_ofn.lpstrTitle="�����������";
	if (file.DoModal()==IDOK)
	{
		save_path1=file.GetPathName();
	}
	SetDlgItemText(IDC_STATIC_OUTBP,save_path1);
}


void CBP::OnBnClickedButtonBpsave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (net->input_n==NULL&&net->hidden_n==NULL&&net->output_n==NULL)
	{
//		printf("��������Ϊ�գ��޷�����ѵ��/n");
		AfxMessageBox("������Ϊ�գ��޷����棡");
		return;
	}

	CFileDialog file(false, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"*.txt(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="�����뱣��·��";
	if (file.DoModal()==IDOK)
	{
		save_path=file.GetPathName();
		bpnn_save(net, (LPSTR)(LPCTSTR)save_path);
	}
}


void CBP::OnBnClickedButtonBpread()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"*.txt(*.txt)|*.txt|*.*(*.*)|*.*||");
	file.m_ofn.lpstrTitle="�������ȡ�ļ�·��";
	if (file.DoModal()==IDOK)
	{ 
		read_path1=file.GetPathName();
		bpnn_read((LPSTR)(LPCTSTR)read_path1);
	}
}


BOOL CBP::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_rate=0.5;
	m_monument=0.95;
	m_diedai=3;
	m_RMS=0.2;
	UpdateData(FALSE);

	bpnn_initialize(1);
	net=bpnn_creat(6,5,5);

	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

int m=0;
void CBP::OnBnClickedButtonTrain()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ѵ��

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

	if(*eo>m_RMS&&*eh>m_RMS) AfxMessageBox("���̫��������ѵ����");

	m=m+1;
	delete[] InData2;

	EndWaitCursor();
	AfxMessageBox("����ѵ�����");
}
