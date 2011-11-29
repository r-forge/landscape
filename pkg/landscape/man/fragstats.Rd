\name{fragstats}

\alias{fragstats}

\title{fragstats}

\description{
Wrapper around fragstats 2.0 code  -- experimental

This is a wrapper to FRAGSTATS version 2, that was released in the public domain in 1995 and described in 
\href{http://www.umass.edu/landeco/pubs/mcgarigal.marks.1995.pdf}{McGarigal and Marks 1995}. 

See this \href{http://andrewsforest.oregonstate.edu/data/tools/software/fragstat.cfm?topnav=149}{link} for more info.

}

\usage{
fragstats(x, ...)
}

\arguments{
  \item{x}{RasterLayer (currently must be from file and of 'raster' format & INT2U data type)}
  \item{...}{Additional arguments (none implemented)}
}


\details{
All patches within the landscape of interest must have patch type values greater than or equal to zero.  Patches within the landscape border (if present) must have values less than zero.

cellsize:    The size of cells in meters in the input image.  Cells {float}  must be square.  (The length of one side of a cell 
		   should be input.)

edge_dist:   The distance from patch edge in meters to use for {float} determining core area (interior habitat).  The core area
		   of a patch is the area remaining after a buffer "edge_dist" wide is removed from the edge of a patch.

background:  Optional; the value of (interior) background cells.  This {int} is only required if there are cells interior or exterior
		   to the landscape of interest that should be ignored.  [default is NONE]

		   Background patch cells inside the landscape should have a positive patch type value.  Background cells in the
		   landscape border and outside the area of interest should be set to a negative patch type value (the negative of
 		   the value used for interior background patch cells). The user must enter a positive value for background 
		   even if the landscape contains only exterior (negative) background cells.
		   
		   Sometimes this convention is difficult to follow.  If only one type of background is found in the image (only
		   interior or only exterior background), fragstats will verify that each patch has been classified correctly.  If
		   fragstats finds that an interior background patch has been incorrectly classified as exterior background, it will be
		   re-classified as interior background and a message will be issued.  Incorrectly classified interior background patches
		   will also be re-classified as exterior, if necessary.  A warning will be issued about any questionable patches
		   (background patches along the landscape boundary).

		   If background patches along the landscape boundary are not classified correctly, the following indices may not
		   be calculated correctly (class and landscape level): landscape shape index, total edge, edge density, contrast
		   weighted edge density, and total edge contrast index. Beware!
	  
      
      max_classes: Optional; the maximum number of patch types (classes) {int} that could be present in the landscape.  This is needed
		   for calculating relative patch richness.  If a value is  not provided, relative patch richness will not be calculated.
                   [default is NONE]


      weight_file: Optional; the name of an ascii file containing weights {char} for each combination of patch type (class). Each record
		   should contain the numeric representation of two patch types and a weight, separated by a comma or a space[s].
		   Weights should range in value from 0 to 1.  For example:

		    1,2,.25
			1,3,.32
			1,4,.60
			2,3,.45    etc.

		   These weights represent the magnitude of edge contrast between adjacent patch types.  If a file is provided,
		   these weights are used to calculate the contrast weighted edge indices.[default is NONE]

		   NOTE:  Background patch type values should NOT be included in this file.  Also, a blank line will cause
		   fragstats to blow up!

      id_image:    Optional; the method for assigning patch IDs to each {char} patch in the landscape:
 
		   1 -  Create and output an image that contains unique ID's for each patch.  This allows the user to relate
 		        a set of patch statistics to a specific patch in the landscape.  This file is given the name "in_image",
	            up to the first dot (".") encountered, and the extension .ID.  For example, for an input image
		        named test.dat, the output ID image would be test.ID 

		   2 -  Do not output an ID image (i.e., it's not important to relate a set of patch statistics to a specific
		        patch in the landscape).

		   3 -  The name of an ID image to read.  The ID associated with each patch in this image will be written to
                to output files.  The "data_type" of this file must be the same as "in_image".

		   Input 1,2, or the name of a file. 
		   [default is 2] 


      desc_file:   Optional; the name of an ascii file containing character
       {char}	   descriptors for each patch type (class).  Each record in
		   the file should contain a numeric patch type value and
		   the character descriptor for that patch type, separated
		   by a comma or a space[s].  For example:

	        1  shrubs
			2  conifers
			3  decidious    etc.

		   Descriptive names can not contain spaces!  Use an underscore ("_") or a hyphen ("-") in place of blanks.
		   
		   The parameter MAX_LABEL_LENGTH in the file stats.h 
                   controls the printed length of labels in the output
                   files.  Fragstats is distributed with MAX_LABEL_LENGTH 
		   set to 10.  To change this, edit the file stats.h, change
		   the parameter to the appropriate length, then re-build 
		   fragstats.  Note that if MAX_LABEL_LENGTH exceeds 22,
		   the columns will no longer be aligned in the file
		   "basename.full".

		   If provided, the character descriptors will be written
 		   to the output files.  Otherwise, the numeric patch type
 		   is written.
		   [default is NONE]	


      bound_wght:  Optional; the proportion (equivalent to contrast weight)
       {float}	   of the landscape boundary and background class edges to 
                   be considered edge.  This affects all the edge indices.
		   [default is 0.0]


      diags:       Optional; should diagonal neighbors be evaluated when
       {y/n}	   finding the cells that make up a patch.  If not, then 
		   the 4 cells (not 8) surrounding the cell of interest
		   will be evaluated.
		   [default is YES]

	
      prox_dist:   Optional; the search radius in meters to use for 
       {float}     calculating the proximity index.  If a value is not
	     	   provided, the proximity index will not be calculated.
	           Note that "nndist" (below) must be "yes" if the proximity
		   index is to be computed as they require the same
		   calculations.
		   [default is NONE]


      nndist:      Optional; should nearest neighbor distance be calculated?
       {y/n}       This can be very time consuming on landscapes with 
		   hundreds of patches per class.  Note that this parameter
		   must be "yes" in order for the proximity index (above)
	           to be computed as they are based on the same calculations.
		   [default is YES]


      patch_stats: Optional; should patch indices be written to the output {y/n} files?  If not, the "basename.patch" file will not be
		   created and the patch level indices will not be written to the "basename.full" file.  [default is YES]


      class_stats: Optional; should class indices be written to the output  {y/n} files?  If not, the "basename.class" file will not be
		   created and the class level indices will not be written to the "basename.full" file. [default is YES]



   NOTES

      (1)  If an index is not calculated, a dot (".") will be output to the "basename.patch", "basename.class", and "basename.land"
	   files.  The abbreviation "NA" will be output to the "basename.full" file.  For nearest neighbor distance, if a
           patch has no neighbors, "NONE" will be output to "basename.full"  and a dot to the other files.


   LIMITATIONS:

   (1)  The input landscape file and the patch ID file are stored as 
       signed shorts (16 bits) in this program.  Therefore, a landscape
	   may not contain more than 32767 different patch types (this
	   shouldn't be a problem!).  The input (or output) patch ID image 
	   is also limited to 32767 unique IDs.
          
}

\value{
list with 4 items

patch:  Each record contains all the patch level indices for a given patch separated by spaces.

class:  Each record contains all the class level indices for a given class separated by spaces.

land:  Each record contains all the landscape level indices for a given landscape separated by spaces. 

full:  patch, class, and landscape level indices for a given landscape.  This file is formatted for displaying results.

}

\author{Robert J. Hijmans wrapped and modified code by Barbara Marks }

\examples{
\dontrun{
f <- system.file("exdata/fragstats.grd", package="landscape")
r <- raster(f)
fs <- fragstats(r)
fs
}
}

\keyword{spatial}

