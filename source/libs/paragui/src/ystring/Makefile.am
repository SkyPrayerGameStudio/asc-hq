noinst_LTLIBRARIES = libystring.la

libystring_la_SOURCES = \
	ychar.cpp \
	ystring.cpp

EXTRA_DIST =

AM_CPPFLAGS = \
	$(SDL_CFLAGS) \
	-I$(top_srcdir)/source/libs/paragui/include

style_cvs:
	astyle --style=kr --indent=tab=4 --indent-switches *.cpp

style_personal:
	astyle *.cpp
