EXTRA_DIST=make_repos.bat make_repos.sh README.txt

noinst_HEADERS= \
	path_test.hpp \
	url_test.hpp

bin_PROGRAMS=libsvncpptest

libsvncpptest_SOURCES= \
	main.cpp \
	path_test.cpp \
	url_test.cpp

libsvncpptest_CPPFLAGS=-I$(SVNCPP_DIR)/include @CPPUNIT_CXXFLAGS@
libsvncpptest_LDFLAGS=-L$(SVNCPP_DIR) @CPPUNIT_LIBS@
libsvncpptest_LDADD=-lsvncpp \
	$(SVN_LIBS) \
	$(APR_LIBS)	
libsvncpptest_DEPENDENCIES=$(SVNCPP_DIR)/libsvncpp.la
