#include "include.h"
#include "rapidsvn_app.h"

#include "folder_browser.h"

FolderBrowser::FolderBrowser (wxWindow * wnd,
                              apr_pool_t * __pool,
                              const wxWindowID id,
                              const wxString & dir,
                              const wxPoint & pos,
                              const wxSize & size,
                              long style,
                              const wxString & filter,
                              int defaultFilter, const wxString & name):
wxGenericDirCtrl (wnd, id, dir, pos, size, style, filter, defaultFilter, name)
{
  pool = __pool;


  wxTreeCtrl *treeCtrl = GetTreeCtrl ();

  if (treeCtrl)
  {
    wxTreeItemId rootId = treeCtrl->GetRootItem ();
    CollapseDir (rootId);
    ExpandDir (rootId);
  }

}

FolderBrowser::~FolderBrowser ()
{
}

void
FolderBrowser::SetupSections ()
{
  wxTreeCtrl *treeCtrl = GetTreeCtrl ();

  if (treeCtrl)
  {
    m_workbenchId = AddSection ("", wxT ("Workbench"), 3);
  }
}


void
FolderBrowser::ExpandDir (const wxTreeItemId & parentId)
{
  if (parentId == m_workbenchId)
  {
    wxTreeCtrl *treeCtrl = GetTreeCtrl ();

    if (treeCtrl)
    {
      const size_t childCount = treeCtrl->GetChildrenCount (parentId, FALSE);

      if (childCount == 0)
      {
        const size_t count = m_workbenchItems.GetCount ();
        size_t index;

        m_workbenchItems.Sort ();
        for (index = 0; index < count; index++)
        {
          const wxString & item = m_workbenchItems.Item (index);
          PatchedDirItemData *itemData =
            new PatchedDirItemData (item, item, TRUE);
          const wxTreeItemId & itemId =
            treeCtrl->AppendItem (parentId, item, 0, -1, itemData);
          treeCtrl->SetItemHasChildren (itemId);
        }
      }
    }
  }
  else
  {
    PatchedGenericDirCtrl::ExpandDir (parentId);
  }
}


void
FolderBrowser::CollapseDir (const wxTreeItemId & parentId)
{
  wxTreeCtrl *treeCtrl = GetTreeCtrl ();
  wxTreeItemId child;

  PatchedDirItemData *data =
    (PatchedDirItemData *) treeCtrl->GetItemData (parentId);

  if (data)
  {
    data->m_isExpanded = FALSE;
    long cookie;
    child = treeCtrl->GetFirstChild (parentId, cookie);
    while (child.IsOk ())
    {
      treeCtrl->Delete (child);
      child = treeCtrl->GetFirstChild (parentId, cookie);
    }
  }
  wxGenericDirCtrl::CollapseDir (parentId);
}

wxArrayString & FolderBrowser::GetWorkbenchItems ()
{
  return m_workbenchItems;
}

void
FolderBrowser::Refresh ()
{
  CollapseDir (m_workbenchId);
  ExpandDir (m_workbenchId);
}

const bool
FolderBrowser::RemoveProject (const wxTreeItemId & id)
{
  wxTreeCtrl *treeCtrl = GetTreeCtrl ();
  wxASSERT (treeCtrl);
  bool success = FALSE;

  const wxTreeItemId parentId = treeCtrl->GetParent (id);

  if (parentId == m_workbenchId)
  {
    const wxDirItemData *itemData =
      (wxDirItemData *) treeCtrl->GetItemData (id);

    if (itemData)
    {
      m_workbenchItems.Remove (itemData->m_path.c_str ());
    }
  }
  return success;
}

void
FolderBrowser::AddProject (const wxString & path)
{
  if (m_workbenchItems.Index (path) == wxNOT_FOUND)
  {
    m_workbenchItems.Add (path);
  }
}

