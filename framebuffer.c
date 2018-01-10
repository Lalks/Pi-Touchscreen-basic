/*
        A simple program that demonstrates how to program for a
        touch screen. Specifically on the Raspberry Pi.
        This prgram can be used for most Linux based systems.
        For more details: www.marks-space.com

    Copyright (C) 2013  Mark Williams
    MODIFIED BY LALKS

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA
*/
#include <stdlib.h>
#include <stdint.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include "font_8x8.c"


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

	static struct fb_fix_screeninfo fix;
	static struct fb_var_screeninfo orig_var;
	static struct fb_var_screeninfo var;
	char *fbp = 0;
	int fb=0;
	long int screensize = 0;



struct Button
{
	// Considering the button rectangle !
	int width; // = BottomRightCorner[0] - TopLeftCorner[0]
	int height; // = BottomRightCorner[1] - TopLeftCorner[1]
	int TopLeftCorner[2]; // {x, y}
	int BottomRightCorner[2]; // {x, y}
	char Text[20]; // The string inside the button
};


void put_pixel_16bpp(int x, int y, uint8_t color[3])
{
        unsigned int pix_offset;
        unsigned short c;

        // calculate the pixel's byte offset inside the buffer
        pix_offset = x*2 + y * fix.line_length;

        //some magic to work out the color
        c = ((color[0] / 8) << 11) + ((color[1] / 4) << 5) + (color[2] / 8);

        // write 'two bytes at once'
        *((unsigned short*)(fbp + pix_offset)) = c;
}

void drawSquare(int x, int y,int height, int width,  uint8_t color[3],  int borderSize, uint8_t borderColor[3])
//void drawSquare(int x, int y)
{
//	int height = 20;
//	int width = 20;
	int h, w, a, b = 0;
	
	// Top border
	for (a=0; a<borderSize; a++) // for lines
		for (b = 0; b<width; b++) // for pixels in the current line
			put_pixel_16bpp(b+(x-2), a+(y-2), borderColor);
	// Left border, rectangle, right border
	for ( h=borderSize; h<height-borderSize;h++)
	{
		// Left border
		for ( a=0; a<borderSize; a++)
			put_pixel_16bpp( a+(x-2), h+(y-2), borderColor );
		// Rectangle
		for ( w=a; w<width-a;w++)
			put_pixel_16bpp( w+(x-2), h+(y-2) , color);
		// Right border
		for ( b=w; b<w+borderSize; b++ )
			put_pixel_16bpp( b+(x-2), h+(y-2), borderColor );
	}
	// Bottom border
	for (a=height-borderSize; a<height; a++) // for lines
		for (b=0; b<width; b++) // for pixels in the current line
			put_pixel_16bpp(b+(x-2), a+(y-2), borderColor);
}

struct Button drawButton(int x, int y,int height, int width,  uint8_t color[3], const char *String,  int borderSize, uint8_t borderColor[3])
{
	int h, w, a, b = 0;

	struct Button button1;

	// Preparation
	// I want a text string in the center of the button
	// reminder : one character is 8 pixels width, and 8 pixels height
	int StringLength = strlen(String);

	int StringPixelWidth = StringLength*8;
	int StringMarginLeft = 0;

	StringMarginLeft = (width-StringPixelWidth)/2;
	printf("Printed from framebuffer.c ----\nText = %s\n", String, StringLength, width, StringPixelWidth, StringMarginLeft);


	button1.width = width;
	button1.height = height;
	button1.TopLeftCorner[0] = x;
	button1.TopLeftCorner[1] = y;
	button1.BottomRightCorner[0] = x+width;
	button1.BottomRightCorner[1] = y+height;
	strcpy(button1.Text, String);
	


	// Top border
	for (a=0; a<borderSize; a++) // for lines
		for (b = 0; b<width; b++) // for pixels in the current line
			put_pixel_16bpp(b+(x-2), a+(y-2), borderColor);
	// Left border, rectangle, right border
	for ( h=borderSize; h<height-borderSize;h++)
	{
		// Left border
		for ( a=0; a<borderSize; a++)
			put_pixel_16bpp( a+(x-2), h+(y-2), borderColor );
		// Rectangle
		for ( w=a; w<width-a;w++)
			put_pixel_16bpp( w+(x-2), h+(y-2) , color);
		// Right border
		for ( b=w; b<w+borderSize; b++ )
			put_pixel_16bpp( b+(x-2), h+(y-2), borderColor );
	}
	// Bottom border
	for (a=height-borderSize; a<height; a++) // for lines
		for (b=0; b<width; b++) // for pixels in the current line
			put_pixel_16bpp(b+(x-2), a+(y-2), borderColor);
	
	int rrr = x+StringMarginLeft;
	//put_string(screenXmin+x+StringMarginLeftForCenter, screenYmin+y+height/2-6, String, 25, borderColor);
	put_string(rrr, y+height/2-6, String, 2, borderColor);

	return(button1);
}

int framebufferInitialize(int *xres, int *yres)
{
	char *fbdevice = "/dev/fb1" ;

	fb = open(fbdevice, O_RDWR);
	if (fb == -1) {
		perror("open fbdevice");
	return -1;
	}

	if (ioctl(fb, FBIOGET_FSCREENINFO, &fix) < 0) {
		perror("ioctl FBIOGET_FSCREENINFO");
		close(fb);
	return -1;
	}

	if (ioctl(fb, FBIOGET_VSCREENINFO, &var) < 0) {
		perror("ioctl FBIOGET_VSCREENINFO");
	close(fb);
	return -1;
	}

	printf("Original %dx%d, %dbpp\n", var.xres, var.yres, 
         var.bits_per_pixel );

	memcpy(&orig_var, &var, sizeof(struct fb_var_screeninfo));



	printf("Framebuffer %s%s%s resolution;\n",KYEL, fbdevice, KWHT);

  	printf("%dx%d, %d bpp\n\n\n", var.xres, var.yres, var.bits_per_pixel );

	// map framebuffer to user memory 
	screensize = fix.smem_len;
	fbp = (char*)mmap(0, 
                    screensize, 
                    PROT_READ | PROT_WRITE, 
                    MAP_SHARED, 
                    fb, 0);
	if ((int)fbp == -1)
		printf("Failed to mmap.\n");

	*xres = var.xres;
	*yres = var.yres;

	//clear framebuffer
	int x, y;
	uint8_t color[3] = {0,0,0};
	for (x = 0; x<var.xres;x++)
		for (y = 0; y < var.yres;y++)
			put_pixel_16bpp(x,y, color);
}

void closeFramebuffer()
{

	int x, y;
	uint8_t color[3] = {20,140,20};
	for (x = 0; x < var.xres; x++)
		for (y = 0; y < var.yres;y++)
			put_pixel_16bpp(x,y, color);

	munmap(fbp, screensize);
	if (ioctl(fb, FBIOPUT_VSCREENINFO, &orig_var)) {
		printf("Error re-setting variable information.\n");
	}
	close(fb);

}

void put_char(int x, int y, int c, int colidx, uint8_t color[3])
{
	int i,j,bits;
	for (i = 0; i < font_vga_8x8.height; i++) {
		bits = font_vga_8x8.data [font_vga_8x8.height * c + i];
		for (j = 0; j < font_vga_8x8.width; j++, bits <<= 1)
			if (bits & 0x80)
				put_pixel_16bpp(x+j,  y+i, color);
	}
}

void put_string(int x, int y, char *s, unsigned colidx, uint8_t color[3])
{
	int i;
	for (i = 0; *s; i++, x += font_vga_8x8.width, s++)
		put_char (x, y, *s, colidx, color);
}
