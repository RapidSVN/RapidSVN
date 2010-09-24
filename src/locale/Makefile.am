EXTRA_DIST=rapidsvn.pot \
	de/rapidsvn.po \
	fr/rapidsvn.po \
	hu/rapidsvn.po \
	it_IT/rapidsvn.po \
	pt_BR/rapidsvn.po \
	ru/rapidsvn.po \
	es/rapidsvn.po \
	uk/rapidsvn.po \
	zh_CN/rapidsvn.po\
	ja/rapidsvn.po

RAPIDSVN_LANGUAGES=de fr hu it_IT pt_BR ru es uk zh_CN ja

localedir = $(datadir)/locale

install-data-local: install-rapidsvn-catalogs

install-rapidsvn-catalogs:
	for i in $(RAPIDSVN_LANGUAGES) ; do \
	    $(mkinstalldirs) $(DESTDIR)$(localedir)/$$i/LC_MESSAGES ; \
	    $(INSTALL_DATA) $(srcdir)/$$i/rapidsvn.mo $(DESTDIR)$(localedir)/$$i/LC_MESSAGES/rapidsvn.mo ; \
	done

# the programs we use (TODO: use configure to detect them)
#On OS/X we dont seem to have msgfmt installed by default
#so we are going to use the python version instead
#(TODO check for msgfmt, then python)
#MSGFMT=msgfmt
MSGFMT=python $(top_srcdir)/tools/msgfmt.py

all-mo:
	for i in $(RAPIDSVN_LANGUAGES) ; do \
	    $(MSGFMT) -o $(srcdir)/$$i/rapidsvn.mo $(srcdir)/$$i/rapidsvn.po ; \
	done

all: all-mo

distclean: clean
clean:
	for i in $(RAPIDSVN_LANGUAGES) ; do \
	    if [ -f $(srcdir)/$$i/rapidsvn.mo ] ; then rm $(srcdir)/$$i/rapidsvn.mo ; fi ; \
	done
.PHONY: all-mo
