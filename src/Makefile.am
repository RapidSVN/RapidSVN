SUBDIRS=svncpp

DIST_SUBDIRS=${SUBDIRS} locale res tests

noinst_HEADERS=${wildcard *.hpp} ${wildcard *.h}
bin_PROGRAMS=rapidsvn

EXTRA_DIST=${wildcard *.fbp}

rapidsvn_SOURCES= \
	about_dlg.cpp \
	action.cpp \
	action_event.cpp \
	action_factory.cpp \
	add_action.cpp \
	add_recursive_action.cpp \
	annotate_action.cpp \
	annotate_dlg.cpp \
	auth_dlg.cpp \
	cert_dlg.cpp \
	checkout_action.cpp \
	checkout_dlg.cpp \
	cleanup_action.cpp \
	columns.cpp \
	commit_action.cpp \
	commit_dlg.cpp \
	commit_log_dlg.cpp \
	delete_action.cpp \
	delete_dlg.cpp \
	destination_dlg.cpp \
	diff_action.cpp \
	diff_dlg.cpp \
	dnd_dlg.cpp \
	drag_n_drop_action.cpp \
	exceptions.cpp \
	export_action.cpp \
	export_dlg.cpp \
	external_program_action.cpp \
	filelist_ctrl.cpp \
	filelist_ctrl_drop_target.cpp \
	file_info.cpp \
	folder_browser.cpp \
	folder_browser_drop_target.cpp \
	folder_item_data.cpp \
	get_action.cpp \
	hist_entries.cpp \
	hist_mgr.cpp \
	hist_val.cpp \
	ignore_action.cpp \
	import_action.cpp \
	import_dlg.cpp \
	listed_dlg.cpp \
	listener.cpp \
	lock_action.cpp \
	lock_dlg.cpp \
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
	rapidsvn_drop_target.cpp \
	rapidsvn_frame.cpp \
	rapidsvn_generated.cpp \
	rename_action.cpp \
	report_dlg.cpp \
	resolve_action.cpp \
	revert_action.cpp \
	revert_dlg.cpp \
	simple_worker.cpp \
	switch_action.cpp \
	switch_dlg.cpp \
	threaded_worker.cpp \
	tracer.cpp \
	unlock_action.cpp \
	unlock_dlg.cpp \
	update_action.cpp \
	update_dlg.cpp \
	userresolve_action.cpp \
	utils.cpp \
	verblist.cpp \
	view_action.cpp 

rapidsvn_LDFLAGS=-Lsvncpp
rapidsvn_LDADD= -lsvncpp \
	$(SVN_LIBS) \
	$(APR_LIBS) \
	$(WX_LIBS) \
	$(EXTRA_LIBS)
rapidsvn_DEPENDENCIES=svncpp/libsvncpp.la
