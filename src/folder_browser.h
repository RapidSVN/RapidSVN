#ifndef _FOLDER_BROWSER_H_INCLUDED_
#define _FOLDER_BROWSER_H_INCLUDED_

#include "unique_array_string.h"
#include "wx/treectrl.h"

static const wxString FolderBrowserNameStr;

class UniqueArrayString;
class wxImageList;

class FolderBrowser:public wxControl
{
public:
  FolderBrowser (wxWindow * parent,
                 apr_pool_t * pool,
                 const wxWindowID id = -1,
                 const wxPoint & pos = wxDefaultPosition,
                 const wxSize & size = wxDefaultSize,
                 const wxString & name = FolderBrowserNameStr);

  virtual ~ FolderBrowser ();

  virtual void Refresh ();

  UniqueArrayString & GetWorkbenchItems ();

  const bool RemoveProject ();
  void AddProject (const wxString & projectPath);
  wxString GetPath ();
  void SetPath(const wxString& path);

private:
  apr_pool_t * m_pool;
  wxTreeCtrl* m_treeCtrl;
  wxTreeItemId m_rootId;
  wxImageList* m_imageList;
  UniqueArrayString m_workbenchItems;

  void OnExpandItem (wxTreeEvent & event);
  void OnCollapseItem (wxTreeEvent & event);
  void OnSize (wxSizeEvent & event);

  DECLARE_EVENT_TABLE ()

};


#endif
