# J server for R utilities
# update this to define full paths to J bin folder, shared library, and profile:

# typical for windows install in home
pathbin= "c:/users/chris/j64-807/bin"
pathdll= paste(pathbin,"/j.dll",sep="")
pathpro= paste(pathbin,"/profile.ijs",sep="")

# typical for linux install in home
#pathbin= "/home/chris/j64-807/bin"
#pathdll= paste(pathbin,"/libj.so",sep="")
#pathpro= paste(pathbin,"/profile.ijs",sep="")

# typical for macos install in home
#pathbin= "/Users/chris/j64-807/bin"
#pathdll= paste(pathbin,"/libj.dylib",sep="")
#pathpro= paste(pathbin,"/profile.ijs",sep="")

# typical for debian install
# pathbin= "/usr/bin"
# pathdll= "libj.so.8.07"
# pathpro= "/etc/j/8.07/profile.ijs"

# ------------------------------------------------
# customize the following and add any enhancements as required:

Jdo = function(exp) { .Call("rjdo",exp) }
Jexec = function(exp) { .Call("rjexec",exp) }
Jget = function(name) { .Call("rjget",name) }
Jgetr = function(name) { .Call("rjgetr") }
Jset = function(name,value) { .Call("rjset",name,value) }

# Jinit - the optional argument determines whether the profile is loaded
# e.g. Jinit()   same as Jinit(TRUE)
Jinit = function(loadprofile=TRUE) {
 os = Sys.info()["sysname"]
 ex = if (os == "Windows") "dll" else if (os == "Darwin") "dylib" else "so"
 so = paste("libjserver4r",ex,sep=".")
 if (is.loaded("rjinit")) dyn.unload(so)
 dyn.load(so)
 .Call("rjinit", pathbin, pathdll, pathpro, loadprofile)
}

# run J sentences until ....
Jrun = function(){
 while (TRUE) {
  s=readline("   ")
  if (s == "....") break
  .Call("rjdo",s)
  cat(.Call("rjgetr"));
 }
}
