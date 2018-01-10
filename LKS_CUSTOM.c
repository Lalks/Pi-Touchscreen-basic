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
	printf ("Y Scale Factor = %f\n\n\n", scaleYvalue);


	// BEGGINING OF CUSTOMIZATION :::LALKS:::
	
	// Clear the display
	uint8_t ClearColor[3] = {0,0,0};
	for(int Y=screenYmin; Y<screenYmin+yres; Y++)
		for(int X=screenXmin; X<screenXmin+xres;X++)
			put_pixel_16bpp(X,Y, ClearColor);


	uint8_t color[3] = {40,40,40};
	uint8_t color2[3] = {140,140,140};
	uint8_t borderColor[3] = {0,187,255};
	uint8_t borderColor2[3] = {255,255,255};
	short margin = 180;
	unsigned short buttonheight = 30;
	struct Button button1, button2;

	RESET:
	button1 = drawButton(screenXmin+margin, screenYmin+100, buttonheight, xres-margin*2, color, "abcdefghi", 3, borderColor);

	button2 = drawButton(screenXmin+margin, screenYmin+200, buttonheight, xres-margin*2, color, "01234", 3, borderColor);

	while(1)
	{
		getTouchSample(&rawX, &rawY, &rawPressure);

		scaledY = rawX/scaleYvalue;
		scaledX = xres-(rawY/scaleXvalue);

		printf("scaledX = %d\nscaledY = %d\n", scaledX, scaledY);
		printf("button1.TopLeftCorner[0] = %d\nbutton1.BottomRightCorner[0] = %d\n", button1.TopLeftCorner[0], button1.BottomRightCorner[0]);
		printf("button1.TopLeftCorner[1] = %d\nbutton1.BottomRightCorner[1] = %d", button1.TopLeftCorner[1], button1.BottomRightCorner[1]);

		printf("\n----------------------------------\n");

		if( button1.TopLeftCorner[1] < scaledY)
			if(scaledY < button1.BottomRightCorner[1]) 
				if(button1.TopLeftCorner[0] < scaledX)
					if(scaledX < button1.BottomRightCorner[0] )
						button1 = drawButton(screenXmin+margin, screenYmin+100, buttonheight, xres-margin*2, color2, "TOUCHED", 3, borderColor2);

		if( button2.TopLeftCorner[1] < scaledY)
			if(scaledY < button2.BottomRightCorner[1])
				if(button2.TopLeftCorner[0] < scaledX)
					if(scaledX < button2.BottomRightCorner[0] )
						goto RESET;
	} // End of while(1)
} // End of main()


