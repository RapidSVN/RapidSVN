
#include "status.h"

namespace svn
{

Status::Status ()
{
}

Status::~Status ()
{
}

void 
Status::reset ()
{
  versioned = false;
  youngest = SVN_INVALID_REVNUM;
}

bool
Status::isVersioned ()
{
  return versioned;
}

void
Status::loadPath (const char * path)
{
  const svn_item_t *item;

  filePath = path;
  reset ();

  Err = svn_client_status (&statushash,
                           &youngest,
                           filePath.c_str (),
                           authenticate (),
                           0,
                           1,
                           0,
                           0,
                           pool);

  // Error present if function is not under version control
  if(Err != NULL)
  {
    versioned = false;
    return;
  }
    
  /* Convert the unordered hash to an ordered, sorted array */
  statusarray = apr_hash_sorted_keys (statushash,
                                      svn_sort_compare_items_as_paths,
                                      pool);

  // We do not recurse, so only the first entry is interesting to us.
  item = &APR_ARRAY_IDX (statusarray, 0, const svn_item_t);
  status = (svn_wc_status_t *) item->value;

  if (!status->entry)  // not under revision control
    versioned = false;
  else
    versioned = true;
}

unsigned long
Status::revision ()
{
  if(versioned == false)
    throw EntryNotVersioned ();

  return status->entry->revision;
}
  
unsigned long
Status::lastChanged ()
{
  if(versioned == false)
    throw EntryNotVersioned ();

  return status->entry->cmt_rev;
}

const char *
Status::statusText ()
{
  _statusText = "";
  
  if(versioned == false)
    throw EntryNotVersioned ();

  switch (status->text_status)
  {
    case svn_wc_status_none:
      _statusText = "Non-svn";
      break;
    case svn_wc_status_normal:
      _statusText = "Normal";
      break;
    case svn_wc_status_added:
      _statusText = "Added";
      break;
    case svn_wc_status_absent:
      _statusText = "Absent";
      break;
    case svn_wc_status_deleted:
      _statusText = "Deleted";
      break;
    case svn_wc_status_replaced:
      _statusText = "Replaced";
      break;
    case svn_wc_status_modified:
      _statusText = "Modified";
      break;
    case svn_wc_status_merged:
      _statusText = "Merged";
      break;
    case svn_wc_status_conflicted:
      _statusText = "Conflicted";
      break;
    case svn_wc_status_unversioned:
    default:
      _statusText = "Unversioned";
      break;
  }
  
  return _statusText.c_str ();
}

svn_wc_status_kind
Status::statusProp ()
{
  if(versioned == false)
    throw EntryNotVersioned ();
  
  return status->prop_status;
}

}
