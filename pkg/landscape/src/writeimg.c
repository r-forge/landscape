/**********************************************************************
 *  File Name:  WRITEIMG.C
 *
 *  Purpose:  Sets the output ID filename and calls the appropriate
 *	routine for writing it.
 *
 *  Passed Parameters:
 *	max_patch_id:    the largest patch ID assigned to a patch in
 *			 the landscape.
 *
 *  Programmer:  Barbara Marks
 *
 *  Date:  December 1992
 * 
 *  ID:  $Id: writeimg.c,v 2.3 1994/11/08 15:02:05 marks Exp marks $
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"
#include <R.h>



void write_image (max_patch_id)
short	max_patch_id;
{

//	short  packtype;
	char   *ptr,*out;
	static char   outname[60] = " "; 



/*
 *  3-1-94 BJM  Per Eric Gustafson's comments, the output ID filename
 *	is being changed.  DOS does not support a filename like
 *	"test.dat.ID".  The new output ID filename will consist of
 *	all characters in the input file name up to the first dot
 * 	encountered, plus the extension ".ID".  For example, the ID 
 *      filename for an input file named test.dat would be test.ID.
 */

	ptr = imagename;
	out = outname;

	while (*ptr && *ptr != '.') 
	        *out++ = *ptr++;


/*
 *  11-8-94 BJM  Error discovered!!  Does not work if the input filename
 *  does not contain a ".".  The output file gets the same name as the 
 *  input -- the .ID does not get appended to the name.  So the input
 *  image was overwritten!  (Used to be "strcat(out,".ID"))
 */
    strcat(outname,".ID");
	Rprintf ("\nOutput ID file is: %s\n",outname);

    write_binary (outname,id,2);
}


