/***************************************************************
 *  Filename:  READBIN.C
 *
 *  Purpose:  Read an input 8 or 16 bit binary image file into
 *	the array pointed to by "image_ptr".
 *
 *  Passed Parameters:
 * 	filename:  name of input image data file
 *	image_ptr: pointer to image to fill with this data
 *      option:    type of binary file:  
 *	 	   1 - 8 bit    2 - 16 bit
 *
 *  Returned Parameters:
 *	min:   minimum data value (>= 0) in image
 *	max:   maximum data value in image
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  11 December 1992
 *
 *  Modifications: 
 *	07-23-96 BJM
 *	   The compiler for the IBM 615 machines (XL C compiler
 *	   version 1.3.0.24) defaults to unsigned chars.  Must
 *	   explicitly specify "signed".
 *
 *  ID: $Id: readbin.c,v 2.4 1994/10/05 16:07:05 marks Exp marks $
 ***************************************************************/
#include <stdio.h>
#include "stats.h"
#include <R.h> 


void read_binary (filename, image_ptr, min, max)
char  *filename;
short *image_ptr;
short *min, *max;
{

	FILE	*fp;
	short	*ptr;
	int	i,j;
	int	count_bck,count_intbck;
/*
 *  7-23-96 BJM  For IBM compiler -- default is unsigned char
 */

	count_bck = count_intbck = 0;
	*min = 9999;
	*max = -9999;
	total_size = 0;
	land_border = FALSE;
	MAX_CLASSES = -9999;
	ptr = image_ptr;

/*
 *  total_size is defined in the include file stats.h.  total_size is
 *  the number of cells inside the landscape (non-background cells).
 */
	total_size = 0;

/*
 *  Open input image file
 */
	if ((fp = fopen(filename,"rb")) == NULL) {
		Rprintf ("\nERROR! Can not open file: %s\n",filename);
		return;
	}

/*  Read 16 bit binary stream file */
	fread (image_ptr,sizeof(short),num_rows*num_cols,fp);
	for (i=0; i < num_rows; i++) {
		for (j=0; j < num_cols; j++) {
			if (*ptr < 0 && *ptr != background && *ptr != -background)
				land_border = TRUE;
			if (*ptr != background && *ptr != -background) {
				if (*ptr < *min && *ptr >= 0) *min = *ptr;
				if (*ptr > *max) *max = *ptr;
				if (*ptr >= 0) total_size ++;
			
				if (*ptr >= 0 && *ptr > MAX_CLASSES)
					MAX_CLASSES = *ptr;
				if (*ptr < 0 && -(*ptr) > MAX_CLASSES)
					MAX_CLASSES = -(*ptr);
			}
			if (*ptr == -background) {
				count_bck ++;
				*ptr = -999;
			}
			if (*ptr == background) {
				count_intbck ++;
				*ptr = -990;
			}
			ptr++;
		}
	}


	fclose (fp);
	MAX_CLASSES ++;

	bcode = 0;
    Rprintf ("\n");
    if (count_bck > 0) {
		Rprintf ("\n... %d cells of background exterior to the landscape found", count_bck);
		bcode ++;
	}
    if (count_intbck > 0) {
		Rprintf ("\n... %d cells of background interior to the landscape found", count_intbck);
		bcode ++;
	}
	if (count_intbck == 0 && count_bck == 0)
	   Rprintf ("\n... landscape does not contain background");

}
 
