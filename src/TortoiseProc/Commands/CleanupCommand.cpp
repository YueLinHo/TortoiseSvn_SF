// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2007 - TortoiseSVN

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "StdAfx.h"
#include "CleanupCommand.h"

#include "MessageBox.h"
#include "ProgressDlg.h"
#include "SVN.h"
#include "SVNGlobal.h"
#include "SVNAdminDir.h"
#include "DirFileEnum.h"
#include "ShellUpdater.h"

bool CleanupCommand::Execute()
{
	CProgressDlg progress;
	progress.SetTitle(IDS_PROC_CLEANUP);
	progress.SetAnimation(IDR_CLEANUPANI);
	progress.SetShowProgressBar(false);
	progress.SetLine(1, CString(MAKEINTRESOURCE(IDS_PROC_CLEANUP_INFO1)));
	progress.SetLine(2, CString(MAKEINTRESOURCE(IDS_PROC_CLEANUP_INFO2)));
	progress.ShowModeless(hwndExplorer);
	for (int i=0; i<pathList.GetCount(); ++i)
	{
		SVN svn;
		if (!svn.CleanUp(pathList[i]))
		{
			progress.Stop();
			CString errorMessage;
			errorMessage.Format(IDS_ERR_CLEANUP, (LPCTSTR)svn.GetLastErrorMessage());
			CMessageBox::Show(hwndExplorer, errorMessage, _T("TortoiseSVN"), MB_ICONERROR);
			break;
		}
		else
		{
			// after the cleanup has finished, crawl the path downwards and send a change
			// notification for every directory to the shell. This will update the
			// overlays in the left treeview of the explorer.
			CDirFileEnum crawler(pathList[i].GetWinPathString());
			CString sPath;
			bool bDir = false;
			CTSVNPathList updateList;
			while (crawler.NextFile(sPath, &bDir))
			{
				if ((bDir) && (!g_SVNAdminDir.IsAdminDirPath(sPath)))
				{
					updateList.AddPath(CTSVNPath(sPath));
				}
			}
			updateList.AddPath(pathList[i]);
			CShellUpdater::Instance().AddPathsForUpdate(updateList);
			CShellUpdater::Instance().Flush();
			updateList.SortByPathname(true);
			for (INT_PTR i=0; i<updateList.GetCount(); ++i)
			{
				SHChangeNotify(SHCNE_UPDATEITEM, SHCNF_PATH, updateList[i].GetWinPath(), NULL);
				ATLTRACE(_T("notify change for path %s\n"), updateList[i].GetWinPath());
			}
		}
	}
	progress.Stop();
	CMessageBox::Show(hwndExplorer, IDS_PROC_CLEANUPFINISHED, IDS_APPNAME, MB_OK | MB_ICONINFORMATION);
	return true;
}