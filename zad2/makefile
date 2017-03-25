CC = gcc
CFLAGS = -std=gnu11                         #-Wall #-pedantic
BIN_NAME = zad2


all: $(BIN_NAME)


$(BIN_NAME): $(BIN_NAME).c
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ $^

#-o write output to file
#$@ -left side of the : (nazwa celu)
#$^ -right side of the : (lista zależności)


clean:
	@find \( -name "$(BIN_NAME)"\
		  \)\
		  -exec rm -f {} \;
	@rm -rf bin

.PHONY: all clean