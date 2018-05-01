/* base.c */

#define PLEN 1000 // path length
#ifdef _WIN32
#include <windows.h>
#define GETPROCADDRESS(h,p) GetProcAddress(h,p)
#else
#include <unistd.h>
#include <dlfcn.h>
#define GETPROCADDRESS(h,p)	dlsym(h,p)
#endif

#include "base.h"

static SEXP rjget1(C* n);
static SEXP rjdo1(C* s);

static void* hjdll;
static J jt=0;
static JDoType JDo;
static JGetRType JGetR;
static JGetMType JGetM;
static JSetAType JSetA;

// ---------------------------------------------------------------------
// load JE, Jinit, getprocaddresses
static J jeload(C* pathdll)
{
#ifdef _WIN32
  WCHAR wpath[PLEN];
  MultiByteToWideChar(CP_UTF8,0,pathdll,1+(int)strlen(pathdll),wpath,PLEN);
  hjdll=LoadLibraryW(wpath);
#else
  hjdll=dlopen(pathdll,RTLD_LAZY);
#endif
  if (!hjdll)return 0;
  jt=((JInitType)GETPROCADDRESS(hjdll,"JInit"))();
  if (!jt) return 0;
  JDo=(JDoType)GETPROCADDRESS(hjdll,"JDo");
  JGetR=(JGetRType)GETPROCADDRESS(hjdll,"JGetR");
  JGetM=(JGetMType)GETPROCADDRESS(hjdll,"JGetM");
  JSetA=(JSetAType)GETPROCADDRESS(hjdll,"JSetA");
  return jt;
}

// ---------------------------------------------------------------------
// do sentence, return result code
SEXP rjdo(SEXP s)
{
  return rjdo1((C*) CHARACTER_VALUE(s));
}

// ---------------------------------------------------------------------
// execute sentence, return result code
SEXP rjdo1(C* s)
{
  return int2sexp(JDo(jt,s));
}

// ---------------------------------------------------------------------
// execute sentence, return result
SEXP rjexec(SEXP s)
{
  C* t=(C*) CHARACTER_VALUE(s);
  C* p="output_jrx_=:";
  C* m=malloc(1+strlen(p)+strlen(t));
  strcpy(m,p);
  strcpy(m+strlen(p),t);
  JDo(jt,m);
  free(m);
  return rjget1("output_jrx_");
}

// ---------------------------------------------------------------------
// get J noun
SEXP rjget(SEXP x)
{
  return rjget1((C*) CHARACTER_VALUE(x));
}

// ---------------------------------------------------------------------
// get J noun
SEXP rjget1(C* n)
{
  I t, r, s, d;
  int rc= JGetM(jt,n,&t,&r,&s,&d);
  return j2sexp(t,r,(I*)s,(I*)d);
}

// ---------------------------------------------------------------------
// initialize J, run profile if given, return 0=OK, or J error code
SEXP rjinit(SEXP pathbin, SEXP pathdll, SEXP pathpro, SEXP loadprofile)
{
  C* bin=(C*) CHARACTER_VALUE(pathbin);
  C* dll=(C*) CHARACTER_VALUE(pathdll);
  C* pro=(C*) CHARACTER_VALUE(pathpro);
  int ldp= LOGICAL_VALUE(loadprofile);
  if (!jt) jt=jeload(dll);
  if (!jt) return int2sexp(20);  // EVSYSTEM  "system error"
  if (!ldp || !strlen(pro)) return int2sexp(0);
  C* b=calloc(2000,1);
  strcat(b,"(3 : '0!:0 y')<'");
  strcat(b,pro);
  strcat(b,"'[BINPATH_z_=:'");
  strcat(b,bin);
  strcat(b,"'[LIBFILE_z_=:'");
  strcat(b,dll);
  strcat(b,"'[ARGV_z_=:''");
  SEXP r=rjdo1(b);
  free(b);
  return r;
}

// ---------------------------------------------------------------------
// set J noun from R noun, return result code
SEXP rjset(SEXP n,SEXP x)
{
  A a = sexp2j(x);
  C* s=(C*) CHARACTER_VALUE(n);
  int rc=JSetA(jt,strlen(s),s,alen(a),(C*) a);
  return int2sexp(rc);
}

// ---------------------------------------------------------------------
// get last output
SEXP rjgetr()
{
  return string2sexp(JGetR(jt));
}
