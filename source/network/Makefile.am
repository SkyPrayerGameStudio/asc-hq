
MAINTAINERCLEANFILES	= Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/source/libs/paragui/include 

AM_CXXFLAGS		= @SDL_CFLAGS@ @SIGC_CFLAGS@ @LIBCURL_CPPFLAGS@  @LokiInclude@ -Dsgmain 

noinst_LTLIBRARIES      = libnetwork.la 

libnetwork_la_SOURCES	= simple_file_transfer.cpp
libnetwork_la_LIBADD		= @SDL_LIBS@ -lpng -lz 

if CURLCLIENT
libnetwork_la_SOURCES	+= pbem-server.cpp pbem-server-interaction.cpp
libnetwork_la_LIBADD		+= @LIBCURL@
else
libnetwork_la_SOURCES	+= pbem-server-dummy.cpp
endif

EXTRA_DIST=$(srcdir)/*.h  