@rem windows jserver4r make
@rem copy the directory ~public/stats/r/jserver4r/lib anywhere and open in the console
@rem copy in R.dll and libj.dll (should be at least 807 beta-b)
@rem update the R include reference used for the actual version on the machine
@rem use gcc (e.g. from mingw), not the Microsoft compiler, and ensure it is on the path

del /Q libjserver4r.dll

gcc -m64 -I. -I"c:/Program Files/R/R-3.4.3/include" ^
 -L. -lR -lj -shared ^
 -Wl,--export-all-symbols -Wl,--enable-auto-import ^
 base.c util.c j2r.c r2j.c -o libjserver4r.dll
