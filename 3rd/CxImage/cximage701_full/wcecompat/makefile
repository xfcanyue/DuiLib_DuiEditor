!INCLUDE <wcedefs.mak>

CFLAGS=/W3 /WX /Ox /O2 /Ob2 /GF /Gy /nologo $(WCETARGETDEFS) -DWIN32_PLATFORM_PSPC -DUNICODE -D_UNICODE -DWIN32 -DWIN32_LEAN_AND_MEAN -Iinclude -D_WINDLL -D_DLL /Foobj/ -D_MSC_VER=1200

SRC =							\
	src/args.cpp				\
	src/assert.cpp				\
	src/ChildData.cpp			\
	src/env.cpp					\
	src/errno.cpp				\
	src/io.cpp					\
	src/pipe.cpp				\
	src/process.cpp				\
	src/redir.cpp				\
	src/stat.cpp				\
	src/stdio_extras.cpp		\
	src/stdlib_extras.cpp		\
	src/string_extras.cpp		\
	src/time.cpp				\
	src/timeb.cpp				\
	src/ts_string.cpp			\
	src/winmain.cpp				\
	src/winsock_extras.cpp

!IF "$(WCEVERSION)"=="211"
SRC =							\
	$(SRC)						\
	src/wce211_ctype.c			\
	src/wce211_string.c
!ENDIF

OBJS = $(SRC:src=obj)
OBJS = $(OBJS:.cpp=.obj)
OBJS = $(OBJS:.c=.obj)

{src}.c{obj}.obj:
	$(CC) $(CFLAGS) -c $<

{src}.cpp{obj}.obj:
	$(CC) $(CFLAGS) -c $<

all: lib\wcecompat.lib lib\wcecompatex.lib
#	echo $(OBJS)

obj:
	@md obj 2> NUL

lib:
	@md lib 2> NUL

$(OBJS): makefile obj

lib\wcecompat.lib: lib $(OBJS) makefile
	@lib /nologo /out:lib\wcecompat.lib $(OBJS)

lib\wcecompatex.lib: lib $(OBJS) makefile
	@lib /nologo /out:lib\wcecompatex.lib $(OBJS)
