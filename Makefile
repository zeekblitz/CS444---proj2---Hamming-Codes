CC=cc # CC is the C compiler to use
CFLAGS=-Wall -O2 # CFLAGS is the list of flags to pass to the compilation command

all: raid diar # command that tells the compiler which files need to be compiled and what to name them

%: %.c # tells the compiler to look for .c files to compile. the -o flag says to name the program the same as its c file.
	$(CC) $(CFLAGS) -o $@ $< 

clean: # this removes any of the partition files or txt.2 or a.out with the command 'make clean'
	rm -f a.out *.part? *.2