# ----------------------------------------------------------------------------
# Makefile for building tapp
#
# 2015 Imed ELHADEF
#


CFLAGS				= -Wall -O2
CC				= arm-linux-gcc
INSTALL				= install

TARGET				= Cap_Button


all: $(TARGET)

Cap_Button: main.o AT42QT2160.o
	$(CC) $(CFLAGS) $^ -o $@


install: $(TARGET)
	$(INSTALL) $^ $(DESTDIR)/usr/bin

clean distclean:
	rm -rf *.o $(TARGET)


# ----------------------------------------------------------------------------

.PHONY: $(PHONY) install clean distclean

# End of file
# vim: syntax=make

