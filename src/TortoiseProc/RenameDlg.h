// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2003-2006, 2009-2010, 2013-2015 - TortoiseSVN

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
#include "IInputValidator.h"

/**
 * \ingroup TortoiseProc
 * Helper dialog, asking for a new name.
 */
class CRenameDlg : public CResizableStandAloneDialog
{
    DECLARE_DYNAMIC(CRenameDlg)

public:
    CRenameDlg(CWnd* pParent = NULL);
    virtual ~CRenameDlg();

    void SetInputValidator(IInputValidator * validator) { m_pInputValidator = validator; }
    void SetRenameRequired(bool renameRequired) { m_renameRequired = renameRequired; }
    void SetFileSystemAutoComplete() { m_bFSAutoComplete = true; }
    enum { IDD = IDD_RENAME };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();

    afx_msg void OnEnSetfocusName();

    DECLARE_MESSAGE_MAP()

public:
    CString m_name;
    CString m_windowtitle;
    CString m_label;

private:
    bool                m_bBalloonVisible;
    bool                m_renameRequired;
    bool                m_bFSAutoComplete;
    CString             m_originalName;
    IInputValidator *   m_pInputValidator;
};
