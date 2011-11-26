/********************************************************************
 *  File Name:  PROMPT.C
 *
 *  Purpose:  Prompt the user for all program inputs.
 *
 *  Programmer:  Barbara Marks
 * 
 *  Date:  February 1993
 *
 *  ID:  $Id: prompt.c,v 2.2 1994/04/06 15:27:28 marks Exp marks $
 *******************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"
#include <R.h> 

void prompt()
{
	char	ans[15];
	char	valid_choice;
	FILE	*fp;


/*
 *  Landscape image name
 */
	Rprintf ("\n\nEnter name of landscape image: ");
	gets (imagename);


/*
 *  Basename for output files
 */
	Rprintf ("\n\nEnter base name for output files.  The extensions");
	Rprintf ("\n   .patch, .class, .land, .full will be added to the");
	Rprintf ("\n   basename to create the output files.");
	Rprintf ("\nEnter base name: ");
	gets (out_file);


/*
 *  Cell size
 */
	Rprintf ("\n\nEnter size of grid cells in meters {must be square}: ");
	cellsize = atof(gets(ans));


/*
 *  Distance from edge to use in core area determination
 */
	Rprintf ("\n\nEnter distance from edge in meters to use in core");
	Rprintf ("\n   area determination: ");
	edge_dist = atof(gets(ans));


/*
 *  Type of input data file
 */
	data_type = -1;
	while (data_type < 0 || data_type > 6) {
	   Rprintf ("\n\nEnter type of input image file:");
	   Rprintf ("\n   1 - SVF file            2 - ascii file");
	   Rprintf ("\n   3 - 8 bit binary file   4 - 16 bit binary file");
           Rprintf ("\n   5 - ERDAS file          6 - IDRISI image");
	   Rprintf ("\n\nEnter choice {1-6}: ");
	   data_type = atoi(gets(ans));

	   if (data_type < 0 || data_type > 6) 
	      Rprintf ("\nInvalid file type: %d!",data_type);
	}


/*
 *  If data_type is 2,3,4, get the number of rows and columns in image
 */
	if (data_type >= 2 && data_type <= 4) {
	   Rprintf ("\n\nEnter number of rows, columns in image: ");
	   gets(ans);
	   num_rows = atoi(strtok(ans," ,"));
	   num_cols = atoi(strtok('\0'," ,"));
	}


/*
 *  Background (not required)
 */
	Rprintf ("\n\n   Fragstats expects a positive value for background cells");
	Rprintf ("\n   inside the landscape, and the negative of this value for");
	Rprintf ("\n   cells outside the landscape and in the landscape border.");
	Rprintf ("\n   If your landscape does not follow this convention, fragstats");
	Rprintf ("\n   will attempt to correct it for you.");
	Rprintf ("\n\nEnter the value for interior (positive) background cells (even");
	Rprintf ("\nif you only have exterior background) {NONE}: ");
        gets (ans);

  	if (ans[0] == 0 || ans[0] == 'N' || ans[0] == 'n')
      	   background = 999;
        else 
	   background = atoi(ans);


/*
 *  Maximum possible class types for relative patch richness (not required)
 */
	Rprintf ("\n\nEnter the maximum possible number of patch types in ");
	Rprintf ("the\n   landscape {for relative patch richness} {NONE}: ");
	gets (ans);
	if (ans[0] == 0 || strcmp(ans,"NONE") == 0 || 
           strcmp(ans,"none") == 0) 
	   do_patchrich = FALSE;
	else {
	   do_patchrich = TRUE;
	   max_num_classes = atoi(ans);
	}


/*
 *  Name of file containing weights for each combination of patch
 *  type (not required).
 */
	valid_choice = FALSE;
	while (!valid_choice) {
	   Rprintf ("\n\nEnter name of file containing weights for each");
	   Rprintf ("\n   combination of patch type {NONE}: ");
	   gets (weight_file);
	   valid_choice = TRUE;

	   if (weight_file[0] == 0 || (strcmp(weight_file,"NONE") == 0) || 
              (strcmp(weight_file,"none") == 0))
	      contrast_indices = FALSE;
	   else {
	      contrast_indices = TRUE;
	      if ((fp=fopen(weight_file,"r")) == NULL) {
	         Rprintf ("\nERROR! Can not open file: %s!\n",weight_file);
	         valid_choice = FALSE;
	      }
	      else
	 	 fclose (fp);
	   }
	}
	   

/*
 *  Name of image containing unique patch ID's for every patch
 *  (not required)
 */
	valid_choice = FALSE;
	while (!valid_choice) {
	   Rprintf ("\n\nEnter option for patch ID image: ");
	   Rprintf ("\n   1.  create and output ID image [output file");
	   Rprintf (" is in_image.ID]");
	   Rprintf ("\n   2.  do not output ID image");
	   Rprintf ("\n   3.  input ID image");
	   Rprintf ("\nEnter option [1,2,3] {default is 2}: ");
	   gets(ans);
	   valid_choice = TRUE;

	   if (ans[0] == 0)
	     id_image = 2;
	   else
	     id_image = atoi(ans);
	
	   if (id_image < 1 || id_image > 3) {
	      Rprintf ("\nInvalid choice!!\n");
	      valid_choice = FALSE;
	      continue;
	   }

	   if (id_image == 3) {
	      Rprintf ("\n\nEnter name of image containing unique ID's for");
	      Rprintf ("\n   each patch: ");
	      gets (id_file);

	      if ((fp = fopen(id_file,"r")) == NULL) {
		 Rprintf ("\nERROR! Can not open file: %s\n",id_file);
		 valid_choice = FALSE;
	      }
	      else 
		fclose (fp);
	   }
	}


/*
 *  Name of ascii file containing character descriptors (10 chars or 
 *  less) for each patch type (not required)
 */
	valid_choice = FALSE;
	while (!valid_choice) {
	   Rprintf ("\n\nEnter name of ascii file containing character");
	   Rprintf ("\n   descriptors for each patch type {NONE}: ");
	   gets (desc_file);
	   valid_choice = TRUE;

	   if (desc_file[0] == 0 || (strcmp(desc_file,"NONE") == 0) || 
              (strcmp(desc_file,"none") == 0))
	      descriptors = FALSE;
	   else {
	      descriptors = TRUE;
	      if ((fp = fopen(desc_file,"r")) == NULL) {
	         Rprintf ("\nERROR! Can not open file: %s\n",desc_file);
		 valid_choice = FALSE;
	      }
	      else
		 fclose(fp);
	   }
	}


/*
 *  Ask how the landscape boundary and any background class edges
 *  (if any) should be treated.
 */
	bound_wght = -1.0;
	while (bound_wght < 0.0 || bound_wght > 1.0) {
	   Rprintf ("\n\nWhat proportion (equivalent to contrast weight) of ");
	   Rprintf ("the\n   landscape boundary and background class edges ");
	   Rprintf ("should be\n   considered edge? This affects all edge indices.");
	   Rprintf ("\n   0.  None; do not count any boundary/background as edge");
	   Rprintf ("\n       (weight = 0.0)");
	   Rprintf ("\n   1.  All; count all boundary/background as maximum edge ");
	   Rprintf ("\n       contrast (weight = 1.0)");
	   Rprintf ("\n   2.  Other; specify a fraction between 0 and 1");
	   Rprintf ("\n\nEnter choice (0,1,or other) [default is 0]: ");
	   gets (ans); 
	   if (ans[0] == 0)
	      bound_wght = 0.0;
	   else
	      bound_wght = atof(ans);

	   if (bound_wght < 0.0 || bound_wght > 1.0) 
	     Rprintf ("\nERROR!  Weight must a value between 0 and 1!");
	}


/*
 *  Ask if diagonals should be considered when searching for cells
 *  making up a patch.
 */
	Rprintf ("\nUse diagonals in patch finding [y/n] {YES}: ");
	gets(ans);
	if (ans[0] == 'N' || ans[0] == 'n')
	   use_diags = FALSE;
	else
	   use_diags = TRUE;


/*
 *  Proximity index search radius in meters (not required)
 */
	Rprintf ("\n\nEnter search radius for proximity index (meters) {NONE}: ");
	gets (ans);
	if (ans[0] == 0 || strcmp(ans,"NONE") == 0)
	   proximity = FALSE;
	else {
	   proximity = TRUE;
	   search_radius = atof(ans);
	}

	   
/*
 *  Ask if nearest neighbor distances should be calculated -- can take
 *  a long time, depending on the input landscape.
 */
	Rprintf ("\nCalculate nearest neighbor distance [y/n] {YES}: ");
	gets (ans);
	if (ans[0] == 'N' || ans[0] == 'n')
	   do_nndist = FALSE;
	else
	   do_nndist = TRUE;


/*
 *  Ask if patch level statistics should be output
 */
	Rprintf ("\nOutput patch level statistics [y/n] {YES}: ");
	gets (ans);
	if (ans[0] == 'N' || ans[0] == 'n')
	   patch_stats = FALSE;
	else
	   patch_stats = TRUE;


/*
 *  Ask if class level statistics should be output
 */
	Rprintf ("\nOutput class level statistics [y/n] {YES}: ");
	gets (ans);
	if (ans[0] == 'N' || ans[0] == 'n')
	   class_stats = FALSE;
	else
	   class_stats = TRUE;

}
