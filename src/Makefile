
CC:=g++
CC_FLAGS:=-std=c++1z -Wall -Wextra -pedantic -g
LD_FLAGS:=-g
LDLIBS:=
RM:=rm -f

SRCS:=main.cpp injector.cpp
OBJS:=$(subst .cpp,.o,$(SRCS))

all: elfector

elfector: $(OBJS)
	$(CC) $(LD_FLAGS) -o elfector $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CC) $(CC_FLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
