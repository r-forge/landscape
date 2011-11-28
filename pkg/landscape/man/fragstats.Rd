\name{fragstats}

\alias{fragstats}

\title{fragstats}

\description{
Wrapper around fragstats 2.0 code  -- experimental
}

\usage{
fragstats(x, ...)
}

\arguments{
  \item{x}{RasterLayer (currently must be from file and of 'raster' format & INT2U data type)}
  \item{...}{Additional arguments (none implemented)}
}

\details{
This is a wrapper to FRAGSTATS version 2, that was released in the public domain in 1995 and described in 
\href{http://www.umass.edu/landeco/pubs/mcgarigal.marks.1995.pdf}{McGarigal and Marks 1995}. 

See this \href{http://andrewsforest.oregonstate.edu/data/tools/software/fragstat.cfm?topnav=149}{link} for more info.
}

\value{
NULL
}

\author{Robert J. Hijmans}

\examples{
\dontrun{
f <- system.file("exdata/fragstats.grd", package="landscape")
r <- raster(f)
fs <- fragstats(r)
fs
}
}

\keyword{spatial}

