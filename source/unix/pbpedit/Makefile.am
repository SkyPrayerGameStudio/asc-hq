vpath %.cpp $(srcdir)/../..

MAINTAINERCLEANFILES	= Makefile.in

AM_CPPFLAGS		= -I$(top_srcdir) -I$(top_srcdir)/source -I$(top_srcdir)/source/libs/paragui/include @LokiInclude@

AM_CXXFLAGS 		= $(SDL_CFLAGS) -Dkarteneditor -Dpbpeditor $(SIGC_CFLAGS)

bin_PROGRAMS 		= asc_pbpedit

nodist_asc_pbpedit_SOURCES 	= edgen.cpp edglobal.cpp edmain.cpp \
edselfnt.cpp edmisc.cpp typen.cpp strtmesg.cpp stack.cpp \
spfst.cpp sgstream.cpp palette.cpp newfont.cpp misc.cpp loadpcxc.cpp \
loaders.cpp loadbi3.cpp dlg_box.cpp dialog.cpp basestrm.cpp basegfx.cpp \
attack.cpp gameoptions.cpp maped-mainscreen.cpp stdio-errorhandler.cpp\
buildingtype.cpp vehicletype.cpp containerbase.cpp ed_mapcomponent.cpp \
mapalgorithms.cpp viewcalculation.cpp gamemap.cpp password.cpp \
research.cpp mapdisplay.cpp ascstring.cpp graphicset.cpp \
vehicle.cpp buildings.cpp networkinterface.cpp resourcenet.cpp textfileparser.cpp \
terraintype.cpp objecttype.cpp textfiletags.cpp itemrepository.cpp \
stringtokenizer.cpp music.cpp messages.cpp paradialog.cpp \
textfile_evaluation.cpp containerbasetype.cpp simplestream.cpp \
mappolygons.cpp prehistoricevents.cpp gameevents.cpp gameeventsystem.cpp \
gameevent_dialogs.cpp clipboard.cpp resourceplacement.cpp resourceplacementdialog.cpp \
geometry.cpp guidimension.cpp unitset.cpp fieldimageloader.cpp \
iconrepository.cpp overviewmapimage.cpp mapfield.cpp objects.cpp overviewmappanel.cpp \
explosivemines.cpp legacynetworkloaders.cpp playercolor.cpp statistics.cpp \
player.cpp cannedmessages.cpp mapimageexport.cpp mainscreenwidget.cpp windowing.cpp \
campaignactionrecorder.cpp luacommandwriter.cpp versionidentifier.cpp package.cpp \
packagemanager.cpp packagerepository.cpp dataversioncheck.cpp i18n.cpp \
accessconstraints-editor.cpp unitcostcalculator-standard.cpp weaponrangelayer.cpp

asc_pbpedit_LDADD   	= $(top_builddir)/source/sdl/libsdl.la \
$(top_builddir)/source/libs/triangul/libtriangul.la $(BZIP2_LIBS) \
$(SIGC_LIBS) $(top_builddir)/source/libs/sdlmm/src/libSDLmm.la \
$(top_builddir)/source/util/libutil.la  \
$(top_builddir)/source/actions/libactions.la  \
$(top_builddir)/source/actions/libdirectunitactions.la  \
$(top_builddir)/source/tasks/libtaskcore.la \
$(top_builddir)/source/actions/libmapactions.la  \
$(top_builddir)/source/graphics/libgraphics.la $(top_builddir)/source/dialogs/libdialogs.la \
$(top_builddir)/source/dialogs/libmapeddialogs.la  $(top_builddir)/source/widgets/libwidgets.la \
$(top_builddir)/source/libs/paragui/src/libparagui.la $(top_builddir)/source/network/libnetwork.la \
$(top_builddir)/source/lua/libluainterfacemaped.la \
$(top_builddir)/source/tasks/libtaskcore.la \
$(LOKI_LIBS) $(SDL_LIBS) $(FREETYPE_LIBS) $(BOOST_LIBS) $(WX_LIBS)






