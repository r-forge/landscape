# Author: Paul Galpern, pgalpern@gmail.com
# Date : November 2011
# Version 1.0
# Licence GPL v3


if (!isGeneric("makeLandscape")) {
	setGeneric("makeLandscape", function(x, y, ...)
		standardGeneric("makeLandscape"))
}	

.checkLandscape <- function(landscape) {
    if (sum(!is.na(landscape[]) > 0)) {
        stop("x must be a RasterLayer with no values set")
    }
}


## Make a random landscape with the given number of patches
setMethod('makeLandscape', signature(x='RasterLayer', y='numeric'), 
	function(x, y, patchSize=3, patchType="circle", ...) {
		.checkLandscape(x)
        patchType <- tolower(patchType)
        if (!(patchType) %in% c("circle", "square")) {
            stop("use patchType='circle' or patchType='square'")
        }
        .randomPatch(x, y, patchSize, patchType)
	}
)

## Make a regular, spaced patch or fragmented patch landscape given a list
## with parameter elements
setMethod('makeLandscape', signature(x='RasterLayer', y='list'), 
	function(x, y, patchSize=3, patchType="circle", verbose=TRUE, plotProgress=TRUE, ...) {
		.checkLandscape(x)
        patchType <- tolower(patchType)
        if (!(patchType) %in% c("circle", "square")) {
            stop("use patchType='circle' or patchType='square'")
        }
        if (length(names(y)) != 2) {
            stop("param list must contain two elements: nX, nY OR spaceX, spaceY OR hCov, hFrag")
        }
        if (all(names(y) %in% c("nX", "nY"))) {
            .regularPatch(x, y$nX, y$nY, patchSize, patchType)
        }
        else if (all(names(y) %in% c("spaceX", "spaceY"))) {
            if ((length(y$spaceX)<=1) || (length(y$spaceY)<=1)) {
                stop("spaceY and spaceX vectors must each contain at least two elements")
            }
            .spacedPatch(x, y$spaceX, y$spaceY, patchSize, patchType)
        }
        else if (all(names(y) %in% c("hCov", "hFrag"))) {
            if ((sum(y$hCov) > 1) || (any(y$hFrag > 1))) {
                stop("sum of hCov elements must not exceed 1; hFrag elements must be less than or equal to 1")
            }
            if (length(y$hCov) != length(y$hFrag)) {
                stop("hCov and hFrag must have the same number of elements")
            }
            .fragPatch(x, y$hCov, y$hFrag, verbose, plotProgress)
        }
        else {
            stop("param list must contain two elements: nX, nY OR spaceX, spaceY OR hCov, hFrag")
        }
	}
)



