// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2003-2017 - TortoiseSVN

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

#include "ResizableDialog.h"
#include "registry.h"
#include "AeroControls.h"
#include "CreateProcessHelper.h"
#include "TaskbarUUID.h"
#include "Tooltip.h"
#include "CommonDialogFunctions.h"
#include "CommonAppUtils.h"

#include <Dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "htmlhelp.lib")

#define DIALOG_BLOCKHORIZONTAL 1
#define DIALOG_BLOCKVERTICAL 2


/**
 * \ingroup TortoiseProc
 *
 * A template which can be used as the base-class of dialogs which form the main dialog
 * of a 'dialog-style application'
 * Just provides the boiler-plate code for dealing with application icons
 *
 * \remark Replace all references to CDialog or CResizableDialog in your dialog class with
 * either CResizableStandAloneDialog, CStandAloneDialog or CStateStandAloneDialog, as appropriate
 */
template <typename BaseType> class CStandAloneDialogTmpl : public BaseType, protected CommonDialogFunctions<BaseType>
{
#ifndef _DLL
    DECLARE_DYNAMIC(CStandAloneDialogTmpl)
#endif
protected:
    CStandAloneDialogTmpl(UINT nIDTemplate, CWnd* pParentWnd = nullptr) : BaseType(nIDTemplate, pParentWnd), CommonDialogFunctions(this)
    {
        m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
        m_margins.cxLeftWidth = 0;
        m_margins.cyTopHeight = 0;
        m_margins.cxRightWidth = 0;
        m_margins.cyBottomHeight = 0;
        m_bkgndIconWidth = 0;
        m_bkgndIconHeight = 0;
        m_hBkgndIcon = 0;
        m_nResizeBlock = 0;
        m_height = 0;
        m_width = 0;

        SetBackgroundIcon(IDI_AEROBACKGROUND, 256, 256);
    }
    ~CStandAloneDialogTmpl()
    {
        if (m_hBkgndIcon)
            DestroyIcon(m_hBkgndIcon);
    }
    virtual BOOL OnInitDialog()
    {
        BaseType::OnInitDialog();

        // Set the icon for this dialog.  The framework does this automatically
        //  when the application's main window is not a dialog
        SetIcon(m_hIcon, TRUE);         // Set big icon
        SetIcon(m_hIcon, FALSE);        // Set small icon

        RECT rect;
        GetWindowRect(&rect);
        m_height = rect.bottom - rect.top;
        m_width = rect.right - rect.left;
        EnableToolTips();
        m_tooltips.Create(this);

        return FALSE;
    }

    virtual BOOL PreTranslateMessage(MSG* pMsg)
    {
        m_tooltips.RelayEvent(pMsg, this);
        if (pMsg->message == WM_KEYDOWN)
        {
            int nVirtKey = (int)pMsg->wParam;

            if (nVirtKey == 'A' && (GetKeyState(VK_CONTROL) & 0x8000))
            {
                wchar_t buffer[129];
                ::GetClassName(pMsg->hwnd, buffer, 128);

                if (_wcsnicmp(buffer, L"EDIT", 128) == 0)
                {
                    ::PostMessage(pMsg->hwnd, EM_SETSEL, 0, -1);
                    return TRUE;
                }
            }
        }
        return BaseType::PreTranslateMessage(pMsg);
    }

    afx_msg void OnPaint()
    {
        if (IsIconic())
        {
            CPaintDC dc(this); // device context for painting

            SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

            // Center icon in client rectangle
            int cxIcon = GetSystemMetrics(SM_CXICON);
            int cyIcon = GetSystemMetrics(SM_CYICON);
            CRect rect;
            GetClientRect(&rect);
            int x = (rect.Width() - cxIcon + 1) / 2;
            int y = (rect.Height() - cyIcon + 1) / 2;

            // Draw the icon
            dc.DrawIcon(x, y, m_hIcon);
        }
        else
        {
            BaseType::OnPaint();
        }
    }

    BOOL OnEraseBkgnd(CDC*  pDC)
    {
        BOOL baseRet = BaseType::OnEraseBkgnd(pDC);
        if (m_aeroControls.AeroDialogsEnabled())
        {
            // draw the frame margins in black
            CRect rc;
            GetClientRect(&rc);
            if (m_margins.cxLeftWidth < 0)
            {
                pDC->FillSolidRect(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, RGB(0,0,0));
                if (m_hBkgndIcon)
                {
                    // center the icon
                    double scale = 1.0;
                    scale = min(scale, double(rc.Width())/double(m_bkgndIconWidth));
                    scale = min(scale, double(rc.Height())/double(m_bkgndIconHeight));
                    if (scale > 1.0)
                        scale = 1.0;
                    DrawIconEx(pDC->GetSafeHdc(),
                        int((rc.Width()-(scale*m_bkgndIconWidth))/1.0),
                        0,
                        m_hBkgndIcon,
                        int(scale*m_bkgndIconWidth), int(scale*m_bkgndIconHeight),
                        0, NULL, DI_NORMAL);
                }
            }
            else
            {
                pDC->FillSolidRect(rc.left, rc.top, m_margins.cxLeftWidth, rc.bottom-rc.top, RGB(0,0,0));
                pDC->FillSolidRect(rc.left, rc.top, rc.right-rc.left, m_margins.cyTopHeight, RGB(0,0,0));
                pDC->FillSolidRect(rc.right-m_margins.cxRightWidth, rc.top, m_margins.cxRightWidth, rc.bottom-rc.top, RGB(0,0,0));
                pDC->FillSolidRect(rc.left, rc.bottom-m_margins.cyBottomHeight, rc.right-rc.left, m_margins.cyBottomHeight, RGB(0,0,0));
            }
        }
        return baseRet;
    }

    LRESULT OnNcHitTest(CPoint pt)
    {
        if (m_aeroControls.AeroDialogsEnabled())
        {
            CRect rc;
            GetClientRect(&rc);
            ClientToScreen(&rc);

            if (m_margins.cxLeftWidth < 0)
            {
                return rc.PtInRect(pt) ? HTCAPTION : BaseType::OnNcHitTest(pt);
            }
            else
            {
                CRect m = rc;
                m.DeflateRect(m_margins.cxLeftWidth, m_margins.cyTopHeight, m_margins.cxRightWidth, m_margins.cyBottomHeight);
                return (rc.PtInRect(pt) && !m.PtInRect(pt)) ? HTCAPTION : BaseType::OnNcHitTest(pt);
            }
        }
        return BaseType::OnNcHitTest(pt);
    }

    void ExtendFrameIntoClientArea(UINT bottomControl)
    {
        ExtendFrameIntoClientArea(0, 0, 0, bottomControl);
    }

    /**
     *
     */
    void ExtendFrameIntoClientArea(UINT leftControl, UINT topControl, UINT rightControl, UINT botomControl)
    {
        if (!m_aeroControls.AeroDialogsEnabled())
            return;
        RECT rc, rc2;
        GetWindowRect(&rc);
        GetClientRect(&rc2);
        ClientToScreen(&rc2);

        RECT rccontrol;
        if (leftControl)
        {
            HWND hw = GetDlgItem(leftControl)->GetSafeHwnd();
            if (hw == NULL)
                return;
            ::GetWindowRect(hw, &rccontrol);
            m_margins.cxLeftWidth = rccontrol.left - rc.left;
            m_margins.cxLeftWidth -= (rc2.left-rc.left);
        }
        else
            m_margins.cxLeftWidth = 0;

        if (topControl)
        {
            HWND hw = GetDlgItem(topControl)->GetSafeHwnd();
            if (hw == NULL)
                return;
            ::GetWindowRect(hw, &rccontrol);
            m_margins.cyTopHeight = rccontrol.top - rc.top;
            m_margins.cyTopHeight -= (rc2.top-rc.top);
        }
        else
            m_margins.cyTopHeight = 0;

        if (rightControl)
        {
            HWND hw = GetDlgItem(rightControl)->GetSafeHwnd();
            if (hw == NULL)
                return;
            ::GetWindowRect(hw, &rccontrol);
            m_margins.cxRightWidth = rc.right - rccontrol.right;
            m_margins.cxRightWidth -= (rc.right-rc2.right);
        }
        else
            m_margins.cxRightWidth = 0;

        if (botomControl)
        {
            HWND hw = GetDlgItem(botomControl)->GetSafeHwnd();
            if (hw == NULL)
                return;
            ::GetWindowRect(hw, &rccontrol);
            m_margins.cyBottomHeight = rc.bottom - rccontrol.bottom;
            m_margins.cyBottomHeight -= (rc.bottom-rc2.bottom);
        }
        else
            m_margins.cyBottomHeight = 0;

        if ((m_margins.cxLeftWidth == 0)&&
            (m_margins.cyTopHeight == 0)&&
            (m_margins.cxRightWidth == 0)&&
            (m_margins.cyBottomHeight == 0))
        {
            m_margins.cxLeftWidth = -1;
            m_margins.cyTopHeight = -1;
            m_margins.cxRightWidth = -1;
            m_margins.cyBottomHeight = -1;
        }
        DwmExtendFrameIntoClientArea(m_hWnd, &m_margins);
    }

    /**
     * Wrapper around the CWnd::EnableWindow() method, but
     * makes sure that a control that has the focus is not disabled
     * before the focus is passed on to the next control.
     */
    BOOL DialogEnableWindow(UINT nID, BOOL bEnable)
    {
        CWnd * pwndDlgItem = GetDlgItem(nID);
        if (pwndDlgItem == NULL)
            return FALSE;
        if (bEnable)
            return pwndDlgItem->EnableWindow(bEnable);
        if (GetFocus() == pwndDlgItem)
        {
            SendMessage(WM_NEXTDLGCTL, 0, FALSE);
        }
        return pwndDlgItem->EnableWindow(bEnable);
    }

    /**
     * Refreshes the cursor by forcing a WM_SETCURSOR message.
     */
    void RefreshCursor()
    {
        POINT pt;
        GetCursorPos(&pt);
        SetCursorPos(pt.x, pt.y);
    }

    bool IsCursorOverWindowBorder()
    {
        RECT wrc, crc;
        this->GetWindowRect(&wrc);
        this->GetClientRect(&crc);
        ClientToScreen(&crc);
        DWORD pos = GetMessagePos();
        POINT pt;
        pt.x = GET_X_LPARAM(pos);
        pt.y = GET_Y_LPARAM(pos);
        return (PtInRect(&wrc, pt) && !PtInRect(&crc, pt));
    }
    void SetBackgroundIcon(HICON hIcon, int width, int height)
    {
        m_hBkgndIcon = hIcon;
        m_bkgndIconWidth = width;
        m_bkgndIconHeight = height;
    }
    void SetBackgroundIcon(UINT idi, int width, int height)
    {
        HICON hIcon = (HICON)LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(idi), IMAGE_ICON, width, height, LR_DEFAULTCOLOR);
        SetBackgroundIcon(hIcon, width, height);
    }
    void BlockResize(int block)
    {
        m_nResizeBlock = block;
    }

    void EnableSaveRestore(LPCTSTR pszSection, bool bRectOnly = FALSE)
    {
        // call the base method with the bHorzResize and bVertResize parameters
        // figured out from the resize block flags.
        BaseType::EnableSaveRestore(pszSection, bRectOnly, (m_nResizeBlock&DIALOG_BLOCKHORIZONTAL) == 0, (m_nResizeBlock&DIALOG_BLOCKVERTICAL) == 0);
    };

protected:
    MARGINS         m_margins;
    AeroControlBase m_aeroControls;
    CToolTips       m_tooltips;
    int             m_nResizeBlock;
    long            m_width;
    long            m_height;

    DECLARE_MESSAGE_MAP()
private:
    HCURSOR OnQueryDragIcon()
    {
        return static_cast<HCURSOR>(m_hIcon);
    }

    virtual void HtmlHelp(DWORD_PTR dwData, UINT nCmd = 0x000F)
    {
        UNREFERENCED_PARAMETER(nCmd);

        if (!CCommonAppUtils::StartHtmlHelp(dwData))
        {
            AfxMessageBox(AFX_IDP_FAILED_TO_LAUNCH_HELP);
        }
    }
protected:
    void OnCompositionChanged()
    {
        if (m_aeroControls.AeroDialogsEnabled())
        {
            DwmExtendFrameIntoClientArea(m_hWnd, &m_margins);
        }
        BaseType::OnCompositionChanged();
    }

    afx_msg LRESULT OnTaskbarButtonCreated(WPARAM /*wParam*/, LPARAM /*lParam*/)
    {
        SetUUIDOverlayIcon(m_hWnd);
        return 0;
    }

    HICON           m_hIcon;
    HICON           m_hBkgndIcon;
    int             m_bkgndIconWidth;
    int             m_bkgndIconHeight;
};

class CStateDialog : public CDialog, public CResizableWndState
{
    DECLARE_DYNAMIC(CStateDialog)
public:
    CStateDialog() : CDialog()
        , m_bEnableSaveRestore(false)
        , m_bRectOnly(false){}
    CStateDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL) : CDialog(nIDTemplate, pParentWnd)
        , m_bEnableSaveRestore(false)
        , m_bRectOnly(false){}
    CStateDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL) : CDialog(lpszTemplateName, pParentWnd)
        , m_bEnableSaveRestore(false)
        , m_bRectOnly(false){}
    virtual ~CStateDialog(){};

private:
    // flags
    bool m_bEnableSaveRestore;
    bool m_bRectOnly;

    // internal status
    CString m_sSection;         // section name (identifies a parent window)

protected:
    // overloaded method, but since this dialog class is for non-resizable dialogs,
    // the bHorzResize and bVertResize params are ignored and passed as false
    // to the base method.
    void EnableSaveRestore(LPCTSTR pszSection, bool bRectOnly = FALSE, BOOL bHorzResize = TRUE, BOOL bVertResize = TRUE)
    {
        UNREFERENCED_PARAMETER(bHorzResize);
        UNREFERENCED_PARAMETER(bVertResize);

        m_sSection = pszSection;

        m_bEnableSaveRestore = true;
        m_bRectOnly = bRectOnly;

        // restore immediately
        LoadWindowRect(pszSection, bRectOnly, false, false);
    };

    virtual CWnd* GetResizableWnd() const
    {
        // make the layout know its parent window
        return CWnd::FromHandle(m_hWnd);
    };

    afx_msg void OnDestroy()
    {
        if (m_bEnableSaveRestore)
            SaveWindowRect(m_sSection, m_bRectOnly);
        CDialog::OnDestroy();
    };

    DECLARE_MESSAGE_MAP()

};

class CResizableStandAloneDialog : public CStandAloneDialogTmpl<CResizableDialog>
{
public:
    CResizableStandAloneDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

private:
    DECLARE_DYNAMIC(CResizableStandAloneDialog)

protected:
    afx_msg void    OnSizing(UINT fwSide, LPRECT pRect);
    afx_msg void    OnMoving(UINT fwSide, LPRECT pRect);
    afx_msg void    OnNcMButtonUp(UINT nHitTest, CPoint point);
    afx_msg void    OnNcRButtonUp(UINT nHitTest, CPoint point);
    afx_msg LRESULT OnNcHitTest(CPoint point);
    void            OnCantStartThread();
    bool            OnEnterPressed();

    DECLARE_MESSAGE_MAP()

private:
    bool        m_bVertical;
    bool        m_bHorizontal;
    CRect       m_rcOrgWindowRect;
    int         m_stickySize;
public:
};

class CStandAloneDialog : public CStandAloneDialogTmpl<CDialog>
{
public:
    CStandAloneDialog(UINT nIDTemplate, CWnd* pParentWnd = nullptr);
protected:
    DECLARE_MESSAGE_MAP()
private:
    DECLARE_DYNAMIC(CStandAloneDialog)
};

class CStateStandAloneDialog : public CStandAloneDialogTmpl<CStateDialog>
{
public:
    CStateStandAloneDialog(UINT nIDTemplate, CWnd* pParentWnd = nullptr);
protected:
    DECLARE_MESSAGE_MAP()
private:
    DECLARE_DYNAMIC(CStateStandAloneDialog)
};