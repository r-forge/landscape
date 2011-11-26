\name{diversity}

\docType{methods}

\alias{diversity}
\alias{diversity,RasterLayer,numeric-method}
\alias{diversity,SpatialPointsDataFrame,Raster-method}

\title{Compute diversity indices}

\description{
describe it here
}

\usage{
\S4method{diversity}{RasterLayer,numeric}(x, y, fun='richness', ...)

\S4method{diversity}{SpatialPointsDataFrame,Raster}(x, y, field=NULL, fun='richness', ...)
}

\arguments{
  \item{x}{A Raster* object or a SpatialPointsDataFrame}
  \item{y}{numeric or Raster* object}  
  \item{field}{variable to use}  
  \item{fun}{Function or character }  
  \item{...}{Additional arguments as for \code{\link{writeRaster}}}  
}

\details{
none 
}

\value{
RasterLayer
}

\seealso{ \code{\link[raster]{aggregate}, \link[raster]{rasterize}} }

\author{Robert J. Hijmans}

\examples{
r <- raster()
r[] <- runif(ncell(r))
x <- diversity(r, 10, 'Shannon')
}

\keyword{methods}
\keyword{spatial}

