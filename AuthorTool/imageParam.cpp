#include "stdafx.h"
#include "imageParam.h"


// Constructor and Desctructors
VideoDisplay::VideoDisplay() 
{
	DataPrimary = NULL;
	DataSecondary = NULL;
	Width = 352;
	Height = 288;
	frameSize = 720;
	primaryPath[0] = 0;
	secondaryPath[0] = 0;
	PrimaryFrameData = NULL;
	SecondaryFrameData = NULL;
}

VideoDisplay::~VideoDisplay()
{
	if ( DataPrimary )
		delete DataPrimary;
	if (DataSecondary)
		delete DataSecondary;
	if (PrimaryFrameData)
		delete PrimaryFrameData;
	if (SecondaryFrameData)
		delete SecondaryFrameData;
}


// Copy constructor
VideoDisplay::VideoDisplay( VideoDisplay *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	frameSize = otherImage->frameSize;
	frameRate = otherImage->frameRate;

	DataPrimary   = new unsigned char[Width*Height*3*frameSize];
	DataSecondary   = new unsigned char[Width*Height*3*frameSize];
	PrimaryFrameData = new unsigned char[Width * Height * 3];
	SecondaryFrameData = new unsigned char[Width * Height * 3];

	strcpy(primaryPath , otherImage->primaryPath);
	strcpy(secondaryPath , otherImage->secondaryPath);

	for ( int i=0; i<(Height*Width*3*frameSize); i++ )
	{
		DataPrimary[i]	= otherImage->DataPrimary[i];
		DataSecondary[i]	= otherImage->DataSecondary[i];
	}

}



// = operator overload
VideoDisplay & VideoDisplay::operator= (const VideoDisplay &otherImage)
{
	
	Height = otherImage.Height;
	Width  = otherImage.Width;
	frameSize = otherImage.frameSize;
	frameRate = otherImage.frameRate;

	DataPrimary   = new unsigned char[Width*Height*3*frameSize];
	DataSecondary   = new unsigned char[Width*Height*3*frameSize];
	PrimaryFrameData = new unsigned char[Width * Height * 3];
	SecondaryFrameData = new unsigned char[Width * Height * 3];

	strcpy( (char *)otherImage.primaryPath, primaryPath );
	strcpy( (char *)otherImage.secondaryPath, secondaryPath );

	for ( int i=0; i<(Height*Width*3*frameSize); i++ )
	{
		DataPrimary[i]	= otherImage.DataPrimary[i];
		DataSecondary[i]	= otherImage.DataSecondary[i];
	}
	
	return *this;

}

void VideoDisplay::setFrameSize()
{
	long filesize;
	FILE *IN_FILE;
	IN_FILE = fopen(primaryPath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
	//	return false;
	}

	fseek (IN_FILE, 0, SEEK_END);
	filesize = ftell(IN_FILE);

//	setFrameSize(filesize/(Width*Height*3));
	frameSize = filesize/(Width*Height*3);
}

unsigned char* VideoDisplay::getPrimaryFrameData(const int framenum)
{
	//delete FrameData;
	int i, j;
	

	j = Width * Height * 3 * framenum;
	for(i = 0; i < Width * Height * 3; i++)
		PrimaryFrameData[i] = DataPrimary[i + j];

	return PrimaryFrameData;
}

unsigned char* VideoDisplay::getSecondaryFrameData(const int framenum)
{
	//delete FrameData;
	int i, j;
	

	j = Width * Height * 3 * framenum;
	for(i = 0; i < Width * Height * 3; i++)
		SecondaryFrameData[i] = DataSecondary[i + j];

	return SecondaryFrameData;
}
// VideoDisplay::ReadImage
// Function to read the image given a path
bool VideoDisplay::ReadPrimaryImage()
{
	FILE *IN_FILE;
	// Verify ImagePath
	if (primaryPath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	
	IN_FILE = fopen(primaryPath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}
	if(DataPrimary)
	{
		delete DataPrimary;
	}
	DataPrimary = new unsigned char[Width*Height*3*frameSize];

	PrimaryFrameData = new unsigned char[Width * Height * 3];
	
	int i, j;
	int frameIndex;
	
	unsigned char *Rbuf = new unsigned char[Height*Width]; 
	unsigned char *Gbuf = new unsigned char[Height*Width]; 
	unsigned char *Bbuf = new unsigned char[Height*Width]; 
	
	
	for(frameIndex = 0; frameIndex < frameSize; frameIndex++)
	{
		for (i = 0; i < Width*Height; i ++)
		{
			Rbuf[i] = fgetc(IN_FILE);
		}
		for (i = 0; i < Width*Height; i ++)
		{
			Gbuf[i] = fgetc(IN_FILE);
		}
		for (i = 0; i < Width*Height; i ++)
		{
			Bbuf[i] = fgetc(IN_FILE);
		}
	
		j = Height*Width*3*frameIndex;

	// Allocate Data structure and copy
	//Data = new char[Width*Height*3];

		for (i = 0; i < Height*Width; i++)
		{
			DataPrimary[j + 3*i]	= Bbuf[i];
			DataPrimary[j + 3*i+1]	= Gbuf[i];
			DataPrimary[j + 3*i+2]	= Rbuf[i];
		}
	
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}

bool VideoDisplay::ReadSecondaryImage()
{
	FILE *IN_FILE;
	
	if (secondaryPath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	IN_FILE = fopen(secondaryPath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}
	if(DataSecondary)
	{
		delete DataSecondary;
	}
	DataSecondary = new unsigned char[Width*Height*3*frameSize];

	SecondaryFrameData = new unsigned char[Width * Height * 3];

	int i, j;
	int frameIndex;
	
	unsigned char *Rbuf = new unsigned char[Height*Width]; 
	unsigned char *Gbuf = new unsigned char[Height*Width]; 
	unsigned char *Bbuf = new unsigned char[Height*Width]; 
	
	
	for(frameIndex = 0; frameIndex < frameSize; frameIndex++)
	{
		for (i = 0; i < Width*Height; i ++)
		{
			Rbuf[i] = fgetc(IN_FILE);
		}
		for (i = 0; i < Width*Height; i ++)
		{
			Gbuf[i] = fgetc(IN_FILE);
		}
		for (i = 0; i < Width*Height; i ++)
		{
			Bbuf[i] = fgetc(IN_FILE);
		}
	
		j = Height*Width*3*frameIndex;

	// Allocate Data structure and copy
	//Data = new char[Width*Height*3];
		
		for (i = 0; i < Height*Width; i++)
		{
			DataSecondary[j + 3*i]		= Bbuf[i];
			DataSecondary[j + 3*i+1]	= Gbuf[i];
			DataSecondary[j + 3*i+2]	= Rbuf[i];
		}
		
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}

// VideoDisplay functions defined here
bool VideoDisplay::WriteImage()
{
	// Verify ImagePath
	//// Verify ImagePath
	//if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	//{
	//	fprintf(stderr, "Image or Image properties not defined");
	//	return false;
	//}
	//
	//// Create a valid output file pointer
	//FILE *OUT_FILE;
	//OUT_FILE = fopen(ImagePath, "wb");
	//if ( OUT_FILE == NULL ) 
	//{
	//	fprintf(stderr, "Error Opening File for Writing");
	//	return false;
	//}

	//// Create and populate RGB buffers
	//int i;
	///*unsigned char *Rbuf = new unsigned char[Height*Width]; 
	//unsigned char *Gbuf = new unsigned char[Height*Width]; 
	//unsigned char *Bbuf = new unsigned char[Height*Width]; */

	//for (i = 0; i < Height*Width; i++)
	//{
	//	Bbuf[i] = Data[3*i];
	//	Gbuf[i] = Data[3*i+1];
	//	Rbuf[i] = Data[3*i+2];
	//}

	//
	//// Write data to file
	//for (i = 0; i < Width*Height; i ++)
	//{
	//	fputc(Rbuf[i], OUT_FILE);
	//}
	//for (i = 0; i < Width*Height; i ++)
	//{
	//	fputc(Gbuf[i], OUT_FILE);
	//}
	//for (i = 0; i < Width*Height; i ++)
	//{
	//	fputc(Bbuf[i], OUT_FILE);
	//}
	//
	//// Clean up and return
	///*delete Rbuf;
	//delete Gbuf;
	//delete Bbuf;*/
	//fclose(OUT_FILE);


	////no operations to be done yet////
	return true;

}