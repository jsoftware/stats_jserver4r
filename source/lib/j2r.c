/* j2r.c */

#include "base.h"

static SEXP get_bool(I,I*,I*);
static SEXP get_char(I,I*,I*);
static SEXP get_integer(I,I*,I*);
static SEXP get_double(I,I*,I*);
static SEXP get_complex(I,I*,I*);
static SEXP get_boxed(I,I*,I*);
static SEXP get_notyet(I,I);
static SEXP rank_notyet(I,I);

static C* box2char(B);
static SEXP box2sexp(B);
static char *jtypename(int);
static void setdimR(SEXP,I,I*);

// ---------------------------------------------------------------------
SEXP j2sexp(I t,I r,I*s,I*d)
{
// *INDENT-OFF*
  switch (t) {
  case 1 : return get_bool(r,s,d);
  case 2 : return get_char(r,s,d);
  case 4 : return get_integer(r,s,d);
  case 8 : return get_double(r,s,d);
  case 16 : return get_complex(r,s,d);
  case 32 : return get_boxed(r,s,d);
  default: return get_notyet(t,r);
  }
// *INDENT-ON*
}

// ---------------------------------------------------------------------
C* box2char(B b)
{
  return (C*)((b->s) + (I)(b->r));
}

// ---------------------------------------------------------------------
SEXP box2sexp(B b)
{
  I t=b->t;
  I r=(I)b->r;
  I* s=b->s;
  I* d=s+r;
  return j2sexp(t,r,s,d);
}

// ---------------------------------------------------------------------
SEXP get_bool(I r,I*s,I*d)
{
  SEXP n;
  int len=1;
  char *p=(char *)d;
  DO(r,len*=s[i]);
  PROTECT(n = NEW_LOGICAL(len));
  DO(len,LOGICAL_POINTER(n)[i] = p[i]);
  setdimR(n, r, s);
  UNPROTECT(1);
  return n;
}

// ---------------------------------------------------------------------
// get boxed list
// if all character return R character, else return R list
SEXP get_boxed(I r,I*s,I*d)
{
  SEXP n;
  if (r>1) return rank_notyet(32,r);
  B* b=(B*)d;
  I len=(r==0) ? 1 : s[0];
  int ifvec=0;
  DO(len,
     ifvec=2 != b[i]->t;
     if (ifvec) break;);
  if (ifvec) {
    PROTECT(n = NEW_LIST(len));
    DO(len,SET_VECTOR_ELT(n,i,box2sexp(b[i])));
  } else {
    PROTECT(n = allocVector(STRSXP, len));
    DO(len,
       char *buffer=strdup(box2char(b[i]));
       SET_STRING_ELT(n,i,mkChar(buffer));
       free(buffer););
  }
  UNPROTECT(1);
  return n;
}

// ---------------------------------------------------------------------
SEXP get_char(I r,I*s,I*d)
{
  if (r>1) return rank_notyet(2,r);
  return string2sexp((char*)(d));
}

// ---------------------------------------------------------------------
SEXP get_complex(I r,I*s,I*d)
{
  SEXP n;
  int len=1;
  double* p=(double*) d;
  DO(r,len*=s[i]);
  PROTECT(n = NEW_NUMERIC(len));
  DO(len,NUMERIC_POINTER(n)[i] = p[i]);
  setdimR(n, r, s);
  UNPROTECT(1);
  return n;
}

// ---------------------------------------------------------------------
SEXP get_double(I r,I*s,I*d)
{
  SEXP n;
  int len=1;
  double* p=(double*) d;
  DO(r,len*=s[i]);
  PROTECT(n = NEW_NUMERIC(len));
  DO(len,NUMERIC_POINTER(n)[i] = p[i]);
  setdimR(n, r, s);
  UNPROTECT(1);
  return n;
}

// ---------------------------------------------------------------------
// set integer
SEXP get_integer(I r,I*s,I*d)
{
  SEXP n;
  int len=1;
  DO(r,len*=s[i]);
  PROTECT(n = NEW_INTEGER(len));
  DO(len,INTEGER_POINTER(n)[i] = (int) d[i]);
  setdimR(n, r, s);
  UNPROTECT(1);
  return n;
}

// ---------------------------------------------------------------------
SEXP get_notyet(I t,I r)
{
  C* p=calloc(100,1);
  strcat(p,"J datatype not yet supported: ");
  strcat(p,jtypename(t));
  SEXP m=string2sexp(p);
  free(p);
  return m;
}

// ---------------------------------------------------------------------
SEXP rank_notyet(I t,I r)
{
  C* p=calloc(100,1);
  strcat(p,"rank greater than ");
  sprintf(p+strlen(p),"%lld",r);
  strcat(p," not supported for ");
  strcat(p,jtypename(t));
  strcat(p," data: ");
  sprintf(p+strlen(p),"%lld",r);
  SEXP m=string2sexp(p);
  free(p);
  return m;
}

// ---------------------------------------------------------------------
void setdimR(SEXP n, I r, I*s)
{
  if (2>r) return;
  SEXP d;
  PROTECT(d = NEW_INTEGER(r));
  DO(r,INTEGER_POINTER(d)[i] = (int) s[i]);
  SET_DIM(n,d);
  UNPROTECT(1);
}

// ---------------------------------------------------------------------
char *jtypename(int type)
{
// *INDENT-OFF*
  switch (type) {
  case 1 : return "boolean";
  case 2 : return "literal";
  case 4 : return "integer";
  case 8 : return "floating";
  case 16 : return "complex";
  case 32 : return "boxed";
  case 64 : return "extended";
  case 128 : return "rational";
  case 1024 : return "sparse boolean";
  case 2048 : return "sparse literal";
  case 4096 : return "sparse integer";
  case 8192 : return "sparse floating";
  case 16384 : return "sparse complex";
  case 32768 : return "sparse boxed";
  case 65536 : return "symbol";
  case 131072 : return "unicode";
  case 262144 : return "unicode4";
	default : return "unknown";
  }
// *INDENT-ON*
}
