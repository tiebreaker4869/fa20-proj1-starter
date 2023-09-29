/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

static uint32_t dx[8] = {1, -1, 1, 1, 0, 0, -1, -1};

static uint32_t dy[8] = {0, 0, 1, -1, 1, -1, 1, -1};

static uint8_t evaluateChannelOfOneCell(Image* image, int row, int col, uint8_t channel) {
	uint8_t alive = 0;
	for (uint32_t i = 0; i < 8; i ++) {
		uint32_t x = row + dx[i], y = col + dy[i];
		x = (x + image->rows) % image->rows, y = (y + image->cols) % image->cols;
		Color c = image->image[x][y];
		uint8_t scale = *(((uint8_t*) (&c)) + channel);
		if (scale != 0) {
			alive ++;
		}
	}

	return alive;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	Color* c = (Color*) malloc(sizeof(Color));
	for (uint8_t channel = 0; channel < 3; channel ++) {
		uint8_t alive = evaluateChannelOfOneCell(image, row, col, channel);
		uint8_t* result_c = (uint8_t*) c;
		uint8_t* image_c = (uint8_t*) (&image->image[row][col]);
		uint8_t scale = *(image_c + channel);
		uint8_t result_scale = 0;
		if (scale == 0) {
			if ((rule >> alive) & 1) {
				result_scale = 255;
			}
		} else {
			if ((rule >> (alive + 9)) & 1) {
				result_scale = 255;
			}
		}
		*(result_c + channel) = result_scale;
	}

	return c;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	// initialize new image
	Image* newImage = (Image*) malloc(sizeof(Image));
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = (Color **) malloc(sizeof(Color*) * newImage->rows);
	// iterate over original image to generate new image content
	for (uint32_t i = 0; i < image->rows; i ++) {
		newImage->image[i] = (Color*) malloc(sizeof(Color) * newImage->cols);
		for (uint32_t j = 0; j < image->cols; j ++) {
			Color* newColor = evaluateOneCell(image, i, j, rule);
			newImage->image[i][j] = *newColor;
			free(newColor);
		}
	}

	return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
		printf("Usage: ./gameoflife <filename> <rule>\n");
		exit(-1);
	}

	Image* image = readData(argv[1]);
	uint32_t rule = strtol(argv[2], NULL, 0);
	Image* nextImage = life(image,  rule);
	writeData(nextImage);
	freeImage(image);
	freeImage(nextImage);

	return 0;
}
