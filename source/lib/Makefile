# linux/osx jserver4r makefile
#
# ensure libj.so is at least 807 beta-b
# and is on the path (or add the path to LIBS)

UNAME=$(shell uname -s)

ifeq ($(UNAME),Darwin)
  R_HOME=/Library/Frameworks/R.framework/Resources
  R_SHARE=$(R_HOME)
	CFLAGS=-O -fPIC -dynamiclib -undefined dynamic_lookup
	TGT=libjserver4r.dylib
else
	R_HOME=/usr/lib/R
	R_SHARE=/usr/share/R
	CFLAGS=-O -fPIC -shared
  TGT=libjserver4r.so
endif

CC=gcc
RCMD=$(R_HOME)/bin/R CMD
INCLUDES=-I. -I$(R_SHARE)/include
LIBS=-L$(R_HOME)/lib -lR -lj

SRC=$(wildcard *.c)

all:
	$(RCMD) $(CC) $(CFLAGS) $(INCLUDES) $(SRC) $(LIBS) -o $(TGT)

clean:
	rm -rf *.o *.so *.dylib
