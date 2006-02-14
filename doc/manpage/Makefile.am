man_MANS=$(MAN_MANS)
EXTRA_DIST=$(man_MANS) $(srcdir)/rapidsvn.xml
MAKEMAN=@XSLTPROC@ --nonet --novalid @DOCBOOK_MANPAGES@

manpage: rapidsvn.1

rapidsvn.1: @TARGET_MANPAGE@

manpage-no:

manpage-yes:
	$(MAKEMAN) $(srcdir)/rapidsvn.xml

clean:
	-rm -f rapidsvn.1
