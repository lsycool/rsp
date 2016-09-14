// PHOTOGRAM.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rsp.h"
#include "PHOTOGRAM.h"
#include "afxdialogex.h"


// CPHOTOGRAM �Ի���

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


// CPHOTOGRAM ��Ϣ�������


void CPHOTOGRAM::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(true, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|�ı�(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="��������Ȥ���ļ�";
	if (file.DoModal()==IDOK)
	{
		XP_IN=file.GetPathName();	
	}
	SetDlgItemText(IDC_STATIC_XPIN,XP_IN);
}


void CPHOTOGRAM::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|�ı�(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="��������Ȥ���ļ�";
	if (file.DoModal()==IDOK)
	{
		DM_OUT=file.GetPathName();	
	}
	SetDlgItemText(IDC_STATIC_DMIN,DM_OUT);
}


void CPHOTOGRAM::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog file(false, "txt", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"����֧���ļ�(*.*)|*.*|�ı�(*.txt)|*.txt||");
	file.m_ofn.lpstrTitle="��������Ȥ���ļ�";
	if (file.DoModal()==IDOK)
	{
		WFW_OUT=file.GetPathName();	
	}
	SetDlgItemText(IDC_STATIC_WFWOUT,WFW_OUT);
}



//�����ⷽλԪ��
//����˵�� double B[4][5]:�洢���Ƶ���Ϣ��float x0,float y0,float fk���ڷ�λԪ�أ�m����Ƭ�����ߣ�FilePath���ⷽλԪ�ر���·��
void CPHOTOGRAM::CaculateWFW(double B[4][5], float x0,float y0,float fk,float m,CString FilePath,CString FilePath1)
{
	double Xs=0.0, Ys=0.0, Zs=0.0,Q=0.0,W=0.0,K=0.0; //�ⷽλԪ��
	
	double 	R[3][3]/*��ת����*/,
			XG[6][1],//�ⷽλԪ�ؽ���ֵ�ĸ�����
			A[8][6],//�ⷽλԪ�ظ�������ϵ��
			AT[6][8],//����A��ת��
			ATA[6][6],//���ڽ����ⷽλԪ�ؽ���ֵ���������м���
			ATL[6][1],
			X,//������Ӱ��������
			Y,
			Z,
			L[8][1];//���

	//ȷ��δ֪���ĳ�ʼֵ
	for(int i=0;i<4;i++)
	{
		Xs=Xs+B[i][2];
		Ys=Ys+B[i][3];
		Zs=Zs+B[i][4];
	}
	Xs=Xs/4; Ys=Ys/4; Zs=/*Zs/4+*/m*fk;//ȷ��δ֪���ĳ�ʼֵ
	int f=0;

	//�æצئʱ�ʾ��������
	do//��������
	{
		f++;
		//�����ת����
		R[0][0]=cos(Q)*cos(K)-sin(Q)*sin(W)*sin(K);//a1
		R[0][1]=-cos(Q)*sin(K)-sin(Q)*sin(W)*cos(K);//a2
		R[0][2]=-sin(Q)*cos(W);//a3
		R[1][0]=cos(W)*sin(K);//b1
		R[1][1]=cos(W)*cos(K);//b2
		R[1][2]=-sin(W);//b3
		R[2][0]=sin(Q)*cos(K)+cos(Q)*sin(W)*sin(K);//c1
		R[2][1]=-sin(Q)*sin(K)+cos(Q)*sin(W)*cos(K);//c2
		R[2][2]=cos(Q)*cos(W);//c3
		//����ϵ����ͳ�����
		for(int i=0,k=0,j=0;i<=3;i++,k++,j++)
		{
			X=R[0][0]*(B[i][2]-Xs)+R[1][0]*(B[i][3]-Ys)+R[2][0]*(B[i][4]-Zs);//������ռ丨������ϵ�������
			Y=R[0][1]*(B[i][2]-Xs)+R[1][1]*(B[i][3]-Ys)+R[2][1]*(B[i][4]-Zs);
			Z=R[0][2]*(B[i][2]-Xs)+R[1][2]*(B[i][3]-Ys)+R[2][2]*(B[i][4]-Zs);

			L[j][0]=B[i][0]-(x0-fk*X/Z);//��ʵ��Ƭֵ�������Ƭ����ֵ���
			L[j+1][0]=B[i][1]-(y0-fk*Y/Z);
			j++;

			A[k][0]=(R[0][0]*fk+R[0][2]*(B[i][0]-x0))/Z;//�ⷽλԪ�ؽ���ֵ��������ϵ��
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

		transpose(A,AT,6,8);//����ת��
		multi(AT,A,ATA,6,8,6);//�������
		inverse(ATA);//(ATA����)���� 
		multi(AT,L,ATL,6,8,1);//(AT,L)�������,������浽ATL������
		multi(ATA,ATL,XG,6,6,1);//(ATA,ATL)�������,�õ��ⷽλԪ�ؽ���ֵ�ĸ����������浽XG�����У�

		Xs=Xs+XG[0][0]; Ys=Ys+XG[1][0]; Zs=Zs+XG[2][0];
		Q=Q+XG[3][0];  W=W+XG[4][0];  K=K+XG[5][0];
	}while(fabs(XG[3][0])>=6.0/206265.0||fabs(XG[4][0])>=6.0/206265.0||fabs(XG[5][0])>=6.0/206265.0);//��������

	//��������
	double AXG[8][1],V[8][1],VT[1][8],VTV[1][1],m0,D[6][6];
	multi(A,XG,AXG,8,6,1);
	for(int i=0;i<8;i++)     //���������
		V[i][0]=AXG[i][0]-L[i][0];
	transpose (V,VT,1,8);
	multi(VT,V,VTV,1,8,1);
	m0=VTV[0][0]/2;
	for(int i=0;i<6;i++)
		for(int j=0;j<6;j++)
			D[i][j]=m0*ATA[i][j];//����δ֪���������

	//��Ļ�������ϵ���󡢳����������
	ofstream outFile;
	outFile.open(FilePath1,ios::app);
	outFile<<"************�ĵ�˵������Ƭ�ռ�󷽽����ⷽλԪ�ؽ���"<<endl;
	outFile<<"************ѧϰ��λ���й����ʴ�ѧ"<<endl;
	outFile<<"************��Ȩ���У�������"<<endl<<endl<<endl;

	outFile<<"��������Ϊ��"<<f<<endl<<endl;

	outFile<<"����ϵ����AΪ��"<<endl<<endl;
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<6;j++)
			outFile<<setw(20)<<A[i][j];
		outFile<<endl;
	}
	outFile<<endl;

	outFile<<endl<<"������LΪ��"<<endl<<endl;
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<1;j++)
			outFile<<setw(20)<<L[i][j];
		outFile<<endl;
	}
	outFile<<endl;

	outFile<<endl<<"������Ϊ��"<<endl<<endl;
	for(int i=0;i<6;i++)
	{
		for(int j=0;j<1;j++)
			outFile<<setw(20)<<XG[i][j];
		outFile<<endl;
	}
	outFile<<endl<<endl;
	outFile.close();
	/*output(A,"����ϵ����AΪ��",8,6);
	output(L,"������LΪ��",8,1);
	output(XG,"������Ϊ��",6,1);*/
	
	outFile.open(FilePath,ios::app);    //�򿪲��������ļ�
	outFile.precision(10);
	//���ļ�����ʽ�����Ƭ�ⷽλԪ�ء���ת���󡢷�����
	outFile<<"************�ĵ�˵������Ƭ�ռ�󷽽����ⷽλԪ�ؽ���"<<endl;
	outFile<<"************ѧϰ��λ���й����ʴ�ѧ"<<endl;
	outFile<<"************��Ȩ���У�������"<<endl<<endl<<endl;
	outFile<<"***��Ƭ���ⷽλԪ��Ϊ��"<<endl<<endl;
	outFile<<"Xs="<<Xs<<setw(10)<<"Ys="<<Ys<<setw(10)<<"Zs="<<Zs<<endl<<endl;
	outFile<<"�������Ϊ��"<<Q<<setw(10)<<endl<<"�������Ϊ��"<<W<<setw(10)<<endl<<"��Ƭ����Ϊ��"<<K<<endl;
	
	outFile<<endl<<"***��ת����RΪ:"<<endl<<endl;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
			outFile<<setw(20)<<R[i][j]<<setw(20);
		outFile<<endl;
	}
	outFile<<endl<<endl;

	outFile<<"***�����������Ϊ��"<<endl<<endl;
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (XP_IN==""||DM_OUT==""||WFW_OUT=="")
	{
		AfxMessageBox("������������Ϊ�գ�");
		return;
	}

	double B[4][5]={0.0};
	TRY {
		input (B,4,5);   //���ļ��ж�ȡ���Ƶ��Ӱ������͵������꣬��������B
		CaculateWFW(B,m_X0,m_Y0,m_FK,m_BLC,WFW_OUT,DM_OUT);

		int IDType=AfxMessageBox("����ɹ����Ƿ������鿴������̲����ĵ����ⷽλԪ�ؽ����ĵ���",MB_YESNO|MB_ICONINFORMATION);
		if (IDType==IDYES)
		{
			ShellExecute(NULL,"open",WFW_OUT,NULL,NULL,SW_SHOWNORMAL);
			ShellExecute(NULL,"open",DM_OUT,NULL,NULL,SW_SHOWNORMAL);
		}
	}
	CATCH (CMemoryException, e)
	{
		AfxMessageBox("�����������󣬽���ʧ�ܣ�");
	}
	END_CATCH
	
	CDialogEx::OnOK();
}

//����ת��
template<typename T1,typename T2>void CPHOTOGRAM::transpose(T1*mat1,T2*mat2,int a,int b)
{  
	int i,j;
	for(i=0;i<b;i++)
		for(j=0;j<a;j++)
			mat2[j][i]=mat1[i][j];
	return;
}

//�������
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

//��������
void CPHOTOGRAM::inverse(double c[n][n])//�������
{  
	int i,j,h,k;
	double p;
	double q[n][12];
	for(i=0;i<n;i++)//�����˹����
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
		for(h=k=0;k<n-1;k++,h++)//��ȥ�Խ������µ�����
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
			for(h=k=n-1;k>0;k--,h--) // ��ȥ�Խ������ϵ�����
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
				for(i=0;i<n;i++)//���Խ��������ݻ�Ϊ1
				{ 
					p=1.0/q[i][i];
					for(j=0;j<12;j++)
						q[i][j]*=p;
				}
				for(i=0;i<n;i++) //��ȡ�����
					for(j=0;j<n;j++)
						c[i][j]=q[i][j+6];
}

BOOL CPHOTOGRAM::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	VERIFY( 1 == SetSingleDialogSkin( m_hWnd, "Dialog" ) );

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
