SUBDIRS=svncpp res

noinst_HEADERS=${wildcard *.hpp}

bin_PROGRAMS=rapidsvn

rapidsvn_SOURCES= \
	about_dlg.cpp \
	action.cpp \
	add_action.cpp \
	auth_dlg.cpp \
	bookmarks.cpp \
	cert_dlg.cpp \
	checkout_action.cpp \
	checkout_dlg.cpp \
	cleanup_action.cpp \
	commit_action.cpp \
	commit_dlg.cpp \
	delete_action.cpp \
	delete_dlg.cpp \
	destination_dlg.cpp \
	exceptions.cpp \
	external_program_action.cpp \
	filelist_ctrl.cpp \
	file_info.cpp \
	folder_browser.cpp \
	folder_item_data.cpp \
	get_action.cpp \
	import_action.cpp \
	import_dlg.cpp \
	listed_dlg.cpp \
	listener.cpp \
	log_action.cpp \
	log_dlg.cpp \
	merge_action.cpp \
	merge_dlg.cpp \
	mkdir_action.cpp \
	move_action.cpp \
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
	switch_action.cpp \
	tracer.cpp \
	trace_update.cpp \
	tviewer.cpp \
	update_action.cpp \
	update_dlg.cpp \
	utils.cpp \
        verblist.cpp \
	view_action.cpp 

rapidsvn_LDFLAGS=-Lsvncpp
rapidsvn_LDADD= -lsvncpp \
	$(SVN_LIBS) \
	$(NEON_LIBS) \
	$(APR_LIBS) \
	$(WX_LIBS)
rapidsvn_DEPENDENCIES=svncpp/libsvncpp.la
