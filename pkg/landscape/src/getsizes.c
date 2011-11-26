/***************************************************************
 *  File Name:  GETSIZES.C
 *
 *  Purpose:  Finds the largest patch in the input image, the
 *	number of classes in the image, and the maximum number 
 *	of patches per class.  These are needed so that memory
 *	can be dynamically allocated for the required arrays.
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  August 1993
 *
 *  ID:  $Id: getsizes.c,v 2.5 1994/10/05 21:19:45 marks Exp marks $
 ***************************************************************/
#include <string.h>
#include <stdlib.h>
#include "stats.h"
#include <R.h>
#include <Rinternals.h>
#include "Rdefines.h"


void get_sizes()
{

	short	i,j;
	short	value;
	short   min_landscape_class;
	short   max_landscape_class;
	char	*class_exists;
	char	*all_classes;
	int	*npts;
 	int	numpts;
	int	*npatches;
	int	MAX_BCKGRND_SIZE;


/*
 *  Get space for 3 arrays needed to determine NUM_CLASSES, the classes
 *  that exist, and the maximum number of patches per class.
 *  (MAX_CLASSES is set in the routines that read in the input image.)
 */

	class_exists = (char *) calloc ((unsigned)MAX_CLASSES,sizeof(char));
	if (class_exists == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for class_exists!");
	   return;
	}

	all_classes = (char *) calloc ((unsigned)MAX_CLASSES,sizeof(char));
	if (all_classes == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for all_classes");
	   return;
	}
	
	npatches = (int *) calloc ((unsigned)MAX_CLASSES,sizeof(int));
	if (npatches == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for npatches");
	   return;
	}
	
	npts = (int *) calloc ((unsigned)MAX_CLASSES,sizeof(int));
	if (npts == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for npts");
	   return;
	}


/*
 *  Initialize 
 */
	for (i=0; i < MAX_CLASSES; i++) {
	   class_exists[i] = FALSE;
	   all_classes[i] = FALSE;
	   npatches[i] = 0;
	}

	MAX_PATCH_SIZE = 0;
	MAX_BCKGRND_SIZE = 0;

/*
 *  Step through all cells in the image.
 */
	
        for (i=0; i < num_rows; i++) {
           for (j=0; j < num_cols; j++) {
	      value = getshort(image,num_cols,i,j);

/*
 *  Flag all classes that exist (must have values >= 0).
 */
	      if (value >= 0) {
	         class_exists[value] = TRUE;
		 all_classes[value] = TRUE;
	      }
/*
 *  Flag any classes in the landscape border that exist (values < 0
 *  but not background).  This is needed to compute total_num_classes
 *  used in contagion.
 */
	      if (value < 0 && value > -990 && value != -777)
		 all_classes[-value] = TRUE;

/*
 *  Find every patch in the landscape.  Keep track of the largest
 *  patch (# cells) and the number of patches per class.
 */
	      if (value >= 0) {
	         numpts = patch_size(j,i,value);
		 npts[value] += numpts;
	         if (numpts > MAX_PATCH_SIZE) MAX_PATCH_SIZE = numpts;
	         npatches[value] ++;
	      }

/*
 *  9-15-94 BJM  Changing so that it checks patch size for patches
 *  in the landscape border as well as background patches.
 */
	      else if (value < 0 && value != -777 ) {
		 numpts = patch_size (j,i,value);
		 if (numpts > MAX_BCKGRND_SIZE) MAX_BCKGRND_SIZE = numpts;
	      }

   	   }
	}


/*
 *  Now find the class that has the greatest number of patches and 
 *  save the number of patches.  This is needed to dimension arrays
 *  that hold patch level indices.  Also find the total number of 
 *  patches in the landscape, the number of classes that exist, and
 *  the total number of classes that exist (including any classes
 *  in the landscape border).
 */
	Rprintf ("\n");
	for(i=0; i < MAX_CLASSES; i++) 
	   if (class_exists[i]) Rprintf ("\nClass %5d: %9d cells, %8d patches",
		i,npts[i],npatches[i]);
	Rprintf ("\n");


	MAX_PATCHES = 0;
	NUM_CLASSES = total_num_classes = total_num_patches = 0;	
	min_landscape_class = 999;
	max_landscape_class = -999;

	for (i=0; i < MAX_CLASSES; i++) {
	   if (npatches[i] > MAX_PATCHES) MAX_PATCHES = npatches[i];
	   total_num_patches += npatches[i];
	   if (class_exists[i]) NUM_CLASSES ++;
	   if (all_classes[i]) {
	      total_num_classes ++;
	      if (i < min_landscape_class) min_landscape_class = i;
	      if (i > max_landscape_class) max_landscape_class = i;
	   }
	}

/*
 *  Allocate space for an array that will hold the values of the
 *  patch types (classes) that exist in the landscape. 
 */
	patchtype = (short *) calloc ((unsigned)NUM_CLASSES,sizeof(short));
	if (patchtype == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for patchtype!");
	   return;
	}

	j = 0;
	for (i=0; i < MAX_CLASSES; i++) {
	   if (class_exists[i]) {
		patchtype[j] = i;
		j++;
	   }
	}


	Rprintf ("\nnumber of classes: %d",NUM_CLASSES);
	Rprintf ("\nmax patches/class: %d",MAX_PATCHES);

	if (MAX_BCKGRND_SIZE > MAX_PATCH_SIZE) {
	   Rprintf ("\nmax_patch_size: %d (background/border patch)",MAX_BCKGRND_SIZE);
	   MAX_PATCH_SIZE = MAX_BCKGRND_SIZE;
	}
	else
	   Rprintf ("\nmax patch size: %d\n",MAX_PATCH_SIZE);
	

/*
 *  If a user did not specify a weight file for contrast indices,
 *  set the min and max weight value to the min and max class 
 *  value in the landscape (includes patchtypes in the landscape border).
 *  These are needed to index the "edge" array.
 */
	if (!contrast_indices) {
	   min_wt = min_landscape_class;
	   max_wt = max_landscape_class;
	   num_wt = max_landscape_class - min_landscape_class + 1;
	}


	free (npatches);
	free (all_classes);
	free (class_exists);
	free (npts);

	return;
}
