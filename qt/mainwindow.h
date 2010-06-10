/*
 * ====================================================================
 * Copyright (c) 2002-2010 The RapidSVN Group.  All rights reserved.
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <svncpp/apr.hpp>

namespace Ui
{
    class MainWindow;
}

namespace svn
{
    class Context;
}

typedef enum
{
  ACTIVEPANE_FOLDER_BROWSER,
  ACTIVEPANE_FILELIST
}
ActivePane;

class Action;
class ActionWorker;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0, const QString & title = "");
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    // File menu
    void OnAddWcBookmark();
    void OnAddRepoBookmark();
    void OnRemoveBookmark();
    void OnEditBookmark();
    void OnQuit();

    // View menu
    void OnPreferences();
    void OnRefresh();
    void OnColumnReset();
    void OnColumn();
    void OnIncludePath();
    void OnSortAscending();
    void OnColumnSorting();
    void OnFlatView();
    void OnRefreshWithUpdate();
    void OnShowUnversioned();
    void OnShowUnmodified();
    void OnShowModified();
    void OnShowConflicted();
    void OnIgnoreExternals();
    void OnShowIgnored();
    void OnStop();
    void OnIndicateModifiedChildren();

    // Query menu
    void OnLog();
    void OnInfo();

    // Modify menu
    void OnFileCommand();

    // Extras menu
    void OnCleanup();

    // Help menu
    void OnHelpContents();
    void OnHelpIndex();
    void OnHelpStartupTips();
    void OnAbout();

    // Test menu
    void OnTestNewWxString();
    void OnTestListener();
    void OnTestCheckout();
    void OnTestCertDlg();
    void OnTestDestinationDlg();
    void OnTestDndDlg();

    // toolbar events
    void OnToolEnter();

    // Events from action threads
    void OnActionEvent();
    void OnListenerEvent();


    // Authentication
    void OnLogin();
    void OnLogout();

    // Folder Browser event handlers
    void OnFolderBrowserSelChanged();
    void OnFolderBrowserKeyDown();

    // File List event handles
    void OnFileListSelected();

    // Enable/disable action menu items
    void OnUpdateCommand();

    void OnIdle();

    virtual void OnActivate();
    virtual void OnSize();
    virtual void OnFolderBrowserSetFocus();
    virtual void OnListCtrlSetFocus();

private:
    Ui::MainWindow *ui;

    // Bookmarks
    void AddWcBookmark();
    void AddRepoBookmark();
    void RemoveBookmark();
    void EditBookmark();

    /**
     * Invoke the default action on the currently selected item(s)
     *
     * For folders, this is to open them within RapidSVN. For files, this is to
     * activate the default external editor.
     *
     * @return false if no items were found to invoke default action on
     */
    bool InvokeDefaultAction();

    /**
     * Update the contents of the filelist
     */
    void RefreshFileList();

    /**
     * Update the contents of the folder browser
     */
    void RefreshFolderBrowser();

    // Auxiliary functions
    void UpdateCurrentPath();
    void UpdateMenuSorting();
    void UpdateMenuIncludePath();
    void UpdateMenuAscending();
    void SetIncludePathVisibility(bool flatMode);

    // Utility functions
    void ShowLog();
    void ShowInfo();

    void DelEntries();
    void MakeRevert();
    void MakeResolve();

    void MakeCopy();
    void Rename();
    void Mkdir();
    void Merge();
    void Contents();
    void ShowPreferences();

    /* void PrintTimeMeasurements(apr_time_t start, apr_time_t end,
                               const QString & name); */
    void Perform(Action * action);

private:
    /** hide implementation data */
    struct Data;
    Data * m;

    ActionWorker * m_actionWorker;

    QString m_title;
    svn::Context * m_context;
};

#endif // MAINWINDOW_H
