// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2011 - TortoiseSVN

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
#pragma once
#include "StandAloneDlg.h"
#include "Tooltip.h"


// CCleanupDlg dialog

class CCleanupDlg : public CStandAloneDialog
{
    DECLARE_DYNAMIC(CCleanupDlg)

public:
    CCleanupDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CCleanupDlg();

    // Dialog Data
    enum { IDD = IDD_CLEANUP };

public:
    BOOL m_bCleanup;
    BOOL m_bRevert;
    BOOL m_bDelUnversioned;
    BOOL m_bRefreshShell;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    void         EnableOKButton();

    DECLARE_MESSAGE_MAP()

private:
    CToolTips           m_tooltips;
public:
    afx_msg void OnBnClickedHelp();
    afx_msg void OnBnClickedCleanup();
    afx_msg void OnBnClickedRevert();
    afx_msg void OnBnClickedDeleteunversioned();
    afx_msg void OnBnClickedRefreshshell();
};
