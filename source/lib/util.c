/* util.c */

#include "base.h"

// ---------------------------------------------------------------------
I alen(A a)
{
// *INDENT-OFF*
  I p;
	switch (a->t) {
  case 1 :
  case 2 : return 40 + 8 * (a->r + a->c/8);
  case 4 :
  case 8 : return 32 + 8 * (a->r + a->c);
  case 16 : return 32 + 8 * (a->r + 2 * a->c);
  case 32 :
	  p=a->s[a->r+a->c-1];
		if (p==0) return 40;
	  return p + alen((A)(p + (char*)a));
	default : return 0;
  }
// *INDENT-ON*
}

// ---------------------------------------------------------------------
// integer to SEXP
SEXP int2sexp(I n)
{
  SEXP r;
  PROTECT(r=NEW_INTEGER(1));
  INTEGER_POINTER(r)[0]=n;
  UNPROTECT(1);
  return r;
}

// ---------------------------------------------------------------------
// char string to SEXP
SEXP string2sexp(char *s)
{
  SEXP r;
  int i, len=strlen(s);
  char *buffer=strdup(s);
  PROTECT(r = allocVector(STRSXP, 1));
  SET_STRING_ELT(r, 0, mkChar(buffer));
  free(buffer);
  UNPROTECT(1);
  return r;
}
