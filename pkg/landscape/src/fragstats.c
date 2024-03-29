/****************************************************************
 *  File Name:  FRAGSTATS.C 
 *
 *  Purpose:  Main program.  Calculates various landscape
 *	fragmentation metrics requested by Kevin McGarigal.
 *
 *  Command Line Arguments:
 *	image_name:   name of input image file
 *  base_name:    basename for output files;  the extensions .patch, .class, .land, .full will be added to the basename for output files.
 *	cellsize:     size of cells in meters (cells must be square)
 *	edge_dist:    the distance from patch edges to use in determing core area
 *  rows:     the number of rows in the image
 *	cols:     the number of columns in the image
 *  ext_bckgrnd:  optional; value of background cells outside the landscape of interest
 *  int_bckgrnd:  optional; value of background cells interior to the landscape
 *	max_classes:  the maximum number of classes possible (for patch richness determination)
 *  weight_file:  optional; the name of a file containing a weight for every combination of patch type (class).  The
 *		      patch type (class) must be a numeric.  This file must be ascii, with one record for each combination.
 *		      The field delimiter can be a comma or spaces.  For example:
 *			    1, 2, .377
 *			    1, 3, .122
 &			    1, 4, .500
 *			    etc.
 *      id_image      optional; type of ID image:
 *		      1 - create and output ID image
 *		      2 - don't output an ID image
 *                      - the name of an image file containing IDs; the 
 *		          data_type must be the same as in_image.
 *      desc_file:    optional; the name of a file containing character
 *		      descriptors for each patch type.  Each record should
 *		      contain the numeric patch type (class) and the
 *		      character descriptor assigned to it.  This file must
 *	 	      be ascii, with one record for each class type.  The
 *		      character name must be 10 characters or less.  The 
 *		      field delimiter can be a comma or spaces.  For 
 *		      example:
 *			    1, shrubs
 *			    2, conifers
 *			    etc.
 *      bound_wght:   the proportion of the landscape boundary and background class edges to count as edge.
 *      use_diag:     optional; use diagonal in patch finding
 *      prox_dist:    optional; the search radius in meters for calculating the proximity index.
 *      nndist:       optional; calculate nearest neighbor
 *      patch_stats:  optional: print patch level stats
 *      class_stats:  optional; print class level stats
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  September 1993
 *
 *  ID:  $Id: fragstats.c,v 1.6 1994/04/05 17:11:30 marks Exp marks $
 *
 *  Modifications:
 *	10-93 Tom Moore:
 *	 (2)  Greatly speeded up the nearest neighbor calculations
 *	 (3)  Changed the way class descriptors are stored
 *   (4)  Ported to DOS
 *
 *   03-94 Barbara Marks   Major Changes for version 2.0
 *	 (1)  Nearest neighbor distance changed.  Before reported the distance from the center of one edge cell to the
 *	      center of its nearest neighbor edge cell.  Now report the edge to edge (not center to center) distance.
 *	 (2)  The weight for the landscape boundary and any background class edges can now be any proportion between
 *	      0 and 1.  Before, limited to 0 (no contrast) or 1 (maximum contrast).
 *   (3)  The name for the output .land file has been changed to .lnd for the PC version ONLY!  Since DOS shortens
 *	      file extensions to 3 characters, the .lan extension conflicted with ERDAS file naming conventions.
 *   (4)  The name for the output ID image has been changed. Now the output name will be the input image name, up
 *	      to the first dot (".") encountered, with the extension .ID added.  Before it was just imagename + .ID.  Since
 *	      DOS can't handle names like test.dat.ID, this corrects the problem.
 *   (5)  Miscellaneous little bugs fixed and improvements made.
 *	
 *	2011 RJ Hijmans
 *		Major changes for use with R
 *
 *
 ****************************************************************/
#include <string.h>
#include <stdlib.h>
#include "stats.h"
#include <R.h> 

#include <Rinternals.h>
#include "Rdefines.h"
#include "R_ext/Rdynload.h"
#include <Rmath.h>

SEXP fragstats(SEXP in, SEXP out, SEXP dim) {
	short	i,j,k;
	short	patch_ID;
	short	max_patch_id;
	short	class;
 	int	numpts;

	Rprintf ("\n\n\t\tFRAGSTATS 2.0");
	Rprintf (" INDICES \n\n");
		
	strcpy(imagename, CHAR(STRING_ELT(in, 0)));
	strcpy(out_file, CHAR(STRING_ELT(out, 0)));

	num_rows = REAL(dim)[0];
	num_cols = REAL(dim)[1];
	cellsize = REAL(dim)[2];  /* size of cell in meters    */
	
	edge_dist = 100;       /* dist from edge - core area*/
	data_type = 4;       /* type of input image file:  */
	
	id_image = 2;

	background = 9999;
	
/*
 *  Convert the core area edge distance from meters to cells.
 *  "min_dist" is defined in stats.h.
 */
	min_dist = (int) (edge_dist / cellsize + .5);
	
	
/*  ext_bckgrnd:  optional; value of background cells outside the landscape of interest
 *  int_bckgrnd:  optional; value of background cells interior to the  landscape
 *	max_classes:  the maximum number of classes possible (for  patch richness determination)
 *  weight_file:  optional; the name of a file containing a weight
 *  id_image      optional; type of ID image:
 *		      1 - create and output ID image
 *		      2 - don't output an ID image
 *                      - the name of an image file containing IDs; the 
 *		          data_type must be the same as in_image.
 *   desc_file:    optional; the name of a file containing character
 *		      descriptors for each patch type.  Each record should
 *		      contain the numeric patch type (class) and the
 *		      character descriptor assigned to it.  This file must
 *	 	      be ascii, with one record for each class type.  The
 *		      character name must be 10 characters or less.  The 
 *		      field delimiter can be a comma or spaces.  For 
 *		      example: 
 *			    1, shrubs
 *			    2, conifers
 *			    etc.
 *  bound_wght:   the proportion of the landscape boundary and background  class edges to count as edge.
 * use_diag:     optional; use diagonal in patch finding
 * prox_dist:    optional; the search radius in meters for calculating the proximity index.
 * nndist:       optional; calculate nearest neighbor
 * patch_stats:  optional: print patch level stats
 *  class_stats:  optional; print class level stats  */
 	
/*
 *  Call a routine that does nothing but declare space for all global
 *  variables.
 */
	space();

/*
 *  Get arguments and read in files, etc.
 */

    setup();

/*
 *  Initialize ....
 */
	for (i=0; i < num_wt; i++) {
	   for (j=0; j < num_wt; j++) 
	      setint(edge,num_wt,i,j,0);
	}

	total_patches = 0;
	patch_ID = 0;
    total_area = 0.0;
    total_area_sq = 0.0;
    total_maxarea = -9999.;
    total_shape = 0.0;
    total_aw_shape = 0.0;
    total_fract = 0.0;
	total_aw_fract = 0.0;
    total_core_area = 0.0;
    total_core_index = 0.0;
    total_core_sq = 0.0;
    total_contrast = 0.0;
    total_aw_contrast = 0.0;
    total_contrast_edge = 0.0;
    total_nndist = 0.0;
    total_nndist_sq = 0.0;
	total_prox = 0.0;
    total_core_patches = 0;
    total_nn = 0;
	max_patch_id = 0;
 
 
/*
 *  Get a clean copy of the image and then find the length of the 
 *  landscape boundary.
 */
	read_image(0);
	boundary = boundary_edge();
	Rprintf ("\n");
	
/*
 *  Loop over all classes present.
 */
	for (k=0; k < NUM_CLASSES; k++) {
	   Rprintf ("\nclass: %d",patchtype[k]);
	   class = patchtype[k];
	   classarea = 0.0;
	   num_patches = 0;
	   all_edge = true_edge = lshape_edge = iji_edge = 0.0;	
/*
 *  Get a clean copy of the image for each class.
 */
	   read_image(0);
	
/*
 *  Find all patches in this class
 */
	   for (i=0; i < num_rows; i++) {
	      for (j=0; j < num_cols; j++) {

		 if (getshort(image,num_cols,i,j) == class) {
		    numpts = get_patch(j,i,class);

		    patch_ID ++;

		    if (patch_ID >= 32767) {
		       if (id_image == 1) {
		          Rprintf ("\n\nERROR! Patch ID exceeds 32767 - the maximum");
		          Rprintf ("\nvalue for a [signed short int].  The output");
		          Rprintf ("\nID image will NOT contain unique IDs for each");
		          Rprintf ("\npatch!!  Continuing .... \n\n");
		       }
		       if (patch_stats && id_image <= 2) {
		          Rprintf ("\n\nERROR! Patch ID exceeds 32767 - the maximum");
			  Rprintf ("\nvalue for a [signed short int].  The output");
			  Rprintf ("\npatch statistics files will NOT contain unique");
			  Rprintf ("\nIDs for each patch!!  Continuing ...\n\n");
		       }
		       Rprintf ("\nsetting patch_id back to 0");
		       patch_ID = 0;
		    }
		    if (patch_ID > max_patch_id) max_patch_id = patch_ID;

		    if (id_image == 3) 
		       patchid[num_patches] = getshort(id,num_cols,i,j);
		    if (id_image < 3 && patch_stats)
		       patchid[num_patches] = patch_ID;

/*
 *  Calculate patch level statistics
 */	
	            patch_statistics (numpts,class,num_patches);

		    if (do_nndist) near_neigh_dist[num_patches] = 0.0;
		    if (proximity) prox[num_patches] = 0.0;
		    num_patches ++;
		 } 
	      }
   	   }
	   total_patches += num_patches;

	   
	   
/*
 *  If the input image includes a landscape border, then edges
 *  of patches outside the landscape of the current patch type
 *  needed to be added in as "edge".  The edge map so far only
 *  contains edges of patches of class "k" inside the landscape.
 *  Call a routine to add in these edges.
 */
	   if (land_border) check_edges(class);

/*
 *  Find the nearest neighbor distance if the user requested
 *  these indices.
 *
 *  Even if there is only one patch, still call the routine in
 *  order that some housekeeping can take place.
 *  TM 30/09/93
 */
	   if (do_nndist) 
	     near_neigh_distance();	   
	   		
/*
 *  Write out patch and class level statistics.
 */
	   if (patch_stats) 
	      write_patch (class,k,num_patches);

	   class_statistics (class,k);
	}

	
/*
 * Calculate and print landscape indices.
 */
	landscape_edge_lengths();

	landscape_statistics();

	
/*
 *  Write out ID image
 */
//	if (id_image == 1) write_image(max_patch_id);

/*
 *  Free up space allocated for arrays
 */
	free_memory();

	Rprintf ("\ndone\n");
	
	SEXP res;
	int *xres;
	PROTECT(res = allocVector( INTSXP, 1) );
	xres = INTEGER(res);
	xres[0] = 1;
	UNPROTECT(1);
	return(res);
}


