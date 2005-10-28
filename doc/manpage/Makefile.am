man_MANS=rapidsvn.1
EXTRA_DIST=$(man_MANS)
MAKEMAN=@XSLTPROC@ --nonet --novalid @DOCBOOK_MANPAGES@

manpage: rapidsvn.1

rapidsvn.1: @TARGET_MANPAGE@

manpage-no:

manpage-yes:
	$(MAKEMAN) $(srcdir)/rapidsvn.xml
