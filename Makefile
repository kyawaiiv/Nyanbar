PROG	= nyanbar
CC	= gcc
PREFIX ?= /usr/bin

LIBS	= `pkg-config --libs gtk+-3.0 cairo pango`
CFLAGS	= `pkg-config --cflags gtk+-3.0 cairo pango`

LIBS   += -I./ -I/applets

SOURCES	= nyanbar-main.c \
	  nyanbar.c \
	  nyanbar-applet.c \
	  applets/clock-applet.c \
	  applets/desktops-applet.c

${PROG}: ${PROG}.c
	@${CC} ${SOURCES} ${CFLAGS} ${LIBS} -o ${PROG}

install:
	install ${PROG} ${PREFIX}/${PROG}

uninstall:
	rm -f ${PREFIX}/${PROG}

clean:
	rm -f ${PROG}
