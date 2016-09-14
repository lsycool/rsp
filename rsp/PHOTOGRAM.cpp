// PHOTOGRAM.cpp : 实现文件
//

#include "stdafx.h"
#include "rsp.h"
#include "PHOTOGRAM.h"
#include "afxdialogex.h"


// CPHOTOGRAM 对话框

IMPLEMENT_DYNAMIC(CPHOTOGRAM, CDialogEx)

CPHOTOGRAM::CPHOTOGRAM(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPHOTOGRAM::IDD, pParent)
{

	m_X0 = 0.0f;
	m_Y0 = 0.0f;
	m_FK = 153.24f;
	m_BLC =1/5000.0f;
}

CPHOTOGRAM::~CPHOTOGRAM()
{
}

void CPHOTOGRAM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_X0);
	DDX_Text(pDX, IDC_EDIT3, m_Y0);
	DDX_Text(pDX, IDC_EDIT4, m_FK);
	DDX_Text(pDX, IDC_EDIT5, m_BLC);
}


BEGIN_MESSAGE_MAP(CPHOTOGRAM, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPHOTOGRAM::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON8, &CPHOTOGRAM::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON2, &CPHOTOGRAM::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CPHOTOGRAM::OnBnClickedOk)
END_MESSAGE_MAP()


// CPHOTOGRAM 消息处理程序


void CPHOTOGRAM::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(true, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|文本(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="请输入兴趣点文件";
	if (file.DoModal()==IDOK)
	{
		XP_IN=file.GetPathName();	
	}
	SetDlgItemText(IDC_STATIC_XPIN,XP_IN);
}


void CPHOTOGRAM::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|文本(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="请输入兴趣点文件";
	if (file.DoModal()==IDOK)
	{
		DM_OUT=file.GetPathName();	
	}
	SetDlgItemText(IDC_STATIC_DMIN,DM_OUT);
}


void CPHOTOGRAM::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog file(false, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"所有支持文件(*.*)|*.*|文本(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="请输入兴趣点文件";
	if (file.DoModal()==IDOK)
	{
		WFW_OUT=file.GetPathName();	
	}
	SetDlgItemText(IDC_STATIC_WFWOUT,WFW_OUT);
}



//解算外方位元素
//参数说明 double B[4][5]:存储控制点信息；float x0,float y0,float fk：内方位元素；m：相片比例尺；FilePath：外方位元素保存路径
void CPHOTOGRAM::CaculateWFW(double B[4][5], float x0,float y0,float fk,float m,CString FilePath,CString FilePath1)
{
	double Xs=0.0, Ys=0.0, Zs=0.0,Q=0.0,W=0.0,K=0.0; //外方位元素
	
	double 	R[3][3]/*旋转矩阵*/,
			XG[6][1],//外方位元素近似值的改正数
			A[8][6],//外方位元素改正数的系数
			AT[6][8],//矩阵A的转置
			ATA[6][6],//用于解算外方位元素近似值改正数的中间量
			ATL[6][1],
			X,//地面摄影测量坐标
			Y,
			Z,
			L[8][1];//误差

	//确定未知数的出始值
	for(int i=0;i<4;i++)
	{
		Xs=Xs+B[i][2];
		Ys=Ys+B[i][3];
		Zs=Zs+B[i][4];
	}
	Xs=Xs/4; Ys=Ys/4; Zs=/*Zs/4+*/m*fk;//确定未知数的初始值
	int f=0;

	//用ψωκ表示方向余弦
	do//迭代计算
	{
		f++;
		//组成旋转矩阵
		R[0][0]=cos(Q)*cos(K)-sin(Q)*sin(W)*sin(K);//a1
		R[0][1]=-cos(Q)*sin(K)-sin(Q)*sin(W)*cos(K);//a2
		R[0][2]=-sin(Q)*cos(W);//a3
		R[1][0]=cos(W)*sin(K);//b1
		R[1][1]=cos(W)*cos(K);//b2
		R[1][2]=-sin(W);//b3
		R[2][0]=sin(Q)*cos(K)+cos(Q)*sin(W)*sin(K);//c1
		R[2][1]=-sin(Q)*sin(K)+cos(Q)*sin(W)*cos(K);//c2
		R[2][2]=cos(Q)*cos(W);//c3
		//计算系数阵和常数项
		for(int i=0,k=0,j=0;i<=3;i++,k++,j++)
		{
			X=R[0][0]*(B[i][2]-Xs)+R[1][0]*(B[i][3]-Ys)+R[2][0]*(B[i][4]-Zs);//解算像空间辅助坐标系里的坐标
			Y=R[0][1]*(B[i][2]-Xs)+R[1][1]*(B[i][3]-Ys)+R[2][1]*(B[i][4]-Zs);
			Z=R[0][2]*(B[i][2]-Xs)+R[1][2]*(B[i][3]-Ys)+R[2][2]*(B[i][4]-Zs);

			L[j][0]=B[i][0]-(x0-fk*X/Z);//真实像片值与计算像片坐标值误差
			L[j+1][0]=B[i][1]-(y0-fk*Y/Z);
			j++;

			A[k][0]=(R[0][0]*fk+R[0][2]*(B[i][0]-x0))/Z;//外方位元素近似值改正数的系数
			A[k][1]=(R[1][0]*fk+R[1][2]*(B[i][0]-x0))/Z;
			A[k][2]=(R[2][0]*fk+R[2][2]*(B[i][0]-x0))/Z;
			A[k][3]=(B[i][1]-y0)*sin(W)-((B[i][0]-x0)*((B[i][0]-x0)*cos(K)-(B[i][1]-y0)*sin(K))/fk+fk*cos(K))*cos(W);
			A[k][4]=-fk*sin(K)-(B[i][0]-x0)*((B[i][0]-x0)*sin(K)+(B[i][1]-y0)*cos(K))/fk;
			A[k][5]=B[i][1]-y0;
			A[k+1][0]=(R[0][1]*fk+R[0][2]*(B[i][1]-y0))/Z;
			A[k+1][1]=(R[1][1]*fk+R[1][2]*(B[i][1]-y0))/Z;
			A[k+1][2]=(R[2][1]*fk+R[2][2]*(B[i][1]-y0))/Z;
			A[k+1][3]=-(B[i][0]-x0)*sin(W)-((B[i][1]-y0)*((B[i][0]-x0)*cos(K)-(B[i][1]-y0)*sin(K))/fk-fk*sin(K))*cos(W);
			A[k+1][4]=-fk*cos(K)-(B[i][1]-y0)*((B[i][0]-x0)*sin(K)+(B[i][1]-y0)*cos(K))/fk;
			A[k+1][5]=-(B[i][0]-x0);
			k++;
		}

		transpose(A,AT,6,8);//矩阵转置
		multi(AT,A,ATA,6,8,6);//矩阵相乘
		inverse(ATA);//(ATA矩阵)求逆 
		multi(AT,L,ATL,6,8,1);//(AT,L)矩阵相乘,结果保存到ATL矩阵中
		multi(ATA,ATL,XG,6,6,1);//(ATA,ATL)矩阵相乘,得到外方位元素近似值的改正数（保存到XG矩阵中）

		Xs=Xs+XG[0][0]; Ys=Ys+XG[1][0]; Zs=Zs+XG[2][0];
		Q=Q+XG[3][0];  W=W+XG[4][0];  K=K+XG[5][0];
	}while(fabs(XG[3][0])>=6.0/206265.0||fabs(XG[4][0])>=6.0/206265.0||fabs(XG[5][0])>=6.0/206265.0);//计算收敛

	//精度评定
	double AXG[8][1],V[8][1],VT[1][8],VTV[1][1],m0,D[6][6];
	multi(A,XG,AXG,8,6,1);
	for(int i=0;i<8;i++)     //计算改正数
		V[i][0]=AXG[i][0]-L[i][0];
	transpose (V,VT,1,8);
	multi(VT,V,VTV,1,8,1);
	m0=VTV[0][0]/2;
	for(int i=0;i<6;i++)
		for(int j=0;j<6;j++)
			D[i][j]=m0*ATA[i][j];//计算未知数的中误差

	//屏幕输出误差方程系数阵、常数项、改正数
	ofstream outFile;
	outFile.open(FilePath1,ios::app);
	outFile<<"************文档说明：单片空间后方交会外方位元素解算"<<endl;
	outFile<<"************学习单位：中国地质大学"<<endl;
	outFile<<"************版权所有：刘世永"<<endl<<endl<<endl;

	outFile<<"迭代次数为："<<f<<endl<<endl;

	outFile<<"误差方程系数阵A为："<<endl<<endl;
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<6;j++)
			outFile<<setw(20)<<A[i][j];
		outFile<<endl;
	}
	outFile<<endl;

	outFile<<endl<<"常数项L为："<<endl<<endl;
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<1;j++)
			outFile<<setw(20)<<L[i][j];
		outFile<<endl;
	}
	outFile<<endl;

	outFile<<endl<<"改正数为："<<endl<<endl;
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<1;j++)
			outFile<<setw(20)<<XG[i][j];
		outFile<<endl;
	}
	outFile<<endl<<endl;
	outFile.close();
	/*output(A,"误差方程系数阵A为：",8,6);
	output(L,"常数项L为：",8,1);
	output(XG,"改正数为：",6,1);*/
	
	outFile.open(FilePath,ios::app);    //打开并添加输出文件
	outFile.precision(10);
	//以文件的形式输出像片外方位元素、旋转矩阵、方差阵
	outFile<<"************文档说明：单片空间后方交会外方位元素解算"<<endl;
	outFile<<"************学习单位：中国地质大学"<<endl;
	outFile<<"************版权所有：刘世永"<<endl<<endl<<endl;
	outFile<<"***像片的外方位元素为："<<endl<<endl;
	outFile<<"Xs="<<Xs<<setw(10)<<"Ys="<<Ys<<setw(10)<<"Zs="<<Zs<<endl<<endl;
	outFile<<"航向倾角为："<<Q<<setw(10)<<endl<<"旁向倾角为："<<W<<setw(10)<<endl<<"像片旋角为："<<K<<endl;
	
	outFile<<endl<<"***旋转矩阵R为:"<<endl<<endl;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			outFile<<setw(20)<<R[i][j]<<setw(20);
		outFile<<endl;
	}
	outFile<<endl<<endl;

	outFile<<"***精度评定结果为："<<endl<<endl;
	outFile.precision(5);
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<6;j++)
			outFile<<setw(20)<<D[i][j]<<setw(20);
		outFile<<endl;
	}
	outFile.close();
}


void CPHOTOGRAM::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (XP_IN==""||DM_OUT==""||WFW_OUT=="")
	{
		AfxMessageBox("输入或输出不能为空！");
		return;
	}

	double B[4][5]={0.0};
	TRY {
		input (B,4,5);   //从文件中读取控制点的影像坐标和地面坐标，存入数组B
		CaculateWFW(B,m_X0,m_Y0,m_FK,m_BLC,WFW_OUT,DM_OUT);

		int IDType=AfxMessageBox("解算成功，是否立即查看程序过程参数文档和外方位元素解算文档？",MB_YESNO|MB_ICONINFORMATION);
		if (IDType==IDYES)
		{
			ShellExecute(NULL,"open",WFW_OUT,NULL,NULL,SW_SHOWNORMAL);
			ShellExecute(NULL,"open",DM_OUT,NULL,NULL,SW_SHOWNORMAL);
		}
	}
	CATCH (CMemoryException, e)
	{
		AfxMessageBox("出现致命错误，解算失败！");
	}
	END_CATCH
	
	CDialogEx::OnOK();
}

//矩阵转置
template<typename T1,typename T2>void CPHOTOGRAM::transpose(T1*mat1,T2*mat2,int a,int b)
{  
	int i,j;
	for(i=0;i<b;i++)
		for(j=0;j<a;j++)
			mat2[j][i]=mat1[i][j];
	return;
}

//矩阵相乘
template<typename T1,typename T2>void CPHOTOGRAM::multi(T1*mat1,T2 * mat2,T2 * result,int a,int b,int c)
{   
	int i,j,k;
	for(i=0;i<a;i++)
	{
		for(j=0;j<c;j++)
		{
			result[i][j]=0;
			for(k=0;k<b;k++)
				result[i][j]+=mat1[i][k]*mat2[k][j];
		}
	}
	return;
}

template <typename T>void CPHOTOGRAM::input (T*mat,int a,int b)
{   
	ifstream inFile;
	inFile.open(XP_IN);
	while(!inFile.eof())
	{
		for (int i=0;i<a;i++)
			for(int j=0;j<b;j++)
				inFile>>mat[i][j];
	}
	inFile.close();
	return;
}

//矩阵求逆
void CPHOTOGRAM::inverse(double c[n][n])//求逆矩阵
{  
	int i,j,h,k;
	double p;
	double q[n][12];
	for(i=0;i<n;i++)//构造高斯矩阵
		for(j=0;j<n;j++)
			q[i][j]=c[i][j];
	for(i=0;i<n;i++)
		for(j=n;j<12;j++)
		{
			if(i+6==j)
				q[i][j]=1;
			else
				q[i][j]=0;
		}
		for(h=k=0;k<n-1;k++,h++)//消去对角线以下的数据
			for(i=k+1;i<n;i++)
			{
				if(q[i][h]==0)
					continue;
				p=q[k][h]/q[i][h];
				for(j=0;j<12;j++)
				{ 
					q[i][j]*=p;
					q[i][j]-=q[k][j];
				}
			}
			for(h=k=n-1;k>0;k--,h--) // 消去对角线以上的数据
				for(i=k-1;i>=0;i--)
				{
					if(q[i][h]==0)
						continue;
					p=q[k][h]/q[i][h];
					for(j=0;j<12;j++)
					{
						q[i][j]*=p;
						q[i][j]-=q[k][j];
					}
				}
				for(i=0;i<n;i++)//将对角线上数据化为1
				{ 
					p=1.0/q[i][i];
					for(j=0;j<12;j++)
						q[i][j]*=p;
				}
				for(i=0;i<n;i++) //提取逆矩阵
					for(j=0;j<n;j++)
						c[i][j]=q[i][j+6];
}

BOOL CPHOTOGRAM::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
