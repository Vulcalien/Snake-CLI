ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

# directories
EXEC_PREFIX := $(PREFIX)
BINDIR      := $(EXEC_PREFIX)/bin

# install command
INSTALL         := install -D
INSTALL_PROGRAM := $(INSTALL)
INSTALL_DATA    := $(INSTALL) -m 664

# remove command
REMOVE      := rm -fv
REMOVE_FILE := $(REMOVE)

.PHONY: install uninstall

install: build-deps build
	$(INSTALL_PROGRAM) bin/vulcalien-snake\
		$(DESTDIR)$(BINDIR)/vulcalien-snake

uninstall:
	$(REMOVE_FILE) $(DESTDIR)$(BINDIR)/vulcalien-snake
