.PHONY: all clean
.DEFAULT: all

CFLAGS = -Wall -Werror -pedantic -g
CC = gcc

MKPLAYDOHFS_OBJS=mkplaydohfs.o
MKPLAYDOHFS_DEPS=$(MKPLAYDOHFS_OBJS:.o=.d)

all: mkplaydohfs

mkplaydohfs: $(MKPLAYDOHFS_OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

%.d: %.c
	$(CC) -MM $< > $@
	$(CC) -MM $< | sed s/\\.o/.d/ >> $@

clean:
	-rm $(MKPLAYDOHFS_OBJS) $(MKPLAYDOHFS_DEPS) mkplaydohfs

-include $(MKPLAYDOHFS_DEPS)

# vim: set noexpandtab:
