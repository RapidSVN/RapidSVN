bin_PROGRAMS=rapidsvn

EXTRA_DIST=rapidsvn.rc res/rapidsvn.ico

rapidsvn_SOURCES= \
	rapidsvn_app.cpp

rapidsvn_CPPFLAGS=-I$(LIBRAPIDSVN_DIR)/include
rapidsvn_LDFLAGS=-L$(SVNCPP_DIR) -L$(LIBRAPIDSVN_DIR)
rapidsvn_LDADD= -lsvncpp -lrapidsvn \
	$(SVN_LIBS) \
	$(APR_LIBS) \
	$(WX_LIBS) \
	$(EXTRA_LIBS)
rapidsvn_DEPENDENCIES=$(SVNCPP_DIR)/libsvncpp.la $(LIBRAPIDSVN_DIR)/librapidsvn.la

