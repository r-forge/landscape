\name{patchSize}

\docType{methods}

\alias{patchSize}
\alias{patchSize,RasterLayer-method}
\alias{patchSize,SpatialPolygons-method}

\title{Patch size}

\description{
Compute the area of patches. Patches are defined on a RasterLayer of integer values (values are rounded in any case). Values are in ha for longitude/latitude data, and for projected coordinate reference systems if the map units are m. In other cases the unit is squared map units times 10000. 
}

\usage{
\S4method{patchSize}{RasterLayer}(x, ...)
}

\arguments{
  \item{x}{RasterLayer}
  \item{...}{Additional arguments (none implemented)}  
}


\value{
matrix
}

\author{Robert J. Hijmans}

\examples{
r <- raster(ncols=12, nrows=12)
r[] <- round(runif(ncell(r))*0.6 )
rc <- clump(r)
patchSize(rc)
}

\keyword{methods}
\keyword{spatial}

