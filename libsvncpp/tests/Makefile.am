EXTRA_DIST=make_repos.bat make_repos.sh README.txt

noinst_HEADERS=${wildcard *.hpp}

bin_PROGRAMS=svncpptest

svncpptest_SOURCES= \
	main.cpp \
	test_url.cpp \
	test_path.cpp

svncpptest_CPPFLAGS=-I$(SVNCPP_DIR)/include @CPPUNIT_CXXFLAGS@
svncpptest_LDFLAGS=-L$(SVNCPP_DIR) @CPPUNIT_LIBS@
svncpptest_LDADD=-lsvncpp \
	$(SVN_LIBS) \
	$(APR_LIBS)	
svncpptest_DEPENDENCIES=$(SVNCPP_DIR)/libsvncpp.la
