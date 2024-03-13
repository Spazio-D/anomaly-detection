
#IDIR=../hdr
#IDIR=../../../con2redis/src
ODIR=../obj
BDIR=../bin

CC=g++
# use this when connecting to Postgresql (library lpq)
#CFLAGS=-std=c++11 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm 
CFLAGS=-std=c++20 -g -ggdb -fpermissive -L/usr/local/lib -Wall -I. -I$(IDIR) -lpq -lm -lhiredis

# use this when no connection to Postgresql (library lpq) is needed
#CFLAGS=-std=c++11 -I. -I$(IDIR) -lm 

DEPS = $(wildcard $(IDIR)/*.h)
#OBJ = $(wildcard $(ODIR)/*.o)
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))

#con2redis_OBJ=$(wildcard ../../../con2redis/obj/*.o)


#all:	con2redis $(BDIR)/main

#con2redis:
#	$(MAKE) -C ../../../con2redis/src
# Makefile principale

# Lista delle sotto-directory contenenti i Makefile da includere ed eseguire
SUBDIRS := SUD/src TestGenerator/src Monitors/AverageAnomalyPresence/src Monitors/CovarianceAnomalyValue/src Monitors/CovarianceAnomalyPresence/src Monitors/AverageAnomalyValue/src


# Target predefinito: esegui tutti i Makefile nelle sotto-directory
all:
	@for dir in $(SUBDIRS); do \
    	echo "Esecuzione di Makefile in $$dir..."; \
    	$(MAKE) -C $$dir; \
	done
	
run:
	TestGenerator/src/delRedisDB.sh
	TestGenerator/bin/main
	../ScriptsDB/create.sh
	SUD/bin/main
	Monitors/AverageAnomalyPresence/bin/main
	Monitors/CovarianceAnomalyValue/bin/main
	Monitors/CovarianceAnomalyPresence/bin/main
	Monitors/AverageAnomalyValue/bin/main

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/main: $(OBJ) $(DEPS)
#	$(CC) -o $@ $(OBJ) $(con2redis_OBJ) $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*  *~ $(IDIR)/*~ core 
