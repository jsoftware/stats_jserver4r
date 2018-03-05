# J server for R utilities
# ensure the shared object is accessible

Jdo = function(exp) { .Call("rjdo",exp) }
Jexec = function(exp) { .Call("rjexec",exp) }
Jget = function(name) { .Call("rjget",name) }
Jgetr = function(name) { .Call("rjgetr") }
Jset = function(name,value) { .Call("rjset",name,value) }

# Jinit - the optional argument is the full pathname of a
#         profile.ijs script in the J bin directory
# e.g. Jinit("/home/elmo/j8/bin/profile.ijs")
Jinit = function(profile="") {
 os = Sys.info()["sysname"]
 ex = if (os == "Windows") "dll" else if (os == "Darwin") "dylib" else "so"
 so = paste("libjserver4r",ex,sep=".")
 if (is.loaded("rjinit")) dyn.unload(so)
 dyn.load(so)
 .Call("rjinit", profile)
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

