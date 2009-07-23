EXTRA_DIST=make_repos.bat make_repos.sh README.txt

noinst_HEADERS=${wildcard *.hpp}

bin_PROGRAMS=svncpptest

svncpptest_SOURCES= \
	main.cpp \
	test_url.cpp \
	test_path.cpp

svncpptest_CPPFLAGS=-I../../../include @CPPUNIT_CXXFLAGS@
svncpptest_LDFLAGS=-L../../svncpp @CPPUNIT_LIBS@
svncpptest_LDADD=-lsvncpp \
	$(SVN_LIBS) \
	$(APR_LIBS)	
svncpptest_DEPENDENCIES=../../svncpp/libsvncpp.la
