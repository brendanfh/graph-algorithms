CC=gcc
OBJ_FILES=$(shell find ./modules -name '*.c' | grep -Eo '^\.[^\.]*' | xargs printf '%s.o\n')
FLAGS=
LIBS=
TARGET=./graphs.so

%.o: %.c
	$(CC) -c -fPIC -o $@ $< $(FLAGS) $(INCLUDES)

compile: $(OBJ_FILES)
link: compile
	$(CC) -shared -o $(TARGET) $(OBJ_FILES) $(FLAGS) $(LIBS)

$(TARGET): link
all: $(TARGET)

clean:
	(rm $(OBJ_FILES) 2>/dev/null) || true
