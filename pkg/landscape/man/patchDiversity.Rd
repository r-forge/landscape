\name{patchDiversity}

\docType{methods}

\alias{patchDiversity}
\alias{patchDiversity,RasterLayer-method}
\alias{patchDiversity,SpatialPolygons-method}

\title{Patch diversity}

\description{
Compute the Shannon diversity index for patches.
}

\usage{
\S4method{patchDiversity}{RasterLayer}(x, ...)

\S4method{patchDiversity}{SpatialPolygons}(x, ...)
}

\arguments{
  \item{x}{A Raster* object or a SpatialPointsDataFrame}
  \item{...}{Additional arguments. None implemented}  
}


\value{numeric}

\seealso{ \code{\link{diversity}} }

\author{Robert J. Hijmans}

\examples{
f <- system.file("exdata/fragstats.grd", package="landscape")
r <- raster(f)
r[r == 9999] <- NA
patchDiversity(r)
}

\keyword{methods}
\keyword{spatial}

