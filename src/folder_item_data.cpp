#ifdef __GNUG__
#pragma implementation "folder_item_data.h"
#endif

#include "folder_item_data.h"

FolderItemData::FolderItemData(const int folderType, const wxString & path, 
			       const wxString & name, const bool hasChildren)
    : wxTreeItemData()
{
    Create(folderType, path, name, hasChildren);
}

FolderItemData::FolderItemData()
    : wxTreeItemData()
{
}

FolderItemData::FolderItemData(const FolderItemData& src)
    : wxTreeItemData()
{
    Create(src.m_folderType, src.m_path, src.m_name, src.m_hasChildren);
}

void
FolderItemData::Create(const int folderType, const wxString & path, 
		       const wxString & name, const bool hasChildren)
{
    m_folderType = folderType;
    m_path = path;
    m_name = name;
    m_hasChildren = hasChildren;
}
		       
