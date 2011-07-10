/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
#include "wx/wx.h"
#include "wx/confbase.h"
#include "wx/stdpaths.h"
#include "wx/sysopt.h"
#include <wx/fs_zip.h> // ZIP filesystem support
#include <wx/tipdlg.h>
#include <wx/cshelp.h>
#include <wx/image.h>

// svncpp
#include "svncpp/wc.hpp"

//app
#include "config.hpp"
#include "hist_mgr.hpp"
#include "main_frame.hpp"
#include "preferences.hpp"
#include "rapidsvn_app.hpp"
#include "version.hpp"

static const wxChar HELP_FILE[] =  wxT("/Resources/HelpFile");

#ifdef USE_STARTUP_TIPS
static const wxChar TIPS_FILE[] = wxT("/Resources/TipsFile");
static const wxChar TIPS_SHOW_AT_STARTUP[] = wxT("/Main/TipsShowAtStartup");
static const wxChar TIPS_LAST_INDEX[] = wxT("/Main/TipsLastIndex");
#endif

IMPLEMENT_APP(RapidSvnApp)

bool RapidSvnApp::OnInit()
{
  // application and vendor name are used by wxConfig to construct the name
  // of the config file/registry key and must be set before the first call
  // to Get() if you want to override the default values (the application
  // name is the name of the executable and the vendor name is the same)
  SetVendorName(APPLICATION_NAME);
  SetAppName(APPLICATION_NAME);

  m_locale.Init();
  m_locale.AddCatalogLookupPathPrefix(wxT("locale"));
#ifdef __WXMAC__
	wxSystemOptions::SetOption(wxT("mac.listctrl.always_use_generic"), true);
  m_locale.AddCatalogLookupPathPrefix(
    wxStandardPaths::Get().GetResourcesDir() +
    wxFileName::GetPathSeparator() +
    wxT("locale"));
#endif //__WXMAC__
  m_locale.AddCatalog(wxT("rapidsvn"));

  // Add support for PNG
  wxImage::AddHandler(new wxPNGHandler);

  // Add support for SVN_ASP_DOT_NET_HACK
  if (wxGetEnv(wxT("SVN_ASP_DOT_NET_HACK"), 0))
    svn::Wc::setAdmDir("_svn");

#ifdef _WIN32
  // Avoiding to conflict with different versioned apr-iconv.
  wxString apr_iconv_path;
  if (wxGetEnv(wxT("APR_ICONV_PATH"), &apr_iconv_path))
  {
    wxSetEnv(wxT("__SAVED_APR_ICONV_PATH"), apr_iconv_path.c_str());
    wxSetEnv(wxT("APR_ICONV_PATH"), wxStandardPaths::Get().GetDataDir() + wxT("\\iconv"));
  }
#endif

#ifdef  USE_HTML_HELP
  // Initialise the HTML help
  wxFileSystem::AddHandler(new wxZipFSHandler);
  m_helpController = new wxHtmlHelpController;
  m_helpController->SetTitleFormat(_("RapidSVN Help: %s"));
  LocateHelp();

  // Initialise a popup-help-text provider system, connected to the main help system
  wxHelpControllerHelpProvider* provider = new wxHelpControllerHelpProvider;
  provider->SetHelpController(m_helpController);
#else
  wxSimpleHelpProvider* provider = new wxSimpleHelpProvider;
#endif
  wxHelpProvider::Set(provider);

  MainFrame * frame = new MainFrame(APPLICATION_NAME, m_locale);
  frame->Show(TRUE);
  SetTopWindow(frame);

  // Show tips if configured
#ifdef USE_STARTUP_TIPS
  wxTipProvider* tipProvider = MakeTipProvider();
  if (tipProvider)
  {
    bool showAtStartup = wxShowTip(frame, tipProvider);
    SaveTipsInfo(showAtStartup, tipProvider->GetCurrentTip());
    delete tipProvider;
  }
#endif

  return TRUE;
}

int
RapidSvnApp::OnExit()
{
  OptionallyPurgeTempFiles();

  TheHistoryManager.Cleanup();

  // destroy application configuration object
  delete wxConfigBase::Set((wxConfigBase *) NULL);

  // Remove the help provider & close down help
  delete wxHelpProvider::Set(NULL);
#ifdef  USE_HTML_HELP
  delete m_helpController;
#endif

  return 0;
}

void
RapidSvnApp::OptionallyRegisterTempFile(const wxString & filename)
{
  Preferences prefs;

  if (prefs.purgeTempFiles)
  {
    m_TempFiles.Add(filename);
  }
  else
  {
    m_TempFiles.Clear();
  }
}

void
RapidSvnApp::OptionallyPurgeTempFiles()
{
  Preferences prefs;

  if (prefs.purgeTempFiles)
  {
    for (size_t i = 0; i < m_TempFiles.GetCount(); ++i)
    {
      ::wxRemoveFile(m_TempFiles.Item(i));
    }
  }
  m_TempFiles.Clear();
}

#ifdef  USE_HTML_HELP
bool
RapidSvnApp::LocateHelp()
{
  wxString appPath = wxFileName(argv[0]).GetPath(wxPATH_GET_VOLUME);
  if (appPath.IsEmpty())
    appPath = wxGetCwd();
  wxString appName = wxFileName(argv[0]).GetName();
#ifdef __WXMAC__
  appPath += appName + wxT(".app/Contents/Resources"));
#endif

  wxString helpfile = appPath + wxFileName::GetPathSeparator() + appName + wxT(".htb");
  wxConfigBase* cfg = wxConfigBase::Get();
  helpfile = cfg->Read(HELP_FILE, helpfile);
  if (helpfile.IsEmpty())
  {
    return false;
  }

  // TODO: Hmmm - I haven't called Initialise yet, but AddBook seems to work directly...
  while (!m_helpController->AddBook(wxFileName(helpfile)))
  {
    wxFileDialog dlg(NULL, _("Locate help"), appPath, wxEmptyString, wxT("*.htb"), wxOPEN | wxFILE_MUST_EXIST);
    if (dlg.ShowModal() == wxID_OK)
    {
      helpfile = dlg.GetPath();
      cfg->Write(HELP_FILE, helpfile);
    }
    else
    {
      if (wxMessageBox(_("No help file was chosen, would you like to be \n" "asked again after next application start?"), _("Locate help"), wxYES_NO | wxICON_QUESTION) == wxYES)
      {
        if (cfg->Exists(HELP_FILE))
        {
          cfg->DeleteEntry(HELP_FILE);
        }
      }
      else
      {
        helpfile = wxEmptyString;
        cfg->Write(HELP_FILE, helpfile);
      }
      return false;
    }
  }
  return true;
}
#endif

#ifdef USE_STARTUP_TIPS
wxTipProvider*
RapidSvnApp::MakeTipProvider(bool force)
{
  wxString appPath = wxFileName(argv[0]).GetPath(wxPATH_GET_VOLUME);
  if (appPath.IsEmpty())
  {
    appPath = wxGetCwd();
  }
  wxString appName = wxFileName(argv[0]).GetName();
#ifdef __WXMAC__
  appPath += appName + wxT(".app/Contents/Resources"));
#endif

  wxString tipsfile = appPath + wxFileName::GetPathSeparator() + appName + wxT("_tips.txt");
                      bool showTips = true;
                                      wxConfigBase* cfg = wxConfigBase::Get();
                                                          cfg->Read(TIPS_FILE, &tipsfile);
                                                          if (!force)
{
  cfg->Read(TIPS_SHOW_AT_STARTUP, &showTips) ;
    if (tipsfile.IsEmpty() || !showTips)
    {
      return NULL;
    }
  }

  int tipIndex = 0;
                 cfg->Read(TIPS_LAST_INDEX, &tipIndex);
                 wxTipProvider* tipProvider = NULL;

                                              while (!wxFileName(tipsfile).FileExists() || (tipProvider = wxCreateFileTipProvider(tipsfile, tipIndex)) == NULL)
  {
    wxFileDialog dlg(NULL, _("Locate tips file"), appPath, wxEmptyString, wxT("*.txt"), wxOPEN | wxFILE_MUST_EXIST);
    if (dlg.ShowModal() == wxID_OK)
    {
      tipsfile = dlg.GetPath();
      cfg->Write(TIPS_FILE, tipsfile);
    }
    else
    {
      if (wxMessageBox(_("No tips file was chosen, would you like to be \n" "asked again after next application start?"), _("Locate tips"), wxYES_NO | wxICON_QUESTION) == wxYES)
      {
        if (cfg->Exists(TIPS_FILE))
        {
          cfg->DeleteEntry(TIPS_FILE);
        }
      }
      else
      {
        tipsfile = wxEmptyString;
        cfg->Write(TIPS_FILE, tipsfile);
      }
      return NULL;
    }
  }

  return tipProvider;
       }

       void
       RapidSvnApp::SaveTipsInfo(bool showAtStartup, int tipIndex)
{
  wxConfigBase* cfg = wxConfigBase::Get();
  cfg->Write(TIPS_SHOW_AT_STARTUP, showAtStartup);
  cfg->Write(TIPS_LAST_INDEX, tipIndex);
}
#endif

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
