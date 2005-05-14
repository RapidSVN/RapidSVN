/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
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

// wxWidgets
#include "wx/wx.h"
#include "wx/filename.h"
#include "wx/dir.h"
#include "wx/imaglist.h"
#include "wx/treectrl.h"
#include "wx/confbase.h"
#include "wx/hashmap.h"

// app
#include "folder_browser.hpp"
#include "folder_item_data.hpp"
#include "ids.hpp"
#include "utils.hpp"
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


const static wxChar ConfigBookmarkFmt[] = wxT("/Bookmarks/Bookmark%ld");
const static wxChar ConfigBookmarkCount[] = wxT("/Bookmarks/Count");

static const wxString EmptyString;


// local functions
static bool
IsValidSeparator (const wxString & sep)
{
  return ((sep == wxT("/")) || (sep == wxT("\\")));
}


/** 
 * data structure that contains information about
 * a single bookmark
 */
struct Bookmark
{
public:
  svn::Context * context;


  Bookmark ()
    : context (0)
  {
  }


  ~Bookmark ()
  {
    ClearContext ();
  }


  void
  ClearContext ()
  {
    if (context != 0)
    {
      delete context;
      context = 0;
    }
  }
};


static const Bookmark InvalidBookmark;


WX_DECLARE_STRING_HASH_MAP (Bookmark, BookmarkHashMap);

struct FolderBrowser::Data
{
private:
  svn::Context * singleContext;
public:
  wxWindow * window;
  svn::ContextListener * listener;
  wxTreeCtrl* treeCtrl;
  wxTreeItemId rootId;
  wxImageList* imageList;
  BookmarkHashMap bookmarks;
  svn::Context defaultContext;

  Data (wxWindow * window, const wxPoint & pos, const wxSize & size)
    : singleContext (0), window (window), listener (0)
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

  /**
   * add a new bookmark, but only if it doesnt
   * exist yet. Add a new context as well.
   *
   * @param name full path/url of the bookmark
   */
  void
  AddBookmark (wxString name)
  {
    TrimString (name);

    std::string nameUtf8;
    LocalToUtf8(name, nameUtf8);
    if (!svn::Url::isValid (nameUtf8.c_str()))
    {
      wxFileName filename (name);
      name = filename.GetFullPath (wxPATH_NATIVE);
    }

    name = BeautifyPath (name);

    bookmarks [name] = Bookmark ();

    if (singleContext == 0)
      bookmarks [name].context = CreateContext ();
  }

  
  /**
   * factory method to create a new context
   */
  svn::Context *
  CreateContext ()
  {
    svn::Context * context = new svn::Context ();

    // disable authentication caching.
    context->setAuthCache (false);

    context->setListener (listener);

    return context;
  }

  
  const wxString
  GetPath ()
  {
    const wxTreeItemId id = treeCtrl->GetSelection ();

    if(!id.IsOk())
    {
      return wxEmptyString;
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
    if (singleContext != 0)
      return singleContext;

    const wxString & path = GetSelectedBookmark ();

    if (path.Length () == 0)
      return &defaultContext;

    BookmarkHashMap::iterator it = bookmarks.find (path);

    if (it == bookmarks.end ())
      return 0;

    return it->second.context;
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
      menu.AppendSeparator ();
      AppendMenuItem (menu, ID_EditBookmark);
      AppendMenuItem (menu, ID_RemoveBookmark);
      menu.AppendSeparator ();
      AppendMenuItem (menu, ID_Login);

      wxString label;
      wxString username;
      bool enabled = false;

      if (context != 0)
      {
        username = Utf8ToLocal (context->getUsername ());
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
      menu.Append (item);
      item->Enable (enabled);
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
      if (filename != Utf8ToLocal (svn::Wc::ADM_DIR_NAME))
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
        BookmarkHashMap::iterator it = bookmarks.begin ();

        for (; it!= bookmarks.end (); it++)
        {
          const wxString & path = it->first;

          FolderItemData* data= new FolderItemData (FOLDER_TYPE_BOOKMARK,
                                                    path, path, TRUE);
          wxTreeItemId newId = treeCtrl->AppendItem (parentId, path,
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
        std::string parentPathUtf8;
        LocalToUtf8 (parentPath, parentPathUtf8);

        if ( svn::Url::isValid (parentPathUtf8.c_str ()))
          RefreshRepository (parentPath, parentId);
        else
          RefreshLocal (parentPath, parentId);
      }
      catch (svn::ClientException & e)
      {
        wxString errtxt (Utf8ToLocal (e.message ()));
        wxLogError(_("Error while refreshing filelist (%s)"),
                    errtxt.c_str ());
      }
      break;
    }

    treeCtrl->SortChildren (parentId);
  }

  void
  OnCollapseItem (wxTreeEvent & event)
  {
    wxTreeItemId parentId = event.GetItem ();

    wxTreeItemIdValue cookie;
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
      if(filename != Utf8ToLocal (svn::Wc::ADM_DIR_NAME))
      {
        parentHasSubdirectories = true;

        wxFileName path(parentPath, filename, wxPATH_NATIVE);
        wxString fullPath = path.GetFullPath ();
        std::string fullPathUtf8;
        LocalToUtf8(fullPath, fullPathUtf8);
        int image = FOLDER_IMAGE_FOLDER;
        int open_image = FOLDER_IMAGE_OPEN_FOLDER;

        if (!svn::Wc::checkWc (fullPathUtf8.c_str ()))
        {
          image = FOLDER_IMAGE_NONSVN_FOLDER;
          open_image = FOLDER_IMAGE_NONSVN_OPEN_FOLDER;
        }
        else
        {
          svn::Status status = client.singleStatus (fullPathUtf8.c_str ());
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
    std::string parentPathUtf8;
    LocalToUtf8(parentPath, parentPathUtf8);
    svn::DirEntries entries =
      client.ls (parentPathUtf8.c_str (), rev, false);
    svn::DirEntries::const_iterator it;

    //bool parentHasSubdirectories = false;
    for (it = entries.begin (); it != entries.end (); it++)
    {
      svn::DirEntry entry = *it;
      int image = FOLDER_IMAGE_FOLDER;
      int open_image = FOLDER_IMAGE_OPEN_FOLDER;

      wxString fullPath = Utf8ToLocal (entry.name ());
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
    wxTreeItemIdValue cookie;
    wxTreeItemId id = treeCtrl->GetFirstChild (parentId, cookie);
    wxTreeItemId childId;

    while (id.IsOk ())
    {
      const FolderItemData * data = GetItemData (id);

      if (data == 0)
        break;

      if (!data->isReal ())
        break;

      const svn::Path nodePath (LocalToUtf8(data->getPath ()));
      if (nodePath.length () == 0)
        break;

      // first check: full match?
      if (path.IsSameAs (Utf8ToLocal (nodePath.c_str ())))
      {
        childId = id;
        break;
      }

      // second check: match until path delimiter
      wxString prefix (path.Left (nodePath.length ()));
      wxString sep = path.Mid (nodePath.length (), 1);

      if (prefix.IsSameAs (Utf8ToLocal (nodePath.c_str ())) &&
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
   * If @a path is an empty string, select the root
   *
   * @param path
   * @retval true selection successful
   */
  bool
  SelectFolder (const wxString & pathP)
  {
    if (pathP.Length () == 0)
    {
      treeCtrl->SelectItem (treeCtrl->GetRootItem ());
      return true;
    }
    
    // Convert the input string to subversion internal representation
    std::string pathPUtf8;
    LocalToUtf8 (pathP, pathPUtf8);
    const wxString path = Utf8ToLocal (svn::Path (pathPUtf8).c_str());

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

      const svn::Path nodePath (LocalToUtf8 (data->getPath ()));
      if (nodePath.length () == 0)
        break;

      // check if @a path and @a nodePath match already
      // in this case we are done
      if (path.IsSameAs (Utf8ToLocal (nodePath.c_str())))
      {
        success = true;
        break;
      }

      wxString prefix (path.Left (nodePath.length ()));
      wxString sep (path.Mid (nodePath.length (), 1));
      if ((!prefix.IsSameAs
          (Utf8ToLocal (nodePath.c_str()))) || !IsValidSeparator (sep))
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
  SelectBookmark (const wxString & bookmarkPath)
  {
    wxTreeItemIdValue cookie;
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


  const wxString & 
  GetSelectedBookmark ()
  {
    wxTreeItemId id = GetSelectedBookmarkId ();

    if (!id.IsOk ())
      return EmptyString;

    FolderItemData * data = GetItemData (id);

    wxASSERT (data);

    return data->getPath ();
  }

  void
  SetAuthPerBookmark (const bool perBookmark)
  {
    if (!perBookmark)
    {
      // one Context for all
      ClearContexts ();
      singleContext = CreateContext ();
    }
    else
    {
      if (singleContext != 0)
      {
        delete singleContext;
        singleContext = 0;
      }

      BookmarkHashMap::iterator it = bookmarks.begin ();

      for (; it!=bookmarks.end (); it++)
      {
        it->second.context = CreateContext ();
      }
    }
  }

  const bool
  GetAuthPerBookmark () const
  {
    return singleContext == 0;
  }


  void ClearContexts ()
  {
    BookmarkHashMap::iterator it = bookmarks.begin ();

    for (; it!=bookmarks.end (); it++)
      it->second.ClearContext ();
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
  wxTreeItemId id = m->treeCtrl->GetSelection ();

  if(!id.IsOk ())
    return false;

  FolderItemData* data = (FolderItemData*) m->treeCtrl->GetItemData (id);

  if (data->getFolderType () != FOLDER_TYPE_BOOKMARK)
    return false;

  SelectFolder (wxEmptyString);

  wxString path = data->getPath ();
  m->Delete (id);
  m->bookmarks.erase (path);

  return true;
}

void
FolderBrowser::AddBookmark (const wxString & path)
{
  m->AddBookmark (path);
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
FolderBrowser::SelectFolder (const wxString & path)
{
  return m->SelectFolder (path);
}

svn::Context *
FolderBrowser::GetContext ()
{
  return m->GetContext ();
}

void
FolderBrowser::SetAuthPerBookmark (const bool value)
{
  m->SetAuthPerBookmark (value);
}

const bool
FolderBrowser::GetAuthPerBookmark () const
{
  return m->GetAuthPerBookmark ();
}

bool
FolderBrowser::SelectBookmark (const wxString & bookmarkPath)
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


void
FolderBrowser::WriteConfig (wxConfigBase * cfg) const
{
  wxASSERT (cfg);

  cfg->Write (ConfigBookmarkCount, (long)m->bookmarks.size ());

  // Save the bookmarks contents
  long item = 0;

  BookmarkHashMap::iterator it = m->bookmarks.begin ();

  for (; it != m->bookmarks.end (); it++)
  {
    wxString key;
    key.Printf (ConfigBookmarkFmt, item);

    cfg->Write (key, it->first);

    item++;
  }
}


void
FolderBrowser::ReadConfig (wxConfigBase * cfg)
{
  wxASSERT (cfg);


  long item, count;
  cfg->Read (ConfigBookmarkCount, &count, 0);
  for (item = 0; item < count; item++)
  {
    wxString key;
    wxString path;

    key.Printf (ConfigBookmarkFmt, item);
    cfg->Read (key, &path, wxEmptyString);

    if (path.Length () > 0)
      m->bookmarks [path] = Bookmark ();
  }
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
