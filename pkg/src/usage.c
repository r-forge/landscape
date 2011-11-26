/********************************************************************
 *  File Name:  USAGE.C
 *
 *  Purpose:  Prints to the screen the usage of the program 
 *	fragstats (i.e., the command line arguments and a 
 *	short description of each one).
 *
 *  Programmer:  Barbara Marks
 * 
 *  Date:  January 1993
 *
 *  ID: $Id: usage.c,v 2.4 1994/04/06 14:27:53 marks Exp marks $
 *******************************************************************/ 
#include <R.h>

 
void usage ()
{


	Rprintf ("\n\nUSAGE: ");
	Rprintf ("\n   fragstats in_image out_file cellsize edge_dist data_type [rows]");
	Rprintf ("\n   [cols] [background] [max_classes] [wt_file] [id_image] [desc_file]");
	Rprintf ("\n   [bound_wght] [diags] [prox_dist] [nndist] [patch_stats] [class_stats]");

	Rprintf ("\n\nin_image:    name of input image file {char}");
	Rprintf ("\nout_file:    basename for output files {char}");
	Rprintf ("\ncellsize:    size of cells in meters in in_image {float}");
	Rprintf ("\nedge_dist:   distance from edge in meters for core area {float}");
	Rprintf ("\ndata_type:   type of input image file {int}:");
	Rprintf ("\n             1 - SVF             2 - ascii  3 - 8 bit binary");
	Rprintf ("\n             4 - 16 bit binary   5 - ERDAS  6 - IDRISI");
	Rprintf ("\nrows:        optional; #rows in image; required if data_type 2,3,4 {int}");
	Rprintf ("\ncols:        optional; #cols in image; required if data_type 2,3,4 {int}");
        Rprintf ("\nbackground:  optional; value of background cells (positive) {int} {NONE]");
	Rprintf ("\nmax_classes: optional; max possible classes in landscape {int} [NONE]");
        Rprintf ("\nwt_file:     optional; name of file containing weights for each");
	Rprintf ("\n             combination of patch type {char} [NONE]");
	Rprintf ("\nid_image:    optional; method for assigning patch IDs:");
	Rprintf ("\n             1 - create and output ID image");
	Rprintf ("\n             2 - do not output an ID image [default]");
	Rprintf ("\n               - name of ID image; data_type must be same as in_image");
	Rprintf ("\ndesc_file:   optional; name of file containing character descriptors");
	Rprintf ("\n             for each patch type {char} [NONE]"); 
	Rprintf ("\nbound_wght:  optional; proportion of landscape boundary and background");
	Rprintf ("\n             class edges to be considered edge {float} [0.0]");
	Rprintf ("\ndiags:       optional; use diagonals in patch finding {y/n} [YES]");
	Rprintf ("\nprox_dist:   optional; search radius in meters for proximity {float} [NONE]");
	Rprintf ("\nnndist:      optional; calculate nearest neighbor {y/n} [YES] ");
	Rprintf ("\npatch_stats: optional; print patch level stats {y/n} [YES]");
	Rprintf ("\nclass_stats: optional; print class level stats {y/n} [YES]");
	Rprintf ("\n\nNOTE:  use a $ in place of skipped optional parameters");

	Rprintf ("\n\n");

}
