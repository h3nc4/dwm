# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.c dwm.c util.c
OBJ = ${SRC:.c=.o}

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

dist.built: all
	mkdir -p dwm-$(VERSION)
	cp dwm dwm.1 dwm-$(VERSION)
	printf '#!/bin/sh\nset -e\n' >dwm-$(VERSION)/install
	echo 'install -Dm755 dwm ${PREFIX}/bin/dwm' >>dwm-$(VERSION)/install
	echo 'install -Dm644 dwm.1 ${MANPREFIX}/man1/dwm.1' >>dwm-$(VERSION)/install
	echo 'sed -i "s/VERSION/$(VERSION)/g" ${MANPREFIX}/man1/dwm.1' >>dwm-$(VERSION)/install
	chmod +x dwm-$(VERSION)/install
	tar czf dwm.tar.gz dwm-$(VERSION)
	rm -rf dwm-${VERSION}

install: all
	install -Dm755 dwm ${DESTDIR}${PREFIX}/bin/dwm
	install -Dm644 dwm.1 ${DESTDIR}${MANPREFIX}/man1/dwm.1
	sed -i "s/VERSION/${VERSION}/g" ${DESTDIR}${MANPREFIX}/man1/dwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm\
		${DESTDIR}${MANPREFIX}/man1/dwm.1

.PHONY: all clean dist install uninstall dist.built
