#pragma once

struct ComplexNumber
{
	float imag;// imaginary虚部
	float real;//实部
};
// CArthmetic 对话框

class CArthmetic : public CDialogEx
{
	DECLARE_DYNAMIC(CArthmetic)

public:
	CArthmetic(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CArthmetic();

// 对话框数据
	enum { IDD = IDD_DIALOG_ATHMTC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void WeiShiClass(double a[][2],double b[][2],int a1,int b1,double Afa);
	template<typename T1,typename T2>void multi(T1*mat1,T2 * mat2,T2 * result,int a,int b,int c);
	afx_msg void OnBnClickedButton1();
	GDALDataset* poDataset;//GDAL数据集
	GDAL_GCP * m_Gcps; //控制点对
	BYTE* DATA;
	double* DATA1;
	double* OUTDATA;
	BYTE* OUTDATA1;
	bool flag;
	int nImageSizeX;
	int nImageSizeY;
	CString file_in;
	CString file_out;
	ComplexNumber* buffer;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	CTreeCtrl m_TreeFile;
	virtual BOOL OnInitDialog();
	CEdit m_edit;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void ReadImage(double* &Data,int &nImageSizeX,int &nImageSizeY,CString FilePath);
	void ReadImage(BYTE* &Data,int &nImageSizeX,int &nImageSizeY,CString FilePath);
	void SaveImage(BYTE* Outdata,int nImageSizeX,int nImageSizeY,int bandcout,CString OutPath, CString ImageType);
	void SaveImage(double* Outdata,int nImageSizeX,int nImageSizeY,int bandcout,CString OutPath, CString ImageType);
	void RGBtoHIS(BYTE* DATA,double* OUTDATA);
	void ShowImageInfo(void);
	afx_msg void OnBnClickedCancel();
	void IHStoRGB(double* DATA1,double* OUTDATA);
	// 一维快速傅里叶
	void ImageFFt2D(BYTE* In_img,int width, int height,double** Out_img);
	void ImageFFt1D(ComplexNumber *arrayBuf, int n);
	//傅里叶反变换
	void ImageRevFFt1D(ComplexNumber *arrayBuf, int n);
	void ImageRevFFt2D(double* In_img, int width, int height,BYTE* Out_img);
	double* ValueToImg(double **value , int nBand, int hang,int lie);
	BYTE* ValueToImg(BYTE **value , int nBand, int hang,int lie);
	BYTE** OneToTwo(BYTE *value , int hang, int lie);
	BYTE*  TwoToOne(BYTE **value , int hang, int lie);

	// 离散傅里叶
	void FFT(BYTE* indata,int width,int hight,double** outdata);
	void FTTN(double* In_img, int width, int height,BYTE* Out_img);
	// 计算影像协方差
	double** XieFangCha(BYTE* value,int nBand,int lie,int hang);
	//求实对称矩阵的特征值及特征向量的雅格比法
	int Eigs(double **a, int n, double *d, double **v, int& nrot);
	//矩阵乘法的函数 
	double** matrix_mutiply(double **Matrix_1, double **Matrix_2, int Row1, int Column1, int Row2, int Column2);
	// 投影变换
	void ProTran(CString ReferenceFileName,CString OriFIleName);
	//高通滤波
	void Hightong(BYTE *PImg,int m_cols,int m_rows);
	//空间域不加权影像融合
	void Unweighting(BYTE *MImg,BYTE *PImg,int m_cols,int m_rows);
	//空间域加权影像融合
	void Weighting(BYTE *MImg,BYTE *PImg,int m_cols,int m_rows);
	void AddGaussNoise(BYTE* In_img, int width, int height,int nband,BYTE* Out_img);
	void AddPepperSaltNoise(BYTE* In_img, int width,int height,int nband,BYTE* Out_img);
	void Convolution(BYTE *PImg,double* temp,int m_cols,int m_rows,int nBand);
	// Robert算法
	void Robert(BYTE* In_img, int width,int height,int nband,BYTE* Out_img);
	void Sobel(BYTE *PImg,BYTE* Out_img,int m_cols,int m_rows,int nBand,int chidu);
	// Prewitt算子
	void Prewitt(BYTE *PImg,BYTE* Out_img,int m_cols,int m_rows,int nBand,int chidu);
	void Laplacian(BYTE *PImg,int m_cols,int m_rows,int nBand);
	// 边界追踪
	void OnEdgeTrace(BYTE* In_img, int width,int height,BYTE* Out_img);
	//区域增长
	void RegionGrow(BYTE* In_img, int width,int height,int nBand,BYTE* Out_img,CPoint SeedPos, int thresh);
	// //影像裁剪
	int ImgSubset(CPoint begin,CPoint end,BYTE* In_img,BYTE* Out_img,CString OutPath,CString ImageType);
	// 重采样
	void ChongCaiYang(BYTE* &Data,int nImageSizeX,int nImageSizeY,CString FilePath);

	void MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd, unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);
    void MatrixSub(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);
	void MatrixSampleLine(unsigned char *matrixInput, unsigned char *matrixOutputOdd,unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);
	void MatrixPlus(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);
	void MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,unsigned char *matrixDest, int heightDest, int widthDest);
	
	void WavlDecmposeTrans(unsigned char *inputData, int inputHeight, int inputWidth,int OutImageSizeX,int OutImageSizeY, int scale, unsigned char *outputData);
	void BasicWaveletTrans(BYTE* m_pImgData,BYTE* m_pImgDataOut,int OutImageSizeX,int OutImageSizeY,int scale);

	void Watershed(unsigned char **OriginalImage, char** SeedImage, BYTE **LabelImage, int row, int col);
};
	void Trans(double * Transform,double x,double y,double &l,double & p);
