
source("jserver.R")      # load R cover functions

Jinit()                  # initialize J

Jset("pies",pi*0:3)      # set a J noun and get it
Jget("pies")

Jset("b",c(TRUE,FALSE,NA,TRUE)) # R booleans are J 0 1 and 2=NA
Jget("b")

Jset("n",c(2,NA,5))      # integer and float NA are __ (-Inf)
Jget("n")

(m=matrix(0:5,2,3))      # R arrays are in column-matrix order
Jset("mat",m)
identical(m,Jget("mat")) # J returns the matrix as defined in R

Jdo("mat")               # J stores the matrix in row order
cat(Jgetr())

Jexec("+/mat")           # confirms the matrix is in row order

(v=c("hello","world"))   # R character vector
Jset("vec",v)
identical(v,Jget("vec")) # J returns the same character vector

Jdo("vec")               # J stores the character vector as a boxed list
cat(Jgetr())

Jexec("2.1;'hello';i.3") # a J boxed list that is not all strings is returned as an R list

Jrun()                   # this runs an interactive REPL

Jget("A")                # read value set in REPL
