/***************************************************************
 *  File Name:  WRITESVF.C
 *
 *  Purpose:  Writes data in Single Variable File (SVF) format.
 *	(This file can be read by arc/info).
 *
 *  Passed Parameters:
 * 	filename:  name of output data file
 *	imageptr:  pointer to image data to write
 *	
 *  Programmer:  Barbara Marks
 *
 *  Date:  10 June 1992
 *
 *  Modifications:
 *	07-23-96 BJM
 *	   The default C compiler on the IBM 615 machines (XL C
 *	   compiler version 1.3.0.24) defaults to unsigned char.
 * 	   Must explicitly specify signed when needed.
 *
 *  ID:  $Id: writesvf.c,v 2.3 1995/01/18 17:31:27 marks Exp marks $
 ****************************************************************/
#include <stdio.h>
#include <string.h>
#include "stats.h"
#include <R.h>


void write_svf (filename,imageptr)
char	*filename;
short	*imageptr;
{
	FILE 		*out;
	short		i,j,k;
	short		sum;
	short		value1,value2;
	short		*data;
	unsigned short  *value;
	unsigned short  numpairs;
	unsigned char   *repeat;
/*
 *  7-23-96 BJM  Changing for IBM compiler -- default is unsigned char
 */
	signed char    	flag;



	data = (short *) calloc ((unsigned)num_cols,sizeof(short));
	if (data == NULL) {
	   Rprintf ("\nERROR! write_svf: Can not allocate space for data");
	   return;
	}
	value = (unsigned short *) calloc ((unsigned)num_cols,sizeof(short));
	if (value == NULL) {
	   Rprintf ("\nERROR! write_svf: Can not allocate space for value");
	   return;
	}
	repeat = (unsigned char *) calloc ((unsigned)num_cols,sizeof(char));
	if (repeat == NULL) {
	   Rprintf ("\nERROR! write_svf: Can not allocate space for value");
	   return;
	}

/*
 *  Open output file
 */
	if ((out = fopen(filename,"wb")) == NULL) {
	   Rprintf ("\nERROR! Can not open file: %s\n",filename);
	   return;
	}

/*
 *  Write header consisting of number of rows and columns to
 *  output SVF file.
 */
	fwrite (&num_rows,sizeof(unsigned short),1,out);
	fwrite (&num_cols,sizeof(unsigned short),1,out);


/*
 *  Run-length encode the image data on a line by line basis.
 */	
	flag = -1;
	
	for (i=0; i < num_rows; i++) {
	
           for (j=0; j < num_cols; j++) 
		data[j] = getshort(imageptr,num_cols,i,j);

           value[0] = data[0];

	   fwrite (&flag,sizeof(char),1,out);
	   k = 0;
	   numpairs = 1;
	   for (j=0; j < num_cols; j++) repeat[j] = 1;
	
	   for (j=0; j < num_cols-1; j++) {
		value1 = data[j];
		value2 = data[j+1];
		if (value1 == value2) {
			repeat[k]++;
		        if (repeat[k] == 255) {
			   if (j < num_cols-2) {
			      k++;
			      numpairs++;
                              value[k] = value2;
			      j++;
			   }
	                }
		}
		else {
			k++;
			numpairs++;
			value[k] = value2;
		}
	   } 
	   if ((short)numpairs > num_cols) {
		Rprintf ("\nERROR! NUMPAIRS > NCOLS!");
		return;
	   }
	   fwrite (&numpairs,sizeof(unsigned short),1,out);
	   sum = 0;
	   for (j=0; j < (short)numpairs; j++) {
	  	fwrite (&repeat[j],sizeof(unsigned char),1,out);
		fwrite (&value[j],sizeof(unsigned short),1,out);
		sum += repeat[j];
	   }
	   if (sum != num_cols) { 
		Rprintf ("\nERROR!  More values than num_cols!");
		Rprintf ("\nsum,num_cols: %d,%d\n",sum,num_cols);
	        for (j=0; j < (short)numpairs; j++) 
	  	   Rprintf ("\n%d,%d",repeat[j],value[j]);
	        return;
	   }
	}
	
	fclose (out);
	free (data);
	free (value);
	free (repeat);
}

