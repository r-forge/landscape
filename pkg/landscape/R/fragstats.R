

fragstats <- function(...) {

# this does not do anything yet.
	if (is.loaded('fragstats')) {
		.Call('fragstats', 1, PACKAGE='landscape')
	} else {
		warning('cannot find fragstats')
	}
	return(NULL)
}

