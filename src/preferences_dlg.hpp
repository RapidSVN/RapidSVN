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
#ifndef _PREFERENCES_DLG_H_INCLUDED_
#define _PREFERENCES_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"
#include "wx/panel.h"

// forward declarations
class wxConfigBase;
class wxNotebook;

/**
 * General settings page for the preferences dialog.
 * Use this as a model for adding new pages.
 *
 */
class GeneralPanel : public wxPanel
{
public:

  /**
   * Create a properly initialised instance of a GeneralPanel.
   * @param parent the parent windows
   * @return the instance created.
   */
  static GeneralPanel* Create(wxWindow* parent);
  
private:
  GeneralPanel(wxWindow* parent); // Call Create please.
  void InitializeData ();
  
  DECLARE_EVENT_TABLE ()
};

/**
 * External settings page for the preferences dialog.
 * Just an example - replace in due course with
 * a real page.
 */
class ExternalsPanel : public wxPanel
{
public:
  /**
   * Create a properly initialised instance of an ExternalsPanel.
   * @param parent the parent windows
   * @return the instance created.
   */
  static ExternalsPanel* Create(wxWindow* parent);
  
private:
  ExternalsPanel(wxWindow* parent); // Call Create please.
  void InitializeData ();
  
  DECLARE_EVENT_TABLE ()
};

/**
 * Preferences dialog that displays the preference pages.
 * Data is transferred between dialog controls and global
 * data, which is in turn stored in a wxConfig object.
 */
class PreferencesDlg : public wxDialog
{
public:
  /**
   * Create a properly initialised instance of an PreferencesDlg.
   * @param parent the parent windows
   * @return the instance created.
   */
  static PreferencesDlg* CreateInstance(wxWindow* parent);

  /**
   * A specialised version of TransferDataFromWindow that calls
   * TransferDataFromWindow for each panel in the dialog. Sadly this
   * is not the default base class behaviour. 
   * @return true if transfer succeeded.
   */
  virtual bool TransferDataFromWindow();
  
  /**
   * A specialised version of TransferDataToWindow that calls
   * TransferDataToWindow for each panel in the dialog. Sadly this
   * is not the default base class behaviour. 
   * @return true if transfer succeeded.
   */
  virtual bool TransferDataToWindow();

  /**
   * The preference data, edited in the dialog.
   */
  static struct SData
  {
    /**
     * Construct a SData object initialising its data to 
     * default values.
     */
    SData();
    
    /**
     * Read the data from a wxConfig object of any type. 
     * @param pConfig points to the config object from which
     * to read the data.
     */
    void Read(wxConfigBase* pConfig);

    /**
     * Writes the data to a wxConfig object of any type. 
     * @param pConfig points to the config object to which
     * to write the data.
     */
    void Write(wxConfigBase* pConfig) const;
    
    // Replace this with the real preference data in due course.
    // Use types supported by wxValidator classes.
    wxString Text;
    bool EnableX, EnableY;
    bool Enable1, Enable2, Enable3;
    int Choice;
  } Data;

private:
    /**
     * Don't call this function, use CreateInstance() instead.
     */
  PreferencesDlg(wxWindow* parent); // Please use CreateInstance().
  void InitializeData ();
  
  wxNotebook *m_pNB;
  
  DECLARE_EVENT_TABLE ()
};

#endif // _PREFERENCES_DLG_H_INCLUDED_

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
