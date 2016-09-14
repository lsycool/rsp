#pragma once

#ifndef _BACKPROP_H_
#define _BACKPROP_H_
#define BIGRND 0x7fffffff


/************************************************************************/
/* ����������ݽṹ�����类�ٶ�Ϊһ��ȫ���ӵ�����ǰ��ṹ��
ÿ��ĵ�Ԫ0����ֵ����ζ����������Ԫ���Ϊ1-n                                                                     */
/************************************************************************/


typedef struct  
{
	int input_n;//��������Ԫ����
	int hidden_n;//���ز����Ԫ����
	int output_n;//��������Ԫ����

	double *input_units;//��������Ԫ
	double *hidden_units;//���ز����Ԫ
	double *output_units;//��������Ԫ

	double *hidden_delta;//���ز�����
	double *output_delta;//���������

	double *target;//Ŀ������

	double **input_weights;//����㵽���ز������Ȩ
	double **hidden_weights;//���ز㵽����������Ȩ


	/*���������ڵ�����ʱ��ʹ��*/
	double **input_prev_weights;//ǰ������㵽���ز�Ȩֵ�øı�
	double **hidden_prev_weights;//ǰ�����ز㵽�����Ȩֵ�ĸı�
}BPNN;

/************************************************************************/
/* �û��ӿ�                                                             */
/************************************************************************/

/*��ʼ�����������*/
void bpnn_initialize(int seed);

//����BP����
BPNN *bpnn_creat(int n_in,int n_hidden,int n_out);

//�ͷ�BP������ռ���ڴ�ռ�
void bpnn_free(BPNN *net);

//ѵ��BP����
void bpnn_train(BPNN *net,double eta,double momentum,double *eo,double *eh);

//ǰ������
void bpnn_feedforward(BPNN *net);

//����BP���絽�ļ���
void bpnn_save(BPNN *net,char *filename);

//���ļ��ж�ȡBP�������
BPNN *bpnn_read(char *filename);

#endif

// CBP �Ի���

class CBP : public CDialogEx
{
	DECLARE_DYNAMIC(CBP)

public:
	CBP(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBP();

// �Ի�������
	enum { IDD = IDD_DIALOG_BP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	CString save_path;//�洢������
	CString save_path1;//�洢���Ӱ��
	CString read_path;//��ȡӰ��
	CString read_path1;//��ȡ������
	double *InData1;
	double *InData2;
	BYTE* InData3;//������
	int nSImageSizeX;
	int nSImageSizeY;

	int nImageSizeX;
	int nImageSizeY;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTrain();
	double m_RMS;
};
