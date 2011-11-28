# Author: Robert J. Hijmans
# Date : November 2011
# Version 1.0
# Licence GPL v3


if (!isGeneric("diversity")) {
	setGeneric("diversity", function(x, y, ...)
		standardGeneric("diversity"))
}	


if (!isGeneric("patchDiversity")) {
	setGeneric("patchDiversity", function(x, ...)
		standardGeneric("patchDiversity"))
}	


.getDivFun <- function(fun) {
	if (!is.function(fun)) {
		fn <- tolower(fun[1])
		if (fn == 'richness') {
			fun <- function(x) length(unique(x))
		} else if (fn == 'shannon') {
			fun <- .shannon
		} else if (fn == 'simpson') {
			fun <- .simpson
		} else if (fn == 'margalef ') {
			fun <- .margalef 
		}
	}
	fun
}


setMethod('patchDiversity', signature(x='RasterLayer'), 
	function(x, ...) {
		a <- patchSize(x)[,2]
		a <- a / sum(a)
		sum( -1 * a * log(a) )
	}
)

setMethod('patchDiversity', signature(x='SpatialPolygons'), 
	function(x, ...) {
		a <- patchSize(x)[,2]
		a <- a / sum(a)
		sum( -1 * a * log(a) )
	}
)




# diversity of raster cells for a given neighborhood defined by y (as in raster::aggregate)
setMethod('diversity', signature(x='RasterLayer', y='numeric'), 
	function(x, y, fun='richness', ...) {
		fun <- .getDivFun(fun)
		y <- round(y)
		focal(x, y, fun, ...)
	}
)

# diversity of raster cells based on point data
setMethod('diversity', signature(x='SpatialPointsDataFrame', y='Raster'), 
	function(x, y, field=NULL, fun='richness', ...) {
		if (is.null(field)) {
			stop('you must provide an argument "field" to indicate\nthe variable in the SpatialPointsDataFrame that you want to use')
		}
		fun <- .getDivFun(fun)
		rasterize(x, y, field=field, fun=fun, ...)
	}
)

