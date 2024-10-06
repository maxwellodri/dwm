# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.c dwm.c util.c
OBJ = ${SRC:.c=.o}

# Default value for dotfile_tag if not provided
ifeq ($(shell id -u), 0)
    ifeq ($(shell test -e /etc/dotfile_tag && echo 1 || echo 0), 1)
    dotfile_tag := $(shell cat /etc/dotfile_tag)
else
    $(error /etc/dotfile_tag needs to be created. e.g. 'echo pc > /etc/dotfile_tag')
endif
else
    dotfile_tag ?= ""
endif
$(info dotfile_tag is '$(dotfile_tag)')

# Check the value of dotfile_tag and set the corresponding flags
#CFLAGS += -DDOTFILE_TAG_PC
ifeq ($(dotfile_tag),pc)
    CFLAGS += -DDOTFILE_TAG_PC
endif
ifeq ($(dotfile_tag),hackerman)
    CFLAGS += -DDOTFILE_TAG_HACKERMAN
endif

all: dwm

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

config.h:
	cp config.def.h $@

dwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f dwm ${OBJ} dwm-${VERSION}.tar.gz

dist: clean
	mkdir -p dwm-${VERSION}
	cp -R LICENSE Makefile README config.def.h config.mk\
		dwm.1 drw.h util.h ${SRC} dwm.png transient.c dwm-${VERSION}
	tar -cf dwm-${VERSION}.tar dwm-${VERSION}
	gzip dwm-${VERSION}.tar
	rm -rf dwm-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dwm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < dwm.1 > ${DESTDIR}${MANPREFIX}/man1/dwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm\
		${DESTDIR}${MANPREFIX}/man1/dwm.1

.PHONY: all clean dist install uninstall
