#ifndef _FOLDER_BROWSER_H_INCLUDED_
#define _FOLDER_BROWSER_H_INCLUDED_

//#define FOLDER_BROWSER_USE_GENERIC_DIRCTRL

#ifdef FOLDER_BROWSER_USE_GENERIC_DIRCTRL
#include "wx/generic/dirctrlg.h"
#else
#include "patched_dirctrlg.h"
#define wxGenericDirCtrl PatchedGenericDirCtrl
#define wxDirItemData PatchedDirItemData
#define wxDIRCTRL_DIR_ONLY PATCHED_DIRCTRL_DIR_ONLY
#endif

class FolderBrowser:public wxGenericDirCtrl
{
private:
  apr_pool_t * pool;

public:
  FolderBrowser (wxWindow * wnd,
                 apr_pool_t * __pool,
                 const wxWindowID id = -1,
                 const wxString & dir = wxDirDialogDefaultFolderStr,
                 const wxPoint & pos = wxDefaultPosition,
                 const wxSize & size = wxDefaultSize,
                 long style = wxDIRCTRL_DIR_ONLY,
                 const wxString & filter = wxEmptyString,
                 int defaultFilter = 0,
                 const wxString & name = wxTreeCtrlNameStr);

   virtual ~ FolderBrowser ();

  void OnSelChanged (wxTreeEvent & event);
  void OnKeyDown (wxTreeEvent & event);

  virtual void SetupSections ();

  void SetWorkbenchItems (const wxArrayString & workbenchItems);
  const wxArrayString GetWorkbenchItems ();

protected:
   virtual void ExpandDir (const wxTreeItemId & parentId);
  virtual void CollapseDir (const wxTreeItemId & parentId);

private:
   DECLARE_EVENT_TABLE () wxTreeItemId m_workbenchId;
  wxArrayString m_workbenchItems;
};


#endif
