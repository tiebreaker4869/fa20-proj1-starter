/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

// build whtie/black color
static Color* buildBinaryColor(uint8_t binary) {
	uint8_t val = 255 * binary;
	Color* c = (Color *) malloc(sizeof(Color));
	c->R = val;
	c->G = val;
	c->B = val;
	return c;
}

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	Color* c = (Color *) malloc(sizeof(Color));
	*c = image->image[row][col];
	return c;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	Image* newImage = (Image *) malloc(sizeof(Image));
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = (Color **) malloc(sizeof(Color *) * newImage->rows);
	for (uint32_t i = 0; i < newImage->rows; i++) {
		newImage->image[i] = (Color *) malloc(sizeof(Color) * newImage->cols);
		for (uint32_t j = 0; j < newImage->cols; j++) {
			Color* c = evaluateOnePixel(image, i, j);
			Color* binaryPixel = buildBinaryColor(c->B & 1);
			newImage->image[i][j] = *binaryPixel;
			free(c);
			free(binaryPixel);
		}
	}

	return newImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2) {
		printf("Usage: ./steganography <filename>\n");
		exit(-1);
	}

	Image* image = readData(argv[1]);
	Image* newImage = steganography(image);
	writeData(newImage);
	freeImage(image);
	freeImage(newImage);

	return 0;
}
