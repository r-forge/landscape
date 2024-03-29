# Author: Robert J. Hijmans, r.hijmans@gmail.com
# 2009
# Version 0.1
# Licence GPL3


.shannon <- function(x, ...) {
#   H(i) := sum((-P(i) * ln P(i))
#   P(i) = the Proportion of objects in the i-th class
	x <- na.omit(x)
	if (length(x) < 1) { return(NA) 
	} else {
		spp <- as.matrix(table(x)) / length(x)
		H <- -1 * spp * log(spp) 	
		return(sum(H))
	} 
}

.margalef <- function(x, ...) {
#    //Margalef. S: Number of species. N: Number of individuals in the S species.
	x <- na.omit(x)
	S <- length(unique(x))
	N <- length(x)
	return((S-1)/log(N))
}

.menhinick <- function(x, ...) {
	x <- na.omit(x)
	S <- length(unique(x))
	N <- length(x)
	return((S)/sqrt(N))
}


.simpson <- function(x, ...) {
	x <- na.omit(x)
	spp <- as.matrix(table(x)) / length(x)
	return(sum(spp^2))
}

.simpson2 <- function(x, ...) {
	x <- na.omit(x)
#Simpson's index, D = sum( n(i)*(n(i)-1) / N*(N-1) )
#n(i) = number of objects in i-th class (species)
#N = total number of objects}
	spp <- as.matrix(table(x)) 
	obs <- length(x)
	n <- spp * (spp - 1)
	N <- obs * (obs - 1)
	return(sum(n/N))
}

.brillouin <- function(x, ...) {
	x <- na.omit(x)

	spp <- as.matrix(table(x)) / length(x)
	obs <- length(x)
	small <- which(spp < 150) 
    big <- which(spp >= 150)
	f <- vector('numeric', length=length(spp))
	f[small] <- log(factorial(spp[small]))
# Stirling's approximation ln(x!) = x.ln(x) - x */
	f[big] <- spp[big] * log(spp[big]) - spp[big]
	if (obs < 150)  {n <- log(factorial(obs)) 
	} else { n <- obs * log(obs) - obs }
	HB <- (n - sum(f)) / obs
	return(HB)
}	
	

.renyi <- function(x, alpha, ...) {
	x <- na.omit(x)

	spp <- as.vector(table(x)) / length(x)
	spp <- spp^alpha
	reyni <- (1 / (1-alpha) ) * log(sum(spp))
	return(reyni)
}
	
