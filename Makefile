.PHONY: release clean debug

CC_=gcc
CFLAGS=-Werror -Wall -Wextra -pedantic -std=c11
INCLDIR=./src/
LINKDIR=./lib/
CC=$(CC_) $(CFLAGS) -I$(INCLDIR) -L$(LINKDIR) 
AR=ar rcs

release: ./obj/main.o ./lib/libnodebug.a
	$(CC) -o ./bin/main ./obj/main.o -lnodebug -lutilio -lutil -lsim -lelf

debug: ./obj/main.o ./lib/libdebug.a
	$(CC) -o ./bin/main ./obj/main.o -ldebug -lutilio -lutil -lsim -lelf

clean:
	rm -f ./obj/*.o
	rm -f ./lib/*.a
	rm -f ./bin/*

./lib/libnodebug.a: ./obj/nodebug.o
	$(AR) ./lib/libdebug.a ./obj/nodebug.o

./lib/libdebug.a: ./obj/debug.o
	$(AR) ./lib/libdebug.a ./obj/debug.o

./lib/libsim.a: ./obj/sim.o
	$(AR) ./lib/libsim.a ./obj/sim.o

./lib/libutil.a: ./obj/util.o
	$(AR) ./lib/libutil.a ./obj/util.o

./lib/libutilio.a: ./obj/uitlio.o
	$(AR) ./lib/libutilio.a ./obj/utilio.o

./lib/libelf.a: ./obj/elf.o
	$(AR) ./lib/libelf.a ./obj/elf.o

./obj/sim.o: ./src/sim.c
	 $(CC) -c -o ./obj/sim.o ./src/sim.c

./obj/util.o: ./src/util.c
	$(CC) -c -o ./obj/util.o ./src/util.c

./obj/utilio.o: ./src/utilio.c
	$(CC) -c -o ./obj/utilio.o ./src/utilio.c

./obj/elf.o: ./src/elf.c
	$(CC_) $(CFLAGS) -c -o ./obj/elf.o ./src/elf.c
