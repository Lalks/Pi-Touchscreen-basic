/*
 * This program is made to test the library made by mwilliams03 (https://github.com/mwilliams03)
 * My goal is to use it, and if I have to and the the ability to improve it, I will.
 */


#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include "touch.h"
#include "touch.c"
#include "framebuffer.c"
#include <signal.h>

//#include <stdint.h>

// Freetype includes
#include <ft2build.h>
#include FT_FREETYPE_H


// Freetype library variable
FT_Library ftlib;


void  INThandler(int sig)
{
        signal(sig, SIG_IGN);
	closeFramebuffer();
        exit(0);
}

int main()
{
	signal(SIGINT, INThandler);
	
	// Check if there is an error when trying to init freetype
	/*short error = FT_Init_FreeType(&ftlib);
	if(error)
	{
		printf("An error occured during freetype library initialization\n");
	}*/

	int  xres,yres,x;

	int Xsamples[20];
	int Ysamples[20];

	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;

	int Xaverage, Yaverage = 0;


	if (openTouchScreen() == 1)
		perror("error opening touch screen");


	getTouchScreenDetails(&screenXmin,&screenXmax,&screenYmin,&screenYmax);

	framebufferInitialize(&xres,&yres);

	scaleXvalue = ((float)screenXmax-screenXmin) / xres;
	printf ("X Scale Factor = %f\n", scaleXvalue);
	scaleYvalue = ((float)screenYmax-screenYmin) / yres;
	printf ("Y Scale Factor = %f\n", scaleYvalue);


	// BEGGINING OF CUSTOMIZATION :::LALKS:::
	

	uint8_t color[3] = {40,40,40}; 
	uint8_t borderColor[3] = {0,187,255};
	short margin = 200;

	struct Button button1;
	// 
	button1 = drawButton(screenXmin+margin, screenYmin+margin, 30, xres-margin*2, color,  2, borderColor);
	
	// I want a text string in the center of the button
	// reminder : one character is 8 pixels width, and 8 pixels height
	char String[] = "Touch me, a surprise will come !";
	int StringLenght = sizeof(String);

	put_string(screenXmin+20, screenYmin+20, String, 25,  borderColor);
	
	while(1)
	{
		getTouchSample(&rawX, &rawY, &rawPressure);
		
		scaledY = rawX/scaleYvalue;
		scaledX = xres-(rawY/scaleXvalue);

		printf("scaledX = %d\nscaledY = %d\n", scaledX, scaledY);
		printf("button1.TopLeftCorner[0] = %d\nbutton1.BottomRightCorner[0] = %d\n", button1.TopLeftCorner[0], button1.BottomRightCorner[0]);
		printf("button1.TopLeftCorner[1] = %d\nbutton1.BottomRightCorner[1] = %d", button1.TopLeftCorner[1], button1.BottomRightCorner[1]);

		printf("\n----------------------------------\n");
		
		uint8_t A[3] = {255,255,255}; 
		drawSquare(scaledX, scaledY, 3,3, A, 1 ,A);

		if( button1.TopLeftCorner[1] < scaledY < button1.BottomRightCorner[1]) 
				if(button1.TopLeftCorner[0] < scaledX < button1.BottomRightCorner[0] )
						printf("OK\n");


	}
}


