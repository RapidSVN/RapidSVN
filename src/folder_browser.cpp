/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// svncpp
#include "svncpp/context.hpp"
#include "svncpp/wc.hpp"

// wxwindows
#include "wx/wx.h"
#include "wx/filename.h"
#include "wx/dir.h"
#include "wx/imaglist.h"
#include "wx/treectrl.h"

// app
#include "folder_browser.hpp"
#include "folder_item_data.hpp"
#include "ids.hpp"
#include "workbench.hpp"

// bitmaps
#include "res/bitmaps/computer.xpm"
#include "res/bitmaps/versioned_folder.xpm"
#include "res/bitmaps/open_folder.xpm"
#include "res/bitmaps/folder.xpm"
#include "res/bitmaps/nonsvn_open_folder.xpm"
#include "res/bitmaps/update.xpm"
#include "res/bitmaps/commit.xpm"
#include "res/bitmaps/earth.xpm"

enum
{
  FOLDER_IMAGE_COMPUTER = 0,
  FOLDER_IMAGE_FOLDER,
  FOLDER_IMAGE_OPEN_FOLDER,
  FOLDER_IMAGE_NONSVN_FOLDER,
  FOLDER_IMAGE_NONSVN_OPEN_FOLDER,
  FOLDER_IMAGE_EARTH,
  FOLDER_IMAGE_COUNT
};

struct FolderBrowser::Data
{
public:
  wxTreeCtrl* treeCtrl;
  wxTreeItemId rootId;
  wxImageList* imageList;
  wxWindow * window;
  Workbench workbench;
  
  static const unsigned int MAXLENGTH_PROJECT;

  Data (wxWindow * window, const wxPoint & pos, const wxSize & size)
    : window (window)
  {
    imageList = new wxImageList (16, 16, TRUE);
    imageList->Add (wxIcon (computer_xpm));
    imageList->Add (wxIcon (versioned_folder_xpm));
    imageList->Add (wxIcon (open_folder_xpm));
    imageList->Add (wxIcon (folder_xpm));
    imageList->Add (wxIcon (nonsvn_open_folder_xpm));
    imageList->Add (wxIcon (earth_xpm));

    treeCtrl = new wxTreeCtrl (window, -1, pos, size, 
                               wxTR_HAS_BUTTONS);
    treeCtrl->AssignImageList (imageList);

    FolderItemData* data = new FolderItemData (FOLDER_TYPE_WORKBENCH);
    rootId = treeCtrl->AddRoot (_("Workbench"), FOLDER_IMAGE_COMPUTER, 
                                    FOLDER_IMAGE_COMPUTER, data);
    treeCtrl->SetItemHasChildren (rootId, TRUE);
  }

  ~Data ()
  {
    DeleteAllItems ();
  }

  const wxString
  GetPath ()
  {
    const wxTreeItemId id = treeCtrl->GetSelection ();
  
    if(!id.IsOk())
    {
      return "";
    }
    else
    {
      FolderItemData* data = GetItemData (id);
      return data->getPath ();
    }
  }

  const FolderItemData *
  GetSelection () const
  {
    const wxTreeItemId id = treeCtrl->GetSelection ();
  
    if(!id.IsOk())
    {
      return NULL;
    }
    else
    {
      FolderItemData* data = GetItemData (id);
      return data;
    }
  }

  svn::Context *
  GetContext () 
  {
    const FolderItemData * data = GetSelection ();
    if (data == 0)
      return 0;

    svn::Context * context = 0;

    bool ok = true;
    while (context == 0)
    {
      switch (data->getFolderType ())
      {
      case FOLDER_TYPE_NORMAL:
        {
          wxTreeItemId id = data->GetId ();
          wxTreeItemId parentId = treeCtrl->GetParent (id);
          data = GetItemData (parentId);
        }
        break;

      case FOLDER_TYPE_PROJECT:
        context = workbench.GetContext (data->getPath ());
        ok = context != 0;

        break;
      default:
        ok = false;
        break;
      }

      if (!ok)
        break;
    }

    return context;
  }


  void
  ShowMenu (long index, wxPoint & pt)
  {
    const FolderItemData * data = GetSelection ();
    if (!data)
      return;
    const svn::Context * context = GetContext ();

    // create menu
    wxMenu menu;
    wxMenuItem *item;
    int type =  data->getFolderType ();

    item = new wxMenuItem (&menu, ID_AddProject, _("&Add to Workbench..."));
    menu.Append (item);

    if (type==FOLDER_TYPE_PROJECT)
    {
      item = new wxMenuItem (&menu, ID_RemoveProject, _("&Remove from Workbench..."));
      menu.Append (item);

      menu.AppendSeparator ();
      item = new wxMenuItem (&menu, ID_Login, _("Login..."));
      menu.Append (item);

      {
        wxString label;
        wxString username;
        bool enabled = false;

        if (context != 0)
        {
          username = context->getUsername ();
        }

        if (username.length () == 0)
        {
          label = _("Logout");
        }
        else
        {
          enabled = true;
          label.Printf (_("Logout '%s'"), username.c_str ());
        }

        item = new wxMenuItem (&menu, ID_Logout, label);
        item->Enable (enabled);
      }
      menu.Append (item);
    }

    if ((type==FOLDER_TYPE_PROJECT)||(type==FOLDER_TYPE_NORMAL))
    {
      menu.AppendSeparator ();
      item = new wxMenuItem (&menu, ID_Update, _("Update"));
      item->SetBitmap (wxBITMAP (update));
      menu.Append (item);
      item = new wxMenuItem (&menu, ID_Commit, _("Commit"));
      item->SetBitmap (wxBITMAP (commit));
      menu.Append (item);
    }
    // show menu
    window->PopupMenu (&menu, pt);
  }


  bool
  HasSubdirectories (const wxString & path)
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
      if (filename != svn::Wc::ADM_DIR_NAME)
        return true;
      ok = dir.GetNext (&filename);
    }

    return false;
  }

  void Delete (const wxTreeItemId & id)
  {
    if( treeCtrl )
    {
      wxTreeItemData * data = treeCtrl->GetItemData(id);

      if( data )
      {
        delete data;
        treeCtrl->SetItemData(id, NULL);
      }

      treeCtrl->Delete(id);
    }
  }

  void DeleteAllItems ()
  {
    if (treeCtrl)
    {
      // this deletes all children and
      // all of the itemdata
      treeCtrl->Collapse (rootId);
    }
  }

  void
  OnExpandItem (wxTreeEvent & event)
  {
    wxTreeItemId parentId = event.GetItem ();
    int type = FOLDER_TYPE_INVALID;

    if(rootId == 0)
    {
      rootId = treeCtrl->GetRootItem ();
    }

    FolderItemData* parentData = GetItemData (parentId);
    if(parentData)
    {
        type = parentData->getFolderType ();
    }

    switch(type)
    {
      case FOLDER_TYPE_WORKBENCH:
      {
        const int count = workbench.Count ();
        int index;

        for(index = 0; index < count; index++)
        {
          const wxString path (workbench.GetProject (index));
          FolderItemData* data= new FolderItemData (FOLDER_TYPE_PROJECT, 
                                                    path, path, TRUE);
          wxString label;
    
          if( path.length() < Data::MAXLENGTH_PROJECT )
          {
            label = path;
          }
          else
          {
            label = "..." + path.Right(Data::MAXLENGTH_PROJECT );
          }

          wxTreeItemId newId = treeCtrl->AppendItem (parentId, label, 
                                                     FOLDER_IMAGE_FOLDER, 
                                                     FOLDER_IMAGE_FOLDER, 
                                                     data);
          treeCtrl->SetItemHasChildren (newId, TRUE);
          treeCtrl->SetItemImage (newId, FOLDER_IMAGE_OPEN_FOLDER,  
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
            if(filename != svn::Wc::ADM_DIR_NAME)
            {
              parentHasSubdirectories = true;
        
              wxFileName path(parentPath, filename, wxPATH_NATIVE);
              wxString fullPath = path.GetFullPath ();
              const char * fullPath_c = fullPath.c_str ();
              int image = FOLDER_IMAGE_FOLDER;
              int open_image = FOLDER_IMAGE_OPEN_FOLDER;

              if (!svn::Wc::checkWc (fullPath_c))
              {
                image = FOLDER_IMAGE_NONSVN_FOLDER;
                open_image = FOLDER_IMAGE_NONSVN_OPEN_FOLDER;
              }
        
              FolderItemData * data = 
                new FolderItemData (FOLDER_TYPE_NORMAL, 
                                    fullPath, 
                                    filename, TRUE);

              wxTreeItemId newId = 
                treeCtrl->AppendItem(
                  parentId, filename, 
                  image, image, data);
              treeCtrl->SetItemHasChildren (
                newId, HasSubdirectories(fullPath) );
              treeCtrl->SetItemImage (newId, open_image,  
                                        wxTreeItemIcon_Expanded);
            }
      
            ok = dir.GetNext (&filename);
          }
    
          // If no subdirectories, don't show the expander
          if (!parentHasSubdirectories)
            treeCtrl->SetItemHasChildren(parentId, FALSE);
    
        }
      }
      break;
    }   

    treeCtrl->SortChildren (parentId);
  }

  void
  OnCollapseItem (wxTreeEvent & event)
  {
    wxTreeItemId parentId = event.GetItem ();

    long cookie;
    wxTreeItemId id = treeCtrl->GetFirstChild (parentId, cookie);

    while(id.IsOk())
    {
      Delete (id);
      id=treeCtrl->GetFirstChild (parentId, cookie);
    }

    treeCtrl->SetItemHasChildren (parentId, TRUE);
  }

  bool
  SelectFolder (const char * path)
  {
    const FolderItemData * data = GetSelection ();

    if (data == 0)
      return false;

    // is this a realy entry?
    if (!data->isReal ())
      return false;

    // make sure the selected tree is open
    const wxTreeItemId parentId = data->GetId ();

    if (!treeCtrl->IsExpanded(parentId))
    {
      treeCtrl->Expand (parentId);
    }

    // no search through the list of children
    long cookie;
    wxTreeItemId id = treeCtrl->GetFirstChild (parentId, cookie);

    bool success = false;

    while (!success)
    {
      const FolderItemData * data = GetItemData (id);

      // if data is not set we have an invalid id
      if (data == 0)
        break;

      if (data->getPath () == path)
      {
        success = true;
      }
      else
      {
        id = treeCtrl->GetNextChild (parentId, cookie);
      }
    }

    if (success)
    {
      treeCtrl->SelectItem (id);
    }

    return success;
  }

  FolderItemData *
  GetItemData (const wxTreeItemId & id) const
  {
    if (!treeCtrl)
      return 0;

    return static_cast<FolderItemData *>(treeCtrl->GetItemData (id));
  }


};

const unsigned int FolderBrowser::Data::MAXLENGTH_PROJECT = 25;

BEGIN_EVENT_TABLE (FolderBrowser, wxControl)
  EVT_TREE_ITEM_EXPANDING (-1, FolderBrowser::OnExpandItem)
  EVT_TREE_ITEM_COLLAPSED (-1, FolderBrowser::OnCollapseItem)
  EVT_TREE_ITEM_RIGHT_CLICK (-1, FolderBrowser::OnItemRightClk)
  EVT_SIZE (FolderBrowser::OnSize) 
END_EVENT_TABLE () 

FolderBrowser::FolderBrowser (wxWindow * parent, const wxWindowID id,
                              const wxPoint & pos, const wxSize & size,
                              const wxString & name)
  : wxControl (parent, id, pos, size, wxCLIP_CHILDREN, wxDefaultValidator, name)
{
  m = new Data (this, pos, size);
}

FolderBrowser::~FolderBrowser ()
{
  delete m;
}

//REMOVE UniqueArrayString & FolderBrowser::GetWorkbenchItems ()
//REMOVE {
//REMOVE return m->workbenchItems;
//REMOVE }

void
FolderBrowser::Refresh ()
{
  m->treeCtrl->Collapse (m->rootId);
  m->treeCtrl->Expand (m->rootId);
}

const bool
FolderBrowser::RemoveProject ()
{
  bool success = FALSE;

  wxTreeItemId id = m->treeCtrl->GetSelection ();

  if(id.IsOk ())
  {
    FolderItemData* data = (FolderItemData*) m->treeCtrl->GetItemData (id);

    if( data->getFolderType() == FOLDER_TYPE_PROJECT )
    {
      wxString path = data->getPath ();
      m->Delete (id);
      m->workbench.RemoveProject (path.c_str ());
      success = TRUE;
    }
  }

  return success;
}

void
FolderBrowser::AddProject (const char * path)
{
  m->workbench.AddProject (path);
  //TODO Refresh();
}

const
wxString 
FolderBrowser::GetPath () const
{
  return m->GetPath ();
}

const FolderItemData *
FolderBrowser::GetSelection () const
{
  return m->GetSelection ();
}

void
FolderBrowser::OnSize (wxSizeEvent & WXUNUSED (event))
{
  if(m->treeCtrl)
  {
    wxSize size = GetClientSize ();
    m->treeCtrl->SetSize (0, 0, size.x, size.y);
  }
}

void
FolderBrowser::OnExpandItem (wxTreeEvent & event)
{
  m->OnExpandItem (event);
}

void
FolderBrowser::OnCollapseItem (wxTreeEvent & event)
{
  m->OnCollapseItem (event);
}

void
FolderBrowser::OnItemRightClk (wxTreeEvent & event)
{
  int flag;
  wxPoint screenPt = wxGetMousePosition ();
  wxPoint clientPt = ScreenToClient (screenPt);

  long index = m->treeCtrl->HitTest (clientPt, flag);
  if (index >= 0)
  {
    m->ShowMenu (index, clientPt);
  }
}

bool
FolderBrowser::SelectFolder (const char * path)
{
  return m->SelectFolder (path);
}

const size_t
FolderBrowser::GetProjectCount () const
{
  return m->workbench.Count ();
}

const char *
FolderBrowser::GetProject (const size_t index) const
{
  return m->workbench.GetProject (index);
}

svn::Context * 
FolderBrowser::GetContext ()
{
  return m->GetContext ();
}

void
FolderBrowser::SetAuthPerProject (const bool value)
{
  m->workbench.SetAuthPerProject (value);
}

/**
 * @return auth per project setting
 */
const bool 
FolderBrowser::GetAuthPerProject () const
{
  return m->workbench.GetAuthPerProject ();
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
