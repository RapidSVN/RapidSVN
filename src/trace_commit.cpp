#include "include.h"
#include "tracer.h"
#include "trace_commit.h"

static const int svn_cl__item_modified = 0;
static const int svn_cl__item_added = 0;
static const int svn_cl__item_added_binary = 0;
static const int svn_cl__item_deleted = 0;
static const int svn_cl__item_replaced = 0;
static const int svn_cl__item_replaced_binary = 0;

#define ITEM_MODIFIED (&svn_cl__item_modified)
#define ITEM_ADDED (&svn_cl__item_added)
#define ITEM_ADDED_BINARY (&svn_cl__item_added_binary)
#define ITEM_DELETED (&svn_cl__item_deleted)
#define ITEM_REPLACED (&svn_cl__item_replaced)
#define ITEM_REPLACED_BINARY (&svn_cl__item_replaced_binary)

/*
 * Baton structures
 */

struct EditBaton
{
  apr_pool_t *pool;
  const char *path;
  svn_boolean_t started_sending_txdeltas;
  Tracer *tracer;
};

struct DirBaton
{
  EditBaton *edit_baton;
  DirBaton *parent_dir_baton;
  const char *path;
  svn_boolean_t prop_changed;
  apr_hash_t *entrymods;
  apr_pool_t *pool;
};


struct FileBaton
{
  EditBaton *edit_baton;
  DirBaton *parent_dir_baton;
  const char *path;
};

/*
 * Helper functions
 */

static DirBaton *
make_dir_baton (const char *path,
                void *parent_baton, void *edit_baton, apr_pool_t * pool)
{
  DirBaton *pb = (DirBaton *) parent_baton;
  EditBaton *eb = (EditBaton *) edit_baton;
  DirBaton *new_db = (DirBaton *) apr_pcalloc (pool, sizeof (DirBaton));
  const char *full_path;

  /* Don't give me a path without a parent baton! */
  if (path && (!pb))
    return NULL;

  /* Construct the "full" path of this node. */
  if (pb)
    full_path = svn_path_join (eb->path, path, pool);
  else
    full_path = apr_pstrdup (pool, eb->path);

  /* Finish populating the baton. */
  new_db->path = full_path;
  new_db->edit_baton = eb;
  new_db->parent_dir_baton = pb;
  new_db->entrymods = apr_hash_make (pool);
  new_db->pool = pool;
  return new_db;
}


static FileBaton *
make_file_baton (const char *path,
                 void *parent_baton, void *edit_baton, apr_pool_t * pool)
{
  FileBaton *new_fb = (FileBaton *) apr_pcalloc (pool, sizeof (FileBaton));
  DirBaton *pb = (DirBaton *) parent_baton;
  EditBaton *eb = (EditBaton *) edit_baton;
  const char *full_path = svn_path_join (eb->path, path, pool);

  /* Finish populating the baton. */
  new_fb->path = full_path;
  new_fb->edit_baton = eb;
  new_fb->parent_dir_baton = pb;
  return new_fb;
}


static svn_error_t *
open_root (void *edit_baton,
           svn_revnum_t base_revision, apr_pool_t * pool, void **root_baton)
{
  EditBaton *eb = (EditBaton *) edit_baton;
  *root_baton = make_dir_baton (NULL, NULL, eb, pool);
  return SVN_NO_ERROR;
}


static svn_error_t *
delete_entry (const char *path,
              svn_revnum_t revision, void *parent_baton, apr_pool_t * pool)
{
  DirBaton *pb = (DirBaton *) parent_baton;
  EditBaton *eb = (EditBaton *) pb->edit_baton;
  void *vp;

  /* Construct the "full" path of this node in the parent directory's
     pool (the same pool that contains the hash). */
  const char *full_path = svn_path_join (eb->path, path, pb->pool);

  /* Let the parent directory know that one of its entries has been
     deleted.  If this thing was just added, this is really a noop. */
  vp = apr_hash_get (pb->entrymods, full_path, APR_HASH_KEY_STRING);
  if (vp == NULL)
    vp = (void *) ITEM_DELETED;
  if ((vp == (void *) ITEM_ADDED) || (vp == (void *) ITEM_ADDED_BINARY))
    vp = NULL;
  apr_hash_set (pb->entrymods, full_path, APR_HASH_KEY_STRING, vp);

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
  EditBaton *eb = (EditBaton *) pb->edit_baton;
  DirBaton *new_db = make_dir_baton (path, pb, eb, pool);
  const char *full_path;
  void *vp;

  /* Copy the path into the parent directorie's pool (where its hash
     lives) and let the parent know that this was added (or replaced) */
  full_path = apr_pstrdup (pb->pool, new_db->path);

  vp = apr_hash_get (pb->entrymods, full_path, APR_HASH_KEY_STRING);
  if (vp == NULL)
    vp = (void *) ITEM_ADDED;
  if (vp == (void *) ITEM_DELETED)
    vp = (void *) ITEM_REPLACED;
  apr_hash_set (pb->entrymods, full_path, APR_HASH_KEY_STRING, vp);

  *child_baton = new_db;
  return SVN_NO_ERROR;
}


static svn_error_t *
open_directory (const char *path,
                void *parent_baton,
                svn_revnum_t base_revision,
                apr_pool_t * pool, void **child_baton)
{
  DirBaton *pb = (DirBaton *) parent_baton;
  EditBaton *eb = (EditBaton *) pb->edit_baton;
  *child_baton = make_dir_baton (path, pb, eb, pool);
  return SVN_NO_ERROR;
}


static svn_error_t *
close_directory (void *dir_baton, apr_pool_t *)
{
  DirBaton *db = (DirBaton *) dir_baton;
  DirBaton *pb = db->parent_dir_baton;
  apr_hash_index_t *hi;
  void *vp = NULL;

  /* See if there is an entry in the parent's hash for this
     directory. */
  if (pb)
    vp = apr_hash_get (pb->entrymods, db->path, APR_HASH_KEY_STRING);

  /* If this item was added to its parent's hash, print such.  Else,
     if it has propchanges, print that.  Otherwise, it should have
     just been 'open'ed, and that's not interesting enough to print.  */
  if (vp == (void *) ITEM_ADDED)
  {
    wxString str = wxString::Format ("Adding          %s", db->path);
    db->edit_baton->tracer->Trace (str);
  }
  else if (db->prop_changed)
  {
    wxString str = wxString::Format ("Sending         %s", db->path);
    db->edit_baton->tracer->Trace (str);
  }

  /* Now remove this from the parent's hash. */
  if (vp)
    apr_hash_set (pb->entrymods, db->path, APR_HASH_KEY_STRING, NULL);

  /* For each modified entry of this directory, print out a
     description of those mods. */
  for (hi = apr_hash_first (db->pool, db->entrymods);
       hi; hi = apr_hash_next (hi))
  {
    const char *pattern;
    const void *key;
    void *val;

    apr_hash_this (hi, &key, NULL, &val);

    if (val == (void *) ITEM_REPLACED)
      pattern = "Replacing       %s";
    else if (val == (void *) ITEM_REPLACED_BINARY)
      pattern = "Replacing (bin) %s";
    else if (val == (void *) ITEM_DELETED)
      pattern = "Deleting        %s";
    else if (val == (void *) ITEM_ADDED)
      pattern = "Adding          %s";
    else if (val == (void *) ITEM_ADDED_BINARY)
      pattern = "Adding   (bin)  %s";
    else if (val == (void *) ITEM_MODIFIED)
      pattern = "Sending         %s";
    else
      // #### TODO: change here
      abort ();                 /* this should never happen */

    wxString str = wxString::Format (pattern, (const char *) key);
    db->edit_baton->tracer->Trace (str);
  }

  return SVN_NO_ERROR;
}


static svn_error_t *
apply_textdelta (void *file_baton,apr_pool_t *,
                 svn_txdelta_window_handler_t * handler, void **handler_baton)
{
  FileBaton *fb = (FileBaton *) file_baton;
  *handler = NULL;
  *handler_baton = NULL;

  if (!fb->edit_baton->started_sending_txdeltas)
  {
    fb->edit_baton->tracer->Trace (_T ("Transmitting file data ..."));
    fb->edit_baton->started_sending_txdeltas = TRUE;
  }

  //fb->edit_baton->tracer->Trace( _T(".") );

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
  EditBaton *eb = pb->edit_baton;
  FileBaton *new_fb = make_file_baton (path, pb, eb, pool);
  void *vp;

  /* Copy the path into the parent's pool (where its hash lives). */
  const char *full_path = apr_pstrdup (pb->pool, new_fb->path);

  /* Tell the parent directory that one of its children has been
     added (or replaced). */
  vp = apr_hash_get (pb->entrymods, full_path, APR_HASH_KEY_STRING);
  if (vp == NULL)
    vp = (void *) ITEM_ADDED;
  if (vp == (void *) ITEM_DELETED)
    vp = (void *) ITEM_REPLACED;
  apr_hash_set (pb->entrymods, full_path, APR_HASH_KEY_STRING, vp);

  *file_baton = new_fb;
  return SVN_NO_ERROR;
}


static svn_error_t *
open_file (const char *path,
           void *parent_baton,
           svn_revnum_t base_revision, apr_pool_t * pool, void **file_baton)
{
  DirBaton *pb = (DirBaton *) parent_baton;
  EditBaton *eb = pb->edit_baton;
  FileBaton *new_fb = make_file_baton (path, pb, eb, pool);

  /* Copy the path into the parent's pool (where its hash lives). */
  const char *path_copy = apr_pstrdup (pb->pool, new_fb->path);

  /* Tell the parent directory that one of its children has been added. */
  apr_hash_set (pb->entrymods, path_copy, APR_HASH_KEY_STRING, ITEM_MODIFIED);

  *file_baton = new_fb;
  return SVN_NO_ERROR;
}

static svn_error_t *
change_file_prop (void *file_baton,
                  const char *name,
                  const svn_string_t * value, apr_pool_t * pool)
{
  FileBaton *fb = (FileBaton *) file_baton;
  DirBaton *pb = fb->parent_dir_baton;

  /* If the mime-type property is being set to non-NULL, and something
     that doesn't start with 'text/', call this a binary file. */
  if ((!strcmp (name, SVN_PROP_MIME_TYPE))
      && value && (strncmp (value->data, "text/", 5)))
  {
    void *vp = apr_hash_get (pb->entrymods, fb->path, APR_HASH_KEY_STRING);
    if (vp == (void *) ITEM_ADDED)
      vp = (void *) ITEM_ADDED_BINARY;
    else if (vp == (void *) ITEM_MODIFIED)
      ;                         /* do nothing. */
    else
      // #### TODO: change here
      abort ();                 /* this shouldn't happen. */
    apr_hash_set (pb->entrymods, fb->path, APR_HASH_KEY_STRING, vp);
  }
  return SVN_NO_ERROR;
}


static svn_error_t *
change_dir_prop (void *dir_baton,
                 const char *name,
                 const svn_string_t * value, apr_pool_t * pool)
{
  DirBaton *db = (DirBaton *) dir_baton;
  db->prop_changed = TRUE;
  return SVN_NO_ERROR;
}


static svn_error_t *
close_edit (void *edit_baton, apr_pool_t*)
{
  EditBaton *eb = (EditBaton *) edit_baton;

  if (eb->started_sending_txdeltas)
  {
    eb->tracer->Trace (_T ("Done!"));
  }

  svn_pool_destroy (eb->pool);

  return SVN_NO_ERROR;
}

/*************************************************************/

svn_error_t *
get_trace_commit_editor (const svn_delta_editor_t ** editor,
                         void **edit_baton,
                         const char *initial_path,
                         Tracer * tr, apr_pool_t * pool)
{
  /* Allocate an edit baton to be stored in every directory baton. */
  svn_delta_editor_t *trace_editor = svn_delta_default_editor (pool);
  apr_pool_t *subpool = svn_pool_create (pool);
  EditBaton *eb = (EditBaton *) apr_pcalloc (subpool, sizeof (EditBaton));

  /* Set up the edit context. */
  eb->pool = subpool;
  eb->started_sending_txdeltas = FALSE;
  if (initial_path && initial_path[0])
    eb->path = apr_pstrdup (subpool, initial_path);
  else
    eb->path = ".";

  eb->tracer = tr;

  /* Set up the editor. */
  trace_editor->open_root = open_root;
  trace_editor->delete_entry = delete_entry;
  trace_editor->add_directory = add_directory;
  trace_editor->open_directory = open_directory;
  trace_editor->change_dir_prop = change_dir_prop;
  trace_editor->close_directory = close_directory;
  trace_editor->add_file = add_file;
  trace_editor->open_file = open_file;
  trace_editor->change_file_prop = change_file_prop;
  trace_editor->apply_textdelta = apply_textdelta;
  trace_editor->close_edit = close_edit;

  *edit_baton = eb;
  *editor = trace_editor;

  return SVN_NO_ERROR;
}
