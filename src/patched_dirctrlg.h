/////////////////////////////////////////////////////////////////////////////
// Name:        patched_dirctrlg.h
// Purpose:     PatchedGenericDirCtrl class
//              This is a modifed copy of the wxWindows class 
//              wxGenericDirCtrl. This class is meant only as a workaround
//              until wxWindows 2.4 is released. The older 
//              implementations of this class do have only static methods
//              making necessary extensions to this class impossible.
//              
//              Builds on wxDirCtrl class written by Robert Roebling for the
//              wxFile application, modified by Harm van der Heijden.
//              Further modified for Windows.
//
// To Do: make sure the local changes made here find their way into
//        wxWindows 2.4
// - ExpandDir
// - CollapseDir
//
// Author:      Robert Roebling, Harm van der Heijden, Julian Smart et al
// Modified by:
// Created:     21/3/2000
// RCS-ID:      $Id: dirctrlg.h,v 1.14.2.1 2002/10/02 13:57:29 JS Exp $
// Copyright:   (c) Robert Roebling, Harm van der Heijden, Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _PATCHED_DIRCTRL_H_
#define _PATCHED_DIRCTRL_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "patched_dirctrlg.h"
#endif

#include "wx/treectrl.h"
#include "wx/dialog.h"
#include "wx/dirdlg.h"
#include "wx/choice.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxTextCtrl;

//-----------------------------------------------------------------------------
// Extra styles for PatchedGenericDirCtrl
//-----------------------------------------------------------------------------

enum
{
  // Only allow directory viewing/selection, no files
  PATCHED_DIRCTRL_DIR_ONLY = 0x0010,
  // When setting the default path, select the first file in the directory
  PATCHED_DIRCTRL_SELECT_FIRST = 0x0020,
  // Show the filter list
  PATCHED_DIRCTRL_SHOW_FILTERS = 0x0040,
  // Use 3D borders on internal controls
  PATCHED_DIRCTRL_3D_INTERNAL = 0x0080,
  // Editable labels
  PATCHED_DIRCTRL_EDIT_LABELS = 0x0100
};

//-----------------------------------------------------------------------------
// PatchedDirItemData
//-----------------------------------------------------------------------------

class PatchedDirItemData:public wxTreeItemData
{
public:
  PatchedDirItemData (const wxString & path, const wxString & name,
                      bool isDir);
  ~PatchedDirItemData ();
  void SetNewDirName (const wxString & path);

  bool HasSubDirs () const;
  bool HasFiles (const wxString & spec = wxEmptyString) const;

  wxString m_path, m_name;
  bool m_isHidden;
  bool m_isExpanded;
  bool m_isDir;
};

//-----------------------------------------------------------------------------
// wxDirCtrl
//-----------------------------------------------------------------------------

class PatchedDirFilterListCtrl;

class PatchedGenericDirCtrl:public wxControl
{
public:
  PatchedGenericDirCtrl ();
  PatchedGenericDirCtrl (wxWindow * parent, const wxWindowID id = -1,
                         const wxString & dir = wxDirDialogDefaultFolderStr,
                         const wxPoint & pos = wxDefaultPosition,
                         const wxSize & size = wxDefaultSize,
                         long style =
                         PATCHED_DIRCTRL_3D_INTERNAL | wxSUNKEN_BORDER,
                         const wxString & filter =
                         wxEmptyString, int defaultFilter =
                         0, const wxString & name = wxTreeCtrlNameStr)
  {
    Init ();
    Create (parent, id, dir, pos, size, style, filter, defaultFilter, name);
  }

  bool Create (wxWindow * parent, const wxWindowID id = -1,
               const wxString & dir = wxDirDialogDefaultFolderStr,
               const wxPoint & pos = wxDefaultPosition,
               const wxSize & size = wxDefaultSize,
               long style = PATCHED_DIRCTRL_3D_INTERNAL | wxSUNKEN_BORDER,
               const wxString & filter = wxEmptyString,
               int defaultFilter = 0,
               const wxString & name = wxTreeCtrlNameStr);

  virtual void Init ();

  virtual ~ PatchedGenericDirCtrl ();

  void OnExpandItem (wxTreeEvent & event);
  void OnCollapseItem (wxTreeEvent & event);
  void OnBeginEditItem (wxTreeEvent & event);
  void OnEndEditItem (wxTreeEvent & event);
  void OnSize (wxSizeEvent & event);

  // Try to expand as much of the given path as possible.
  virtual bool ExpandPath (const wxString & path);

  // Accessors

  virtual inline wxString GetDefaultPath () const
  {
    return m_defaultPath;
  }
  virtual void SetDefaultPath (const wxString & path)
  {
    m_defaultPath = path;
  }

  // Get dir or filename
  virtual wxString GetPath () const;

  // Get selected filename path only (else empty string).
  // I.e. don't count a directory as a selection
  virtual wxString GetFilePath () const;
  virtual void SetPath (const wxString & path);

  virtual void ShowHidden (bool show);
  virtual bool GetShowHidden ()
  {
    return m_showHidden;
  }

  virtual wxString GetFilter () const
  {
    return m_filter;
  }
  virtual void SetFilter (const wxString & filter);

  virtual int GetFilterIndex () const
  {
    return m_currentFilter;
  }
  virtual void SetFilterIndex (int n);

  virtual wxTreeItemId GetRootId ()
  {
    return m_rootId;
  }

  virtual wxTreeCtrl *GetTreeCtrl () const
  {
    return m_treeCtrl;
  }
  virtual PatchedDirFilterListCtrl *GetFilterListCtrl () const
  {
    return m_filterListCtrl;
  }

  // Helper
  virtual void SetupSections ();

  // Parse the filter into an array of filters and an array of descriptions
  virtual int ParseFilter (const wxString & filterStr,
                           wxArrayString & filters,
                           wxArrayString & descriptions);

  // Find the child that matches the first part of 'path'.
  // E.g. if a child path is "/usr" and 'path' is "/usr/include"
  // then the child for /usr is returned.
  // If the path string has been used (we're at the leaf), done is set to TRUE
  virtual wxTreeItemId FindChild (wxTreeItemId parentId,
                                  const wxString & path, bool & done);

  // Resize the components of the control
  virtual void DoResize ();

  // Collapse & expand the tree, thus re-creating it from scratch:
  virtual void ReCreateTree ();

protected:
  virtual void ExpandDir (const wxTreeItemId & parentId);
  virtual void CollapseDir (const wxTreeItemId & parentId);
  virtual const wxTreeItemId AddSection (const wxString & path,
                                         const wxString & name, int imageId =
                                         0);
    /**
     * works just like wxTreeCtrl.AppendItem with one major exception:
     * if there is no valid treeCtrl or if an overrided AppendItem version
     * doesnt like what you want to add it will return wxINVALID_TREE_ITEM_ID
     *
     * @return appended id if valid, otherwise wxINVALID_TREE_ITEM_ID
     */
  virtual wxTreeItemId AppendItem (const wxTreeItemId & parent,
                                   const wxString & text,
                                   int image = -1, int selectedImage = -1,
                                   wxTreeItemData * data = NULL);
  //void FindChildFiles(wxTreeItemId id, int dirFlags, wxArrayString& filenames);

  // Extract description and actual filter from overall filter string
  bool ExtractWildcard (const wxString & filterStr, int n, wxString & filter,
                        wxString & description);


private:
  bool m_showHidden;
  wxTreeItemId m_rootId;
  wxImageList *m_imageList;
  wxString m_defaultPath;       // Starting path
  long m_styleEx;               // Extended style
  wxString m_filter;            // Wildcards in same format as per wxFileDialog
  int m_currentFilter;          // The current filter index
  wxString m_currentFilterStr;  // Current filter string
  wxTreeCtrl *m_treeCtrl;
  PatchedDirFilterListCtrl *m_filterListCtrl;

private:
DECLARE_EVENT_TABLE ()DECLARE_DYNAMIC_CLASS (PatchedGenericDirCtrl)};

//-----------------------------------------------------------------------------
// PatchedDirFilterListCtrl
//-----------------------------------------------------------------------------

class PatchedDirFilterListCtrl:public wxChoice
{
public:
  PatchedDirFilterListCtrl ()
  {
    Init ();
  }
  PatchedDirFilterListCtrl (PatchedGenericDirCtrl * parent,
                            const wxWindowID id = -1, const wxPoint & pos =
                            wxDefaultPosition, const wxSize & size =
                            wxDefaultSize, long style = 0)
  {
    Init ();
    Create (parent, id, pos, size, style);
  }

  bool Create (PatchedGenericDirCtrl * parent, const wxWindowID id = -1,
               const wxPoint & pos = wxDefaultPosition,
               const wxSize & size = wxDefaultSize, long style = 0);

  void Init ();

  ~PatchedDirFilterListCtrl ()
  {
  };

  //// Operations
  void FillFilterList (const wxString & filter, int defaultFilter);

  //// Events
  void OnSelFilter (wxCommandEvent & event);

protected:
  PatchedGenericDirCtrl * m_dirCtrl;

DECLARE_EVENT_TABLE ()DECLARE_CLASS (PatchedDirFilterListCtrl)};

// Symbols for accessing individual controls
#define wxID_TREECTRL          7000
#define wxID_FILTERLISTCTRL    7001

#endif
    // _WX_DIRCTRLG_H_
