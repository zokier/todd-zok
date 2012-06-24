BINDIR   = bin
OBJDIR   = obj
SRCDIR   = src
TARGET  ?= $(BINDIR)/todd
SRCS     = $(wildcard $(SRCDIR)/*.c)
_SRCS    = $(notdir $(SRCS))
_OBJS    = $(_SRCS:.c=.o)
OBJS     = $(addprefix $(OBJDIR)/, $(_OBJS))
LIBS	 = -lzmq -lpq -lncursesw
INCPATHS = -I/usr/include/postgresql
REVID    = $(shell git describe --always --dirty=*)

CC      ?= gcc
CFLAGS ?= -Wall -Wextra -Wno-unused-parameter -g -std=gnu99 -DREVID="\"$(REVID)\""

PREFIX    = /usr/local

-include Makefile.config

all: $(BINDIR) $(OBJDIR) $(TARGET)

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/*.h Makefile
	$(CC) $(CFLAGS) $(INCPATHS) -o $@ -c $<

# TODO: fix dailybot
DAILYBOT = dailybot
$(DAILYBOT): dailybot/dailybot.c
	gcc $(CFLAGS) $(INCPATHS) $(LIBS) -o bin/dailybot dailybot/dailybot.c src/database.c


clean:
	rm $(OBJS) $(TARGET) || true
	test -d obj && rmdir obj || true
	test -d bin && rmdir bin || true

install: $(TARGET)
	install -D $(TARGET) $(DESTDIR)$(PREFIX)/bin/
