# test J server for R
# ensure shared object is accessible
# command line: Rscript jservertest.R

source("jserver.R")

pid = function(x,y){print(identical(x,y))}
psg = function(x){
 Jset("abc",x)
 pid(x,Jget("abc"))
}

Jinit();

a=Jexec("2 3$0 1 0 0 0 1")
pid(a,matrix(c(FALSE,TRUE,FALSE,FALSE,FALSE,TRUE),nrow=2))

a=Jexec("2.1;'hello';i.3")
pid(a,list(2.1,"hello",0:2))

psg(c(2.7,88.123))
psg(0.1 * matrix(1:6,nrow=3))
psg("hello world")
psg(c("hello","world"))

