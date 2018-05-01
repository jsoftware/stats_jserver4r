#ifndef JSVR_H
#define JSVR_H

#include <R.h>
#include <Rdefines.h>

#define DO(n,x)	{I i=0,_i=(n);for(;i<_i;++i){x;}}

typedef char C;
typedef long long I;
typedef void* J;
typedef unsigned short US;

#ifndef _WIN32
#define __stdcall
#endif

// j engine defs
typedef J     (__stdcall *JInitType)     ();
typedef int   (__stdcall *JDoType)       (J,C*);
typedef C*    (__stdcall *JGetRType)     (J);
typedef int   (__stdcall *JGetMType)     (J,C*,I*,I*,I*,I*);
typedef int   (__stdcall *JSetAType)     (J,I,C*,I,C*);

// array rep: flag, type, count, rank, shape ...
typedef struct AREP {I f;I t;I c;I r;I s[1];} *A;

// boxed array rep: d,flag,d,type,d,atoms,US rank,US d,int d,shape ...
typedef struct BREP {I d0;I f;I d1;I t;I d2;I n;US r;US d3;int d4;I s[1];} *B;

// all fns callable from R are named "rjxxx"
SEXP rjdo(SEXP);
SEXP rjexec(SEXP);
SEXP rjinit();
SEXP rjget(SEXP);
SEXP rjgetr();
SEXP rjset(SEXP, SEXP);

I alen(A);
A sexp2j(SEXP s);
SEXP int2sexp(I);
SEXP j2sexp(I,I,I*,I*);
SEXP string2sexp(C*);

#endif
