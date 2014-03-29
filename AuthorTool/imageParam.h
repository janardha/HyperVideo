#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"
#include "stdafx.h"
// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

class VideoDisplay 
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	primaryPath[_MAX_PATH];	// Image location
	char	secondaryPath[_MAX_PATH];
	unsigned char*	DataPrimary;	// RGB data of the image
	unsigned char*	DataSecondary;
	unsigned char*	PrimaryFrameData;
	unsigned char*	SecondaryFrameData;
	int		frameSize;
	int		frameRate;
	int primarySelect;

public:
	// Constructor
	VideoDisplay();
	// Copy Constructor
	VideoDisplay::VideoDisplay( VideoDisplay *otherImage);
	// Destructor
	~VideoDisplay();

	// operator overload
	VideoDisplay & operator= (const VideoDisplay & otherImage);

	// Reader & Writer functions
	void	setWidth( const int w)  { Width = w; }; 
	void	setHeight(const int h) { Height = h; }; 
	/////////////////////////////////////////////////////
	void	setImageData( const unsigned char *img ) 
	{ 
		if(primarySelect == 0)
			DataPrimary = (unsigned char *)img; 
		else
			DataSecondary = (unsigned char *)img; 
	};
	/////////////////////////////////////////////////
	void	setPrimaryImagePath( const char *path){strcpy(primaryPath, path); };
	void	setSecondaryImagePath( const char *path){strcpy(secondaryPath, path); };
	////////////////////////////////////////////////////
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	unsigned char*	getPrimaryImageData() { return DataPrimary; };
	unsigned char*	getSecondaryImageData() { return DataSecondary; };
	char*	getPrimaryImagePath() { return primaryPath; };
	char*	getSecondaryImagePath() { return secondaryPath; };

	////////////////////////////////////////////////////////
	int		getPrimarySelect() { return primarySelect;};
	void	setPrimarySelect(const int n) { primarySelect = n;};
	///////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	void	setFrameSize();
	void	setFrameRate(const int f){frameRate = f;};
	unsigned char*	getPrimaryFrameData(const int framenum);
	unsigned char*	getSecondaryFrameData(const int framenum);
	int		getFrameRate(){return frameRate;};
	int		getFrameSize(){return frameSize;};
	////////////////////////////////////////////////////

	// Input Output operations
	bool	ReadPrimaryImage();
	bool	ReadSecondaryImage();
	bool	WriteImage();

	// Modifications
	bool	Modify();

};

#endif //IMAGE_DISPLAY