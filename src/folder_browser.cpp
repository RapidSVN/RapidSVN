/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
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
#include "svncpp/client.hpp"
#include "svncpp/dirent.hpp"
#include "svncpp/status.hpp"
#include "svncpp/url.hpp"
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
#include "utils.hpp"
#include "bookmarks.hpp"
#include "rapidsvn_app.hpp"
#include "rapidsvn_frame.hpp"

// bitmaps
#include "res/bitmaps/computer.xpm"
#include "res/bitmaps/versioned_folder.xpm"
#include "res/bitmaps/open_folder.xpm"
#include "res/bitmaps/folder.xpm"
#include "res/bitmaps/nonsvn_open_folder.xpm"
#include "res/bitmaps/earth.xpm"
#include "res/bitmaps/modified_versioned_folder.xpm"
#include "res/bitmaps/modified_open_folder.xpm"
#include "res/bitmaps/bookmark.xpm"

enum
{
  FOLDER_IMAGE_COMPUTER = 0,
  FOLDER_IMAGE_FOLDER,
  FOLDER_IMAGE_OPEN_FOLDER,
  FOLDER_IMAGE_NONSVN_FOLDER,
  FOLDER_IMAGE_NONSVN_OPEN_FOLDER,
  FOLDER_IMAGE_EARTH,
  FOLDER_IMAGE_MODIFIED_FOLDER,
  FOLDER_IMAGE_MODIFIED_OPEN_FOLDER,
  FOLDER_IMAGE_BOOKMARK,
  FOLDER_IMAGE_COUNT
};

static const unsigned int MAXLENGTH_BOOKMARK = 35;

/**
 * beautify a path, that is too long, so not everything can be
 * displayed
 *
 * Examples:
 * 1. Local Unix path
 *    Before:  /home/users/xela/work/rapidsvn/src/svncpp
 *    After:   /...ork/rapidsvn/src/svncpp
 *
 * 2. Local Windows path
 *    Before:  d:\Documents and Settings\alex\Application Data
 *    After:   D:\...alex\Application Data
 * 
 * 3. Repository URL
 *    Before:  https://svn.collab.net/repos/rapidsvn/trunk/src/svncpp
 *    After:   https://...apidsvn/trunk/src/svncpp
 *
 * Jobs to do:
 * - Uppercase Windows drive letters
 * - shorten path while preserving root path/url
 *
 * @param path input path
 * @return beatified path
 */
static wxString 
TruncatePath (const wxString & path_)
{
  wxString path (BeautifyPath (path_));

  if (path.length() <= MAXLENGTH_BOOKMARK)
    return path;

  size_t pos = path.Find (":");
  wxString newPath;

  pos++;
  if (pos > 0)
    newPath = path.Left (pos);


  // Now add chars until a different char than
  // / or \ appears
  while (pos < path.Length ())
  {
    char c = path.GetChar (pos);

    if ( (c == '/') || (c == '\\'))
    {
      newPath += c;
      pos++;
    }
    else
      break;
  }

  newPath += "...";

  const int restPos = path.Length () - MAXLENGTH_BOOKMARK + 
    newPath.Length ();

  newPath += path.Mid (restPos);

  return newPath;
}


static bool
IsValidSeparator (const wxString & sep)
{
  return ((sep == "/") || (sep == "\\"));
}


struct FolderBrowser::Data
{
public:
  wxWindow * window;
  svn::ContextListener * listener;
  wxTreeCtrl* treeCtrl;
  wxTreeItemId rootId;
  wxImageList* imageList;
  Bookmarks bookmarks;
  svn::Context defaultContext;
  
  Data (wxWindow * window, const wxPoint & pos, const wxSize & size)
    : window (window), listener (0)
  {
    imageList = new wxImageList (16, 16, TRUE);
    imageList->Add (wxIcon (computer_xpm));
    imageList->Add (wxIcon (versioned_folder_xpm));
    imageList->Add (wxIcon (open_folder_xpm));
    imageList->Add (wxIcon (folder_xpm));
    imageList->Add (wxIcon (nonsvn_open_folder_xpm));
    imageList->Add (wxIcon (earth_xpm));
    imageList->Add (wxIcon (modified_versioned_folder_xpm));
    imageList->Add (wxIcon (modified_open_folder_xpm));
    imageList->Add (wxIcon (bookmark_xpm));

    treeCtrl = new wxTreeCtrl (window, -1, pos, size, 
                               wxTR_HAS_BUTTONS|wxTR_SINGLE);
    treeCtrl->AssignImageList (imageList);

    FolderItemData* data = new FolderItemData (FOLDER_TYPE_BOOKMARKS);
    rootId = treeCtrl->AddRoot (_("Bookmarks"), FOLDER_IMAGE_COMPUTER, 
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

    FolderItemData* data = GetItemData (id);
    return data->getPath ();
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
    svn::Context * context = 0;

    if (data == 0)
      context = &defaultContext;
    else
    {
      bool ok = true;
      while (context == 0)
      {
        switch (data->getFolderType ())
        {
        case FOLDER_TYPE_NORMAL:
          {
            wxTreeItemId id = data->GetId ();
            wxTreeItemId parentId = treeCtrl->GetItemParent (id);
            data = GetItemData (parentId);
          }
          break;

        case FOLDER_TYPE_BOOKMARK:
          context = bookmarks.GetContext (data->getPath ());
          ok = context != 0;

          break;
        default:
          ok = false;
          break;
        }

        if (!ok)
          break;
      }
    }

    if (context != 0)
      context->setListener (listener);

    return context;
  }


  void
  ShowMenu (wxPoint & pt)
  {
    const FolderItemData * data = GetSelection ();
    if (!data)
      return;
    const svn::Context * context = GetContext ();

    // create menu
    wxMenu menu;
    int type =  data->getFolderType ();

    AppendMenuItem (menu, ID_AddWcBookmark);
    AppendMenuItem (menu, ID_Checkout);
    menu.AppendSeparator ();
    AppendMenuItem (menu, ID_AddRepoBookmark);
    AppendMenuItem (menu, ID_CreateRepository);

    if (type==FOLDER_TYPE_BOOKMARK)
    {
      AppendMenuItem (menu, ID_RemoveBookmark);
      menu.AppendSeparator ();
      AppendMenuItem (menu, ID_Login);

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

      wxMenuItem * item = new wxMenuItem (&menu, ID_Logout, label);
      item->Enable (enabled);
      menu.Append (item);
    }

    if ((type==FOLDER_TYPE_BOOKMARK)||(type==FOLDER_TYPE_NORMAL))
    {
      menu.AppendSeparator ();
      AppendMenuItem (menu, ID_Update);
      AppendMenuItem (menu, ID_Commit);
    }
    
    // Check for disabled items
    RapidSvnFrame* frame = (RapidSvnFrame*) wxGetApp ().GetTopWindow ();
    frame->TrimDisabledMenuItems (menu);
  
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
#ifndef __WXMAC__ //wxMac crashes here
      treeCtrl->Collapse (rootId);
#endif
    }
  }

  void
  OnExpandItem (wxTreeEvent & event)
  {
    wxTreeItemId parentId = event.GetItem ();
    int type = FOLDER_TYPE_INVALID;

    if(!rootId.IsOk ())
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
      case FOLDER_TYPE_BOOKMARKS:
      {
        const int count = bookmarks.Count ();
        int index;

        for(index = 0; index < count; index++)
        {
          const wxString path (bookmarks.GetBookmark (index));
          FolderItemData* data= new FolderItemData (FOLDER_TYPE_BOOKMARK, 
                                                    path, path, TRUE);
          wxString label (TruncatePath (path));
          wxTreeItemId newId = treeCtrl->AppendItem (parentId, label, 
                                                     FOLDER_IMAGE_BOOKMARK, 
                                                     FOLDER_IMAGE_BOOKMARK, 
                                                     data);
          treeCtrl->SetItemHasChildren (newId, TRUE);
          treeCtrl->SetItemImage (newId, FOLDER_IMAGE_BOOKMARK,  
                                    wxTreeItemIcon_Expanded);
        }
      }
      break;

      case FOLDER_TYPE_BOOKMARK:
      case FOLDER_TYPE_NORMAL:
      try
      {
        const wxString& parentPath = parentData->getPath ();

        if ( svn::Url::isValid (parentPath) )
          RefreshRepository (parentPath, parentId);
        else
          RefreshLocal (parentPath, parentId);
      }
      catch (svn::ClientException & e)
      {
        wxLogError(_("Error while refreshing filelist (%s)"),
                    e.message ());
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

  void
  RefreshLocal (const wxString & parentPath, 
                const wxTreeItemId & parentId)
  {
    svn::Client client (GetContext ());
    wxDir dir (parentPath);
    if(!dir.IsOpened ())
      return;

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
        else
        {
          svn::Status status = client.singleStatus (fullPath_c);
          if ((status.textStatus () == svn_wc_status_modified) ||
              (status.propStatus () == svn_wc_status_modified))
          {
            image = FOLDER_IMAGE_MODIFIED_FOLDER;
            open_image = FOLDER_IMAGE_MODIFIED_OPEN_FOLDER;
          }
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

  void
  RefreshRepository (const wxString & parentPath, 
                     const wxTreeItemId & parentId)
  {
    svn::Client client (GetContext ());
    svn::Revision rev (svn::Revision::HEAD);
    svn::DirEntries entries = 
      client.ls (parentPath, rev, false);
    svn::DirEntries::const_iterator it;

    //bool parentHasSubdirectories = false;
    for (it = entries.begin (); it != entries.end (); it++)
    {
      svn::DirEntry entry = *it;
      int image = FOLDER_IMAGE_FOLDER;
      int open_image = FOLDER_IMAGE_OPEN_FOLDER;

      wxString fullPath = entry.name ();
      wxString filename (fullPath.Mid (parentPath.Length () + 1));
      //parentHasSubdirectories = true;
      
      if (entry.kind () != svn_node_dir)
        continue;

      FolderItemData * data = 
        new FolderItemData (FOLDER_TYPE_NORMAL, 
                            fullPath, 
                            filename, TRUE);

      wxTreeItemId newId = 
        treeCtrl->AppendItem(
          parentId, filename, 
          image, image, data);
      treeCtrl->SetItemHasChildren (
        newId, true );
      treeCtrl->SetItemImage (newId, open_image,  
                                wxTreeItemIcon_Expanded);
      
    }
  }


  /**
   * Finds the child entry with @a path
   */
  wxTreeItemId 
  FindClosestChild (const wxTreeItemId & parentId, const wxString & path)
  {
    long cookie;
    wxTreeItemId id = treeCtrl->GetFirstChild (parentId, cookie);
    wxTreeItemId childId;

    while (id.IsOk ())
    {
      const FolderItemData * data = GetItemData (id);

      if (data == 0)
        break;

      if (!data->isReal ())
        break;

      const svn::Path nodePath (data->getPath ());
      if (nodePath.length () == 0)
        break;

      // first check: full match?
      if (path.IsSameAs (nodePath.c_str()))
      {
        childId = id;
        break;
      }

      // second check: match until path delimiter
      wxString prefix (path.Left (nodePath.length ()));
      wxString sep = path.Mid (nodePath.length (), 1);

      if (prefix.IsSameAs (nodePath.c_str()) &&
           IsValidSeparator (sep))
      {
        childId = id;
        break;
      }

      id = treeCtrl->GetNextChild (parentId, cookie);
    } // while (id.IsOk ())

    return childId;
  }

  /**
   * Tries to select @a path in the current selected bookmark.
   * If it cannot be found in there (because it is deeper in
   * the folder hierarchy) then we try to open folder nodes
   * until we find the path. If the path as whole is not found
   * (folder removed or deleted) we open up the directory structure
   * as close as possible.
   *
   * @param path
   */
  bool
  SelectFolder (const wxString & path)
  {
    wxTreeItemId bookmarkId = GetSelectedBookmarkId ();

    // found a valid bookmark? otherwise quit
    if (!bookmarkId.IsOk ())
      return false;

    // begin searching in the folder hierarchy
    bool success=false;
    wxTreeItemId id = bookmarkId;
    while (id.IsOk ())
    {
      const FolderItemData * data = GetItemData (id);

      // first some validity checkings:
      // - there has to be a valid @a data item
      // - the node has to be "real" 
      // - the path of the node has to be non-empty
      // - the prefix of @a path has to match
      //   the path of the @a node
      if (data == 0)
        break;

      if (!data->isReal ())
        break;

      svn::Path nodePath (data->getPath ());
      if (nodePath.length () == 0)
       break;

      // check if @a path and @a nodePath match already
      // in this case we are done
      if (path.IsSameAs (nodePath.c_str()))
      {
        success = true;
        break;
      }

      wxString prefix (path.Left (nodePath.length ()));
      wxString sep (path.Mid (nodePath.length (), 1));
      if ((!prefix.IsSameAs (nodePath.c_str())) || !IsValidSeparator (sep))
       break;

      if (!data->hasChildren ())
        break;

      // try to find @a path in one of the children
      // make sure node is open
      if (!treeCtrl->IsExpanded (id))
        treeCtrl->Expand (id);

      id = FindClosestChild (id, path);
    } // while (id.IsOk ())

    if (success)
      treeCtrl->SelectItem (id);

    return success;
  }

  FolderItemData *
  GetItemData (const wxTreeItemId & id) const
  {
    if (!treeCtrl)
      return 0;

    return static_cast<FolderItemData *>(treeCtrl->GetItemData (id));
  }

  bool
  SelectBookmark (const char *bookmarkPath)
  {
    long cookie;
    wxTreeItemId id = treeCtrl->GetFirstChild (rootId, cookie);

    bool success = false;
    while (!success)
    {
      FolderItemData * data = GetItemData (id);

      // if id is not valid, data will be 0
      // This is the case if there are no
      // or no more children for rootId
      if (data == 0)
        break;

      // bookmark match?
      if (data->getPath () == bookmarkPath)
      {
        // select bookmark
        success = true;
        treeCtrl->SelectItem (id);
      }
      else
      {
        // otherwise move to next
        id = treeCtrl->GetNextChild (rootId, cookie);
      }
    }

    return success;
  }


  /**
   * Try to go up in the folder hierarchy until we
   * find a bookmark node. 
   *
   * @return id of bookmark for selected node
   */
  wxTreeItemId 
  GetSelectedBookmarkId () const
  {
    wxTreeItemId id = treeCtrl->GetSelection ();
    const FolderItemData * data;
    wxTreeItemId bookmarkId;

    while (id.IsOk ())
    {
      data = GetItemData (id);

      if (data == 0)
        break;

      if (data->getFolderType () == FOLDER_TYPE_BOOKMARK)
      {
        bookmarkId = id;
        break;
      }

      // step up one level
      id = treeCtrl->GetItemParent (id);
    }

    return bookmarkId;
  }


  wxString 
  GetSelectedBookmark () const
  {
    wxTreeItemId id = GetSelectedBookmarkId ();

    wxString path ("");

    if (id.IsOk ())
    {
      FolderItemData * data = GetItemData (id);

      if (data != 0)
        path = data->getPath ();
    }

    return path;
  }
    
};

BEGIN_EVENT_TABLE (FolderBrowser, wxControl)
  EVT_TREE_ITEM_EXPANDING (-1, FolderBrowser::OnExpandItem)
  EVT_TREE_ITEM_COLLAPSED (-1, FolderBrowser::OnCollapseItem)
  EVT_SIZE (FolderBrowser::OnSize) 
  EVT_TREE_ITEM_RIGHT_CLICK (-1, FolderBrowser::OnItemRightClk)
  EVT_CONTEXT_MENU (FolderBrowser::OnContextMenu)
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

void
FolderBrowser::Refresh ()
{
  // remember selected
  wxString bookmarkPath = m->GetSelectedBookmark ();
  wxString path = m->GetPath ();

  // refresh contents
  m->treeCtrl->Collapse (m->rootId);
  m->treeCtrl->Expand (m->rootId);

  // now try to find the remembered selection
  if (bookmarkPath.Length () == 0)
    return;

  if(!m->SelectBookmark (bookmarkPath))
    return;

  wxTreeItemId bookmarkId = m->treeCtrl->GetSelection ();
  if (m->treeCtrl->ItemHasChildren (bookmarkId))
    m->treeCtrl->Expand (bookmarkId);

  SelectFolder (path);
}

const bool
FolderBrowser::RemoveBookmark ()
{
  bool success = FALSE;

  wxTreeItemId id = m->treeCtrl->GetSelection ();

  if(id.IsOk ())
  {
    FolderItemData* data = (FolderItemData*) m->treeCtrl->GetItemData (id);

    if( data->getFolderType() == FOLDER_TYPE_BOOKMARK )
    {
      wxString path = data->getPath ();
      m->Delete (id);
      m->bookmarks.RemoveBookmark (path.c_str ());
      success = TRUE;
    }
  }

  return success;
}

void
FolderBrowser::AddBookmark (const char * path)
{
  m->bookmarks.AddBookmark (path);
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
  RapidSvnFrame* frame = (RapidSvnFrame*) wxGetApp ().GetTopWindow ();
  frame->SetActivePane (ACTIVEPANE_FOLDER_BROWSER);

#ifdef __WXGTK__
  // wxGTK doesn't seem to select an item upon right clicking - try doing it ourselves for now
  wxPoint clientPt = event.GetPoint ();
  const wxTreeItemId id = m->treeCtrl->HitTest (clientPt);
  if (id.IsOk ())
  {
    m->treeCtrl->SelectItem (id);
  }
  // Show the menu now rather than waiting for OnContextMenu,
  // and so don't Skip().
  m->ShowMenu (clientPt);
#else
  // Let the OnContextMenu handler do the menu on MouseUp
  event.Skip ();
#endif
}

void 
FolderBrowser::OnContextMenu (wxContextMenuEvent & event)
{
  wxPoint clientPt = ScreenToClient (event.GetPosition ());
  m->ShowMenu (clientPt);
}

bool
FolderBrowser::SelectFolder (const char * path)
{
  return m->SelectFolder (path);
}

const size_t
FolderBrowser::GetBookmarkCount () const
{
  return m->bookmarks.Count ();
}

const char *
FolderBrowser::GetBookmark (const size_t index) const
{
  return m->bookmarks.GetBookmark (index);
}

svn::Context * 
FolderBrowser::GetContext ()
{
  return m->GetContext ();
}

void
FolderBrowser::SetAuthPerBookmark (const bool value)
{
  m->bookmarks.SetAuthPerBookmark (value);
}

const bool 
FolderBrowser::GetAuthPerBookmark () const
{
  return m->bookmarks.GetAuthPerBookmark ();
}

bool
FolderBrowser::SelectBookmark (const char * bookmarkPath)
{
  return m->SelectBookmark (bookmarkPath);
}


void
FolderBrowser::SetListener (svn::ContextListener * listener)
{
  m->listener = listener;
}

svn::ContextListener *
FolderBrowser::GetListener () const
{
  return m->listener;
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
