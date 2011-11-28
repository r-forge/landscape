# Author: Paul Galpern pgalpern@gmail.com
# Date : November 2011
# Version 1.0
# Licence GPL v3

## Produce a circular object of class Polygon by finding points on the edge of a circle
## given a centre, a radius and the number of pts.
## Optionally return a two-column matrix with pts coordinates.
.circlePoly <- function(centreX, centreY, radius, pts=radius*2, poly=TRUE) {
    if (pts<4) pts <- 4
    unit <- 2*pi/pts
    x <- vector("numeric", pts)
    y <- x
    for (i in 0:(pts-1)) {
       x[i+1] <- centreX + radius*sin(unit*i)
       y[i+1] <- centreY + radius*cos(unit*i)
    }
    
    coords <- cbind(x,y)
    coords <- rbind(coords, coords[1,])
    if (poly) {
        return(Polygon(coords))
    }
    else {
        return(coords)
    }
}

## Produce a landscape consisting of randomly positioned patches
.randomPatch <- function(landscape, nPatch, patchSize, patchType) {
    patchCentre <- xyFromCell(landscape, sample(ncell(landscape))[1:nPatch])
    if (patchType=="circle") {
        patch <- SpatialPolygons(list(Polygons(apply(patchCentre, 1,
                    function(x) .circlePoly(x[1], x[2], patchSize)), ID="1")))
    }
    else if (patchType=="square") {
        sq <- patchSize/2
        patch <- SpatialPolygons(list(Polygons(apply(patchCentre, 1,
                    function(x) Polygon(rbind(c(x[1]-sq, x[2]-sq),
                                              c(x[1]+sq, x[2]-sq),
                                              c(x[1]+sq, x[2]+sq),
                                              c(x[1]-sq, x[2]+sq),
                                              c(x[1]-sq, x[2]-sq)), hole=FALSE)), ID="1")))
    }
    rasterize(patch, landscape)
}
    
## Produce a landscape consisting of a regular grid of patches
.regularPatch <- function(landscape, nX, nY, patchSize, patchType) {
    offsetX <- (ncol(landscape)/nX)*0.5
    offsetY <- (nrow(landscape)/nY)*0.5
    regularX <- cellFromCol(landscape,colFromX(landscape, seq(xmin(landscape)-offsetX, xmax(landscape)+offsetX, length=nX+2)[2:(nX+1)]))
    regularY <- cellFromRow(landscape,rowFromY(landscape, seq(ymin(landscape)-offsetY, ymax(landscape)+offsetY, length=nY+2)[2:(nY+1)]))
    patchCentre <- xyFromCell(landscape, regularX[regularX %in% regularY])
    if (patchType=="circle") {
        patch <- SpatialPolygons(list(Polygons(apply(patchCentre, 1,
                    function(x) .circlePoly(x[1], x[2], patchSize)), ID="1")))
    }
    else if (patchType=="square") {
        if ((patchSize %% 2)==0) patchSize <- patchSize + 1
        sq <- patchSize/2
        patch <- SpatialPolygons(list(Polygons(apply(patchCentre, 1,
                    function(x) Polygon(rbind(c(x[1]-sq, x[2]-sq),
                                              c(x[1]+sq, x[2]-sq),
                                              c(x[1]+sq, x[2]+sq),
                                              c(x[1]-sq, x[2]+sq),
                                              c(x[1]-sq, x[2]-sq)), hole=FALSE)), ID="1")))
    }
    rasterize(patch, landscape)
}

## Produce a landscape consisting of an irregular grid of patches
.spacedPatch <- function(landscape, spaceX, spaceY, patchSize, patchType) { 
    regularX <- cellFromCol(landscape,cumsum(((spaceX/sum(spaceX))*ncol(landscape))[1:length(spaceX)-1]))
    regularY <- cellFromRow(landscape, cumsum(((spaceY/sum(spaceY))*nrow(landscape))[1:length(spaceY)-1]))
    patchCentre <- xyFromCell(landscape, regularX[regularX %in% regularY])
    if (patchType=="circle") {
        patch <- SpatialPolygons(list(Polygons(apply(patchCentre, 1,
                    function(x) .circlePoly(x[1], x[2], patchSize)), ID="1")))
    }
    else if (patchType=="square") {
        sq <- patchSize/2
        patch <- SpatialPolygons(list(Polygons(apply(patchCentre, 1,
                    function(x) Polygon(rbind(c(x[1]-sq, x[2]-sq),
                                              c(x[1]+sq, x[2]-sq),
                                              c(x[1]+sq, x[2]+sq),
                                              c(x[1]-sq, x[2]+sq),
                                              c(x[1]-sq, x[2]-sq)), hole=FALSE)), ID="1")))
    }
    rasterize(patch, landscape)
}

## Produce a fragmented landscape using the algorithm described by
## Rayfield et al. (2010) Landscape Ecology 25:519-532.
.fragPatch <- function(landscape, hCov, hFrag, verbose, plotProgress) {

    landscape[] <- 0
    cells <- ncell(landscape)
    lastMessage <- ""
    
    for (iPatch in 1:length(hCov)) {
        cellValue <- iPatch
        if (verbose) cat("Patch class ",cellValue, "\n")

        repeat {
            focalCell <- sample(which(landscape[] == 0))[1]
            adj <- adjacent(landscape, focalCell, directions=8)
            if (any(landscape[adj] == cellValue, na.rm=T)) {
                landscape[focalCell] <- cellValue
            }
            else if (runif(1) < hFrag[iPatch]^2) {
                landscape[focalCell] <- cellValue
            }
            done <- sum(landscape[] == cellValue, na.rm=T) / cells
            message <- paste(round(done*100), "% cover\n", sep="")
            if (message != lastMessage) {
                if (verbose) cat(message)
                if (plotProgress) plot(landscape)
                lastMessage <- message
            }
           if (done >= hCov[iPatch]) break    
        }
    }
    landscape[landscape[]==0] <- NA
    return(landscape)
}
	
