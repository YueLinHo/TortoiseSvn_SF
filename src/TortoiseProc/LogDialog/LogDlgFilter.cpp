// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2009-2010 - TortoiseSVN

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
#include "stdafx.h"
#include "LogDlgFilter.h"
#include "LogDlg.h"
#include "PathUtils.h"
#include "UnicodeUtils.h"

namespace
{
    // case-sensitivity optimization functions

    bool IsAllASCII7 (const CString& s)
    {
        for (int i = 0, count = s.GetLength(); i < count; ++i)
            if (s[i] >= 0x80)
                return false;

        return true;
    }

    void FastLowerCaseConversion (wchar_t* s)
    {
        for (wchar_t c = *s; c != 0; c = *++s)
            if ((c <= 'Z') && (c >= 'A'))
                *s += 'a' - 'A';
    }
};

// filter utiltiy method

bool CLogDlgFilter::Match (wstring& text) const
{
    // empty text does not match

    if (text.size() == 0)
        return false;

    if (patterns.empty())
    {
        // normalize to lower case

        if (!caseSensitive)
            if (fastLowerCase)
                FastLowerCaseConversion (&text.at(0));
            else
                _wcslwr_s (&text.at(0), text.length()+1);

        // require all strings to be present

        assert (subStrings.size() == exclude.size());
        for (size_t i = 0, count = subStrings.size(); i < count; ++i)
        {
            bool found = wcsstr (text.c_str(), subStrings[i].c_str()) != NULL;
            if (found == exclude[i])
                return false;
        }
    }
    else
    {
        for (vector<tr1::wregex>::const_iterator it = patterns.begin(); it != patterns.end(); ++it)
            if (!regex_search(text, *it, tr1::regex_constants::match_any))
                return false;
    }

    return true;
}

std::vector<CHARRANGE> CLogDlgFilter::GetMatchRanges (wstring& text) const
{
    std::vector<CHARRANGE> ranges;
    if (patterns.empty())
    {
        // normalize to lower case

        if (!caseSensitive && !text.empty())
            if (fastLowerCase)
                FastLowerCaseConversion (&text.at(0));
            else
                _wcslwr_s (&text.at(0), text.length()+1);

        // require all strings to be present

        assert (subStrings.size() == exclude.size());
        const wchar_t* toScan = text.c_str();
        for (size_t i = 0, count = subStrings.size(); i < count; ++i)
        {
            if (!exclude[i])
            {
                const wchar_t * toFind = subStrings[i].c_str();
                size_t toFindLength = subStrings[i].length();
                const wchar_t * pFound = wcsstr (toScan, toFind);
                while (pFound)
                {
                    CHARRANGE range;
                    range.cpMin = (LONG)(pFound - toScan);
                    range.cpMax = (LONG)(range.cpMin + toFindLength);
                    ranges.push_back(range);
                    pFound = wcsstr (pFound+1, toFind);
                }
            }
        }
    }
    else
    {
        for (vector<tr1::wregex>::const_iterator it = patterns.begin(); it != patterns.end(); ++it)
        {
            const tr1::wsregex_iterator end;
            for (tr1::wsregex_iterator it2(text.begin(), text.end(), *it); it2 != end; ++it2)
            {
                ptrdiff_t matchposID = it2->position(0);
                CHARRANGE range = {(LONG)(matchposID), (LONG)(matchposID+(*it2)[0].str().size())};
                ranges.push_back(range);
            }
        }
    }

    // the caller expects the result to be ordered by position which
    // which may not be the case after scanninf for multiple sub-strings

    if (ranges.size() > 1)
    {
        auto begin = ranges.begin();
        auto end = ranges.end();

        std::sort(begin, end, 
                    [] (const CHARRANGE& lhs, const CHARRANGE& rhs) -> bool
                        {return lhs.cpMin < rhs.cpMin;}
                 );

        // once we are at it, merge adjacent and / or overlapping ranges

        auto target = begin;
        for (auto source = begin + 1; source != end; ++source)
            if (target->cpMax < source->cpMin)
                *(++target) = *source;
            else
                target->cpMax = max (target->cpMax, source->cpMax);

        ranges.erase (++target, end);
    }

    return ranges;
}

// called to parse a (potentially incorrect) regex spec

bool CLogDlgFilter::ValidateRegexp (LPCTSTR regexp_str, vector<tr1::wregex>& patterns)
{
    try
    {
        tr1::wregex pat;
        tr1::regex_constants::syntax_option_type type
            = caseSensitive
            ? tr1::regex_constants::ECMAScript
            : tr1::regex_constants::ECMAScript | tr1::regex_constants::icase;

        pat = tr1::wregex(regexp_str, type);
        patterns.push_back(pat);
        return true;
    }
    catch (exception) {}
    return false;
}

// construction utility

void CLogDlgFilter::AddSubString (CString token, bool negate)
{
    if (token.IsEmpty())
        return;

    if (!caseSensitive)
    {
        token.MakeLower();

        // If the search string (UTF16!) is pure ASCII-7,
        // no locale specifics should apply.
        // Exceptions to C-locale are usually either limited
        // to lowercase -> uppercase conversion or they map
        // their lowercase chars beyond U+0x80.

        fastLowerCase |= IsAllASCII7 (token);
    }

    // handle token exclusion

    exclude.push_back (negate);

    // store token & get the next one

    subStrings.push_back ((LPCTSTR)token);
}

// construction

CLogDlgFilter::CLogDlgFilter()
    : attributeSelector(UINT_MAX)
    , caseSensitive(false)
    , from(0)
    , to(0)
    , scanRelevantPathsOnly(false)
    , revToKeep(0)
    , negate(false)
    , fastLowerCase(false)
{
}

CLogDlgFilter::CLogDlgFilter
    ( const CString& filter
    , bool filterWithRegex
    , DWORD selectedFilter
    , bool caseSensitive
    , __time64_t from
    , __time64_t to
    , bool scanRelevantPathsOnly
    , svn_revnum_t revToKeep)

    : negate (false)
    , attributeSelector ( selectedFilter )
    , caseSensitive (caseSensitive)
    , fastLowerCase (false)
    , from (from)
    , to (to)
    , scanRelevantPathsOnly (scanRelevantPathsOnly)
    , revToKeep (revToKeep)
{
    // initialize scratch objects

    scratch.reserve (0xfff0);
    utf8PathScratch.reserve (0xff0);
    utf16PathScratch.reserve (0xff0);

    // decode string matching spec

    bool useRegex = filterWithRegex && !filter.IsEmpty();
    CString filterText = filter;

    // if the first char is '!', negate the filter
    if (filter.GetLength() && filter[0] == '!')
    {
        negate = true;
        filterText = filterText.Mid(1);
    }

    if (useRegex)
        useRegex = ValidateRegexp (filterText, patterns);

    if (!useRegex)
    {
        fastLowerCase = !caseSensitive;

        // now split the search string into words so we can search for each of them

        int curPos = 0;
        int length = filterText.GetLength();

        while ((curPos < length) && (curPos >= 0))
        {
            // skip spaces

            for (; (curPos < length) && (filterText[curPos] == ' '); ++curPos)
            {
            }

            // negation?

            bool negation = false;
            if ((curPos < length) && (filterText[curPos] == '-'))
            {
                negation = true;
                ++curPos;
            }

            // escaped string?

            if ((curPos < length) && (filterText[curPos] == '"'))
            {
                CString subString;
                while (++curPos < length)
                {
                    if (filterText[curPos] == '"')
                    {
                        // double double quotes?

                        if ((++curPos < length) && (filterText[curPos] == '"'))
                        {
                            // keep one and continue within sub-string

                            subString.AppendChar ('"');
                        }
                        else
                        {
                            // end of sub-string?

                            if ((curPos >= length) || (filterText[curPos] == ' '))
                            {
                                break;
                            }
                            else
                            {
                                // add to sub-string & continue within it

                                subString.AppendChar ('"');
                                subString.AppendChar (filterText[curPos]);
                            }
                        }
                    }
                    else
                    {
                        subString.AppendChar (filterText[curPos]);
                    }
                }

                AddSubString (subString, negation);
                ++curPos;
                continue;
            }

            // ordinary sub-string

            AddSubString (filterText.Tokenize (_T(" "), curPos), negation);
        }
    }
}

 // apply filter

namespace
{
    // concatenate strings

    void AppendString
        ( wstring& target
        , const wchar_t* toAppend
        , size_t length
        , wchar_t separator)
    {
        if (target.size() + length + 1 > target.capacity())
            target.reserve (2 * target.capacity());

        target.push_back (separator);
        target.append (toAppend, length);
    }

    void AppendString (wstring& target, const CString& toAppend)
    {
        AppendString (target, toAppend, toAppend.GetLength(), ' ');
    }

    void AppendString (wstring& target, const wstring& toAppend)
    {
        AppendString (target, toAppend.c_str(), toAppend.length(), ' ');
    }

    void AppendPath (wstring& target, const CString& toAppend)
    {
        AppendString (target, toAppend, toAppend.GetLength(), '|');
    }

    void AppendPath (wstring& target, const wstring& toAppend)
    {
        AppendString (target, toAppend.c_str(), toAppend.length(), '|');
    }

    // convert path objects

    void GetPath
        ( const LogCache::CDictionaryBasedPath& path
        , std::string& utf8Path
        , std::wstring& utf16Path)
    {
        path.GetPath (utf8Path);

        // relative path strings are never empty

        CPathUtils::Unescape (&utf8Path[0]);

        // we don't need to adjust the path length as
        // the conversion will automatically stop at \0.

        CUnicodeUtils::UTF8ToUTF16 (utf8Path, utf16Path);
    }
}

bool CLogDlgFilter::operator() (const CLogEntryData& entry) const
{
    // quick checks

    if (entry.GetRevision() == revToKeep)
        return true;

    __time64_t date = entry.GetDate();
    if (attributeSelector & LOGFILTER_DATERANGE)
    {
        if ((date < from) || (date > to))
            return false;
    }

    if (patterns.empty() && subStrings.empty())
        return !negate;

    // we need to perform expensive string / pattern matching

    scratch.clear();
    if (attributeSelector & LOGFILTER_BUGID)
        AppendString (scratch, entry.GetBugIDs());

    if (attributeSelector & LOGFILTER_MESSAGES)
        AppendString (scratch, entry.GetMessage());

    if (attributeSelector & LOGFILTER_PATHS)
    {
        const CLogChangedPathArray& paths = entry.GetChangedPaths();
        for ( size_t cpPathIndex = 0, pathCount = paths.GetCount()
            ; cpPathIndex < pathCount
            ; ++cpPathIndex)
        {
            const CLogChangedPath& cpath = paths[cpPathIndex];
            if (!scanRelevantPathsOnly || cpath.IsRelevantForStartPath())
            {
                GetPath (cpath.GetCachedPath(), utf8PathScratch, utf16PathScratch);
                AppendPath (scratch, utf16PathScratch);
                AppendPath (scratch, cpath.GetActionString());

                if (cpath.GetCopyFromRev() > 0)
                {
                    GetPath (cpath.GetCachedCopyFromPath(), utf8PathScratch, utf16PathScratch);
                    AppendPath (scratch, utf16PathScratch);

                    scratch.push_back ('|');

                    wchar_t buffer[10];
                    _itow_s (cpath.GetCopyFromRev(), buffer, 10);
                    scratch.append (buffer);
                }
            }
        }
    }

    if (attributeSelector & LOGFILTER_AUTHORS)
        AppendString (scratch, entry.GetAuthor());
    if (attributeSelector & LOGFILTER_DATE)
        AppendString (scratch, entry.GetDateString());

    if (attributeSelector & LOGFILTER_REVS)
    {
        scratch.push_back (' ');

        wchar_t buffer[10];
        _itow_s (entry.GetRevision(), buffer, 10);
        scratch.append (buffer);
    }

    return Match (scratch) ^ negate;
}

// tr1::regex is very slow when running concurrently
// in multiple threads. Empty filters don't need MT as well.

bool CLogDlgFilter::BenefitsFromMT() const
{
    // case-insensitive regular expressions don't like MT

    if (!patterns.empty() && !caseSensitive)
        return false;

    // empty filters don't need MT (and its potential overhead)

    if (patterns.empty() && subStrings.empty())
        return false;

    // otherwise, go mult-threading!

    return true;
}

bool CLogDlgFilter::IsFilterActive() const
{
    return !(patterns.empty() && subStrings.empty());
}