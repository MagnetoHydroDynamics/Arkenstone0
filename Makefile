.PHONY: release clean debug

CC_=gcc
CFLAGS=-Werror -Wall -Wextra -pedantic -std=c11
INCLDIR=./incl/
LINKDIR=./lib/
CC=$(CC_) $(CFLAGS) -I$(INCLDIR) -L$(LINKDIR) 
AR=ar rcs

release: ./obj/main.o all_libs ./lib/libnodebug.a
	$(CC) -o ./bin/main ./obj/main.o -lnodebug -lutil -lmips

debug: ./obj/main.o all_libs ./lib/libdebug.a
	$(CC) -o ./bin/main ./obj/main.o -ldebug -lutil -lmips

all_libs: ./lib/libutil.a ./lib/libmips.a

clean:
	rm -f ./obj/*.o
	rm -f ./lib/*.a
	rm -f ./bin/*

# LIBS

./lib/libnodebug.a: ./obj/nodebug.o
	$(AR) ./lib/libnodebug.a ./obj/nodebug.o

./lib/libdebug.a: ./obj/debug.o
	$(AR) ./lib/libdebug.a ./obj/debug.o

./lib/libmips.a: ./obj/sim.o ./obj/elf.o ./obj/inst.o
	$(AR) ./lib/libmips.a ./obj/sim.o ./obj/elf.o ./obj/inst.o

./lib/libutil.a: ./obj/utils.o ./obj/ioutils.o
	$(AR) ./lib/libutil.a ./obj/utils.o ./obj/ioutils.o

# OBJECT FILES

./obj/main.o: ./src/main.c
	$(CC) -c -o ./obj/main.o ./src/main.c

./obj/debug.o: ./src/debug.c
	$(CC) -c -o ./obj/debug.o ./src/debug.c

./obj/nodebug.o: ./src/nodebug.c
	$(CC) -c -o ./obj/nodebug.o ./src/nodebug.c

./obj/sim.o: ./src/sim.c
	 $(CC) -c -o ./obj/sim.o ./src/sim.c

./obj/utils.o: ./src/utils.c
	$(CC) -c -o ./obj/utils.o ./src/utils.c

./obj/ioutils.o: ./src/ioutils.c
	$(CC) -c -o ./obj/ioutils.o ./src/ioutils.c

./obj/elf.o: ./src/elf.c
	$(CC_) $(CFLAGS) -I$(INCLDIR) -c -o ./obj/elf.o ./src/elf.c

./obj/inst.o: ./src/inst.c
	$(CC) -c -o ./obj/inst.o ./src/inst.c

# SOURCE DEPENDENCIES

./src/debug.c: ./incl/debug.h

./src/elf.c: ./incl/elf.h

./src/inst.c: ./incl/inst.h

./src/ioutils.c: ./incl/ioutils.h ./incl/utils.h

./src/nodebug.c: ./incl/debug.h

./src/sim.c: ./incl/sim.h

./src/utils.c: ./incl/utils.h
./src/utils.h: ./incl/utils.c
