/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#include "include.hpp"
#include "folder_browser.hpp"
#include "folder_item_data.hpp"
#include "svn_wc.h"
#include "wx/filename.h"
#include "wx/dir.h"
#include "wx/imaglist.h"
#include "rapidsvn_app.hpp"

#include "res/bitmaps/computer.xpm"
#include "res/bitmaps/versioned_folder.xpm"
#include "res/bitmaps/open_folder.xpm"
#include "res/bitmaps/update.xpm"
#include "res/bitmaps/commit.xpm"
#include "res/bitmaps/earth.xpm"

//IMPLEMENT_DYNAMIC_CLASS (FolderBrowser, wxControl)
BEGIN_EVENT_TABLE (FolderBrowser, wxControl)
  EVT_TREE_ITEM_EXPANDING (-1, FolderBrowser::OnExpandItem)
  EVT_TREE_ITEM_COLLAPSED (-1, FolderBrowser::OnCollapseItem)
  EVT_TREE_ITEM_RIGHT_CLICK (-1, FolderBrowser::OnItemRightClk)
  EVT_SIZE (FolderBrowser::OnSize) 
END_EVENT_TABLE () 

enum
{
  FOLDER_IMAGE_COMPUTER = 0,
  FOLDER_IMAGE_FOLDER,
  FOLDER_IMAGE_OPEN_FOLDER,
  FOLDER_IMAGE_EARTH,
  FOLDER_IMAGE_COUNT
};

const unsigned int FolderBrowser::MAXLENGTH_PROJECT = 25;

FolderBrowser::FolderBrowser (wxWindow * parent, const wxWindowID id,
                              const wxPoint & pos, const wxSize & size,
                              const wxString & name)
  : wxControl (parent, id, pos, size, wxCLIP_CHILDREN, wxDefaultValidator, name)
{
  m_imageList = new wxImageList (16, 16, TRUE);
  m_imageList->Add (wxIcon (computer_xpm));
  m_imageList->Add (wxIcon (versioned_folder_xpm));
  m_imageList->Add (wxIcon (open_folder_xpm));
  m_imageList->Add (wxIcon (earth_xpm));

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
      Delete (id);
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
        wxString label;
        
        if( path.length() < MAXLENGTH_PROJECT )
        {
          label = path;
        }
        else
        {
          label = "..." + path.Right(MAXLENGTH_PROJECT );
        }

        wxTreeItemId newId = m_treeCtrl->AppendItem (parentId, label, 
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
        bool parentHasSubdirectories = false;
        
        while(ok)
        {
          if(filename != SVN_WC_ADM_DIR_NAME)
          {
            parentHasSubdirectories = true;
            
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
            m_treeCtrl->SetItemHasChildren (newId, HasSubdirectories(fullPath.GetFullPath()) );
            m_treeCtrl->SetItemImage (newId, FOLDER_IMAGE_OPEN_FOLDER,  
                                      wxTreeItemIcon_Expanded);
          }
          
          ok = dir.GetNext (&filename);
        }
        
        // If no subdirectories, don't show the expander
        if (!parentHasSubdirectories)
          m_treeCtrl->SetItemHasChildren(parentId, FALSE);
        
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
    Delete (id);
    id=m_treeCtrl->GetFirstChild (parentId, cookie);
  }

  m_treeCtrl->SetItemHasChildren (parentId, TRUE);
}
  
void
FolderBrowser::OnItemRightClk (wxTreeEvent & event)
{
  int flag;
  wxPoint screenPt = wxGetMousePosition ();
  wxPoint clientPt = ScreenToClient (screenPt);

  long index = m_treeCtrl->HitTest (clientPt, flag);
  if (index >= 0)
  {
    ShowMenu (index, clientPt);
  }
}

const
wxString 
FolderBrowser::GetPath () const
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

const FolderItemData *
FolderBrowser::GetSelection () const
{
  const wxTreeItemId id = m_treeCtrl->GetSelection ();
  
  if(!id.IsOk())
  {
    return NULL;
  }
  else
  {
    FolderItemData* data = (FolderItemData*)m_treeCtrl->GetItemData (id);
    return data;
  }
}

bool
FolderBrowser::HasSubdirectories (const wxString & path)
{
  wxString filename;

  wxDir dir (path);

  bool ok = dir.GetFirst(&filename, wxEmptyString, 
                           wxDIR_DIRS);
  if(!dir.IsOpened ())
    return false;
  
  if (!ok)
    return false;
  
  while (ok)
  {
    if (filename != SVN_WC_ADM_DIR_NAME)
      return true;
    ok = dir.GetNext (&filename);
  }

  return false;
}

void
FolderBrowser::ShowMenu (long index, wxPoint & pt)
{
  const FolderItemData * data = GetSelection ();
  if (!data)
  {
    return;
  }

  // create menu
  wxMenu menu;
  wxMenuItem *item;
  int type =  data->getFolderType ();

  item = new wxMenuItem (&menu, ID_AddProject, _T ("&Add to Workbench..."));
  menu.Append (item);

  if (type==FOLDER_TYPE_PROJECT)
  {
    item = new wxMenuItem (&menu, ID_RemoveProject, _T ("&Remove from Workbench..."));
    menu.Append (item);
  }

  if ((type==FOLDER_TYPE_PROJECT)||(type==FOLDER_TYPE_NORMAL))
  {
    menu.AppendSeparator ();
    item = new wxMenuItem (&menu, ID_Update, _T ("Update"));
    item->SetBitmap (wxBITMAP (update));
    menu.Append (item);
    item = new wxMenuItem (&menu, ID_Commit, _T ("Commit"));
    item->SetBitmap (wxBITMAP (commit));
    menu.Append (item);
  }
  // show menu
  PopupMenu (&menu, pt);
}

void
FolderBrowser::Delete (const wxTreeItemId & id )
{
  if( m_treeCtrl )
  {
    wxTreeItemData * data = m_treeCtrl->GetItemData(id);

    if( data )
    {
      delete data;
      m_treeCtrl->SetItemData(id, NULL);
    }

    m_treeCtrl->Delete(id);
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
