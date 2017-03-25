CC = gcc
CFLAGS = -std=gnu11                         #-Wall #-pedantic
BIN_NAME = zad1


_LIB_OBJ = generator.o
LIB_OBJ = $(patsubst %,obj/%,$(_LIB_OBJ))
LIB_NAME = generator


all: run


$(LIB_NAME): $(LIB_OBJ)
	@mkdir -p lib
	ar rcs lib/lib$(LIB_NAME).a $^

#$^ -right side of the : (lista zależności)


obj/%.o: %.c %.h
	@mkdir -p obj
	$(CC) -c $(CFLAGS) -o $@ $<

#-c compile or assemble the source files, but do not link
#$@ -left side of the : (nazwa celu)
#$< oznacza nazwę danego dopasowanego pliku (w tym przypadku *.c lub *.h)


test-$(LIB_NAME): $(LIB_NAME)
	@mkdir -p bin
	$(CC) $(BIN_NAME).c -o bin/$@ lib/lib$(LIB_NAME).a


run: test-$(LIB_NAME)
	./bin/test-$(LIB_NAME)


file: test-$(LIB_NAME)
	./bin/test-$(LIB_NAME) > wyniki.txt

clean:
	@find \( -name "*.o" -o\
	 		 -name "*.a" -o\
			 -name "test1.txt" -o\
			 -name "test2.txt" -o\
			 -name "test-$(LIB_NAME)"\
		  \)\
		  -exec rm -f {} \;
	@rm -rf bin
	@rm -rf lib
	@rm -rf obj

#-o OR, domyślnie -a AND

.PHONY: all clean run test-$(LIB_NAME) $(LIB_NAME)