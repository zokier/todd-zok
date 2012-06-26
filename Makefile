BINDIR   = bin
OBJDIR   = obj
SRCDIR   = src
TARGET  ?= $(BINDIR)/todd
DAILYBOT?= $(BINDIR)/dailybot
SRCS     = $(wildcard $(SRCDIR)/*.c)
_SRCS    = $(notdir $(SRCS))
_OBJS    = $(_SRCS:.c=.o)
OBJS     = $(addprefix $(OBJDIR)/, $(_OBJS))
LIBS	 = -lzmq -lpq -lncursesw
INCPATHS = -I/usr/include/postgresql -I./include
REVID    = $(shell git describe --always --dirty=*)

CC      ?= gcc
CFLAGS ?= -Wall -Wextra -Wno-unused-parameter -g -std=gnu99 -DREVID="\"$(REVID)\""

PREFIX    = /usr/local

-include Makefile.config

all: $(BINDIR) $(OBJDIR) $(TARGET) $(DAILYBOT)

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(wildcard include/*.h) Makefile
	$(CC) $(CFLAGS) $(INCPATHS) -o $@ -c $<

# TODO: fix dailybot
$(DAILYBOT): dailybot/dailybot.c $(OBJDIR)/database.o
	gcc $(CFLAGS) $(INCPATHS) $(LIBS) -o $@ dailybot/dailybot.c $(OBJDIR)/database.o


clean:
	rm $(OBJS) $(TARGET) $(DAILYBOT) || true
	test -d obj && rmdir obj || true
	test -d bin && rmdir bin || true

install: $(TARGET) $(DAILYBOT)
	install -D $(TARGET) $(DESTDIR)$(PREFIX)/bin/
	install -D $(DAILYBOT) $(DESTDIR)$(PREFIX)/bin/
