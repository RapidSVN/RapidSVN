#ifndef _RAPIDSVN_UNIQUE_ARRAY_STRING_H_
#define _RAPIDSVN_UNIQUE_ARRAY_STRING_H_

#include "wx/wxprec.h"

class wxArrayString;

class UniqueArrayString
{
public:
  UniqueArrayString (int autoSort = FALSE):m_array (autoSort)
  {
  }
  virtual ~ UniqueArrayString ()
  {
  }

  void Clear ()
  {
    m_array.Clear ();
  }
  const size_t GetCount ()
  {
    return m_array.Count ();
  }
  const wxString & Item (const size_t nIndex)
  {
    return m_array.Item (nIndex);
  }
  const int Index (const wxChar * sz, bool bCase = TRUE, bool bFromEnd =
                   FALSE)
  {
    return m_array.Index (sz, bCase, bFromEnd);
  }
  void Add (const wxString & str)
  {
    if (m_array.Index (str.c_str (), TRUE, FALSE) == wxNOT_FOUND)
    {
      m_array.Add (str);
    }
  }
  void Remove (const wxChar * sz)
  {
    m_array.Remove (sz);
  }
  void Remove (size_t nIndex)
  {
    m_array.Remove (nIndex);
  }
  void Sort ()
  {
    m_array.Sort ();
  }
  const bool IsEmpty ()
  {
    return m_array.IsEmpty ();
  }
private:
  wxArrayString m_array;
};


#endif
