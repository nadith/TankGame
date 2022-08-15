CC = gcc
CFLAGS = -Wall -pedantic -ansi -g
OBJ = main.o envinit.o gameops.o map.o newSleep.o util.o validate.o
EXEC = TankGame

# Add DEBUG to the CFLAGS and recompile the program
ifdef DEBUG_PRINT
CFLAGS += -D DEBUG # appends "-D DEBUG" to the exsting flags defined in CFLAGS
endif

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

main.o : main.c map.h util.h macros.h envinit.h gameops.h
	$(CC) -c main.c $(CFLAGS)

envinit.o : envinit.c map.h macros.h validate.h
	$(CC) -c envinit.c $(CFLAGS)

gameops.o : gameops.c map.h util.h macros.h gameops.h validate.h
	$(CC) -c gameops.c $(CFLAGS)

map.o : map.c map.h util.h macros.h newSleep.h
	$(CC) -c map.c $(CFLAGS)

newSleep.o : newSleep.c newSleep.h
	$(CC) -c newSleep.c $(CFLAGS)

util.o : util.c util.h macros.h
	$(CC) -c util.c $(CFLAGS)

validate.o : validate.c util.h macros.h
	$(CC) -c validate.c $(CFLAGS)

clean :
	rm -f $(EXEC) $(OBJ)