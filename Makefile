EXTRAS = Makefile LICENSE README.md

HEADERS = include/test.h include/macros.h tests/dog.h

SOURCES = tests/dog.c

OBJECTS = $(SOURCES:tests/%.c=obj/%.o)

CC = gcc
DBG = -g -Wall -fstack-protector-all -pedantic
#OPT = -march=native -O3 -DNDEBUG -ffast-math -fomit-frame-pointer
INCLUDES = -Iinclude
CFLAGS = -std=gnu99 $(DBG) $(OPT) $(INCLUDES)

all: depend tests/dog

depend: .depend
.depend: $(SOURCES)
	@echo "Building dependencies" 
ifneq ($(wildcard ./.depend),)
	@rm -f "./.depend"
endif
	@$(CC) $(CFLAGS) -MM $^ > .depend
# Make .depend use the 'obj' directory
	@sed -i.bak -e :a -e '/\\$$/N; s/\\\n//; ta' .depend
	@sed -i.bak 's/^/obj\//' .depend
	@rm -f .depend.bak
-include .depend

$(OBJECTS): obj/%.o : src/%.c Makefile
	@echo "Compiling "$<""
	@[ -d obj ] || mkdir -p obj
	@$(CC) $(CFLAGS) -c $< -o $@

tests/dog: tests/dog.c
	$(CC) $(CFLAGS) tests/dog.c -o tests/dog

clean:
	rm -rf *~ */*~ $(OBJECTS) ./.depend tests/dog

edit:
	emacs -nw $(EXTRAS) $(HEADERS) $(SOURCES)