/***************************************************************
 *  File Name:  SETUP.C
 *
 *  Purpose:  Gets required inputs, allocates memory for arrays,
 *	reads input image(s), and opens output files.
 *
 *  Passed arguements:
 *	argc:  	number of command line arguements
 *	argv:   command line arguements
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  August 1993
 *
 *  ID: $Id: setup.c,v 2.2 1994/04/06 15:28:28 marks Exp marks $
 ***************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stats.h"
#include <R.h> 



void setup()
{
	FILE	*fp,*out;
	int	size;
	time_t	lt;
	struct  tm *time_ptr;

	do_patchrich = FALSE;
	contrast_indices = FALSE;
	if (contrast_indices) { 
		contrast_indices = TRUE;
		if ((fp=fopen(weight_file,"r")) == NULL) {
			Rprintf ("\nERROR! Can not open file: %s\n",weight_file);
			return;
		}
		fclose (fp);
    }
 	   
	id_image = 2;
	descriptors = FALSE;
	if (descriptors != 0) {
//		strcpy(desc_file, argv[12]);   /* class descriptors file */
        descriptors = TRUE;
		if ((fp=fopen(desc_file,"r")) == NULL) {
			Rprintf ("\nERROR! Can not open file: %s\n",desc_file);
			return;
		}
		fclose(fp);
	}
    
	bound_wght = 0;
	use_diags = TRUE;
	proximity = FALSE;
	do_nndist = TRUE;
	patch_stats = TRUE;
	class_stats = TRUE;
/*
*  If the user selected to calculate proximity indices and not 
*  nearest neighbor indices, issue a message that the two go
*  hand in hand -- you can't do one without the other.
*/
	if (proximity && !do_nndist) {
	   Rprintf ("\n\nERROR!! The proximity indices require all");
	   Rprintf ("\nthe nearest neighbor calculations.  You must"); 
	   Rprintf ("\nselect to calculate nearest neighbor indices if");
	   Rprintf ("\nyou want to calculate proximity indices!");
	   Rprintf ("\nExiting ....\n");
	}

/*
 *  Allocate space to hold the image file. 
 */
	size = num_rows * num_cols;
	image = (short *) calloc ((unsigned)size,sizeof(short));
	if (image == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for input");
	   Rprintf ("\n  image!");
	   return;
	}

/*
 *  Allocate space for a patch ID image, if requested.
 */
	if (id_image != 2) {
		id = (short *)calloc ((unsigned)size, sizeof(short));
		if (id == NULL) {
	      	Rprintf ("\nERROR! Can not allocate space - id map");
			return;
		}
	}

/*  If a patch ID file was input, read it in.  */
	if (id_image == 3) {
		read_binary (id_file, id, &min_class, &max_class);
	}
	
	Rprintf ("\nInterior Background Value: %d",background);
	Rprintf ("\nExterior Background Value: %d",-background);

/*  Read in image data */
	read_image (0, &bcode);

/* Read in weight file if one was specified */
	if (contrast_indices) read_weights(weight_file);

/*  Find the number of patch types, and the maximum patch size in 
 *  the image.  These are needed to dynamically allocate memory for 
 *  several arrays.  (This requires a pass through the image and
 *  time to find all patches!) */
 
	get_sizes();
	allocate_memory();
	
/* If a file containing character descriptors for each class type was
 * input, read it in. */
	if (descriptors) {
	     read_classnames(desc_file);
	}

/* 7-19-94 BJM  If only negative or only postive background patches were
 *  found in the image, verify that these patches were classified correctly.
 *  If the user did not follow the convention of classifying internal 
 *  background patches as postive and external patches as negative, this
 *  routine tries to bail them out.  If both positive and negative backgroud
 *  patches were found, the user probably knows what he's doing, so don't
 *  bother checking.  (bcode is returned from the read routines:  0 is no
 *  background, 1 = positive or negative found, 2 = positive and negative 
 *  found) */
	if (bcode == 1) {
	   read_image (0);
	   check_background (image);
	}


/*  Open output files.  Check to see if they exist first.  If they
 *  don't, then header information will be added to the files.  If
 *  they do, just append data records.  */
   	strcpy (file1,out_file);   /* patch stats in dump format */ 
   	strcat (file1,".patch");
    strcpy (file2,out_file);   /* class stats in dump format */
 	strcat (file2,".class");
   	strcpy (file3,out_file);   /* landscape stats in dump format*/
	strcpy (file4,out_file);   /* formatted file with all stats */
   	strcat (file4,".full");
 
	if (patch_stats) {
		if ((out = fopen(file1,"a")) == NULL) {
			Rprintf ("\nERROR opening file: %s\n",file1);
			return;
		}
		head_patch(out);
		fclose (out);
	}

	if (class_stats) {
		if ((out = fopen(file2,"a")) == NULL) {
			Rprintf ("\nERROR opening file: %s\n",file2);
			return;
		}
		head_class(out);
		fclose (out);
	}

	if ((out = fopen(file3,"a")) == NULL) {
		Rprintf ("\nERROR opening file: %s\n",file3);
		return;
	}
	head_land(out);
	fclose (out);

	if ((out = fopen(file4,"a")) == NULL) {
		Rprintf ("\nERROR opening file: %s\n",file4);
		return;
	}

/*
 *  Echo user inputs to output formatted (.full) file
 */
	lt = time(NULL);
	time_ptr = localtime(&lt);
	fprintf (out,"\n\nDate:  %s",asctime(time_ptr));
	fprintf (out,"Image Name: %s",imagename);
	fprintf (out,"\nBasename For Output Files: %s",out_file);
	fprintf (out,"\nRows: %d   Cols: %d   Cellsize: %.1f", num_rows,num_cols,cellsize);
	fprintf (out,"   Data Type: %d",data_type);
	fprintf (out,"\nEdge Dist: %.1f   ",edge_dist);
	if (do_patchrich)
	   fprintf (out,"Max Patch Types Possible: %d   ",max_num_classes);
	else
	   fprintf (out,"Max Patch Type Possible: NA   ");
	fprintf (out,"   Background: %d",background);
	if (contrast_indices) 
	   fprintf (out,"\nWeight File: %s",weight_file);
	if (id_image == 1)
	   fprintf (out,"\nID Image Will Be Created");
	if (id_image == 2)
	   fprintf (out,"\nNo ID Image Will Be Output");
	if (id_image == 3)
	   fprintf (out,"\nID Image: %s",id_file);
	if (descriptors)
	   fprintf (out,"\nDescriptor File: %s",desc_file);
	if (land_border) 
	   fprintf (out,"\nImage Includes a Landscape Border");
  	else
	   fprintf (out,"\nImage Does Not Include a Landscape Border");
	fprintf (out,"\nProportion of Boundary/Background to Count as Edge: %.2f",  bound_wght);
	if (use_diags)
	   fprintf (out,"\nDiagonals Used");
	else
	   fprintf (out,"\nDiagonals Not Used");
	if (proximity)
	   fprintf (out,";   Proximity Dist (m): %.1f",search_radius);
	else
	   fprintf (out,";   Proximity Indices Not Calculated");
	if (do_nndist)
	   fprintf (out,"\nNearest Neighbor Calcs");
	else
	   fprintf (out,"\nNo Nearest Neighbor Calcs");
	if (patch_stats)
	   fprintf (out,"\nWrite Patch Indices;   ");
	else
	   fprintf (out,"\nDo not Write Patch Indices;   ");
	if (class_stats)
	   fprintf (out,"Write Class Indices");
	else
	   fprintf (out,"Do Not Write Class Indices");

	fprintf (out,"\n\n");

	fclose (out);
}
