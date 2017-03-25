CC = gcc
CFLAGS = -std=gnu11                         #-Wall #-pedantic
BIN_NAME = zad3


all: $(BIN_NAME)


$(BIN_NAME): $(BIN_NAME).c
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ $^

#-o write output to file
#$@ -left side of the : (nazwa celu)
#$^ -right side of the : (lista zależności)


test: $(BIN_NAME)
	@./bin/$(BIN_NAME) /media/sf_SysOpy/TrzebinskiMaciej/cw2/zad3/test.txt

#@ przed komendą nie wypisuje jej wyniku na konsole

clean:
	@find \( -name "results.txt" -o\
			 -name "$(BIN_NAME)"\
		  \)\
		  -exec rm -f {} \;
	@rm -rf bin

#w find -o OR, domyślnie -a AND

.PHONY: all clean test