#ifndef _TRACE_UPDATE_EDITOR_H_INCLUDED_
#define _TRACE_UPDATE_EDITOR_H_INCLUDED_

#include "tracer.h"

svn_error_t *get_trace_update_editor (const svn_delta_editor_t ** editor,
                                      void **edit_baton,
                                      const char *initial_path,
                                      svn_boolean_t is_checkout,
                                      svn_boolean_t no_final_line,
                                      Tracer * tracer, apr_pool_t * pool);

#endif
