#pragma once

#ifndef _BACKPROP_H_
#define _BACKPROP_H_
#define BIGRND 0x7fffffff


/************************************************************************/
/* 神经网络的数据结构，网络被假定为一个全连接的三层前向结构，
每层的单元0是阈值这意味着真正的神经元编号为1-n                                                                     */
/************************************************************************/


typedef struct  
{
	int input_n;//输入层的神经元个数
	int hidden_n;//隐藏层的神经元个数
	int output_n;//输出层的神经元个数

	double *input_units;//输入层的神经元
	double *hidden_units;//隐藏层的神经元
	double *output_units;//输出层的神经元

	double *hidden_delta;//隐藏层的误差
	double *output_delta;//输出层的误差

	double *target;//目标向量

	double **input_weights;//输入层到隐藏层的连接权
	double **hidden_weights;//隐藏层到输出层的连接权


	/*下面两个在迭代的时候使用*/
	double **input_prev_weights;//前次输入层到隐藏层权值得改变
	double **hidden_prev_weights;//前次隐藏层到输出层权值的改变
}BPNN;

/************************************************************************/
/* 用户接口                                                             */
/************************************************************************/

/*初始化随机数种子*/
void bpnn_initialize(int seed);

//创建BP网络
BPNN *bpnn_creat(int n_in,int n_hidden,int n_out);

//释放BP网络所占的内存空间
void bpnn_free(BPNN *net);

//训练BP网络
void bpnn_train(BPNN *net,double eta,double momentum,double *eo,double *eh);

//前向运算
void bpnn_feedforward(BPNN *net);

//保存BP网络到文件中
void bpnn_save(BPNN *net,char *filename);

//从文件中读取BP网络参数
BPNN *bpnn_read(char *filename);

#endif

// CBP 对话框

class CBP : public CDialogEx
{
	DECLARE_DYNAMIC(CBP)

public:
	CBP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBP();

// 对话框数据
	enum { IDD = IDD_DIALOG_BP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	float m_rate;
	float m_monument;
	float m_diedai;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonBpsave();
	afx_msg void OnBnClickedButtonBpread();
	BPNN *net;
	CString save_path;//存储神经网络
	CString save_path1;//存储结果影像
	CString read_path;//读取影像
	CString read_path1;//读取神经网络
	double *InData1;
	double *InData2;
	BYTE* InData3;//输出结果
	int nSImageSizeX;
	int nSImageSizeY;

	int nImageSizeX;
	int nImageSizeY;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTrain();
	double m_RMS;
};
