# test J server for R
# ensure shared object is accessible
# command line: Rscript jservertest.R

source("jserver.R")

p = function(x){print(x)}
pi = function(x,y){p(identical(x,y))}
ps = function(x){
 Jset("abc",x)
 pi(x,Jget("abc"))
}

Jinit();

a=Jcmd("2 3$0 1 0 0 0 1")
pi(a,matrix(c(FALSE,TRUE,FALSE,FALSE,FALSE,TRUE),nrow=2))

ps(c(2.7,88.123))
ps(0.1 * matrix(1:6,nrow=3))
ps("hello world")

