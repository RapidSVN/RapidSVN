SUBDIRS=manpage
DOXYGEN=@DOXYGEN@
EXTRA_DIST=svncpp.dox svncpp/html/*

all: @TARGET_DOXYGEN@

doxygen-no:

doxygen-yes: svncpp.dep

svncpp.dep: 
	$(DOXYGEN) svncpp.dox && touch svncpp.dep
