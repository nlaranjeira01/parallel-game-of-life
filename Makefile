CPUC      = gcc
CFLAGS    = -O3 --std=c11 -m64	 #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS      =
INCLUDES  =
LIBDIR   = -L/usr/lib
LIBS     = -lm
LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE_C  = $(CPUC) $(DEFS) $(INCLUDES) $(CFLAGS)

all:
	$(CPU_COMPILE_C) main-gol.c  $(LINK) -o gol
	$(CPU_COMPILE_C) main-gol-parallel.c $(LINK) -o gol-parallel -fopenmp

clean:
	rm gol
