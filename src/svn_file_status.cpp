#include "include.h"
#include "utils.h"
#include "svn_file_status.h"
#include "svn_sorts.h"

SvnFileStatus::SvnFileStatus (apr_pool_t * thepool)
{
  subpool = svn_pool_create (NULL);// (thepool);
}

SvnFileStatus::~SvnFileStatus ()
{
  svn_pool_destroy (subpool);
}

bool
SvnFileStatus::retrieveStatus (const wxString & path, AuthBaton & auth_baton)
{
  apr_hash_t *statushash;
  svn_revnum_t youngest = SVN_INVALID_REVNUM;
  apr_array_header_t *statusarray;

  status = NULL;

  if(subpool == NULL)
    return false;

  svn_error_t *err = svn_client_status (&statushash,
                                        &youngest,
                                        path.c_str (),
                                        auth_baton.auth_obj,
                                        0,
                                        1,
                                        0,
                                        0,
                                        subpool);

  // Error present if function is not under version control
  if(err != NULL)
    return true;

  /* Convert the unordered hash to an ordered, sorted array */
  statusarray = apr_hash_sorted_keys (statushash,
                                      svn_sort_compare_items_as_paths,
                                      subpool);

  const svn_item_t *item;

  // We do not recurse, 
  // so only the first entry is interesting to us.
  item = &APR_ARRAY_IDX (statusarray, 0, const svn_item_t);
  status = (svn_wc_status_t *) item->value;

  if (!status->entry)           // not under revision control
    return true;

  return true;
}

svn_wc_status_kind
SvnFileStatus::getFileStatus () const
{
  if (!status)
    return svn_wc_status_none;

  return status->text_status;
}

svn_wc_status_kind
SvnFileStatus::getFilePropStatus () const
{
  if (!status)
    return svn_wc_status_none;

  return status->prop_status;
}

svn_revnum_t
SvnFileStatus::getRevision () const
{
  if (!status || !status->entry)
    return SVN_INVALID_REVNUM;

  return status->entry->revision;
}

svn_revnum_t
SvnFileStatus::getLastChange () const
{
  if (!status || !status->entry)
    return SVN_INVALID_REVNUM;

  return status->entry->cmt_rev;
}

