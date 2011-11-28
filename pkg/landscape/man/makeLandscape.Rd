\name{makeLandscape}

\docType{methods}

\alias{makeLandscape}
\alias{makeLandscape,RasterLayer,numeric-method}
\alias{makeLandscape,RasterLayer,list-method}

\title{Generate artificial patchy landscapes}

\description{
Generate artificial patchy landscapes of various types:  (1) Randomly positioned patches;
(2) Patches positioned on a regular grid;  (3) Patches positioned
on an irregular grid;  (4) Patches generated using a fragmentation algorithm.
}

\usage{
\S4method{makeLandscape}{RasterLayer,numeric}(x, y, patchSize=3, patchType="circle", ...)

\S4method{makeLandscape}{RasterLayer,list}(x, y, patchSize=3, patchType="circle", verbose=TRUE, plotProgress=TRUE, ...)

}

\arguments{
  \item{x}{A RasterLayer object}
  \item{y}{integer giving the number of patches or list giving parameters; see Details}  
  \item{patchSize}{size of patch (radius if patchType='circle'; dim/2 if patchType='square')}  
  \item{patchType}{type of patch 'circle' or 'square'}
  \item{verbose}{Report progress for fragmentation algorithm}
  \item{plotProgress}{Plot progress for fragmentation algorithm}
  \item{...}{Additional arguments as for \code{\link{writeRaster}}}  
}

\value{
RasterLayer
}

\details{
  (1) Randomly positioned patches.  y should be an integer giving the number of patches to
  generate.
  
  (2) Patches positioned on a regular gird.  y should be a list containing two elements
  nX and nY.  e.g. \code{y=list(nX=4, nY=5)} gives a grid of 4 by 5 patches.
  
  (3) Patches positioned on an irregular grid.  y should be a list containing two vectors
  spaceX and spaceY.  e.g. \code{y=list(spaceX=c(1,2,3,1), spaceY=c(1,5,3,1))} gives a pattern
  consisting of nine patches with proportional spacings in X and Y dimensions.
  
  (4) Patches generated using a fragmentation algorithm.  y should be a a list containing two
  vectors hCov and hFrag.  e.g. \code{y=list(hCov=c(0.15), hFrag=c(0.5))} creates a landscape
  with one patch cover class with 15\% cover and a 0.5^2=0.25 probability of fragmenting.
  e.g. \code{y=list(hCov=c(0.15, 0.35), hFrag=c(0.05, 0.1))} creates a landscape with
  two patch cover classes, etc.  When hFrag is small landscape generation can take a long time.
  Algorithm implemented as described in Rayfield et al. (2010).  See this description for
  more description of the hCov and hFrag parameters and their implementation.

}

\author{Paul Galpern}

\references{
Rayfield et al. (2010) Landscape Ecology 25:519-532.
}

\examples{
r <- raster()

## Generate a landscape with 10 randomly-positioned square patches
r1 <- makeLandscape(r, 10, patchType="square")

## Generate a landscape with circular patches positioned on a regular grid
r2 <- makeLandscape(r, list(nX=5, nY=7), patchSize=6)

## Generate a landscape with square patches positioned on an irregular grid
r3 <- makeLandscape(r, list(spaceX=c(1,3,6,3,1), spaceY=c(2,8,3,2)), patchType="square")

## Generate a landscape using a patch cover/fragmentation algorithm
## Patch class 1 has 25\% cover and a 0.2^2 = 0.04 probability of fragmenting
## Patch class 2 has 15\% cover and a 0.15^2 = 0.0225 probability of fragmenting
#r <- raster(nrow=25, ncol=25, xmn=0, xmx=25, ymn=0, ymx=25, crs=NA)
#r4 <- makeLandscape(r, list(hCov=c(0.25, 0.15), hFrag=c(0.5, 0.2)))
}

\keyword{methods}
\keyword{spatial}

