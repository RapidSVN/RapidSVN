
#include "include.h"
#include "wx/imaglist.h"
#include "wx/filename.h"
#include "rapidsvn_app.h"

#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__)
#include "icons.h"
#include "bitmaps.h"
#endif

/**
 * Number of items in the IMAGE_INDEX table.
 * This should be large enough to include the range of status codes.
 */
#define N_STATUS_KIND  21

/**
 * The index from where there will be only images not related 
 * to the status.
 */
#define N_START_EXTRA_IMGS 15

#define IMG_INDX_FOLDER   N_START_EXTRA_IMGS
#define IMG_INDX_VERSIONED_FOLDER (N_START_EXTRA_IMGS + 1)
#define IMG_INDX_SORT_DOWN (N_START_EXTRA_IMGS + 2)
#define IMG_INDX_SORT_UP (N_START_EXTRA_IMGS + 3)

/**
 * This table holds information about image index in a image list.
 * It will be accessed using a status code as an index.
 * For every index that equals a valid status code, there should be
 * a valid value which represents an index in the image list.
 */
int IMAGE_INDEX[N_STATUS_KIND];

/**
 * Tags for wxConfig file settings, defined here to avoid duplicate
 * hard coded strings.
 */
static const char* pSortColumnTag = "/FileListCtrl/SortColumn";
static const char* pSortOrderTag = "/FileListCtrl/SortOrder";
static const char* pSortColumn0Width = "/FileListCtrl/Column0Width";
static const char* pSortColumn1Width = "/FileListCtrl/Column1Width";
static const char* pSortColumn2Width = "/FileListCtrl/Column2Width";
static const char* pSortColumn3Width = "/FileListCtrl/Column3Width";
static const char* pSortColumn4Width = "/FileListCtrl/Column4Width";

static int GetImageIndex(int Index)
/*
  A safe wrapper for getting images - avoids array bounds
  exceptions.
*/
{
  if ((Index >= 0) && (Index <= N_STATUS_KIND))
    return IMAGE_INDEX[Index];
  else
    return 0;    
}

BEGIN_EVENT_TABLE (FileListCtrl, wxListCtrl)
EVT_KEY_DOWN (FileListCtrl::OnKeyDown)
EVT_LIST_ITEM_ACTIVATED (-1, FileListCtrl::OnItemActivated)
EVT_LIST_ITEM_RIGHT_CLICK (LIST_CTRL, FileListCtrl::OnItemRightClk)
EVT_LIST_COL_CLICK(LIST_CTRL, FileListCtrl::OnColumnLeftClick)
END_EVENT_TABLE ()

FileListCtrl::FileListCtrl (wxWindow * parent,
                                              apr_pool_t * __pool,
                                              const wxWindowID id,
                                              const wxPoint & pos,
                                              const wxSize & size):
wxListCtrl (parent, id, pos, size, wxLC_REPORT)
{
  pool = __pool;
  m_imageListSmall = new wxImageList (16, 16, TRUE);

  // add images to image list
  m_imageListSmall->Add (wxICON (nonsvn_file));
  m_imageListSmall->Add (wxICON (normal_file));
  m_imageListSmall->Add (wxICON (added_file));
  m_imageListSmall->Add (wxICON (absent_file));
  m_imageListSmall->Add (wxICON (deleted_file));
  m_imageListSmall->Add (wxICON (replaced_file));
  m_imageListSmall->Add (wxICON (modified_file));
  m_imageListSmall->Add (wxICON (merged_file));
  m_imageListSmall->Add (wxICON (conflicted_file));

  m_imageListSmall->Add (wxICON (folder));
  m_imageListSmall->Add (wxICON (versioned_folder));

  m_imageListSmall->Add (wxICON (sort_down));
  m_imageListSmall->Add (wxICON (sort_up));

  // set the indexes
  IMAGE_INDEX[svn_wc_status_none] = 0;
  IMAGE_INDEX[svn_wc_status_unversioned] = 0;
  IMAGE_INDEX[svn_wc_status_normal] = 1;
  IMAGE_INDEX[svn_wc_status_added] = 2;
  IMAGE_INDEX[svn_wc_status_absent] = 3;
  IMAGE_INDEX[svn_wc_status_deleted] = 4;
  IMAGE_INDEX[svn_wc_status_replaced] = 5;
  IMAGE_INDEX[svn_wc_status_modified] = 6;
  IMAGE_INDEX[svn_wc_status_merged] = 7;
  IMAGE_INDEX[svn_wc_status_conflicted] = 8;

  IMAGE_INDEX[IMG_INDX_FOLDER] = 9;
  IMAGE_INDEX[IMG_INDX_VERSIONED_FOLDER] = 10;

  IMAGE_INDEX[IMG_INDX_SORT_DOWN] = 11;
  IMAGE_INDEX[IMG_INDX_SORT_UP] = 12;

  // set this file list control to use the image list
  SetImageList (m_imageListSmall, wxIMAGE_LIST_SMALL);

  m_path.Empty ();
  
  // Get settings from config file:
  wxConfigBase *pConfig = wxConfigBase::Get();
  SortColumn = pConfig->Read(pSortColumnTag, (long) 0);
  SortIncreasing = pConfig->Read(pSortOrderTag, (long) 1)
    ? true : false;
  
  wxListItem itemCol;
  itemCol.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE;
  itemCol.m_text = wxT("Name");
  itemCol.m_image = -1;
  InsertColumn (0, itemCol);

  itemCol.m_text = wxT("Revision");
  InsertColumn (1, itemCol);

  itemCol.m_text = wxT("Last Changed");
  InsertColumn (2, itemCol);

  itemCol.m_text = wxT("Status");
  InsertColumn (3, itemCol);

  itemCol.m_text = wxT("Prop Status");
  InsertColumn (4, itemCol);

  SetColumnImages();

  // Set the column widths stored in the config:
  SetColumnWidth (0, pConfig->Read(pSortColumn0Width, (long) 150));
  SetColumnWidth (1, pConfig->Read(pSortColumn1Width, (long) 75));
  SetColumnWidth (2, pConfig->Read(pSortColumn2Width, (long) 100));
  SetColumnWidth (3, pConfig->Read(pSortColumn3Width, (long) 75));
  SetColumnWidth (4, pConfig->Read(pSortColumn4Width, (long) 75));
}

FileListCtrl::~FileListCtrl ()
{
  // Write settings to config file:
  wxConfigBase *pConfig = wxConfigBase::Get();
  pConfig->Write(pSortColumnTag, (long) SortColumn);
  pConfig->Write(pSortOrderTag, (long) (SortIncreasing ? 1 : 0));
  pConfig->Write(pSortColumn0Width, (long) GetColumnWidth(0));
  pConfig->Write(pSortColumn1Width, (long) GetColumnWidth(1));
  pConfig->Write(pSortColumn2Width, (long) GetColumnWidth(2));
  pConfig->Write(pSortColumn3Width, (long) GetColumnWidth(3));
  pConfig->Write(pSortColumn4Width, (long) GetColumnWidth(4));

  delete m_imageListSmall;
}

int wxCALLBACK FileListCtrl::wxListCompareFunction(
  long item1, long item2, long sortData)
{
  svn::Status* ps1 = (svn::Status*) item1;
  svn::Status* ps2 = (svn::Status*) item2;
  FileListCtrl* pList = (FileListCtrl*) sortData;
  
  if (ps1 && ps2)   // Defensive programming.
    return CompareItems(ps1, ps2, pList->SortColumn, pList->SortIncreasing);
  else
    return 0;
}

int wxCALLBACK FileListCtrl::CompareItems(
  svn::Status* ps1, svn::Status* ps2, int SortColumn, bool SortIncreasing)
{
  int rc = 0;
  unsigned long r1, r2;
  bool ok1, ok2;
  
  switch (SortColumn)
  {
    case 0: // Name
      // Directories always precede files:
      if (ps1->isDir() && !ps2->isDir())
        rc = -1;
      else if (!ps1->isDir() && ps2->isDir())
        rc = 1;
      else
      {
        rc = wxString(ps1->getPath()).CmpNoCase(ps2->getPath());
        if (!SortIncreasing)
          rc = rc * -1; // Reverse the sort order.
      }
      break;
    
    case 1: // Revision
    case 2: // Last change
      ok1 = ok2 = true;
      
      switch(SortColumn)
      {
        case 1: // Revision
          try { r1 = ps1->revision(); }
          catch(svn::EntryNotVersioned&) { ok1 = false; }
          try { r2 = ps2->revision(); }
          catch(svn::EntryNotVersioned&) { ok2 = false; }
          break;
          
        case 2: // Last change
          try { r1 = ps1->lastChanged(); }
          catch(svn::EntryNotVersioned&) { ok1 = false; }
          try { r2 = ps2->lastChanged(); }
          catch(svn::EntryNotVersioned&) { ok2 = false; }
          break;
      }
      
      // Unversioned always come last.
      if (ok1 && !ok2)
        rc = -1;
      else if (ok2 && !ok1)
        rc = 1;
      else if (r1 == r2)
        rc = 0;
      else
      {
        rc = r1 > r2 ? 1 : -1;
        if (!SortIncreasing)
          rc = rc * -1; // Reverse the sort order.
      } 
      break;

    case 3: // Status
      rc = wxString(ps1->textDescription()).CmpNoCase(ps2->textDescription());
      if (!SortIncreasing)
        rc = rc * -1; // Reverse the sort order.
      break;

    case 4: // Prop status
      rc = wxString(ps1->propDescription()).CmpNoCase(ps2->propDescription());
      if (!SortIncreasing)
        rc = rc * -1; // Reverse the sort order.
      break;
  }

  // If the items are equal, we revert to a sort on the item name,
  // being careful to avoid recursion.
  if ((SortColumn != 0) && (rc == 0)) 
    rc = CompareItems(ps1, ps2, 0, SortIncreasing);
   
  return rc;
}

void
FileListCtrl::UpdateFileList (const wxString & path)
{
  svn::Status* pStatus = NULL;
  m_path = path;

  // delete all the items in the list to display the new ones
  DeleteAllItems ();

  // cycle through the files and folders in the current path
  wxFileName fullpath (path, wxT ("*"));
  wxString name;
  wxString f =
    wxFindFirstFile (fullpath.GetFullPath ().c_str (), wxDIR | wxFILE);

  wxLogStatus (_T ("Listing entries in '%s'"), path.c_str ());

  // Hide the list to speed up inserting
  Hide(); 

  while (!f.IsEmpty ())
  {
    f = wxFindNextFile ();
    if (f.IsEmpty ())
      continue;

    int i = GetItemCount ();
    name = wxFileNameFromPath (f);
    wxString text;

    pStatus = new svn::Status;
    
    bool isdir = wxDirExists (f);
    pStatus->loadPath (UnixPath (f), isdir);

    // in the parent directory
    if (name == ".." || name == SVN_WC_ADM_DIR_NAME)
      continue;

    if (isdir)    // a directory
    {
      if (pStatus->isVersioned ())
        InsertItem (i, name, GetImageIndex(IMG_INDX_VERSIONED_FOLDER));
      else
        InsertItem (i, name, GetImageIndex(IMG_INDX_FOLDER));
    }
    else
      InsertItem (i, name, GetImageIndex(fileStatus (pStatus)));
      
    // The item data will be used to sort the list:
    SetItemData (i, (long) pStatus);  // The control now owns this data
                                      // and must delete it in due course.

    if (pStatus->isVersioned ())
      text.Printf (_T ("%ld"), pStatus->revision ());
    else
      text = _T (" ");

    SetItem (i, 1, text);

    if (pStatus->isVersioned ())
      text.Printf (_T ("%ld"), pStatus->lastChanged ());
    else
      text = _T (" ");

    SetItem (i, 2, text);

    text = pStatus->textDescription ();
    SetItem (i, 3, text);

    text = pStatus->propDescription ();
    SetItem (i, 4, text);
  }

  SortItems(wxListCompareFunction, (long) this);
  
  Show();
}

void
FileListCtrl::OnKeyDown (wxKeyEvent & event)
{
  if (event.GetKeyCode () == WXK_F5)
  {
    // F5 was pressed, force a refresh
    UpdateFileList (wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->
                    GetPath ());
  }
  else
  {
    event.Skip ();
  }
}

void
FileListCtrl::OnItemActivated (wxListEvent & event)
{
  wxString name = GetItemText (event.GetIndex ());
  wxFileName fullpath (m_path, name);

  if (fullpath.DirExists ())
  {
    // If the path is a directory, change the path in the folder browser.
    // There is no need to call UpdateFileList() as SetPath() will 
    // trigger a EVT_TREE_SEL_CHANGED message in the fiolder browser.

    wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->SetPath (fullpath.
                                                              GetFullPath ());
  }
}

void
FileListCtrl::OnItemRightClk (wxListEvent & event)
{
  int flag;
  wxPoint screenPt = wxGetMousePosition ();
  wxPoint clientPt = ScreenToClient (screenPt);

  long index = HitTest (clientPt, flag);
  if (index >= 0)
  {
    ShowMenu (index, clientPt);
  }
}

void
FileListCtrl::OnColumnLeftClick (wxListEvent & event)
{
  int Column = event.GetColumn();

  // A second click on the current sort column reverses the order of sorting.
  if (Column == SortColumn)
    SortIncreasing = !SortIncreasing;
  else
  {
    SortColumn = Column;
    SortIncreasing = true;
  }
  
  SetColumnImages();  
  SortItems(wxListCompareFunction, (long) this);
}

void
FileListCtrl::SetColumnImages()
{
  // Update the column titles to reflect the sort column.
  for (int i = 0; i < GetColumnCount(); i++)
  {
    wxListItem LI;
    LI.m_mask = wxLIST_MASK_IMAGE;
    if (i == SortColumn)
      LI.m_image = SortIncreasing ? 
        IMAGE_INDEX[IMG_INDX_SORT_DOWN] : IMAGE_INDEX[IMG_INDX_SORT_UP]; 
    else
      LI.m_image = -1; 
    SetColumn(i, LI);
  }
}

svn_wc_status_kind
FileListCtrl::fileStatus (svn::Status * status)
{
  if(status->textType () != svn_wc_status_none &&
     status->textType () != svn_wc_status_normal)
    return status->textType ();

  return status->propType ();
}

void
FileListCtrl::ShowMenu (long index, wxPoint & pt)
{
  wxFileName filepath (m_path, GetItemText (index));
  wxMenu popup_menu;
  wxString path = filepath.GetFullPath ();

  buildMenu (popup_menu, UnixPath (path));

  PopupMenu (&popup_menu, pt);
}

void
FileListCtrl::buildMenu (wxMenu & menu, const wxString & path)
{
  wxMenuItem *pItem;

  pItem = new wxMenuItem (&menu, ID_Update, _T ("Update"));
  pItem->SetBitmap (wxBITMAP (update));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_Commit, _T ("Commit"));
  pItem->SetBitmap (wxBITMAP (commit));
  menu.Append (pItem);

  menu.AppendSeparator ();

  pItem = new wxMenuItem (&menu, ID_Revert, _T ("Revert"));
  pItem->SetBitmap (wxBITMAP (revert));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_Resolve, _T ("Resolve"));
  pItem->SetBitmap (wxBITMAP (resolve));
  menu.Append (pItem);

  menu.AppendSeparator ();

  pItem = new wxMenuItem (&menu, ID_Info, _T ("Info"));
  pItem->SetBitmap (wxBITMAP (info));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_Log, _T ("Log"));
  pItem->SetBitmap (wxBITMAP (log));
  menu.Append (pItem);
}

const IndexArray &
FileListCtrl::GetSelectedItems ()
{
  int sel = GetSelectedItemCount ();

  if (sel <= 0)
    indx_arr.Clear ();
  else
  {
    indx_arr.Alloc (sel);

    long item = -1;
    for (;;)
    {
      item = GetNextItem (item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if (item == -1)
        break;

      indx_arr.Add (item);
    }
  }

  sel = indx_arr.GetCount ();

  return indx_arr;
}

apr_array_header_t *
FileListCtrl::GetTargets (apr_pool_t * pool)
{
  IndexArray arr = GetSelectedItems ();
  size_t i;
  apr_array_header_t *targets = apr_array_make (pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  for (i = 0; i < arr.GetCount (); i++)
  {
    wxFileName fname (m_path, GetItemText (arr[i]));
    wxString path = fname.GetFullPath ();
    const char *target = apr_pstrdup (pool, UnixPath (path));

    (*((const char **) apr_array_push (targets))) = target;
  }

  return targets;
}

void
FileListCtrl::GetFullUnixPath (long index, wxString & fullpath)
{
  fullpath.Empty ();

  if (index < 0)
    return;

  wxFileName fname (wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->
                    GetPath (), GetItemText (index));
  fullpath = fname.GetFullPath ();
  UnixPath (fullpath);
}

void 
FileListCtrl::DeleteAllItems()
{
  // Delete the item data before deleting the items:
  for (int i = 0; i < GetItemCount(); i++)
  {
    svn::Status* p = (svn::Status*) GetItemData(i);
    if (p)
      delete p;
  }
  wxListCtrl::DeleteAllItems();
}

void 
FileListCtrl::DeleteItem( long item )
{
  svn::Status* p = (svn::Status*) GetItemData(item);
  if (p)
    delete p;
  wxListCtrl::DeleteItem(item);
}

