
#include "status.h"

namespace Svn
{

Status::Status ()
{
}

Status::~Status ()
{
}

void 
Status::Reset ()
{
  bVersioned = false;
  status = NULL;
  youngest = SVN_INVALID_REVNUM;
  svn_revnum_t youngest = SVN_INVALID_REVNUM;  
}

bool
Status::IsVersioned ()
{
  return bVersioned;
}

bool
Status::LoadPath (char * sFilePath)
{
  const svn_item_t *item;

  sPath = sFilePath;
  Reset ();

  Err = svn_client_status (&statushash,
                                        &youngest,
                                        sPath,
                                        Authenticate (),
                                        0,
                                        1,
                                        0,
                                        0,
                                        pool);

  // Error present if function is not under version control
  if(Err != NULL)
    return false;
    
  /* Convert the unordered hash to an ordered, sorted array */
  statusarray = apr_hash_sorted_keys (statushash,
                                      svn_sort_compare_items_as_paths,
                                      pool);

  // We do not recurse, 
  // so only the first entry is interesting to us.
  item = &APR_ARRAY_IDX (statusarray, 0, const svn_item_t);
  status = (svn_wc_status_t *) item->value;

  if (!status->entry)           // not under revision control
    return false;
  
  bVersioned = true;

  return true;
}

long
Status::Revision ()
{
  if(bVersioned == false)
    return -1;

  return status->entry->revision;
}
  
long
Status::LastChanged ()
{
  if(bVersioned == false)
    return -1;

  return status->entry->cmt_rev;
}

char *
Status::StatusText ()
{
  char * sStatus = NULL;
  
  if(bVersioned == false)
    return NULL;

  switch (status->text_status)
  {
    case svn_wc_status_none:
      sStatus = "Non-svn";
      break;
    case svn_wc_status_normal:
      sStatus = "Normal";
      break;
    case svn_wc_status_added:
      sStatus = "Added";
      break;
    case svn_wc_status_absent:
      sStatus = "Absent";
      break;
    case svn_wc_status_deleted:
      sStatus = "Deleted";
      break;
    case svn_wc_status_replaced:
      sStatus = "Replaced";
      break;
    case svn_wc_status_modified:
      sStatus = "Modified";
      break;
    case svn_wc_status_merged:
      sStatus = "Merged";
      break;
    case svn_wc_status_conflicted:
      sStatus = "Conflicted";
      break;
    case svn_wc_status_unversioned:
    default:
      sStatus = "Unversioned";
      break;
  }
  
  return sStatus;
}

char *
Status::StatusProp ()
{
  //return status->prop_status;
  return NULL;
}

}
