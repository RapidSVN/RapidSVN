SUBDIRS=svncpp res

noinst_HEADERS=${wildcard *.hpp}

bin_PROGRAMS=rapidsvn

rapidsvn_SOURCES= \
	action.cpp \
	add_action.cpp \
	auth_dlg.cpp \
	checkout_action.cpp \
	checkout_dlg.cpp \
	cleanup_action.cpp \
	commit_action.cpp \
	commit_dlg.cpp \
	copy_action.cpp \
	copymove_action.cpp \
	delete_action.cpp \
	delete_dlg.cpp \
 	destination_dlg.cpp \
	exceptions.cpp \
	external_program_action.cpp \
	filelist_ctrl.cpp \
	folder_browser.cpp \
	folder_item_data.cpp \
	import_action.cpp \
	import_dlg.cpp \
	log_action.cpp \
	log_dlg.cpp \
	merge_action.cpp \
	merge_dlg.cpp \
	mkdir_action.cpp \
	mkdir_dlg.cpp \
        preferences.cpp \
	preferences_dlg.cpp \
	property_action.cpp \
	property_dlg.cpp \
	proportional_splitter.cpp \
	rapidsvn_app.cpp \
	rapidsvn_frame.cpp \
	report_dlg.cpp \
	resolve_action.cpp \
	revert_action.cpp \
	simple_worker.cpp \
	svn_file_info.cpp \
	svn_notify.cpp \
	trace_commit.cpp \
	tracer.cpp \
	trace_update.cpp \
	update_action.cpp \
	update_dlg.cpp \
	utils.cpp \
        verblist.cpp \
	workbench.cpp

rapidsvn_LDFLAGS=-Lsvncpp -L$(SVN_LIB)
rapidsvn_LDADD= -lsvncpp \
	-lsvn_client-1 -lsvn_wc-1 -lsvn_ra-1 -lsvn_delta-1 -lsvn_subr-1 \
	-lneon $(APR_LIBS) \
	$(WX_LIBS)
rapidsvn_DEPENDENCIES=svncpp/libsvncpp.la

