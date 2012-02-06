bin_PROGRAMS=librapidsvn_tests

#noinst_HEADERS= 

TESTS=librapidsvn_tests

librapidsvn_tests_SOURCES= \
	main.cpp 

librapidsvn_tests_CPPFLAGS= \
	-I$(LIBRAPIDSVN_DIR)/include \
	@CPPUNIT_CXXFLAGS@
librapidsvn_tests_LDFLAGS= \
	-L$(LIBRAPIDSVN_DIR) \
	@CPPUNIT_LIBS@
librapidsvn_tests_LDADD= -lsvncpp -lrapidsvn \
	$(SVN_LIBS) \
	$(APR_LIBS) \
	$(WX_LIBS) \
	$(EXTRA_LIBS)
librapidsvn_tests_DEPENDENCIES=$(LIBRAPIDSVN_DIR)/librapidsvn.la

