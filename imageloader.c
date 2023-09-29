/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *fp = fopen(filename, "r");
	// exit if file does not exist
	if (fp == NULL) {
		printf("Error opening file\n");
		exit(1);
	}
	Image *image = (Image *) malloc(sizeof(Image));
	memset(image, 0, sizeof(Image));
	// read in the first line: P3
	char _s[20];
	fscanf(fp, "%s", _s);
	// read in the second line: cols rows
	fscanf(fp, "%u %u", &image->cols, &image->rows);
	// read in the third line: 255
	int _i;
	fscanf(fp, "%d", &_i);
	// read in the rest of the file: pixels
	image->image = (Color **) malloc(sizeof(Color *) * image->rows);
	for (uint32_t i = 0; i < image->rows; i ++) {
		image->image[i] = (Color *) malloc(sizeof(Color) * image->cols);
		for (uint32_t j = 0; j < image->cols; j ++) {
			fscanf(fp, "%hhu %hhu %hhu", &(image->image[i][j].R), &(image->image[i][j].G), &(image->image[i][j].B));
		}
	}
	// close file
	fclose(fp);
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	// print format
	printf("P3\n");
	// print cols rows
	printf("%d %d\n", image->cols, image->rows);
	// print scale
	printf("255\n");
	// print pixels
	for (uint32_t i = 0; i < image->rows; i ++) {
		for (uint32_t j = 0; j < image->cols; j ++) {
			printf("%3u %3u %3u", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
			if (j != image->cols - 1) {
				printf("   ");
			}
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	for (uint32_t i = 0; i < image->rows; i ++) {
		free(image->image[i]);
	}

	free(image->image);

	free(image);
}