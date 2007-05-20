// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2003-2006 - Stefan Kueng

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

#include "SVNPrompt.h"
#include "ILogReceiver.h"

#include <map>

typedef int (__cdecl *GENERICCOMPAREFN)(const void * elem1, const void * elem2);

class log_entry
{
public:

	std::auto_ptr<LogChangedPathArray> changes;
	svn_revnum_t rev;
	CString author;
	apr_time_t timeStamp;
	CString message;

	// default construction

	log_entry() {};

private:

	// copy is not allowed (due to std::auto_ptr)

	log_entry (const log_entry&);
	log_entry& operator= (const log_entry&);
};

struct source_entry
{
	CString	pathto;
	LONG	revisionto;
};

/**
 * \ingroup TortoiseProc
 * Helper class, representing a revision with all the required information
 * which we need to draw a revision graph.
 */
class CRevisionEntry
{
public:
	enum Action
	{
		nothing,
		modified,
		deleted,
		added,
		addedwithhistory,
		renamed,
		replaced,
		lastcommit,
		initial,
		source
	};
	//methods
	CRevisionEntry(void) : revision(0), url(), realurl(), author(), date(0),
		message(), action(nothing), level(1), bUsed(false),
		leftconnections(0),	rightconnections(0), bottomconnections(0),
		rightlines(0), bottomlines(0),
		leftconnectionsleft(0),	rightconnectionsleft(0), bottomconnectionsleft(0),
		rightlinesleft(0), bottomlinesleft(0) {};
	//members
	svn_revnum_t	revision;
	CString			url;
	CString			realurl;
	CString			author;
	apr_time_t		date;
	CString			message;
	Action			action;
	int				level;
	CPtrArray		sourcearray;
	bool			bUsed;
	int				leftconnections;
	int				rightconnections;
	int				bottomconnections;
	int				rightlines;
	int				bottomlines;

	int				leftconnectionsleft;
	int				rightconnectionsleft;
	int				bottomconnectionsleft;
	int				rightlinesleft;
	int				bottomlinesleft;
	std::set<INT_PTR>	connections;
	
	CRect			drawrect;
};

/**
 * \ingroup TortoiseProc
 * Handles and analyzes log data to produce a revision graph.
 * 
 * Since Subversion only stores information where each entry is copied \b from
 * and not where it is copied \b to, the first thing we do here is crawl all
 * revisions and create separate CRevisionEntry objects where we store the
 * information where those are copied \b to.
 *
 * In a next step, we go again through all the CRevisionEntry objects to find
 * out if they are related to the path we're looking at. If they are, we mark
 * them as \b in-use.
 */
class CRevisionGraph : private ILogReceiver
{
public:
	CRevisionGraph(void);
	~CRevisionGraph(void);
	BOOL						FetchRevisionData(CString path);
	BOOL						AnalyzeRevisionData(CString path, bool bShowAll = false, bool bArrangeByPath = false);
	virtual BOOL				ProgressCallback(CString text1, CString text2, DWORD done, DWORD total);
	svn_revnum_t				GetHeadRevision() {return m_lHeadRevision;}
	bool						SetFilter(svn_revnum_t minrev, svn_revnum_t maxrev, const CString& sPathFilter);

	CString						GetLastErrorMessage();
	static bool					IsParentOrItself(const char * parent, const char * child);
	static bool					IsParentOrItself(const wchar_t * parent, const wchar_t * child);
	CPtrArray					m_arEntryPtrs;
	size_t						m_maxurllength;
	CString						m_maxurl;
	int							m_maxlevel;
	svn_revnum_t				m_numRevisions;

	typedef std::multimap<svn_revnum_t, CRevisionEntry*>::iterator EntryPtrsIterator;
	typedef std::pair<svn_revnum_t, CRevisionEntry*> EntryPair;
	std::multimap<svn_revnum_t, CRevisionEntry*> m_mapEntryPtrs;
	CString						GetReposRoot() {return CString(m_sRepoRoot);}

	BOOL						m_bCancelled;

	typedef std::map<svn_revnum_t, log_entry*> TLogDataMap;
	TLogDataMap					m_logdata;
	apr_pool_t *				pool;			///< memory pool
	apr_pool_t *				graphpool;
	svn_client_ctx_t 			m_ctx;
	SVNPrompt					m_prompt;

private:
	bool						BuildForwardCopies();
	bool						AnalyzeRevisions(CString url, svn_revnum_t startrev, bool bShowAll);
	bool						Cleanup(bool bArrangeByPath);
	
	bool						SetCopyTo(const CString& uiCopyFromPath, svn_revnum_t copyfrom_rev, 
											const CString& copyto_path, svn_revnum_t copyto_rev);
	CRevisionEntry *			GetRevisionEntry(const CString& uiPath, svn_revnum_t rev, 
													bool bCreate = true);
	CString 					GetRename(const CString& url, LONG rev);

#ifdef DEBUG	
	void						PrintDebugInfo();
#endif

	static int __cdecl			SortCompareRevUrl(const void * pElem1, const void * pElem2);	///< sort callback function
	static int __cdecl			SortCompareRevLevels(const void * pElem1, const void * pElem2);	///< sort callback function
	static int __cdecl			SortCompareSourceEntry(const void * pElem1, const void * pElem2);	///< sort callback function
	CStringA					m_sRepoRoot;
	LONG						m_lHeadRevision;

	std::set<std::wstring>		m_filterpaths;
	svn_revnum_t				m_FilterMinRev;
	svn_revnum_t				m_FilterMaxRev;

	int							m_nRecurseLevel;
	svn_error_t *				Err;			///< Global error object struct
	apr_pool_t *				parentpool;
	static svn_error_t*			cancel(void *baton);

	// implement ILogReceiver

	void ReceiveLog ( LogChangedPathArray* changes
					, svn_revnum_t rev
					, const CString& author
					, const apr_time_t& timeStamp
					, const CString& message);

};
