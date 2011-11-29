/***************************************************************
 *  Filename:  WRITEBIN.C
 *
 *  Purpose:  Writes an 8 or 16 bit binary file depending on
 *	the "option" flag passed.
 *
 *  Passed Parameters:
 * 	filename:  name of output data file
 *	image_ptr: pointer to image to write
 *      option:    type of binary file:  
 *	 	   1 - 8 bit    2 - 16 bit
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  11 December 1992
 *
 *  ID:  $Id: writebin.c,v 2.1 1994/04/01 17:56:06 marks Exp $
 ***************************************************************/
#include <stdio.h>
#include "stats.h"
#include <R.h>


void write_binary(filename,image_ptr,option)
char  *filename;
short *image_ptr;
short option;
{
	FILE	*fp;

/*
 *  Open output image file
 */
	if ((fp = fopen(filename,"wb")) == NULL) {
		Rprintf ("\nERROR! Can not open file: %s\n",filename);
		return;
	}

/*
 *  Write 16 bit binary stream file
 */

   Rprintf ("\nWriting 16 bit binary file ... \n");
   fwrite (image_ptr,sizeof(short),num_rows*num_cols,fp);

	fclose (fp);
}
 
