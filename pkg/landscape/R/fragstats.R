

fragstats <- function(x, ...) {

#	x[is.na(x)] <- 9999
#	f <- rasterTmpFile()
#	x <- writeRaster(x, f, datatype='INT2U') 
	out <- 'fragstats'
	f <- extension(filename(x), '.gri')

	ff <- paste(out, c('.full', '.class', '.patch', ''), sep='')
	e <- file.exists(ff)
	file.remove(ff[e])
	
	res <- .Call("fragstats", f, out, c(dim(x), res(x)[1]), PACKAGE = "landscape")
	if (res) {
		res <- list()
		res[[1]] <- readLines(ff[1])
		res[[2]] <- read.table(ff[2], header=TRUE)
		res[[3]] <- read.table(ff[3], header=TRUE)
		res[[4]] <- read.table(ff[4], header=TRUE)
		names(res) <- c('full', 'class', 'patch', '')
		return(res)
	} else {
		warning('an error occurred')
	}

}

