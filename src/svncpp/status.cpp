
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
Status::loadPath (char * path)
{
  const svn_item_t *item;

  filePath = path;
  reset ();

  Err = svn_client_status (&statushash,
                           &youngest,
                           filePath,
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
    throw ClientException (Err);
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

char *
Status::statusText ()
{
  char * statusText = NULL;
  
  if(versioned == false)
    throw EntryNotVersioned ();

  switch (status->text_status)
  {
    case svn_wc_status_none:
      statusText = "Non-svn";
      break;
    case svn_wc_status_normal:
      statusText = "Normal";
      break;
    case svn_wc_status_added:
      statusText = "Added";
      break;
    case svn_wc_status_absent:
      statusText = "Absent";
      break;
    case svn_wc_status_deleted:
      statusText = "Deleted";
      break;
    case svn_wc_status_replaced:
      statusText = "Replaced";
      break;
    case svn_wc_status_modified:
      statusText = "Modified";
      break;
    case svn_wc_status_merged:
      statusText = "Merged";
      break;
    case svn_wc_status_conflicted:
      statusText = "Conflicted";
      break;
    case svn_wc_status_unversioned:
    default:
      statusText = "Unversioned";
      break;
  }
  
  return statusText;
}

svn_wc_status_kind
Status::statusProp ()
{
  if(versioned == false)
    throw EntryNotVersioned ();
  
  return status->prop_status;
}

}
