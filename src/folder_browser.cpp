
#include "include.h"
#include "folder_browser.h"
#include "folder_item_data.h"
#include "svn_wc.h"
#include "wx/filename.h"
#include "wx/dir.h"
#include "wx/imaglist.h"

#include "res/bitmaps/computer.xpm"
#include "res/bitmaps/folder.xpm"
#include "res/bitmaps/open_folder.xpm"

//IMPLEMENT_DYNAMIC_CLASS (FolderBrowser, wxControl)
BEGIN_EVENT_TABLE (FolderBrowser, wxControl)
  EVT_TREE_ITEM_EXPANDING (-1, FolderBrowser::OnExpandItem)
  EVT_TREE_ITEM_COLLAPSED (-1, FolderBrowser::OnCollapseItem)
  EVT_SIZE (FolderBrowser::OnSize) 
END_EVENT_TABLE () 

enum
{
    FOLDER_IMAGE_COMPUTER = 0,
    FOLDER_IMAGE_FOLDER,
    FOLDER_IMAGE_OPEN_FOLDER,
    FOLDER_IMAGE_COUNT
};
    

FolderBrowser::FolderBrowser (wxWindow * parent,
                              apr_pool_t *pool,
                              const wxWindowID id,
                              const wxPoint & pos,
                              const wxSize & size,
                              const wxString & name)
  : wxControl (parent, id, pos, size, 0, wxDefaultValidator, name)
{
  m_pool = pool;

  SetBackgroundColour (wxSystemSettings::GetColour (wxSYS_COLOUR_3DFACE));
 
  m_imageList = new wxImageList (16, 16, TRUE);
  m_imageList->Add (wxIcon (computer_xpm));
  m_imageList->Add (wxIcon (folder_xpm));
  m_imageList->Add (wxIcon (open_folder_xpm));

  m_treeCtrl = new wxTreeCtrl (this, -1, pos, size, 
                   wxTR_HAS_BUTTONS);
  m_treeCtrl->AssignImageList (m_imageList);

  FolderItemData* data = new FolderItemData (FOLDER_TYPE_WORKBENCH);
  m_rootId = m_treeCtrl->AddRoot (_("Workbench"), FOLDER_IMAGE_COMPUTER, 
                                  FOLDER_IMAGE_COMPUTER, data);
  m_treeCtrl->SetItemHasChildren (m_rootId, TRUE);
}

FolderBrowser::~FolderBrowser ()
{
}

UniqueArrayString & FolderBrowser::GetWorkbenchItems ()
{
  return m_workbenchItems;
}

void
FolderBrowser::Refresh ()
{
  m_treeCtrl->Collapse (m_rootId);
  m_treeCtrl->Expand (m_rootId);
}

const bool
FolderBrowser::RemoveProject ()
{
  bool success = FALSE;

  wxTreeItemId id = m_treeCtrl->GetSelection ();

  if(id.IsOk ())
  {
    FolderItemData* data = (FolderItemData*) m_treeCtrl->GetItemData (id);

    if( data->getFolderType() == FOLDER_TYPE_PROJECT )
    {
      wxString path = data->getPath ();
      m_treeCtrl->Delete (id);
      m_workbenchItems.Remove (path.c_str ());
      success = TRUE;
    }
  }

  return success;
}

void
FolderBrowser::AddProject (const wxString & path)
{
  m_workbenchItems.Add (path);
  //TODO Refresh();
}

void
FolderBrowser::OnSize (wxSizeEvent & WXUNUSED (event))
{
  if(m_treeCtrl)
  {
    wxSize size = GetClientSize ();
    m_treeCtrl->SetSize (0, 0, size.x, size.y);
  }
}

void
FolderBrowser::OnExpandItem (wxTreeEvent & event)
{
  wxTreeItemId parentId = event.GetItem ();
  int type = FOLDER_TYPE_INVALID;

  if(m_rootId == 0)
  {
    m_rootId = m_treeCtrl->GetRootItem ();
  }

  FolderItemData* parentData = (FolderItemData*) 
                                m_treeCtrl->GetItemData (parentId);
  if(parentData)
  {
      type = parentData->getFolderType ();
  }

  switch(type)
  {
    case FOLDER_TYPE_WORKBENCH:
    {
    const int count = m_workbenchItems.GetCount ();
    int index;

    for(index = 0; index < count; index++)
    {
      const wxString& path = m_workbenchItems.Item (index);
      FolderItemData* data= new FolderItemData (FOLDER_TYPE_PROJECT, 
                                                path, path, TRUE);
      wxTreeItemId newId = m_treeCtrl->AppendItem (parentId, path, 
                                                   FOLDER_IMAGE_FOLDER, 
                                                   FOLDER_IMAGE_FOLDER, 
                                                   data);
        m_treeCtrl->SetItemHasChildren (newId, TRUE);
        m_treeCtrl->SetItemImage (newId, FOLDER_IMAGE_OPEN_FOLDER,  
                                  wxTreeItemIcon_Expanded);
    }
    }
    break;

    case FOLDER_TYPE_PROJECT:
    case FOLDER_TYPE_NORMAL:
    {
    const wxString& parentPath = parentData->getPath ();
    wxDir dir (parentPath);

    if(dir.IsOpened ())
    {
      wxString filename;

      bool ok = dir.GetFirst(&filename, wxEmptyString, 
                             wxDIR_DIRS);

      while(ok)
      {
        if(filename != SVN_WC_ADM_DIR_NAME)
        {
            wxFileName fullPath(parentPath, filename, wxPATH_NATIVE);

            FolderItemData * data = 
            new FolderItemData (FOLDER_TYPE_NORMAL, 
                                fullPath.GetFullPath(), 
                                filename, TRUE);

            wxTreeItemId newId = 
            m_treeCtrl->AppendItem(
                parentId, filename, 
                FOLDER_IMAGE_FOLDER, 
                FOLDER_IMAGE_FOLDER, data);
            m_treeCtrl->SetItemHasChildren (newId, TRUE);
            m_treeCtrl->SetItemImage (newId, FOLDER_IMAGE_OPEN_FOLDER,  
                                      wxTreeItemIcon_Expanded);
        }

        ok = dir.GetNext (&filename);
      }
    }
    }
    break;
  }   

  m_treeCtrl->SortChildren (parentId);
}

void
FolderBrowser::OnCollapseItem (wxTreeEvent & event)
{
  wxTreeItemId parentId = event.GetItem ();

  long cookie;
  wxTreeItemId id = m_treeCtrl->GetFirstChild (parentId, cookie);

  while(id.IsOk())
  {
    m_treeCtrl->Delete (id);
    id=m_treeCtrl->GetFirstChild (parentId, cookie);
  }

  m_treeCtrl->SetItemHasChildren (parentId, TRUE);
}
  
wxString 
FolderBrowser::GetPath ()
{
  const wxTreeItemId id = m_treeCtrl->GetSelection ();
  
  if(!id.IsOk())
  {
    return "";
  }
  else
  {
    FolderItemData* data = (FolderItemData*)m_treeCtrl->GetItemData (id);
    return data->getPath ();
  }
}

void 
FolderBrowser::SetPath (const wxString& path)
{
  //TODO 
}
