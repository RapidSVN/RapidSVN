DOXYGEN=@DOXYGEN@
DOXYGEN_DIST=@DOXYGEN_DIST@
EXTRA_DIST=$(DOXYGEN_DIST)

all: @TARGET_DOXYGEN@

doxygen-no:

doxygen-yes: 
	$(DOXYGEN) svncpp.dox

clean:
	-rm -rf html
