EXTRA_DIST=make_repos.bat make_repos.sh README.txt

noinst_HEADERS= \
	path_test.hpp \
	url_test.hpp

bin_PROGRAMS=libsvncpp_tests

libsvncpp_tests_SOURCES= \
	main.cpp \
	path_test.cpp \
	url_test.cpp

libsvncpp_tests_CPPFLAGS=-I$(SVNCPP_DIR)/include @CPPUNIT_CXXFLAGS@
libsvncpp_tests_LDFLAGS=-L$(SVNCPP_DIR) @CPPUNIT_LIBS@
libsvncpp_tests_LDADD=-lsvncpp \
	$(SVN_LIBS) \
	$(APR_LIBS)	
libsvncpp_tests_DEPENDENCIES=$(SVNCPP_DIR)/libsvncpp.la

