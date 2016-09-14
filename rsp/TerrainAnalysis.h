#pragma once
#include "rsp.h"
#include "gdal_priv.h"
#include "ProgressBar.h"

class TerrainAnalysis
{
public:
	TerrainAnalysis();
	TerrainAnalysis(const CString& inputFile, const CString& outputFile, const CString& outputFormat, int Mark);
	~TerrainAnalysis(void);
public:
	int processRaster( CProgressBar* p, CString title );
	GDALDatasetH openInputFile( int& nCellsX, int& nCellsY );
	GDALDriverH openOutputDriver();
	GDALDatasetH openOutputFile( GDALDatasetH inputDataset, GDALDriverH outputDriver );

protected:
	CString mInputFile;
	CString mOutputFile;
	CString mOutputFormat;

	double mCellSizeX;
	double mCellSizeY;
	/**The nodata value of the input layer*/
	float mInputNodataValue;
	/**The nodata value of the output layer*/
	float mOutputNodataValue;
	/**Scale factor for z-value if x-/y- units are different to z-units (111120 for degree->meters and 370400 for degree->feet)*/
	double mZFactor;
	//判断类别
	int mark;


public:
	double cellSizeX() const { return mCellSizeX; }
    void setCellSizeX( double size ) { mCellSizeX = size; }
    double cellSizeY() const { return mCellSizeY; }
    void setCellSizeY( double size ) { mCellSizeY = size; }

    double zFactor() const { return mZFactor; }
    void setZFactor( double factor ) { mZFactor = factor; }

    double inputNodataValue() const { return mInputNodataValue; }
    void setInputNodataValue( double value ) { mInputNodataValue = value; }
    double outputNodataValue() const { return mOutputNodataValue; }
    void setOutputNodataValue( double value ) { mOutputNodataValue = value; }

	//山体阴影参数设置
	float lightAzimuth() const { return mLightAzimuth; }
	void setLightAzimuth( float azimuth ) { mLightAzimuth = azimuth; }
	float lightAngle() const { return mLightAngle; }
	void setLightAngle( float angle ) { mLightAngle = angle; }

    /**Calculates output value from nine input values. The input values and the output value can be equal to the
      nodata value if not present or outside of the border. */
    float processNineCellWindow( float* x11, float* x21, float* x31,
                                         float* x12, float* x22, float* x32,
                                         float* x13, float* x23, float* x33 );

	float calcFirstDerX( float* x11, float* x21, float* x31, float* x12, float* x22, float* x32, float* x13, float* x23, float* x33 );
	/**Calculates the first order derivative in y-direction according to Horn (1981)*/
	float calcFirstDerY( float* x11, float* x21, float* x31, float* x12, float* x22, float* x32, float* x13, float* x23, float* x33 );
private:
	float mLightAzimuth;
	float mLightAngle;
};

