#include "include.h"
#include "tracer.h"
#include "trace_update.h"

/*
 * Baton structures
 */

struct EditBaton
{
  apr_pool_t *pool;
  const char *path;
  svn_revnum_t revision;
  svn_boolean_t changed;
  svn_boolean_t is_checkout;
  svn_boolean_t no_final_line;
  Tracer *tracer;
};

struct DirBaton
{
  EditBaton *edit_baton;
  DirBaton *parent_baton;
  const char *path;
  svn_boolean_t prop_changed;
};

struct FileBaton
{
  EditBaton *edit_baton;
  DirBaton *parent_baton;
  const char *path;
  svn_boolean_t added;
  svn_boolean_t text_changed;
  svn_boolean_t prop_changed;
};

/*
 * Helper functions
 */

static DirBaton *
make_dir_baton (const char *path,
                void *edit_baton, void *parent_dir_baton, apr_pool_t * pool)
{
  EditBaton *eb = (EditBaton *) edit_baton;
  DirBaton *pb = (DirBaton *) parent_dir_baton;
  DirBaton *new_db = (DirBaton *) apr_pcalloc (pool, sizeof (*new_db));
  const char *full_path;

  // A path relative to nothing
  if (path && (!pb))
    return NULL;

  if (pb)
    full_path = svn_path_join (eb->path, path, pool);
  else
    full_path = apr_pstrdup (pool, eb->path);

  new_db->edit_baton = eb;
  new_db->parent_baton = pb;
  new_db->path = full_path;

  return new_db;
}

static FileBaton *
make_file_baton (const char *path,
                 void *parent_dir_baton,
                 svn_boolean_t added, apr_pool_t * pool)
{
  DirBaton *pb = (DirBaton *) parent_dir_baton;
  EditBaton *eb = pb->edit_baton;
  FileBaton *new_fb = (FileBaton *) apr_pcalloc (pool, sizeof (FileBaton));
  const char *full_path = svn_path_join (eb->path, path, pool);

  new_fb->edit_baton = eb;
  new_fb->parent_baton = pb;
  new_fb->path = full_path;
  new_fb->added = added;
  new_fb->text_changed = FALSE;
  new_fb->prop_changed = FALSE;

  return new_fb;
}

/*
 * See svn_delta.h for details about the following callbacks.
 */

static svn_error_t *
open_root (void *edit_baton,
           svn_revnum_t base_revision, apr_pool_t * pool, void **root_baton)
{
  *root_baton = make_dir_baton (NULL, edit_baton, NULL, pool);
  return SVN_NO_ERROR;
}

static svn_error_t *
set_target_revision (void *edit_baton, svn_revnum_t target_revision)
{
  EditBaton *eb = (EditBaton *) edit_baton;

  eb->revision = target_revision;
  return SVN_NO_ERROR;
}

static svn_error_t *
open_directory (const char *path,
                void *parent_baton,
                svn_revnum_t base_revision,
                apr_pool_t * pool, void **child_baton)
{
  DirBaton *pb = (DirBaton *) parent_baton;
  EditBaton *eb = pb->edit_baton;
  DirBaton *new_db = make_dir_baton (path, eb, pb, pool);

  *child_baton = new_db;
  return SVN_NO_ERROR;
}

static svn_error_t *
add_directory (const char *path,
               void *parent_baton,
               const char *copyfrom_path,
               svn_revnum_t copyfrom_revision,
               apr_pool_t * pool, void **child_baton)
{
  DirBaton *pb = (DirBaton *) parent_baton;
  EditBaton *eb = pb->edit_baton;
  DirBaton *new_db = make_dir_baton (path, eb, pb, pool);

  eb->changed = TRUE;

  {
    //printf ("A  %s\n", new_db->path);
    wxString str = wxString::Format ("A  %s", new_db->path);
    eb->tracer->Trace (str);
  }

  *child_baton = new_db;
  return SVN_NO_ERROR;
}

static svn_error_t *
close_directory (void *dir_baton)
{
  DirBaton *db = (DirBaton *) dir_baton;
  EditBaton *eb = db->edit_baton;
  char statchar_buf[3] = "_ ";

  if (db->prop_changed)
  {
    // Check for conflicted state.
    svn_wc_entry_t *entry;
    svn_boolean_t merged, tc, pc;
    apr_pool_t *subpool = svn_pool_create (eb->pool);

    SVN_ERR (svn_wc_entry (&entry, db->path, FALSE, subpool));
    SVN_ERR (svn_wc_conflicted_p (&tc, &pc, db->path, entry, subpool));

    if (!pc)
      SVN_ERR (svn_wc_props_modified_p (&merged, db->path, subpool));

    if (pc)
      statchar_buf[1] = 'C';
    else if (merged)
      statchar_buf[1] = 'G';
    else
      statchar_buf[1] = 'U';
    {
      //printf ("%s %s\n", statchar_buf, db->path);
      wxString str = wxString::Format ("%s %s", statchar_buf, db->path);
      eb->tracer->Trace (str);
    }

    /* Destroy the subpool. */
    svn_pool_destroy (subpool);
  }

  return SVN_NO_ERROR;
}

static svn_error_t *
open_file (const char *path,
           void *parent_baton,
           svn_revnum_t ancestor_revision,
           apr_pool_t * pool, void **file_baton)
{
  DirBaton *pb = (DirBaton *) parent_baton;
  FileBaton *new_fb = make_file_baton (path, pb, FALSE, pool);

  *file_baton = new_fb;

  return SVN_NO_ERROR;
}

static svn_error_t *
add_file (const char *path,
          void *parent_baton,
          const char *copyfrom_path,
          svn_revnum_t copyfrom_revision,
          apr_pool_t * pool, void **file_baton)
{
  DirBaton *pb = (DirBaton *) parent_baton;
  FileBaton *new_fb = make_file_baton (path, pb, TRUE, pool);

  new_fb->edit_baton->changed = TRUE;
  *file_baton = new_fb;

  return SVN_NO_ERROR;
}

static svn_error_t *
close_file (void *file_baton)
{
  FileBaton *fb = (FileBaton *) file_baton;
  EditBaton *eb = fb->edit_baton;
  char statchar_buf[3] = "_ ";

  if (fb->added)
    statchar_buf[0] = 'A';

  /* We need to check the state of the file now to see if it was
     merged or is in a state of conflict.  Believe it or not, this can
     be the case even when FB->ADDED is set.  */
  {
    /* First, check for conflicted state. */
    svn_wc_entry_t *entry;
    svn_boolean_t merged, tc, pc;
    apr_pool_t *subpool = svn_pool_create (eb->pool);
    const char *pdir = svn_path_remove_component_nts (fb->path, subpool);


    SVN_ERR (svn_wc_entry (&entry, fb->path, FALSE, subpool));
    if (entry)
    {
      SVN_ERR (svn_wc_conflicted_p (&tc, &pc, pdir, entry, subpool));
      if (fb->text_changed)
      {
        if (!tc)
          SVN_ERR (svn_wc_text_modified_p (&merged, fb->path, subpool));

        if (tc)
          statchar_buf[0] = 'C';
        else if (merged)
          statchar_buf[0] = 'G';
        else if (!fb->added)
          statchar_buf[0] = 'U';
      }

      if (fb->prop_changed)
      {
        if (!pc)
          SVN_ERR (svn_wc_props_modified_p (&merged, fb->path, subpool));

        if (pc)
          statchar_buf[1] = 'C';
        else if (merged)
          statchar_buf[1] = 'G';
        else if (!fb->added)
          statchar_buf[1] = 'U';
      }
    }

    /* Destroy the subpool. */
    svn_pool_destroy (subpool);
  }

  {
    //printf ("%s %s\n", statchar_buf, fb->path);
    wxString str = wxString::Format ("%s %s", statchar_buf, fb->path);
    eb->tracer->Trace (str);
  }

  return SVN_NO_ERROR;
}

static svn_error_t *
change_file_prop (void *file_baton,
                  const char *name,
                  const svn_string_t * value, apr_pool_t * pool)
{
  FileBaton *fb = (FileBaton *) file_baton;

  if (svn_wc_is_normal_prop (name))
  {
    fb->prop_changed = TRUE;
    fb->edit_baton->changed = TRUE;
  }

  return SVN_NO_ERROR;
}

static svn_error_t *
change_dir_prop (void *parent_baton,
                 const char *name,
                 const svn_string_t * value, apr_pool_t * pool)
{
  DirBaton *db = (DirBaton *) parent_baton;

  if (svn_wc_is_normal_prop (name))
  {
    db->prop_changed = TRUE;
    db->edit_baton->changed = TRUE;
  }

  return SVN_NO_ERROR;
}

static svn_error_t *
delete_entry (const char *path,
              svn_revnum_t revision, void *parent_baton, apr_pool_t * pool)
{
  DirBaton *pb = (DirBaton *) parent_baton;
  EditBaton *eb = pb->edit_baton;

  eb->changed = TRUE;

  {
    //printf( "D  %s\n", svn_path_join( eb->path, path, pool ) );
    wxString str =
      wxString::Format ("D  %s", svn_path_join (eb->path, path, pool));
    eb->tracer->Trace (str);
  }
  return SVN_NO_ERROR;
}

static svn_error_t *
apply_textdelta (void *file_baton,
                 svn_txdelta_window_handler_t * handler, void **handler_baton)
{
  FileBaton *fb = (FileBaton *) file_baton;

  fb->edit_baton->changed = TRUE;
  fb->text_changed = TRUE;
  *handler = NULL;
  *handler_baton = NULL;

  return SVN_NO_ERROR;
}

static svn_error_t *
close_edit (void *edit_baton)
{
  EditBaton *eb = (EditBaton *) edit_baton;

  if (!eb->no_final_line)
  {
    if (eb->is_checkout)
      //printf( "Checked out revision %" SVN_REVNUM_T_FMT ".\n",
      //  eb->revision );
    {
      wxString str =
        wxString::Format ("Checked out revision %" SVN_REVNUM_T_FMT ".",
                          eb->revision);
      eb->tracer->Trace (str);
    }
    else
    {
      if (eb->changed)
      {
        //printf( "Updated to revision %" SVN_REVNUM_T_FMT ".\n",
        //  eb->revision );
        wxString str =
          wxString::Format ("Updated to revision %" SVN_REVNUM_T_FMT ".",
                            eb->revision);
        eb->tracer->Trace (str);
      }

      else
      {
        //printf( "At revision %" SVN_REVNUM_T_FMT ".\n",
        //  eb->revision );
        wxString str = wxString::Format ("At revision %" SVN_REVNUM_T_FMT ".",
                                         eb->revision);
        eb->tracer->Trace (str);
      }
    }
  }

  svn_pool_destroy (eb->pool);

  return SVN_NO_ERROR;
}

svn_error_t *
get_trace_update_editor (const svn_delta_editor_t ** editor,
                         void **edit_baton,
                         const char *initial_path,
                         svn_boolean_t is_checkout,
                         svn_boolean_t no_final_line,
                         Tracer * tracer, apr_pool_t * pool)
{
  apr_pool_t *subpool = svn_pool_create (pool);
  EditBaton *eb = (EditBaton *) apr_pcalloc (subpool, sizeof (EditBaton));
  svn_delta_editor_t *te = svn_delta_default_editor (pool);

  // Set up the edit context.
  eb->pool = subpool;
  eb->path = apr_pstrdup (subpool, initial_path);
  eb->revision = SVN_INVALID_REVNUM;
  eb->is_checkout = is_checkout;
  eb->tracer = tracer;

  // Set up the the editor.
  te->open_root = open_root;
  te->set_target_revision = set_target_revision;
  te->delete_entry = delete_entry;

  te->apply_textdelta = apply_textdelta;
  te->close_edit = close_edit;

  te->open_directory = open_directory;
  te->add_directory = add_directory;
  te->close_directory = close_directory;
  te->change_dir_prop = change_dir_prop;

  te->open_file = open_file;
  te->add_file = add_file;
  te->close_file = close_file;
  te->change_file_prop = change_file_prop;

  *edit_baton = eb;
  *editor = te;

  return SVN_NO_ERROR;
}
