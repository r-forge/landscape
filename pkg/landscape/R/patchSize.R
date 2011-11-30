# Author: Robert J. Hijmans
# Date : November 2011
# Version 1.0
# Licence GPL v3


if (!isGeneric("patchSize")) {
	setGeneric("patchSize", function(x, ...)
		standardGeneric("patchSize"))
}	


setMethod('patchSize', signature(x='RasterLayer'), 
function(x, ...) {

	if (raster:::.couldBeLonLat(x)) {
		
		a <- raster::area(x)  
		x <- zonal(a, x, 'sum') 
		x[,2] <- x[,2] / 10
		
	} else {
		
		rs <- res(x)
		x <- freq(x)
		x[,2] <- x[,2] * prod(rs)
		i <- which(is.na(x[,1]))
		if (length(i) > 0) {
			x <- x[-i,]
		}
	}
	colnames(x) <- c('patch', 'area')
	data.frame(x)
}
)


setMethod('patchSize', signature(x='SpatialPolygons'), 
function(x, id=NULL, ...) {

	if (is.null(id)) {
		patch <- row.names(x)
	} else {
		rn <- row.names(x)
		if (length(id) == length(rn)) {
			patch <- id
		} else {
			warning('length of id does not match the number of polygons')
			patch <- rn
		}
	}

	if (raster:::.couldBeLonLat(raster(x))) {
		require(geosphere)
		x <- areaPolygon(x)
	} else {
		x <- sapply(x@polygons, function(y) slot(y, 'area'))
	}
	x <- data.frame(patch=patch, area=x)
	return(x)	
}
)

