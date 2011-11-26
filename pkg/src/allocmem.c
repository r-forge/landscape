/*****************************************************************
 *  File Name:  ALLOCMEM.C
 *
 *  Purpose:  Allocate memory for arrays.
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  August 1993
 *
 *  ID:  $Id: allocmem.c,v 2.1 1994/04/01 17:43:30 marks Exp $
 *****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "stats.h"
#include <R.h> 


void allocate_memory()
{
	int	size;

	if (descriptors) {
	   name = (char **) calloc ((unsigned)NUM_CLASSES,sizeof(char *));
	   if (name == NULL) {
	     Rprintf ("\nERROR! Can not allocate space for name!");
	     return;
	   }
	}

	stackx = (short *) calloc ((unsigned)MAX_PATCH_SIZE,sizeof(short));
	if (stackx == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for stackx!");
	   return;
	}

	stacky = (short *) calloc ((unsigned)MAX_PATCH_SIZE,sizeof(short));
	if (stacky == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for stacky!");
	   return;
	}

	check = (char *) calloc ((unsigned)MAX_PATCH_SIZE,sizeof(char));
	if (check == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for check!");
	   return;
	}

	ext_npts = (int *) calloc ((unsigned)MAX_PATCHES,sizeof(int));
	if (ext_npts == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for ext_npts!");
	   return;
	}

	core_area_patches = (int *) calloc ((unsigned)MAX_PATCHES,sizeof(int));
	if (core_area_patches == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for core_area_patches!");
	   return;
	}

	patchid = (short *) calloc ((unsigned)MAX_PATCHES,sizeof(short));
	if (patchid == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for patchid!");
	   return;
	}

	area = (double *) calloc ((unsigned)MAX_PATCHES,sizeof(double));
	if (area == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for area!");
	   return;
	}

	perimeter = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	if (perimeter == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for perimeter!");
	   return;
	}

	core_area = (double *) calloc ((unsigned)MAX_PATCHES,sizeof(double));
	if (core_area == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for core_area!");
	   return;
	}

	core_area_index = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	if (core_area_index == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for core_area_index!");
	   return;
	}

	shape = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	if (shape == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for shape!");
	   return;
	}

	fractal = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	if (fractal == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for fractal!");
	   return;
	}

	class_area = (float *) calloc ((unsigned)NUM_CLASSES,sizeof(float));
	if (class_area == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for class_area!");
	   return;
	}

	xx = (double *) calloc ((unsigned)total_num_patches,sizeof(double));
	if (xx == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for xx!");
	   return;
	}

	yy = (double *) calloc ((unsigned)total_num_patches,sizeof(double));
	if (yy == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for yy!");
	   return;
	}
	
	size = num_wt * num_wt;
	edge = (int *) calloc ((unsigned)size,sizeof(int));
	if (edge == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for edge!");
	   return;
	}

	edgetype = (int *) calloc ((unsigned)num_wt,sizeof(int));
	if (edgetype == NULL) {
	   Rprintf ("\nERROR! Can not allocate space for edgetype!");
	   return;
	}

	if (contrast_indices) {
	   edge_contrast = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	   if (edge_contrast == NULL) {
	      Rprintf ("\nERROR! Can not allocate space for edge_contrast!");
	      return;
	   }
	}

	if (do_nndist) {
	   near_neigh_dist = (double *)calloc ((unsigned)MAX_PATCHES,sizeof(double));
	   if (near_neigh_dist == NULL) {
	      Rprintf ("\nERROR! Can not allocate space for near_neigh_dist!");
	      return;
	   }
	}

	if (proximity) {
	   prox = (float *) calloc ((unsigned)MAX_PATCHES,sizeof(float));
	   if (prox == NULL) {
	      Rprintf ("\nERROR! Can not allocate space for prox!");
	      return;
	   }
	}
}
