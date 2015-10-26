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
	rm -f ./mips/*.o
	rm -f ./mips/*.elf

# LIBS

./lib/libnodebug.a: ./obj/nodebug.o
	$(AR) ./lib/libnodebug.a ./obj/nodebug.o

./lib/libdebug.a: ./obj/debug.o
	$(AR) ./lib/libdebug.a ./obj/debug.o

./lib/libmips.a: ./obj/sim.o ./obj/elf.o ./obj/mod.o ./obj/mem.o
	$(AR) ./lib/libmips.a ./obj/sim.o ./obj/elf.o ./obj/mod.o ./obj/mem.o

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

./obj/mod.o: ./src/mod.c
	$(CC) -c -o ./obj/mod.o ./src/mod.c

./obj/mem.o: ./src/mem.c
	$(CC) -c -o ./obj/mem.o ./src/mem.c

./obj/utils.o: ./src/utils.c
	$(CC) -c -o ./obj/utils.o ./src/utils.c

./obj/ioutils.o: ./src/ioutils.c
	$(CC) -c -o ./obj/ioutils.o ./src/ioutils.c

./obj/elf.o: ./src/elf.c
	$(CC_) $(CFLAGS) -I$(INCLDIR) -c -o ./obj/elf.o ./src/elf.c
