.PHONY: all clean
.DEFAULT: all

CFLAGS = -Wall -Werror -pedantic -g
CC = gcc

MKPLAYDOHFS_OBJS=mkplaydohfs.o
MKPLAYDOHFS_DEPS=$(MKPLAYDOHFS_OBJS:.o=.d)

PLAYDOHFSMODULE_OBJS=super.o
PLAYDOHFSMODULE_DEPS=$(PLAYDOHFSMODULE_OBJS:.o=.d)

TESTPLAYDOHFS_OBJS=$(PLAYDOHFSMODULE_OBJS) testplaydohfs.o
TESTPLAYDOHFS_DEPS=$(TESTPLAYDOHFS_OBJS:.o=.d)

all: mkplaydohfs testplaydohfs

mkplaydohfs: $(MKPLAYDOHFS_OBJS)
	$(CC) -o $@ $^

testplaydohfs: $(TESTPLAYDOHFS_OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

%.d: %.c
	$(CC) -MM $< > $@
	$(CC) -MM $< | sed s/\\.o/.d/ >> $@

clean:
	-rm $(MKPLAYDOHFS_OBJS) $(MKPLAYDOHFS_DEPS) $(TESTPLAYDOHFS_OBJS) $(TESTPLAYDOHFS_DEPS)
	-rm mkplaydohfs testplaydohfs

-include $(MKPLAYDOHFS_DEPS)
-include $(TESTPLAYDOHFS_DEPS)

# vim: set noexpandtab:
