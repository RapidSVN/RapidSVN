noinst_HEADERS=${wildcard *.hpp}

bin_PROGRAMS=svncpptest

svncpptest_SOURCES= \
	main.cpp \
	test_url.cpp \
	test_path.cpp
#BROKEN	svncpp.cpp 


svncpptest_CPPFLAGS=-I../../../include @CPPUNIT_CXXFLAGS@
svncpptest_LDFLAGS=-L../../svncpp @CPPUNIT_LIBS@
svncpptest_LDADD=-lsvncpp \
	$(SVN_LIBS) \
	$(NEON_LIBS) \
	$(APR_LIBS)	
svncpptest_DEPENDENCIES=../../svncpp/libsvncpp.la
