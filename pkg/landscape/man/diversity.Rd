\name{diversity}

\docType{methods}

\alias{diversity}
\alias{diversity,RasterLayer,numeric-method}
\alias{diversity,SpatialPointsDataFrame,Raster-method}

\title{diversity indices}

\description{
Compute diversity indices based for a neighborhood of raster cells. Or for points falling into a raster cell.
}

\usage{
\S4method{diversity}{RasterLayer,numeric}(x, y, fun='richness', ...)

\S4method{diversity}{SpatialPointsDataFrame,Raster}(x, y, field=NULL, fun='richness', ...)
}

\arguments{
  \item{x}{A Raster* object or a SpatialPointsDataFrame}
  \item{y}{numeric as in \code{\link{focal}}, or Raster* object}  
  \item{field}{variable to use}  
  \item{fun}{Function or character }  
  \item{...}{Additional arguments as for \code{\link{writeRaster}}}  
}


\value{
RasterLayer
}

\seealso{ \code{\link[raster]{focal}, \link[raster]{rasterize}} }

\author{Robert J. Hijmans}

\examples{
r <- raster()
r[] <- runif(ncell(r))
x <- diversity(r, 9, 'Shannon')
}

\keyword{methods}
\keyword{spatial}

