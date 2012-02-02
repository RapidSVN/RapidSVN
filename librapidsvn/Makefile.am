SUBDIRS=src/locale

DIST_SUBDIRS=${SUBDIRS} src/res src/res/bitmaps

noinst_HEADERS=${wildcard src/*.hpp} ${wildcard src/*.h} ${wildcard include/*.hpp}

EXTRA_DIST=${wildcard src/*.fbp} \
	${wildcard src/res/*.ico} \
	${wildcard src/res/*.icns} \
	${wildcard src/res/bitmaps/*.xpm} 
	${wildcard src/res/bitmaps/*.h} \
	${wildcard src/res/bitmaps/*.png}

lib_LTLIBRARIES=librapidsvn.la

librapidsvn_la_CPPFLAGS=-Iinclude
librapidsvn_la_DEPENDENCIES=$(SVNCPP_DIR)/libsvncpp.la
librapidsvn_la_LDFLAGS=-version-info 0:0:0 \
	$(WX_LIBS)
librapidsvn_la_SOURCES= \
	src/about_dlg.cpp \
	src/action.cpp \
	src/action_event.cpp \
	src/action_factory.cpp \
	src/add_action.cpp \
	src/add_recursive_action.cpp \
	src/annotate_action.cpp \
	src/annotate_dlg.cpp \
	src/auth_dlg.cpp \
	src/cert_dlg.cpp \
	src/checkout_action.cpp \
	src/checkout_dlg.cpp \
	src/cleanup_action.cpp \
	src/columns.cpp \
	src/commit_action.cpp \
	src/commit_dlg.cpp \
	src/commit_log_dlg.cpp \
	src/create_repos_action.cpp \
	src/create_repos_dlg.cpp \
	src/delete_action.cpp \
	src/delete_dlg.cpp \
	src/destination_dlg.cpp \
	src/diff_action.cpp \
	src/diff_dlg.cpp \
	src/dnd_dlg.cpp \
	src/drag_n_drop_action.cpp \
	src/entry_dlg.cpp \
	src/exceptions.cpp \
	src/export_action.cpp \
	src/export_dlg.cpp \
	src/external_program_action.cpp \
	src/filelist_ctrl.cpp \
	src/filelist_ctrl_drop_target.cpp \
	src/file_info.cpp \
	src/folder_browser.cpp \
	src/folder_browser_drop_target.cpp \
	src/folder_item_data.cpp \
	src/get_action.cpp \
	src/hist_entries.cpp \
	src/hist_mgr.cpp \
	src/hist_val.cpp \
	src/ignore_action.cpp \
	src/import_action.cpp \
	src/import_dlg.cpp \
	src/listed_dlg.cpp \
	src/listener.cpp \
	src/lock_action.cpp \
	src/lock_dlg.cpp \
	src/log_action.cpp \
	src/log_dlg.cpp \
	src/main_frame.cpp \
	src/main_frame_helper.cpp \
	src/merge_action.cpp \
	src/merge_dlg.cpp \
	src/mkdir_action.cpp \
	src/move_action.cpp \
	src/preferences.cpp \
	src/preferences_dlg.cpp \
	src/property_action.cpp \
	src/property_dlg.cpp \
	src/rapidsvn_drop_target.cpp \
	src/rapidsvn_generated.cpp \
	src/rename_action.cpp \
	src/report_dlg.cpp \
	src/resolve_action.cpp \
	src/revert_action.cpp \
	src/revert_dlg.cpp \
	src/simple_worker.cpp \
	src/splitter.cpp \
	src/switch_action.cpp \
	src/switch_dlg.cpp \
	src/threaded_worker.cpp \
	src/tracer.cpp \
	src/unlock_action.cpp \
	src/unlock_dlg.cpp \
	src/update_action.cpp \
	src/update_dlg.cpp \
	src/userresolve_action.cpp \
	src/utils.cpp \
	src/verblist.cpp \
	src/view_action.cpp

