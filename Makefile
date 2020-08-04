CPUC      = gcc
CFLAGS_SEQ    = -O3 --std=c11 -m64	 #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
CFLAGS_PARALLEL    = -fopenmp -Ofast --std=c11 -m64	 #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS      =
INCLUDES  =
LIBDIR   = -L/usr/lib
LIBS     = -lm
LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE_C  = $(CPUC) $(DEFS) $(INCLUDES) $(CFLAGS)

all:
	$(CPU_COMPILE_C) main-gol.c  $(LINK) -o gol $(CFLAGS_SEQ)
	$(CPU_COMPILE_C) main-gol-parallel.c $(LINK) -o gol-parallel $(CFLAGS_PARALLEL)

clean:
	rm gol
	rm gol-parallel
