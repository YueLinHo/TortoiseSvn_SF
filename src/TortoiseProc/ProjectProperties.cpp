// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2003-2011 - TortoiseSVN

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
#include "TortoiseProc.h"
#include "UnicodeUtils.h"
#include "ProjectProperties.h"
#include "SVNProperties.h"
#include "SVN.h"
#include "SVNHelpers.h"
#include "TSVNPath.h"
#include "AppUtils.h"
#include <regex>

using namespace std;

#define LOG_REVISIONREGEX _T("\\b(r\\d+)|\\b(revisions?(\\(s\\))?\\s#?\\d+([, ]+(and\\s?)?\\d+)*)|\\b(revs?\\.?\\s?\\d+([, ]+(and\\s?)?\\d+)*)")


ProjectProperties::ProjectProperties(void)
    : regExNeedUpdate (true)
    , bNumber (TRUE)
    , bWarnIfNoIssue (FALSE)
    , nLogWidthMarker (0)
    , nMinLogSize (0)
    , nMinLockMsgSize (0)
    , bFileListInEnglish (TRUE)
    , bAppend (TRUE)
    , lProjectLanguage (0)
    , nBugIdPos(-1)
{
}

ProjectProperties::~ProjectProperties(void)
{
}


BOOL ProjectProperties::ReadPropsPathList(const CTSVNPathList& pathList)
{
    for(int nPath = 0; nPath < pathList.GetCount(); nPath++)
    {
        if (ReadProps(pathList[nPath]))
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL ProjectProperties::ReadProps(CTSVNPath path)
{
    regExNeedUpdate = true;

    BOOL bFoundBugtraqLabel = FALSE;
    BOOL bFoundBugtraqMessage = FALSE;
    BOOL bFoundBugtraqNumber = FALSE;
    BOOL bFoundBugtraqLogRe = FALSE;
    BOOL bFoundBugtraqURL = FALSE;
    BOOL bFoundBugtraqWarnIssue = FALSE;
    BOOL bFoundBugtraqAppend = FALSE;
    BOOL bFoundLogWidth = FALSE;
    BOOL bFoundLogTemplate = FALSE;
    BOOL bFoundLogTemplateCommit = FALSE;
    BOOL bFoundLogTemplateBranch = FALSE;
    BOOL bFoundLogTemplateImport = FALSE;
    BOOL bFoundLogTemplateDelete = FALSE;
    BOOL bFoundLogTemplateMove = FALSE;
    BOOL bFoundLogTemplateMkDir = FALSE;
    BOOL bFoundLogTemplatePropset = FALSE;
    BOOL bFoundLogTemplateLock = FALSE;
    BOOL bFoundMinLogSize = FALSE;
    BOOL bFoundMinLockMsgSize = FALSE;
    BOOL bFoundFileListEnglish = FALSE;
    BOOL bFoundProjectLanguage = FALSE;
    BOOL bFoundUserFileProps = FALSE;
    BOOL bFoundUserDirProps = FALSE;
    BOOL bFoundWebViewRev = FALSE;
    BOOL bFoundWebViewPathRev = FALSE;
    BOOL bFoundAutoProps = FALSE;
    BOOL bFoundLogSummary = FALSE;
    BOOL bFoundBugtraqProviderUuid = FALSE;
    BOOL bFoundBugtraqProviderUuid64 = FALSE;
    BOOL bFoundBugtraqProviderParams = FALSE;
    BOOL bFoundLogRevRegex = FALSE;
    BOOL bFoundStartCommit = FALSE;
    BOOL bFoundPreCommit = FALSE;
    BOOL bFoundPostCommit = FALSE;
    BOOL bFoundStartUpdate = FALSE;
    BOOL bFoundPreUpdate = FALSE;
    BOOL bFoundPostUpdate = FALSE;

    if (!path.IsDirectory())
        path = path.GetContainingDirectory();

    SVN svn;
    for (;;)
    {
        SVNProperties props(path, SVNRev::REV_WC, false);
        for (int i=0; i<props.GetCount(); ++i)
        {
            std::string sPropName = props.GetItemName(i);
            CString sPropVal = CUnicodeUtils::GetUnicode(((char *)props.GetItemValue(i).c_str()));
            if ((!bFoundBugtraqLabel)&&(sPropName.compare (BUGTRAQPROPNAME_LABEL)==0))
            {
                sLabel = sPropVal;
                bFoundBugtraqLabel = TRUE;
            }
            if ((!bFoundBugtraqMessage)&&(sPropName.compare(BUGTRAQPROPNAME_MESSAGE)==0))
            {
                sMessage = sPropVal;
                nBugIdPos = sMessage.Find(L"%BUGID%");
                bFoundBugtraqMessage = TRUE;
            }
            if ((!bFoundBugtraqNumber)&&(sPropName.compare(BUGTRAQPROPNAME_NUMBER)==0))
            {
                CString val;
                val = sPropVal;
                val = val.Trim(_T(" \n\r\t"));
                if ((val.CompareNoCase(_T("false"))==0)||(val.CompareNoCase(_T("no"))==0))
                    bNumber = FALSE;
                else
                    bNumber = TRUE;
                bFoundBugtraqNumber = TRUE;
            }
            if ((!bFoundBugtraqLogRe)&&(sPropName.compare(BUGTRAQPROPNAME_LOGREGEX)==0))
            {
                sCheckRe = sPropVal;
                if (sCheckRe.Find('\n')>=0)
                {
                    sBugIDRe = sCheckRe.Mid(sCheckRe.Find('\n')).Trim();
                    sCheckRe = sCheckRe.Left(sCheckRe.Find('\n')).Trim();
                }
                if (!sCheckRe.IsEmpty())
                {
                    sCheckRe = sCheckRe.Trim();
                }
                bFoundBugtraqLogRe = TRUE;
            }
            if ((!bFoundBugtraqURL)&&(sPropName.compare(BUGTRAQPROPNAME_URL)==0))
            {
                sUrl = sPropVal;
                bFoundBugtraqURL = TRUE;
            }
            if ((!bFoundBugtraqWarnIssue)&&(sPropName.compare(BUGTRAQPROPNAME_WARNIFNOISSUE)==0))
            {
                CString val;
                val = sPropVal;
                val = val.Trim(_T(" \n\r\t"));
                if ((val.CompareNoCase(_T("true"))==0)||(val.CompareNoCase(_T("yes"))==0))
                    bWarnIfNoIssue = TRUE;
                else
                    bWarnIfNoIssue = FALSE;
                bFoundBugtraqWarnIssue = TRUE;
            }
            if ((!bFoundBugtraqAppend)&&(sPropName.compare(BUGTRAQPROPNAME_APPEND)==0))
            {
                CString val;
                val = sPropVal;
                val = val.Trim(_T(" \n\r\t"));
                if ((val.CompareNoCase(_T("true"))==0)||(val.CompareNoCase(_T("yes"))==0))
                    bAppend = TRUE;
                else
                    bAppend = FALSE;
                bFoundBugtraqAppend = TRUE;
            }
            if ((!bFoundBugtraqProviderUuid)&&(sPropName.compare(BUGTRAQPROPNAME_PROVIDERUUID)==0))
            {
                sProviderUuid = sPropVal;
                bFoundBugtraqProviderUuid = TRUE;
            }
            if ((!bFoundBugtraqProviderUuid64)&&(sPropName.compare(BUGTRAQPROPNAME_PROVIDERUUID64)==0))
            {
                sProviderUuid64 = sPropVal;
                bFoundBugtraqProviderUuid64 = TRUE;
            }
            if ((!bFoundBugtraqProviderParams)&&(sPropName.compare(BUGTRAQPROPNAME_PROVIDERPARAMS)==0))
            {
                sProviderParams = sPropVal;
                bFoundBugtraqProviderParams = TRUE;
            }
            if ((!bFoundLogWidth)&&(sPropName.compare(PROJECTPROPNAME_LOGWIDTHLINE)==0))
            {
                CString val;
                val = sPropVal;
                if (!val.IsEmpty())
                {
                    nLogWidthMarker = _ttoi(val);
                }
                bFoundLogWidth = TRUE;
            }
            if ((!bFoundLogTemplate)&&(sPropName.compare(PROJECTPROPNAME_LOGTEMPLATE)==0))
            {
                sLogTemplate = sPropVal;
                sLogTemplate.Remove(_T('\r'));
                sLogTemplate.Replace(_T("\n"), _T("\r\n"));
                bFoundLogTemplate = TRUE;
            }
            if ((!bFoundLogTemplateCommit)&&(sPropName.compare(PROJECTPROPNAME_LOGTEMPLATECOMMIT)==0))
            {
                sLogTemplateCommit = sPropVal;
                sLogTemplateCommit.Remove(_T('\r'));
                sLogTemplateCommit.Replace(_T("\n"), _T("\r\n"));
                bFoundLogTemplateCommit = TRUE;
            }
            if ((!bFoundLogTemplateBranch)&&(sPropName.compare(PROJECTPROPNAME_LOGTEMPLATEBRANCH)==0))
            {
                sLogTemplateBranch = sPropVal;
                sLogTemplateBranch.Remove(_T('\r'));
                sLogTemplateBranch.Replace(_T("\n"), _T("\r\n"));
                bFoundLogTemplateBranch = TRUE;
            }
            if ((!bFoundLogTemplateImport)&&(sPropName.compare(PROJECTPROPNAME_LOGTEMPLATEIMPORT)==0))
            {
                sLogTemplateImport = sPropVal;
                sLogTemplateImport.Remove(_T('\r'));
                sLogTemplateImport.Replace(_T("\n"), _T("\r\n"));
                bFoundLogTemplateImport = TRUE;
            }
            if ((!bFoundLogTemplateDelete)&&(sPropName.compare(PROJECTPROPNAME_LOGTEMPLATEDEL)==0))
            {
                sLogTemplateDelete = sPropVal;
                sLogTemplateDelete.Remove(_T('\r'));
                sLogTemplateDelete.Replace(_T("\n"), _T("\r\n"));
                bFoundLogTemplateDelete = TRUE;
            }
            if ((!bFoundLogTemplateMove)&&(sPropName.compare(PROJECTPROPNAME_LOGTEMPLATEMOVE)==0))
            {
                sLogTemplateMove = sPropVal;
                sLogTemplateMove.Remove(_T('\r'));
                sLogTemplateMove.Replace(_T("\n"), _T("\r\n"));
                bFoundLogTemplateMove = TRUE;
            }
            if ((!bFoundLogTemplateMkDir)&&(sPropName.compare(PROJECTPROPNAME_LOGTEMPLATEMKDIR)==0))
            {
                sLogTemplateMkDir = sPropVal;
                sLogTemplateMkDir.Remove(_T('\r'));
                sLogTemplateMkDir.Replace(_T("\n"), _T("\r\n"));
                bFoundLogTemplateMkDir = TRUE;
            }
            if ((!bFoundLogTemplatePropset)&&(sPropName.compare(PROJECTPROPNAME_LOGTEMPLATEPROPSET)==0))
            {
                sLogTemplatePropset = sPropVal;
                sLogTemplatePropset.Remove(_T('\r'));
                sLogTemplatePropset.Replace(_T("\n"), _T("\r\n"));
                bFoundLogTemplatePropset = TRUE;
            }
            if ((!bFoundLogTemplateLock)&&(sPropName.compare(PROJECTPROPNAME_LOGTEMPLATELOCK)==0))
            {
                sLogTemplateLock = sPropVal;
                sLogTemplateLock.Remove(_T('\r'));
                sLogTemplateLock.Replace(_T("\n"), _T("\r\n"));
                bFoundLogTemplateLock = TRUE;
            }
            if ((!bFoundMinLogSize)&&(sPropName.compare(PROJECTPROPNAME_LOGMINSIZE)==0))
            {
                CString val;
                val = sPropVal;
                if (!val.IsEmpty())
                {
                    nMinLogSize = _ttoi(val);
                }
                bFoundMinLogSize = TRUE;
            }
            if ((!bFoundMinLockMsgSize)&&(sPropName.compare(PROJECTPROPNAME_LOCKMSGMINSIZE)==0))
            {
                CString val;
                val = sPropVal;
                if (!val.IsEmpty())
                {
                    nMinLockMsgSize = _ttoi(val);
                }
                bFoundMinLockMsgSize = TRUE;
            }
            if ((!bFoundFileListEnglish)&&(sPropName.compare(PROJECTPROPNAME_LOGFILELISTLANG)==0))
            {
                CString val;
                val = sPropVal;
                val = val.Trim(_T(" \n\r\t"));
                if ((val.CompareNoCase(_T("false"))==0)||(val.CompareNoCase(_T("no"))==0))
                    bFileListInEnglish = FALSE;
                else
                    bFileListInEnglish = TRUE;
                bFoundFileListEnglish = TRUE;
            }
            if ((!bFoundProjectLanguage)&&(sPropName.compare(PROJECTPROPNAME_PROJECTLANGUAGE)==0))
            {
                CString val;
                val = sPropVal;
                if (!val.IsEmpty())
                {
                    LPTSTR strEnd;
                    lProjectLanguage = _tcstol(val, &strEnd, 0);
                }
                bFoundProjectLanguage = TRUE;
            }
            if ((!bFoundUserFileProps)&&(sPropName.compare(PROJECTPROPNAME_USERFILEPROPERTY)==0))
            {
                sFPPath = sPropVal;
                sFPPath.Replace(_T("\r\n"), _T("\n"));
                bFoundUserFileProps = TRUE;
            }
            if ((!bFoundUserDirProps)&&(sPropName.compare(PROJECTPROPNAME_USERDIRPROPERTY)==0))
            {
                sDPPath = sPropVal;
                sDPPath.Replace(_T("\r\n"), _T("\n"));
                bFoundUserDirProps = TRUE;
            }
            if ((!bFoundAutoProps)&&(sPropName.compare(PROJECTPROPNAME_AUTOPROPS)==0))
            {
                sAutoProps = sPropVal;
                sAutoProps.Replace(_T("\r\n"), _T("\n"));
                bFoundAutoProps = TRUE;
            }
            if ((!bFoundWebViewRev)&&(sPropName.compare(PROJECTPROPNAME_WEBVIEWER_REV)==0))
            {
                sWebViewerRev = sPropVal;
                bFoundWebViewRev = TRUE;
            }
            if ((!bFoundWebViewPathRev)&&(sPropName.compare(PROJECTPROPNAME_WEBVIEWER_PATHREV)==0))
            {
                sWebViewerPathRev = sPropVal;
                bFoundWebViewPathRev = TRUE;
            }
            if ((!bFoundLogSummary)&&(sPropName.compare(PROJECTPROPNAME_LOGSUMMARY)==0))
            {
                sLogSummaryRe = sPropVal;
                bFoundLogSummary = TRUE;
            }
            if ((!bFoundLogRevRegex)&&(sPropName.compare(PROJECTPROPNAME_LOGREVREGEX)==0))
            {
                sLogRevRegex = sPropVal;
                bFoundLogRevRegex = TRUE;
            }
            if ((!bFoundStartCommit)&&(sPropName.compare(PROJECTPROPNAME_STARTCOMMITHOOK)==0))
            {
                sStartCommitHook = sPropVal;
                sStartCommitHook.Replace(_T("\r\n"), _T("\n"));
                bFoundStartCommit = TRUE;
            }
            if ((!bFoundPreCommit)&&(sPropName.compare(PROJECTPROPNAME_PRECOMMITHOOK)==0))
            {
                sPreCommitHook = sPropVal;
                sPreCommitHook.Replace(_T("\r\n"), _T("\n"));
                bFoundPreCommit = TRUE;
            }
            if ((!bFoundPostCommit)&&(sPropName.compare(PROJECTPROPNAME_POSTCOMMITHOOK)==0))
            {
                sPostCommitHook = sPropVal;
                sPostCommitHook.Replace(_T("\r\n"), _T("\n"));
                bFoundPostCommit = TRUE;
            }
            if ((!bFoundStartUpdate)&&(sPropName.compare(PROJECTPROPNAME_STARTUPDATEHOOK)==0))
            {
                sStartUpdateHook = sPropVal;
                sStartUpdateHook.Replace(_T("\r\n"), _T("\n"));
                bFoundStartUpdate = TRUE;
            }
            if ((!bFoundPreUpdate)&&(sPropName.compare(PROJECTPROPNAME_PREUPDATEHOOK)==0))
            {
                sPreUpdateHook = sPropVal;
                sPreUpdateHook.Replace(_T("\r\n"), _T("\n"));
                bFoundPreUpdate = TRUE;
            }
            if ((!bFoundPostUpdate)&&(sPropName.compare(PROJECTPROPNAME_POSTUPDATEHOOK)==0))
            {
                sPostUpdateHook = sPropVal;
                sPostUpdateHook.Replace(_T("\r\n"), _T("\n"));
                bFoundPostUpdate = TRUE;
            }
        }
        if (PathIsRoot(path.GetWinPath()))
            return FALSE;
        propsPath = path;
        path = path.GetContainingDirectory();
        if (!bFoundLogRevRegex)
            sLogRevRegex = LOG_REVISIONREGEX;
        if ((!SVNHelper::IsVersioned(path, true))||(path.IsEmpty()))
        {
            if (bFoundBugtraqLabel | bFoundBugtraqMessage | bFoundBugtraqNumber
                | bFoundBugtraqURL | bFoundBugtraqWarnIssue | bFoundLogWidth
                | bFoundLogTemplate | bFoundLogTemplateBranch | bFoundLogTemplateCommit
                | bFoundLogTemplateImport | bFoundLogTemplateMove | bFoundLogTemplateDelete
                | bFoundLogTemplateMkDir | bFoundLogTemplatePropset | bFoundLogTemplateLock
                | bFoundBugtraqLogRe | bFoundMinLockMsgSize
                | bFoundUserFileProps | bFoundUserDirProps | bFoundAutoProps
                | bFoundWebViewRev | bFoundWebViewPathRev | bFoundLogSummary | bFoundLogRevRegex
                | bFoundBugtraqProviderUuid | bFoundBugtraqProviderUuid64
                | bFoundBugtraqProviderParams | bFoundStartCommit | bFoundPostCommit
                | bFoundStartUpdate | bFoundPreUpdate | bFoundPostUpdate
                | bFoundPreCommit)
            {
                sRepositoryRootUrl = svn.GetRepositoryRoot(propsPath);
                sRepositoryPathUrl = svn.GetURLFromPath(propsPath);

                return TRUE;
            }
            propsPath.Reset();
            return FALSE;
        }
    }
    //return FALSE;     //never reached
}

CString ProjectProperties::GetBugIDFromLog(CString& msg)
{
    CString sBugID;

    if (!sMessage.IsEmpty())
    {
        CString sBugLine;
        CString sFirstPart;
        CString sLastPart;
        BOOL bTop = FALSE;
        if (nBugIdPos<0)
            return sBugID;
        sFirstPart = sMessage.Left(nBugIdPos);
        sLastPart = sMessage.Mid(nBugIdPos+7);
        msg.TrimRight('\n');
        if (msg.ReverseFind('\n')>=0)
        {
            if (bAppend)
                sBugLine = msg.Mid(msg.ReverseFind('\n')+1);
            else
            {
                sBugLine = msg.Left(msg.Find('\n'));
                bTop = TRUE;
            }
        }
        else
        {
            if (bNumber)
            {
                // find out if the message consists only of numbers
                bool bOnlyNumbers = true;
                for (int i=0; i<msg.GetLength(); ++i)
                {
                    if (!_istdigit(msg[i]))
                    {
                        bOnlyNumbers = false;
                        break;
                    }
                }
                if (bOnlyNumbers)
                    sBugLine = msg;
            }
            else
                sBugLine = msg;
        }
        if (sBugLine.IsEmpty() && (msg.ReverseFind('\n') < 0))
        {
            sBugLine = msg.Mid(msg.ReverseFind('\n')+1);
        }
        if (sBugLine.Left(sFirstPart.GetLength()).Compare(sFirstPart)!=0)
            sBugLine.Empty();
        if (sBugLine.Right(sLastPart.GetLength()).Compare(sLastPart)!=0)
            sBugLine.Empty();
        if (sBugLine.IsEmpty())
        {
            if (msg.Find('\n')>=0)
                sBugLine = msg.Left(msg.Find('\n'));
            if (sBugLine.Left(sFirstPart.GetLength()).Compare(sFirstPart)!=0)
                sBugLine.Empty();
            if (sBugLine.Right(sLastPart.GetLength()).Compare(sLastPart)!=0)
                sBugLine.Empty();
            bTop = TRUE;
        }
        if (sBugLine.IsEmpty())
            return sBugID;
        sBugID = sBugLine.Mid(sFirstPart.GetLength(), sBugLine.GetLength() - sFirstPart.GetLength() - sLastPart.GetLength());
        if (bTop)
        {
            msg = msg.Mid(sBugLine.GetLength());
            msg.TrimLeft('\n');
        }
        else
        {
            msg = msg.Left(msg.GetLength()-sBugLine.GetLength());
            msg.TrimRight('\n');
        }
    }
    return sBugID;
}

void ProjectProperties::AutoUpdateRegex()
{
    if (regExNeedUpdate)
    {
        try
        {
            regCheck = tr1::wregex (sCheckRe);
            regBugID = tr1::wregex (sBugIDRe);
        }
        catch (std::exception)
        {
        }

        regExNeedUpdate = false;
    }
}

std::vector<CHARRANGE> ProjectProperties::FindBugIDPositions(const CString& msg)
{
    size_t offset1 = 0;
    size_t offset2 = 0;
    std::vector<CHARRANGE> result;

    // first use the checkre string to find bug ID's in the message
    if (!sCheckRe.IsEmpty())
    {
        if (!sBugIDRe.IsEmpty())
        {

            // match with two regex strings (without grouping!)
            try
            {
                AutoUpdateRegex();
                const tr1::wsregex_iterator end;
                wstring s = msg;
                for (tr1::wsregex_iterator it(s.begin(), s.end(), regCheck); it != end; ++it)
                {
                    // (*it)[0] is the matched string
                    wstring matchedString = (*it)[0];
                    ptrdiff_t matchpos = it->position(0);
                    for (tr1::wsregex_iterator it2(matchedString.begin(), matchedString.end(), regBugID); it2 != end; ++it2)
                    {
                        ATLTRACE(_T("matched id : %s\n"), (*it2)[0].str().c_str());
                        ptrdiff_t matchposID = it2->position(0);
                        CHARRANGE range = {(LONG)(matchpos+matchposID), (LONG)(matchpos+matchposID+(*it2)[0].str().size())};
                        result.push_back (range);
                    }
                }
            }
            catch (exception) {}
        }
        else
        {
            try
            {
                AutoUpdateRegex();
                const tr1::wsregex_iterator end;
                wstring s = msg;
                for (tr1::wsregex_iterator it(s.begin(), s.end(), regCheck); it != end; ++it)
                {
                    const tr1::wsmatch match = *it;
                    // we define group 1 as the whole issue text and
                    // group 2 as the bug ID
                    if (match.size() >= 2)
                    {
                        ATLTRACE(_T("matched id : %s\n"), wstring(match[1]).c_str());
                        CHARRANGE range = {(LONG)(match[1].first-s.begin()), (LONG)(match[1].second-s.begin())};
                        result.push_back (range);
                    }
                }
            }
            catch (exception) {}
        }
    }
    else if (result.empty()&&(!sMessage.IsEmpty()))
    {
        CString sBugLine;
        CString sFirstPart;
        CString sLastPart;
        BOOL bTop = FALSE;
        if (nBugIdPos<0)
            return result;

        sFirstPart = sMessage.Left(nBugIdPos);
        sLastPart = sMessage.Mid(nBugIdPos+7);
        CString sMsg = msg;
        sMsg.TrimRight('\n');
        if (sMsg.ReverseFind('\n')>=0)
        {
            if (bAppend)
                sBugLine = sMsg.Mid(sMsg.ReverseFind('\n')+1);
            else
            {
                sBugLine = sMsg.Left(sMsg.Find('\n'));
                bTop = TRUE;
            }
        }
        else
            sBugLine = sMsg;
        if (sBugLine.Left(sFirstPart.GetLength()).Compare(sFirstPart)!=0)
            sBugLine.Empty();
        if (sBugLine.Right(sLastPart.GetLength()).Compare(sLastPart)!=0)
            sBugLine.Empty();
        if (sBugLine.IsEmpty())
        {
            if (sMsg.Find('\n')>=0)
                sBugLine = sMsg.Left(sMsg.Find('\n'));
            if (sBugLine.Left(sFirstPart.GetLength()).Compare(sFirstPart)!=0)
                sBugLine.Empty();
            if (sBugLine.Right(sLastPart.GetLength()).Compare(sLastPart)!=0)
                sBugLine.Empty();
            bTop = TRUE;
        }
        if (sBugLine.IsEmpty())
            return result;

        CString sBugIDPart = sBugLine.Mid(sFirstPart.GetLength(), sBugLine.GetLength() - sFirstPart.GetLength() - sLastPart.GetLength());
        if (sBugIDPart.IsEmpty())
            return result;

        //the bug id part can contain several bug id's, separated by commas
        if (!bTop)
            offset1 = sMsg.GetLength() - sBugLine.GetLength() + sFirstPart.GetLength();
        else
            offset1 = sFirstPart.GetLength();
        sBugIDPart.Trim(_T(","));
        while (sBugIDPart.Find(',')>=0)
        {
            offset2 = offset1 + sBugIDPart.Find(',');
            CHARRANGE range = {(LONG)offset1, (LONG)offset2};
            result.push_back (range);
            sBugIDPart = sBugIDPart.Mid(sBugIDPart.Find(',')+1);
            offset1 = offset2 + 1;
        }
        offset2 = offset1 + sBugIDPart.GetLength();
        CHARRANGE range = {(LONG)offset1, (LONG)offset2};
        result.push_back (range);
    }

    return result;
}

BOOL ProjectProperties::FindBugID(const CString& msg, CWnd * pWnd)
{
    std::vector<CHARRANGE> positions = FindBugIDPositions (msg);
    CAppUtils::SetCharFormat (pWnd, CFM_LINK, CFE_LINK, positions);

    return positions.empty() ? FALSE : TRUE;
}

std::set<CString> ProjectProperties::FindBugIDs (const CString& msg)
{
    std::vector<CHARRANGE> positions = FindBugIDPositions (msg);
    std::set<CString> bugIDs;

    for ( auto iter = positions.begin(), end = positions.end()
        ; iter != end
        ; ++iter)
    {
        bugIDs.insert (msg.Mid (iter->cpMin, iter->cpMax - iter->cpMin));
    }

    return bugIDs;
}

CString ProjectProperties::FindBugID(const CString& msg)
{
    CString sRet;
    if (!sCheckRe.IsEmpty() || (nBugIdPos >= 0))
    {
        std::set<CString> bugIDs = FindBugIDs(msg);

        for (std::set<CString>::iterator it = bugIDs.begin(); it != bugIDs.end(); ++it)
        {
            sRet += *it;
            sRet += _T(" ");
        }
        sRet.Trim();
    }

    return sRet;
}

bool ProjectProperties::MightContainABugID()
{
    return !sCheckRe.IsEmpty() || (nBugIdPos >= 0);
}

CString ProjectProperties::GetBugIDUrl(const CString& sBugID)
{
    CString ret;
    if (sUrl.IsEmpty())
        return ret;
    if (!sMessage.IsEmpty() || !sCheckRe.IsEmpty())
    {
        ret = sUrl;
        ret.Replace(_T("%BUGID%"), sBugID);
    }
    return ret;
}

BOOL ProjectProperties::CheckBugID(const CString& sID)
{
    if (bNumber)
    {
        // check if the revision actually _is_ a number
        // or a list of numbers separated by colons
        TCHAR c = 0;
        int len = sID.GetLength();
        for (int i=0; i<len; ++i)
        {
            c = sID.GetAt(i);
            if ((c < '0')&&(c != ',')&&(c != ' '))
            {
                return FALSE;
            }
            if (c > '9')
                return FALSE;
        }
    }
    return TRUE;
}

BOOL ProjectProperties::HasBugID(const CString& sMessage)
{
    if (!sCheckRe.IsEmpty())
    {
        try
        {
            AutoUpdateRegex();
            return tr1::regex_search((LPCTSTR)sMessage, regCheck);
        }
        catch (exception) {}
    }
    return FALSE;
}

void ProjectProperties::InsertAutoProps(svn_config_t *cfg)
{
    // every line is an autoprop
    CString sPropsData = sAutoProps;
    bool bEnableAutoProps = false;
    while (!sPropsData.IsEmpty())
    {
        int pos = sPropsData.Find('\n');
        CString sLine = pos >= 0 ? sPropsData.Left(pos) : sPropsData;
        sLine.Trim();
        if (!sLine.IsEmpty())
        {
            // format is '*.something = property=value;property=value;....'
            // find first '=' char
            int equalpos = sLine.Find('=');
            if ((equalpos >= 0)&&(sLine[0] != '#'))
            {
                CString key = sLine.Left(equalpos);
                CString value = sLine.Mid(equalpos);
                key.Trim(_T(" ="));
                value.Trim(_T(" ="));
                svn_config_set(cfg, SVN_CONFIG_SECTION_AUTO_PROPS, (LPCSTR)CUnicodeUtils::GetUTF8(key), (LPCSTR)CUnicodeUtils::GetUTF8(value));
                bEnableAutoProps = true;
            }
        }
        if (pos >= 0)
            sPropsData = sPropsData.Mid(pos).Trim();
        else
            sPropsData.Empty();
    }
    if (bEnableAutoProps)
        svn_config_set(cfg, SVN_CONFIG_SECTION_MISCELLANY, SVN_CONFIG_OPTION_ENABLE_AUTO_PROPS, "yes");
}

bool ProjectProperties::AddAutoProps(const CTSVNPath& path)
{
    if (!path.IsDirectory())
        return true;    // no error, but nothing to do

    bool bRet = true;

    char buf[1024] = {0};
    SVNProperties props(path, SVNRev::REV_WC, false);
    if (!sLabel.IsEmpty())
        bRet = props.Add(BUGTRAQPROPNAME_LABEL, CUnicodeUtils::StdGetUTF8((LPCTSTR)sLabel)) && bRet;
    if (!sMessage.IsEmpty())
        bRet = props.Add(BUGTRAQPROPNAME_MESSAGE, CUnicodeUtils::StdGetUTF8((LPCTSTR)sMessage)) && bRet;
    if (!bNumber)
        bRet = props.Add(BUGTRAQPROPNAME_NUMBER, "false") && bRet;
    if (!sCheckRe.IsEmpty())
        bRet = props.Add(BUGTRAQPROPNAME_LOGREGEX, CUnicodeUtils::StdGetUTF8((LPCTSTR)(sCheckRe + _T("\n") + sBugIDRe))) && bRet;
    if (!sUrl.IsEmpty())
        bRet = props.Add(BUGTRAQPROPNAME_URL, CUnicodeUtils::StdGetUTF8((LPCTSTR)sUrl)) && bRet;
    if (bWarnIfNoIssue)
        bRet = props.Add(BUGTRAQPROPNAME_WARNIFNOISSUE, "true") && bRet;
    if (!bAppend)
        bRet = props.Add(BUGTRAQPROPNAME_APPEND, "false") && bRet;
    if (!sProviderUuid.IsEmpty())
        bRet = props.Add(BUGTRAQPROPNAME_PROVIDERUUID, CUnicodeUtils::StdGetUTF8((LPCTSTR)sProviderUuid)) && bRet;
    if (!sProviderUuid64.IsEmpty())
        bRet = props.Add(BUGTRAQPROPNAME_PROVIDERUUID64, CUnicodeUtils::StdGetUTF8((LPCTSTR)sProviderUuid64)) && bRet;
    if (!sProviderParams.IsEmpty())
        bRet = props.Add(BUGTRAQPROPNAME_PROVIDERPARAMS, CUnicodeUtils::StdGetUTF8((LPCTSTR)sProviderParams)) && bRet;
    if (nLogWidthMarker)
    {
        sprintf_s(buf, "%ld", nLogWidthMarker);
        bRet = props.Add(PROJECTPROPNAME_LOGWIDTHLINE, buf) && bRet;
    }
    if (!sLogTemplate.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_LOGTEMPLATE, CUnicodeUtils::StdGetUTF8((LPCTSTR)sLogTemplate)) && bRet;
    if (nMinLogSize)
    {
        sprintf_s(buf, "%ld", nMinLogSize);
        bRet = props.Add(PROJECTPROPNAME_LOGMINSIZE, buf) && bRet;
    }
    if (nMinLockMsgSize)
    {
        sprintf_s(buf, "%ld", nMinLockMsgSize);
        bRet = props.Add(PROJECTPROPNAME_LOCKMSGMINSIZE, buf) && bRet;
    }
    if (!bFileListInEnglish)
        bRet = props.Add(PROJECTPROPNAME_LOGFILELISTLANG, "false") && bRet;
    if (!sLogSummaryRe.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_LOGSUMMARY, CUnicodeUtils::StdGetUTF8((LPCTSTR)sLogSummaryRe)) && bRet;
    if (!sLogRevRegex.IsEmpty() && sLogRevRegex.Compare(LOG_REVISIONREGEX))
        bRet = props.Add(PROJECTPROPNAME_LOGREVREGEX, CUnicodeUtils::StdGetUTF8((LPCTSTR)sLogRevRegex)) && bRet;
    if (lProjectLanguage)
    {
        sprintf_s(buf, "%ld", lProjectLanguage);
        bRet = props.Add(PROJECTPROPNAME_PROJECTLANGUAGE, buf) && bRet;
    }
    if (!sFPPath.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_USERFILEPROPERTY, CUnicodeUtils::StdGetUTF8((LPCTSTR)sFPPath)) && bRet;
    if (!sDPPath.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_USERDIRPROPERTY, CUnicodeUtils::StdGetUTF8((LPCTSTR)sDPPath)) && bRet;
    if (!sWebViewerRev.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_WEBVIEWER_REV, CUnicodeUtils::StdGetUTF8((LPCTSTR)sWebViewerRev)) && bRet;
    if (!sWebViewerPathRev.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_WEBVIEWER_PATHREV, CUnicodeUtils::StdGetUTF8((LPCTSTR)sWebViewerPathRev)) && bRet;
    if (!sAutoProps.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_AUTOPROPS, CUnicodeUtils::StdGetUTF8((LPCTSTR)sAutoProps)) && bRet;
    if (!sStartCommitHook.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_STARTCOMMITHOOK, CUnicodeUtils::StdGetUTF8((LPCTSTR)sStartCommitHook)) && bRet;
    if (!sPreCommitHook.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_PRECOMMITHOOK, CUnicodeUtils::StdGetUTF8((LPCTSTR)sPreCommitHook)) && bRet;
    if (!sPostCommitHook.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_POSTCOMMITHOOK, CUnicodeUtils::StdGetUTF8((LPCTSTR)sPostCommitHook)) && bRet;
    if (!sStartUpdateHook.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_STARTUPDATEHOOK, CUnicodeUtils::StdGetUTF8((LPCTSTR)sStartUpdateHook)) && bRet;
    if (!sPreUpdateHook.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_PREUPDATEHOOK, CUnicodeUtils::StdGetUTF8((LPCTSTR)sPreUpdateHook)) && bRet;
    if (!sPostUpdateHook.IsEmpty())
        bRet = props.Add(PROJECTPROPNAME_POSTUPDATEHOOK, CUnicodeUtils::StdGetUTF8((LPCTSTR)sPostUpdateHook)) && bRet;
    return bRet;
}

CString ProjectProperties::GetLogSummary(const CString& sMessage)
{
    CString sRet;

    if (!sLogSummaryRe.IsEmpty())
    {
        try
        {
            const tr1::wregex regSum(sLogSummaryRe);
            const tr1::wsregex_iterator end;
            wstring s = sMessage;
            for (tr1::wsregex_iterator it(s.begin(), s.end(), regSum); it != end; ++it)
            {
                const tr1::wsmatch match = *it;
                // we define the first group as the summary text
                if ((*it).size() >= 1)
                {
                    ATLTRACE(_T("matched summary : %s\n"), wstring(match[0]).c_str());
                    sRet += (CString(wstring(match[1]).c_str()));
                }
            }
        }
        catch (exception) {}
    }
    sRet.Trim();

    return sRet;
}

CString ProjectProperties::MakeShortMessage(const CString& message)
{
    enum
    {
        MAX_SHORT_MESSAGE_LENGTH = 240
    };

    bool bFoundShort = true;
    CString sShortMessage = GetLogSummary(message);
    if (sShortMessage.IsEmpty())
    {
        bFoundShort = false;
        sShortMessage = message;
    }
    // Remove newlines and tabs 'cause those are not shown nicely in the list control
    sShortMessage.Remove(_T('\r'));
    sShortMessage.Replace(_T('\t'), _T(' '));

    if (!bFoundShort)
    {
        // Suppose the first empty line separates 'summary' from the rest of the message.
        int found = sShortMessage.Find(_T("\n\n"));
        // To avoid too short 'short' messages
        // (e.g. if the message looks something like "Bugfix:\n\n*done this\n*done that")
        // only use the empty newline as a separator if it comes after at least 15 chars.
        if (found >= 15)
            sShortMessage = sShortMessage.Left(found);

        // still too long? -> truncate after about 2 lines
        if (sShortMessage.GetLength() > MAX_SHORT_MESSAGE_LENGTH)
            sShortMessage = sShortMessage.Left(MAX_SHORT_MESSAGE_LENGTH) + _T("...");
    }

    sShortMessage.Replace('\n', ' ');
    return sShortMessage;
}

const CString& ProjectProperties::GetLogMsgTemplate( const CStringA& prop ) const
{
    if (prop.Compare(PROJECTPROPNAME_LOGTEMPLATECOMMIT) == 0)
        return sLogTemplateCommit.IsEmpty() ? sLogTemplate : sLogTemplateCommit;
    if (prop.Compare(PROJECTPROPNAME_LOGTEMPLATEBRANCH) == 0)
        return sLogTemplateBranch.IsEmpty() ? sLogTemplate : sLogTemplateBranch;
    if (prop.Compare(PROJECTPROPNAME_LOGTEMPLATEIMPORT) == 0)
        return sLogTemplateImport.IsEmpty() ? sLogTemplate : sLogTemplateImport;
    if (prop.Compare(PROJECTPROPNAME_LOGTEMPLATEDEL) == 0)
        return sLogTemplateDelete.IsEmpty() ? sLogTemplate : sLogTemplateDelete;
    if (prop.Compare(PROJECTPROPNAME_LOGTEMPLATEMOVE) == 0)
        return sLogTemplateMove.IsEmpty() ? sLogTemplate : sLogTemplateMove;
    if (prop.Compare(PROJECTPROPNAME_LOGTEMPLATEMKDIR) == 0)
        return sLogTemplateMkDir.IsEmpty() ? sLogTemplate : sLogTemplateMkDir;
    if (prop.Compare(PROJECTPROPNAME_LOGTEMPLATEPROPSET) == 0)
        return sLogTemplatePropset.IsEmpty() ? sLogTemplate : sLogTemplatePropset;
    if (prop.Compare(PROJECTPROPNAME_LOGTEMPLATELOCK) == 0)
        return sLogTemplateLock;    // we didn't use sLogTemplate before for lock messages, so we don't do that now either

    return sLogTemplate;
}

#ifdef DEBUG
static class PropTest
{
public:
    PropTest()
    {
        CString msg = _T("this is a test logmessage: issue 222\nIssue #456, #678, 901  #456");
        CString sUrl = _T("http://tortoisesvn.tigris.org/issues/show_bug.cgi?id=%BUGID%");
        CString sCheckRe = _T("[Ii]ssue #?(\\d+)(,? ?#?(\\d+))+");
        CString sBugIDRe = _T("(\\d+)");
        ProjectProperties props;
        props.sCheckRe = _T("PAF-[0-9]+");
        props.sUrl = _T("http://tortoisesvn.tigris.org/issues/show_bug.cgi?id=%BUGID%");
        CString sRet = props.FindBugID(_T("This is a test for PAF-88"));
        ATLASSERT(sRet.IsEmpty());
        props.sCheckRe = _T("[Ii]ssue #?(\\d+)");
        props.regExNeedUpdate = true;
        sRet = props.FindBugID(_T("Testing issue #99"));
        sRet.Trim();
        ATLASSERT(sRet.Compare(_T("99"))==0);
        props.sCheckRe = _T("[Ii]ssues?:?(\\s*(,|and)?\\s*#\\d+)+");
        props.sBugIDRe = _T("(\\d+)");
        props.sUrl = _T("http://tortoisesvn.tigris.org/issues/show_bug.cgi?id=%BUGID%");
        props.regExNeedUpdate = true;
        sRet = props.FindBugID(_T("This is a test for Issue #7463,#666"));
        ATLASSERT(props.HasBugID(_T("This is a test for Issue #7463,#666")));
        ATLASSERT(!props.HasBugID(_T("This is a test for Issue 7463,666")));
        sRet.Trim();
        ATLASSERT(sRet.Compare(_T("666 7463"))==0);
        props.sCheckRe = _T("^\\[(\\d+)\\].*");
        props.sUrl = _T("http://tortoisesvn.tigris.org/issues/show_bug.cgi?id=%BUGID%");
        props.regExNeedUpdate = true;
        sRet = props.FindBugID(_T("[000815] some stupid programming error fixed"));
        sRet.Trim();
        ATLASSERT(sRet.Compare(_T("000815"))==0);
        props.sCheckRe = _T("\\[\\[(\\d+)\\]\\]\\]");
        props.sUrl = _T("http://tortoisesvn.tigris.org/issues/show_bug.cgi?id=%BUGID%");
        props.regExNeedUpdate = true;
        sRet = props.FindBugID(_T("test test [[000815]]] some stupid programming error fixed"));
        sRet.Trim();
        ATLASSERT(sRet.Compare(_T("000815"))==0);
        ATLASSERT(props.HasBugID(_T("test test [[000815]]] some stupid programming error fixed")));
        ATLASSERT(!props.HasBugID(_T("test test [000815]] some stupid programming error fixed")));
        props.sLogSummaryRe = _T("\\[SUMMARY\\]:(.*)");
        ATLASSERT(props.GetLogSummary(_T("[SUMMARY]: this is my summary")).Compare(_T("this is my summary"))==0);
    }
} PropTest;
#endif

