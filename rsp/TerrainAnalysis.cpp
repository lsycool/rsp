#include "StdAfx.h"
#include "TerrainAnalysis.h"
#define M_PI 3.14159265358979323846

TerrainAnalysis::TerrainAnalysis()
{
}

TerrainAnalysis::TerrainAnalysis(const CString& inputFile, const CString& outputFile, const CString& outputFormat, int Mark): mInputFile( inputFile ), mOutputFile( outputFile ), mOutputFormat( outputFormat ), mCellSizeX( -1 ), mCellSizeY( -1 ),
	mInputNodataValue( -1 ), mOutputNodataValue( -1 ), mZFactor( 1.0 ), mark( Mark )

{
}

TerrainAnalysis::~TerrainAnalysis(void)
{
}

int TerrainAnalysis::processRaster( CProgressBar* p, CString title )
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");	//open input file
	int xSize, ySize;
	GDALDatasetH  inputDataset = openInputFile( xSize, ySize );
	if ( inputDataset == NULL )
	{
		return 1; //opening of input file failed
	}

	//output driver
	GDALDriverH outputDriver = openOutputDriver();
	if ( outputDriver == 0 )
	{
		return 2;
	}

	GDALDatasetH outputDataset = openOutputFile( inputDataset, outputDriver );
	if ( outputDataset == NULL )
	{
		return 3; //create operation on output file failed
	}

	//open first raster band for reading (operation is only for single band raster)
	GDALRasterBandH rasterBand = GDALGetRasterBand( inputDataset, 1 );
	if ( rasterBand == NULL )
	{
		GDALClose( inputDataset );
		GDALClose( outputDataset );
		return 4;
	}
	mInputNodataValue = GDALGetRasterNoDataValue( rasterBand, NULL );

	GDALRasterBandH outputRasterBand = GDALGetRasterBand( outputDataset, 1 );
	if ( outputRasterBand == NULL )
	{
		GDALClose( inputDataset );
		GDALClose( outputDataset );
		return 5;
	}
	//try to set -9999 as nodata value
	GDALSetRasterNoDataValue( outputRasterBand, -9999 );
	mOutputNodataValue = GDALGetRasterNoDataValue( outputRasterBand, NULL );

	if ( ySize < 3 ) //we require at least three rows (should be true for most datasets)
	{
		GDALClose( inputDataset );
		GDALClose( outputDataset );
		return 6;
	}

	//keep only three scanlines in memory at a time
	float* scanLine1 = ( float * ) CPLMalloc( sizeof( float ) * xSize );
	float* scanLine2 = ( float * ) CPLMalloc( sizeof( float ) * xSize );
	float* scanLine3 = ( float * ) CPLMalloc( sizeof( float ) * xSize );

	float* resultLine = ( float * ) CPLMalloc( sizeof( float ) * xSize );

	if ( p )
	{
		p->ReSetProcess();  
		p->SetStepCount(ySize); //设置进度条的总步长
		p->SetMessage(title);
	}

	//values outside the layer extent (if the 3x3 window is on the border) are sent to the processing method as (input) nodata values
	for ( int i = 0; i < ySize; ++i )
	{
		if ( p )
		{
			p->StepIt();
		}

		/*	if ( p && p->wasCanceled() )
		{
		break;
		}*/

		if ( i == 0 )
		{
			//fill scanline 1 with (input) nodata for the values above the first row and feed scanline2 with the first row
			for ( int a = 0; a < xSize; ++a )
			{
				scanLine1[a] = mInputNodataValue;
			}
			GDALRasterIO( rasterBand, GF_Read, 0, 0, xSize, 1, scanLine2, xSize, 1, GDT_Float32, 0, 0 );
		}
		else
		{
			//normally fetch only scanLine3 and release scanline 1 if we move forward one row
			CPLFree( scanLine1 );
			scanLine1 = scanLine2;
			scanLine2 = scanLine3;
			scanLine3 = ( float * ) CPLMalloc( sizeof( float ) * xSize );
		}

		if ( i == ySize - 1 ) //fill the row below the bottom with nodata values
		{
			for ( int a = 0; a < xSize; ++a )
			{
				scanLine3[a] = mInputNodataValue;
			}
		}
		else
		{
			GDALRasterIO( rasterBand, GF_Read, 0, i + 1, xSize, 1, scanLine3, xSize, 1, GDT_Float32, 0, 0 );
		}

		for ( int j = 0; j < xSize; ++j )
		{
			if ( j == 0 )
			{
				resultLine[j] = processNineCellWindow( &mInputNodataValue, &scanLine1[j], &scanLine1[j+1], &mInputNodataValue, &scanLine2[j],
					&scanLine2[j+1], &mInputNodataValue, &scanLine3[j], &scanLine3[j+1] );
			}
			else if ( j == xSize - 1 )
			{
				resultLine[j] = processNineCellWindow( &scanLine1[j-1], &scanLine1[j], &mInputNodataValue, &scanLine2[j-1], &scanLine2[j],
					&mInputNodataValue, &scanLine3[j-1], &scanLine3[j], &mInputNodataValue );
			}
			else
			{
				resultLine[j] = processNineCellWindow( &scanLine1[j-1], &scanLine1[j], &scanLine1[j+1], &scanLine2[j-1], &scanLine2[j],
					&scanLine2[j+1], &scanLine3[j-1], &scanLine3[j], &scanLine3[j+1] );
			}
		}

		GDALRasterIO( outputRasterBand, GF_Write, 0, i, xSize, 1, resultLine, xSize, 1, GDT_Float32, 0, 0 );
	}

	if ( p )
	{
		p->SetPosition( 1 );
	}

	CPLFree( resultLine );
	CPLFree( scanLine1 );
	CPLFree( scanLine2 );
	CPLFree( scanLine3 );

	GDALClose( inputDataset );

	//if ( p && p->wasCanceled() )
	//{
	//	//delete the dataset without closing (because it is faster)
	//	GDALDeleteDataset( outputDriver, mOutputFile );
	//	return 7;
	//}
	GDALClose( outputDataset );

	return 0;
}

GDALDatasetH TerrainAnalysis::openInputFile( int& nCellsX, int& nCellsY )
{
	GDALDatasetH inputDataset = GDALOpen( mInputFile , GA_ReadOnly );
	if ( inputDataset != NULL )
	{
		nCellsX = GDALGetRasterXSize( inputDataset );
		nCellsY = GDALGetRasterYSize( inputDataset );

		//we need at least one band
		if ( GDALGetRasterCount( inputDataset ) < 1 )
		{
			GDALClose( inputDataset );
			return NULL;
		}
	}
	return inputDataset;
}

GDALDriverH TerrainAnalysis::openOutputDriver()
{
	char **driverMetadata;

	//open driver
	GDALDriverH outputDriver = GDALGetDriverByName( mOutputFormat );

	if ( outputDriver == NULL )
	{
		return outputDriver; //return NULL, driver does not exist
	}

	driverMetadata = GDALGetMetadata( outputDriver, NULL );
	if ( !CSLFetchBoolean( driverMetadata, GDAL_DCAP_CREATE, false ) )
	{
		return NULL; //driver exist, but it does not support the create operation
	}

	return outputDriver;
}

GDALDatasetH TerrainAnalysis::openOutputFile( GDALDatasetH inputDataset, GDALDriverH outputDriver )
{
	if ( inputDataset == NULL )
	{
		return NULL;
	}

	int xSize = GDALGetRasterXSize( inputDataset );
	int ySize = GDALGetRasterYSize( inputDataset );;

	//open output file
	char **papszOptions = NULL;
	GDALDatasetH outputDataset = GDALCreate( outputDriver, mOutputFile, xSize, ySize, 1, GDT_Float32, papszOptions );
	if ( outputDataset == NULL )
	{
		return outputDataset;
	}

	//get geotransform from inputDataset
	double geotransform[6];
	if ( GDALGetGeoTransform( inputDataset, geotransform ) != CE_None )
	{
		GDALClose( outputDataset );
		return NULL;
	}
	GDALSetGeoTransform( outputDataset, geotransform );

	//make sure mCellSizeX and mCellSizeY are always > 0
	mCellSizeX = geotransform[1];
	if ( mCellSizeX < 0 )
	{
		mCellSizeX = -mCellSizeX;
	}
	mCellSizeY = geotransform[5];
	if ( mCellSizeY < 0 )
	{
		mCellSizeY = -mCellSizeY;
	}

	const char* projection = GDALGetProjectionRef( inputDataset );
	GDALSetProjection( outputDataset, projection );

	return outputDataset;
}

float TerrainAnalysis::calcFirstDerX( float* x11, float* x21, float* x31, float* x12, float* x22, float* x32, float* x13, float* x23, float* x33 )
{
	//the basic formula would be simple, but we need to test for nodata values...
	//return (( (*x31 - *x11) + 2 * (*x32 - *x12) + (*x33 - *x13) ) / (8 * mCellSizeX));

	int weight = 0;
	double sum = 0;

	//first row
	if ( *x31 != mInputNodataValue && *x11 != mInputNodataValue ) //the normal case
	{
		sum += ( *x31 - *x11 );
		weight += 2;
	}
	else if ( *x31 == mInputNodataValue && *x11 != mInputNodataValue && *x21 != mInputNodataValue ) //probably 3x3 window is at the border
	{
		sum += ( *x21 - *x11 );
		weight += 1;
	}
	else if ( *x11 == mInputNodataValue && *x31 != mInputNodataValue && *x21 != mInputNodataValue ) //probably 3x3 window is at the border
	{
		sum += ( *x31 - *x21 );
		weight += 1;
	}

	//second row
	if ( *x32 != mInputNodataValue && *x12 != mInputNodataValue ) //the normal case
	{
		sum += 2 * ( *x32 - *x12 );
		weight += 4;
	}
	else if ( *x32 == mInputNodataValue && *x12 != mInputNodataValue && *x22 != mInputNodataValue )
	{
		sum += 2 * ( *x22 - *x12 );
		weight += 2;
	}
	else if ( *x12 == mInputNodataValue && *x32 != mInputNodataValue && *x22 != mInputNodataValue )
	{
		sum += 2 * ( *x32 - *x22 );
		weight += 2;
	}

	//third row
	if ( *x33 != mInputNodataValue && *x13 != mInputNodataValue ) //the normal case
	{
		sum += ( *x33 - *x13 );
		weight += 2;
	}
	else if ( *x33 == mInputNodataValue && *x13 != mInputNodataValue && *x23 != mInputNodataValue )
	{
		sum += ( *x23 - *x13 );
		weight += 1;
	}
	else if ( *x13 == mInputNodataValue && *x33 != mInputNodataValue && *x23 != mInputNodataValue )
	{
		sum += ( *x33 - *x23 );
		weight += 1;
	}

	if ( weight == 0 )
	{
		return mOutputNodataValue;
	}

	return sum / ( weight * mCellSizeX * mZFactor );
}

float TerrainAnalysis::calcFirstDerY( float* x11, float* x21, float* x31, float* x12, float* x22, float* x32, float* x13, float* x23, float* x33 )
{
	//the basic formula would be simple, but we need to test for nodata values...
	//return (((*x11 - *x13) + 2 * (*x21 - *x23) + (*x31 - *x33)) / ( 8 * mCellSizeY));

	double sum = 0;
	int weight = 0;

	//first row
	if ( *x11 != mInputNodataValue && *x13 != mInputNodataValue ) //normal case
	{
		sum += ( *x11 - *x13 );
		weight += 2;
	}
	else if ( *x11 == mInputNodataValue && *x13 != mInputNodataValue && *x12 != mInputNodataValue )
	{
		sum += ( *x12 - *x13 );
		weight += 1;
	}
	else if ( *x31 == mInputNodataValue && *x11 != mInputNodataValue && *x12 != mInputNodataValue )
	{
		sum += ( *x11 - *x12 );
		weight += 1;
	}

	//second row
	if ( *x21 != mInputNodataValue && *x23 != mInputNodataValue )
	{
		sum += 2 * ( *x21 - *x23 );
		weight += 4;
	}
	else if ( *x21 == mInputNodataValue && *x23 != mInputNodataValue && *x22 != mInputNodataValue )
	{
		sum += 2 * ( *x22 - *x23 );
		weight += 2;
	}
	else if ( *x23 == mInputNodataValue && *x21 != mInputNodataValue && *x22 != mInputNodataValue )
	{
		sum += 2 * ( *x21 - *x22 );
		weight += 2;
	}

	//third row
	if ( *x31 != mInputNodataValue && *x33 != mInputNodataValue )
	{
		sum += ( *x31 - *x33 );
		weight += 2;
	}
	else if ( *x31 == mInputNodataValue && *x33 != mInputNodataValue && *x32 != mInputNodataValue )
	{
		sum += ( *x32 - *x33 );
		weight += 1;
	}
	else if ( *x33 == mInputNodataValue && *x31 != mInputNodataValue && *x32 != mInputNodataValue )
	{
		sum += ( *x31 - *x32 );
		weight += 1;
	}

	if ( weight == 0 )
	{
		return mOutputNodataValue;
	}

	return sum / ( weight * mCellSizeY * mZFactor );
}

//计算坡度、坡向
float TerrainAnalysis::processNineCellWindow( float* x11, float* x21, float* x31,
	float* x12, float* x22, float* x32, float* x13, float* x23, float* x33 )
{
	float derX = calcFirstDerX( x11, x21, x31, x12, x22, x32, x13, x23, x33 );
	float derY = calcFirstDerY( x11, x21, x31, x12, x22, x32, x13, x23, x33 );
	if (mark==1)//计算坡度
	{
		if ( derX == mOutputNodataValue || derY == mOutputNodataValue )
		{
			return mOutputNodataValue;
		}
		return atan( sqrt( derX * derX + derY * derY ) ) * 180.0 / M_PI;
	}

	else if (mark==2)//计算坡向
	{
		if ( derX == mOutputNodataValue ||
			derY == mOutputNodataValue ||
			( derX == 0.0 && derY == 0.0 ) )
		{
			return mOutputNodataValue;
		}
		else
		{
			return 180.0 + atan2( derX, derY ) * 180.0 / M_PI;
		}
	}

	else if (mark==3)//计算TPI（地形耐用指数）
	{
		/*
		//return *x22; //test: write the raster value of the middle cell
		float diff1 = *x11 - *x22;
		float diff2 = *x21 - *x22;
		float diff3 = *x31 - *x22;
		float diff4 = *x12 - *x22;
		float diff5 = *x32 - *x22;
		float diff6 = *x13 - *x22;
		float diff7 = *x23 - *x22;
		float diff8 = *x33 - *x22;
		return sqrt(diff1 * diff1 + diff2 * diff2 + diff3 * diff3 + diff4 * diff4 + diff5 * diff5 + diff6 * diff6 + diff7 * diff7 + diff8 * diff8);
		*/
		if ( *x22 == mInputNodataValue )
		{
			return mOutputNodataValue;
		}

		double sum = 0;
		if ( *x11 != mInputNodataValue )
		{
			sum += ( *x11 - *x22 ) * ( *x11 - *x22 );
		}
		if ( *x21 != mInputNodataValue )
		{
			sum += ( *x21 - *x22 ) * ( *x21 - *x22 );
		}
		if ( *x31 != mInputNodataValue )
		{
			sum += ( *x31 - *x22 ) * ( *x31 - *x22 );
		}
		if ( *x12 != mInputNodataValue )
		{
			sum += ( *x12 - *x22 ) * ( *x12 - *x22 );
		}
		if ( *x32 != mInputNodataValue )
		{
			sum += ( *x32 - *x22 ) * ( *x32 - *x22 );
		}
		if ( *x13 != mInputNodataValue )
		{
			sum += ( *x13 - *x22 ) * ( *x13 - *x22 );
		}
		if ( *x23 != mInputNodataValue )
		{
			sum += ( *x23 - *x22 ) * ( *x23 - *x22 );
		}
		if ( *x33 != mInputNodataValue )
		{
			sum += ( *x33 - *x22 ) * ( *x33 - *x22 );
		}
		return sqrt( sum );
	}

	else if (mark==4)//计算山体阴影
	{
		if ( derX == mOutputNodataValue || derY == mOutputNodataValue )
		{
			return mOutputNodataValue;
		}
		float zenith_rad = mLightAngle * M_PI / 180.0;
		float slope_rad = atan( sqrt( derX * derX + derY * derY ) );
		float azimuth_rad = mLightAzimuth * M_PI / 180.0;
		float aspect_rad = 0;
		if ( derX == 0 && derY == 0 ) //aspect undefined, take a neutral value. Better solutions?
		{
			aspect_rad = azimuth_rad / 2.0;
		}
		else
		{
			aspect_rad = M_PI + atan2( derX, derY );
		}
		return max( 0.0, 255.0 * (( cos( zenith_rad ) * cos( slope_rad ) ) + ( sin( zenith_rad ) * sin( slope_rad ) * cos( azimuth_rad - aspect_rad ) ) ) );
	}

	else if (mark==5)//计算地形位置指数
	{
		if ( *x22 == mInputNodataValue )
		{
			return mOutputNodataValue;
		}

		double sum = 0;
		if ( *x11 != mInputNodataValue )
		{
			sum += ( *x11 )/8;
		}
		if ( *x21 != mInputNodataValue )
		{
			sum += ( *x21 )/8;
		}
		if ( *x31 != mInputNodataValue )
		{
			sum += ( *x31 )/8;
		}
		if ( *x12 != mInputNodataValue )
		{
			sum += ( *x12 )/8;
		}
		if ( *x32 != mInputNodataValue )
		{
			sum += ( *x32 )/8;
		}
		if ( *x13 != mInputNodataValue )
		{
			sum += ( *x13 )/8;
		}
		if ( *x23 != mInputNodataValue )
		{
			sum += ( *x23 )/8;
		}
		if ( *x33 != mInputNodataValue )
		{
			sum += ( *x33 )/8;
		}
		return *x22-sum;
	}

	else if (mark==6)//计算粗糙度
	{
		if ( *x22 == mInputNodataValue )
		{
			return mOutputNodataValue;
		}

		double RoughnessMax = 0;
		double RoughnessMin = 10000000;
		if ( *x11 != mInputNodataValue )
		{
			if (RoughnessMax<( *x11 - *x22 ) * ( *x11 - *x22 ))
			{
				RoughnessMax=( *x11 - *x22 ) * ( *x11 - *x22 );
			}
			if (RoughnessMin>( *x11 - *x22 ) * ( *x11 - *x22 ))
			{
				RoughnessMin=( *x11 - *x22 ) * ( *x11 - *x22 );
			}			
		}
		if ( *x21 != mInputNodataValue )
		{
			if (RoughnessMax<( *x21 - *x22 ) * ( *x21 - *x22 ))
			{
				RoughnessMax=( *x21 - *x22 ) * ( *x21 - *x22 );
			}
			if (RoughnessMin>( *x21 - *x22 ) * ( *x21 - *x22 ))
			{
				RoughnessMin=( *x21 - *x22 ) * ( *x21 - *x22 );
			}
		}
		if ( *x31 != mInputNodataValue )
		{
			if (RoughnessMax<( *x31 - *x22 ) * ( *x31 - *x22 ))
			{
				RoughnessMax=( *x31 - *x22 ) * ( *x31 - *x22 );
			}
			if (RoughnessMin>( *x31 - *x22 ) * ( *x31 - *x22 ))
			{
				RoughnessMin=( *x31 - *x22 ) * ( *x31 - *x22 );
			}
		}
		if ( *x12 != mInputNodataValue )
		{
			if (RoughnessMax< ( *x12 - *x22 ) * ( *x12 - *x22 ))
			{
				RoughnessMax= ( *x12 - *x22 ) * ( *x12 - *x22 );
			}
			if (RoughnessMin> ( *x12 - *x22 ) * ( *x12 - *x22 ))
			{
				RoughnessMin= ( *x12 - *x22 ) * ( *x12 - *x22 );
			}
		}
		if ( *x32 != mInputNodataValue )
		{
			if (RoughnessMax<( *x32 - *x22 ) * ( *x32 - *x22 ))
			{
				RoughnessMax=( *x32 - *x22 ) * ( *x32 - *x22 );
			}
			if (RoughnessMin>( *x32 - *x22 ) * ( *x32 - *x22 ))
			{
				RoughnessMin=( *x32 - *x22 ) * ( *x32 - *x22 );
			}
		}
		if ( *x13 != mInputNodataValue )
		{
			if (RoughnessMax<( *x13 - *x22 ) * ( *x13 - *x22 ))
			{
				RoughnessMax=( *x13 - *x22 ) * ( *x13 - *x22 );
			}
			if (RoughnessMin>( *x13 - *x22 ) * ( *x13 - *x22 ))
			{
				RoughnessMin=( *x13 - *x22 ) * ( *x13 - *x22 );
			}
		}
		if ( *x23 != mInputNodataValue )
		{
			if (RoughnessMax<( *x23 - *x22 ) * ( *x23 - *x22 ))
			{
				RoughnessMax=( *x23 - *x22 ) * ( *x23 - *x22 );
			}
			if (RoughnessMin>( *x23 - *x22 ) * ( *x23 - *x22 ))
			{
				RoughnessMin=( *x23 - *x22 ) * ( *x23 - *x22 );
			}
		}
		if ( *x33 != mInputNodataValue )
		{
			if (RoughnessMax<( *x33 - *x22 ) * ( *x33 - *x22 ))
			{
				RoughnessMax=( *x33 - *x22 ) * ( *x33 - *x22 );
			}
			if (RoughnessMin>( *x33 - *x22 ) * ( *x33 - *x22 ))
			{
				RoughnessMin=( *x33 - *x22 ) * ( *x33 - *x22 );
			}
		}
		return sqrt( RoughnessMax )-sqrt(RoughnessMin);
	}
	return 1;
}




