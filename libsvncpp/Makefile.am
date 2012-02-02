#SUBDIRS=tests

noinst_HEADERS=${wildcard src/*.hpp} ${wildcard include/svncpp/*.hpp}

lib_LTLIBRARIES=libsvncpp.la

libsvncpp_la_LDFLAGS=-version-info 3:0:0
libsvncpp_la_SOURCES= \
	src/apr.cpp \
	src/client.cpp \
	src/client_annotate.cpp \
	src/client_cat.cpp \
	src/client_diff.cpp \
	src/client_ls.cpp \
	src/client_modify.cpp \
	src/client_property.cpp \
	src/client_status.cpp \
	src/context.cpp \
	src/datetime.cpp \
	src/dirent.cpp \
	src/exception.cpp \
	src/entry.cpp \
	src/info.cpp \
	src/log_entry.cpp \
	src/m_is_empty.cpp \
	src/path.cpp \
	src/pool.cpp \
	src/property.cpp \
	src/revision.cpp \
	src/status.cpp \
	src/status_selection.cpp \
	src/targets.cpp \
	src/url.cpp \
	src/wc.cpp

