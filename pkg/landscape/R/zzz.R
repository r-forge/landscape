
.onLoad <- function(lib, pkg)  {

	#pkg.info <- drop(read.dcf(file=system.file("DESCRIPTION", package=pkg), fields=c("Version","Date")))
	#packageStartupMessage(paste(pkg, " version ", pkg.info["Version"], " (", pkg.info["Date"], ")", sep=""))
	library.dynam("landscape", pkg, lib)	

	return(invisible(0))
}

