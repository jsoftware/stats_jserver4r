/* r2j.c */

#include "base.h"

static int get_logical(int);
static int get_rank(SEXP s);
static double jna(double);
static A mallocA(I);
static A mallocB(I,I);
static char *type2name(int type);

static A set_char(SEXP);
static A set_character(SEXP);
static A set_double(SEXP);
static A set_complex(SEXP);
static A set_integer(SEXP);
static A set_logical(SEXP);
static A set_vector(SEXP);
static A set_notyet(SEXP);

// ---------------------------------------------------------------------
int get_logical(int i)
{
  if (i==0 || i==1) return i;
  return 2;
}

// ---------------------------------------------------------------------
// get J rank (setting singleton list to rank 0)
int get_rank(SEXP s)
{
  SEXP dim = GET_DIM(s);
  int r = isNull(dim) ? 1 : LENGTH(dim);
  return (r==1 && LENGTH(s)==1) ? 0 : r;
}

// ---------------------------------------------------------------------
// J NA is -infinity
double jna(double s)
{
  return ISNA(s) ? R_NegInf : s;
}

// ---------------------------------------------------------------------
// A from character string
A jstring(char *s)
{
  I len=strlen(s);
  I wid=40+8*(1+len/8);
  A r=mallocA(wid);
  r->t=2;
  r->c=len;
  r->r=1;
  r->s[0]=len;
  strcpy((char *)(r->s+1),s);
  return r;
}

// ---------------------------------------------------------------------
// malloc A structure
A mallocA(I len)
{
  A r=(A)malloc(len);
  r->f=227;
  return r;
}

// ---------------------------------------------------------------------
// malloc boxed list
A mallocB(I len, I count)
{
  A r=(A)mallocA(len);
  r->t=32;
  r->c=count;
  r->r=1;
  r->s[0]=count;
  return r;
}

// ---------------------------------------------------------------------
A sexp2j(SEXP s)
{
  int type = TYPEOF(s);
// printf("sexp2j type=%s\n",type2name(type));
// *INDENT-OFF*
  switch (type) {
  case CHARSXP : return set_char(s);
  case INTSXP : return set_integer(s);
  case LGLSXP : return set_logical(s);
  case REALSXP : return set_double(s);
  case STRSXP : return set_character(s);
  //case VECSXP : return set_vector(s);
	default: return set_notyet(s);
  }
// *INDENT-ON*
}

// ---------------------------------------------------------------------
A set_notyet(SEXP s)
{
  char *t=type2name(TYPEOF(s));
  char *v=malloc(10+strlen(t));
  strcpy(v,"not yet: ");
  strcat(v,t);
  return jstring(v);
}

// ---------------------------------------------------------------------
A set_char(SEXP s)
{
  return jstring((char*) CHAR(STRING_ELT(s,0)));
}

// ---------------------------------------------------------------------
// single string or list of strings
A set_character(SEXP s)
{
  A r,t;
  I i, p, w, len = LENGTH(s);
  if (len == 1)
    r=jstring((char*) CHAR(STRING_ELT(s,0)));
  else {
    p=40+48*len;;
    DO(len,p+=8*(1+strlen((char*) CHAR(STRING_ELT(s,i)))/8));
    r=mallocB(p,len);
    p=40+8*len;
    DO(len,
       r->s[i+1]=p;
       t=jstring((char*) CHAR(STRING_ELT(s,i)));
       w=alen(t);
       memcpy((char*)r+p,t,w);
       p+=w;
       free(t););
  }
  return r;
}

// ---------------------------------------------------------------------
A set_double(SEXP s)
{
  int len = LENGTH(s);
  int rnk = get_rank(s);
  int p=0;
  I wid=32+8*(rnk + len);
  A r=mallocA(wid);
  r->t=8;
  r->c=len;
  r->r=rnk;
  if (rnk==1)
    r->s[p++]=len;
  else if (rnk>1) {
    SEXP dim=GET_DIM(s);
    DO(rnk,r->s[p++]=INTEGER_POINTER(dim)[i]);
  }
  double *d=(double*)r->s+p;
  DO(len,d[i]=jna(REAL(s)[i]));
  return r;
}

// ---------------------------------------------------------------------
A set_integer(SEXP s)
{
  int len = LENGTH(s);
  int rnk = get_rank(s);
  int p=0;
  I wid=32+8*(rnk + len);
  A r=mallocA(wid);
  r->t=4;
  r->c=len;
  r->r=rnk;
  if (rnk==1)
    r->s[p++]=len;
  else if (rnk>1) {
    SEXP dim=GET_DIM(s);
    DO(rnk,r->s[p++]=INTEGER_POINTER(dim)[i]);
  }
  DO(len,r->s[p+i]=INTEGER_POINTER(s)[i]);
  return r;
}

// ---------------------------------------------------------------------
// set boolean if possible, otherwise integer
A set_logical(SEXP s)
{
  I len = LENGTH(s);
  I rnk = get_rank(s);
  SEXP dim=GET_DIM(s);
  I p=0;
  int ifint=0;
  DO(len,
     ifint=NA_LOGICAL==(LOGICAL_POINTER(s)[i]);
     if (ifint) break;);
  I wid=32+8*(rnk + (ifint ? len : (1 + len/8)));
  A r=mallocA(wid);
  r->t=ifint ? 4 : 1;
  r->c=len;
  r->r=rnk;
  if (rnk==1)
    r->s[p++]=len;
  else if (rnk>1) {
    DO(rnk,r->s[p++]=INTEGER_POINTER(dim)[i]);
  }
  if (ifint) {
    DO(len,r->s[p+i]=get_logical((int)LOGICAL_POINTER(s)[i]));
  } else {
    char *t = (char *)(r->s+p);
    DO(len,t[i]=(char)LOGICAL_POINTER(s)[i]);
  }
  return r;
}

// ---------------------------------------------------------------------
A set_vector(SEXP s)
{
  return (A) 0;
}

// ---------------------------------------------------------------------
char *type2name(int type)
{
// *INDENT-OFF*
  switch (type) {
  case ANYSXP : return "any";
  case BUILTINSXP : return "builtin";
  case CHARSXP : return "char";
  case CLOSXP : return "closure";
  case CPLXSXP : return "complex";
  case DOTSXP : return "...";
  case ENVSXP : return "environment";
  case EXPRSXP : return "expression";
  case INTSXP : return "integer";
  case LANGSXP : return "language";
  case LGLSXP : return "logical";
  case LISTSXP : return "pairlist";
  case NILSXP : return "NULL";
  case PROMSXP : return "promise";
  case REALSXP : return "double";
  case SPECIALSXP : return "special";
  case STRSXP : return "character";
  case SYMSXP : return "symbol";
  case VECSXP : return "vector";
	default : return "unknown";
  }
// *INDENT-ON*
}
