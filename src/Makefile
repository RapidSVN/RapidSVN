PROGRAM=rapidsvn

CXX=g++

OBJECTS=action_thread.o \
		add_action.o \
		auth_baton.o \
		checkout_action.o \
		checkout_dlg.o \
		commit_action.o \
		commit_dlg.o \
		copy_action.o \
		copy_dlg.o \
		delete_action.o \
		delete_dlg.o \
		exceptions.o \
		filelist_ctrl.o \
		folder_browser.o \
		import_action.o \
		import_dlg.o \
		include.o \
		merge_action.o \
		merge_dlg.o \
		mkdir_action.o \
		mkdir_dlg.o \
		notify.o \
		report_dlg.o \
		resolve_action.o \
		revert_action.o \
		svn_file_info.o \
		svn_file_log.o \
		svn_file_status.o \
		trace_commit.o \
		trace_update.o \
		tracer.o \
		update_action.o \
		update_dlg.o \
		utils.o \
		rapidsvn.o \
		rapidsvn_app.o

##APR_LIBS=-L/usr/share/apache2/lib
#APR_CFLAGS="`apr-config --cflags --cppflags --includes`"
APR_CFLAGS=-g -O2 -DLINUX=2 -D_REENTRANT -D_XOPEN_SOURCE=500 -D_BSD_SOURCE -D_SVID_SOURCE -D_GNU_SOURCE -I/data/local/apache/include
#APU_CFLAGS="`apu-config --includes`"
APU_CFLAGS=-I/data/local/apache/include -I/data/local/db4/include
#SVN_CFLAGS="`svn-config --cflags --cppflags --includes`"
SVN_CFLAGS=-g -O2  -DNEON_ZLIB -DNEON_SSL   -DLINUX=2 -D_REENTRANT -D_XOPEN_SOURCE=500 -D_BSD_SOURCE -D_SVID_SOURCE -D_GNU_SOURCE -I/usr/local/include/subversion-1 -I/usr/local/neon-0.21.3/include/neon   -I/data/local/db4/include  -I/data/local/apache/include   -I/data/local/apache/include -I/data/local/db4/include

#APR_LIBS="`apr-config --libs --link-ld`"
APR_LIBS=-lm -lcrypt -lnsl  -L/data/local/apache/lib -lapr
#APU_LIBS="`apu-config --libs --link-ld`"
APU_LIBS=-ldb -lexpat -L/data/local/apache/lib -laprutil
#SVN_LIBS="`svn-config --libs --link-ld`"
SVN_LIBS=-L/data/local/neon-0.21.3/lib -lneon -lssl -lcrypto -lz -lxml2  -L/data/local/apache/lib -laprutil -ldb -lexpat  -L/data/local/apache/lib -lapr -lm -lcrypt -lnsl  -L/data/local/db4/lib -ldb

##LIBS=-lsvn_client-1 -lsvn_wc-1 -lsvn_ra-1 -lsvn_delta-1 -lsvn_subr-1 \
##		$(SVN_APRUTIL_LIBS) $(SVN_APR_LIBS) $(NEON_LIBS) $(APR_LIBS)
##SVN_INC=-I/usr/local/include/subversion-1

#WX_CFLAGS=`wx-config --cxxflags`
WX_CFLAGS=-I/usr/local/lib/wx/include/gtk-2.3 -I/usr/local/include -DGTK_NO_CHECK_CASTS -D__WXGTK__ -D_REENTRANT -fno-rtti -fno-exceptions

WX_LIBS=-lwx_gtk-2.3

CFLAGS=$(APR_CFLAGS) $(APU_CFLAGS) $(SVN_CFLAGS) $(WX_CFLAGS)

LIBS=-lsvn_client-1 -lsvn_wc-1 -lsvn_ra-1 -lsvn_delta-1 -lsvn_subr-1 \
	$(APR_LIBS) $(APU_LIBS) $(SVN_LIBS) $(WX_LIBS)

# implementation

.SUFFIXES:	.o .cpp

##.cpp.o:
##	$(CXX) -c -g `wx-config --cxxflags` $(SVN_INC) -o $@ $<
.cpp.o:
	$(CXX) -c -Wall -g $(CFLAGS) -o $@ $<

all:    $(PROGRAM)

##$(PROGRAM):	$(OBJECTS)
##	$(CXX) -o $(PROGRAM)  $(LIBS) `wx-config --libs` $(OBJECTS)
$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(LIBS) $(OBJECTS)

clean: 
	rm -f *.o $(PROGRAM)
