man_MANS=rapidsvn.1
EXTRA_DIST=$(man_MANS)
MAKEMAN=@XSLTPROC@ --nonet --novalid @DOCBOOK_XSL@/manpages/docbook.xsl

manpage: @TARGET_MANPAGE@

manpage-no:

manpage-yes: rapidsvn.1

rapidsvn.1:
	$(MAKEMAN) $(srcdir)/rapidsvn.xml
