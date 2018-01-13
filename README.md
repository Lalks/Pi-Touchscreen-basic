#Note
Source codes made to use a TFT LCD screen on a raspberry pi.
This git is forked. Why have I forked it ? I did it to use its basic functions to draw pixels to the screen. My goal is to create a complete source to use such a screen with ANY (or almost) controller by using as few as possible libraries to make it portable.
For now the goal is far far away, but it's only a beginning, or at least a test to learn and study.

#Prerequisite
To be able to use the source inside this git you first need to have a working function called `put_pixel(int x, int y, color_t color)` which hence its name puts a pixel to the given (x,y) coordinates with the color you want to have it. The struct of `color_t` is available at the top of `framebuffer.c`.
In a near future, all the files will be cleaned up to make order and to simplify the use of the code.
