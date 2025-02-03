INC := ./raylib-5.5_linux_amd64/include
LIBS := ./raylib-5.5_linux_amd64/lib

CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -Werror $(addprefix -I, $(INC))
LDFLAGS := -l:libraylib.a $(addprefix -L, $(LIBS))

EXE := space

SRCS := $(wildcard ./*.c)
OBJS := $(SRCS:%.c=%.o)

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean all

clean:
	$(RM) -v ./*.o $(EXE)
