#SUBDIRS=tests

noinst_HEADERS= \
	include/svncpp/annotate_line.hpp \
	include/svncpp/apr.hpp \
	include/svncpp/client.hpp \
	include/svncpp/context.hpp \
	include/svncpp/context_listener.hpp \
	include/svncpp/datetime.hpp \
	include/svncpp/dirent.hpp \
	include/svncpp/entry.hpp \
	include/svncpp/exception.hpp \
	include/svncpp/info.hpp \
	include/svncpp/log_entry.hpp \
	include/svncpp/map_wrapper.hpp \
	include/svncpp/path.hpp \
	include/svncpp/pool.hpp \
	include/svncpp/property.hpp \
	include/svncpp/revision.hpp \
	include/svncpp/status.hpp \
	include/svncpp/status_selection.hpp \
	include/svncpp/string_wrapper.hpp \
	include/svncpp/targets.hpp \
	include/svncpp/url.hpp \
	include/svncpp/utility_wrapper.hpp \
	include/svncpp/vector_wrapper.hpp \
	include/svncpp/version.hpp \
	include/svncpp/wc.hpp \
	src/m_check.hpp \
	src/m_is_empty.hpp

lib_LTLIBRARIES=libsvncpp.la

libsvncpp_la_LDFLAGS=-version-info 4:0:0
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

