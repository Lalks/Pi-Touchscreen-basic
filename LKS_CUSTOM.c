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

	
	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;


	if (openTouchScreen() == 1)
		perror("error opening touch screen");


	getTouchScreenDetails(&screenXmin,&screenXmax,&screenYmin,&screenYmax);

	framebufferInitialize(&xres,&yres);

	scaleXvalue = ((float)screenXmax-screenXmin) / xres;
	printf ("X Scale Factor = %f\n", scaleXvalue);
	scaleYvalue = ((float)screenYmax-screenYmin) / yres;
	printf ("Y Scale Factor = %f\n", scaleYvalue);



	// BEGGINING OF CUSTOMIZATION :::LALKS:::
	int color[3] = {40,40,40}; 
	int borderColor[3] = {0,187,255};
	//	      x                y         h    w        rgb    bs   brgb
	drawSquare(screenXmin+20, screenYmin+20, 50, xres-40, color,  60, borderColor);
	//put_string(screenXmin+20, screenYmin+20, "Isn't so cool ?", 25,  255,0,0);

}


