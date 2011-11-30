
setClass('Patch',	
	representation (
		n = 'integer'
	),
	prototype (
		n = as.integer(0)
	),
)


setClass('RasterPatch',  contains=c('RasterLayer'),	
	representation (
		patch = 'Patch'
	)
)



setMethod ('show' , 'RasterPatch', 
	function(object) {
		callNextMethod(object)
		cat('nr. patches :', object@patch@n, '\n')
	}
)	
