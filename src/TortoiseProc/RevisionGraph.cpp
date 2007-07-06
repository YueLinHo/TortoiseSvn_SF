// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2003-2007 - Stefan Kueng

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
#include "resource.h"
#include "client.h"
#include "UnicodeUtils.h"
#include "registry.h"
#include "AppUtils.h"
#include "PathUtils.h"
#include "SVN.h"
#include "TSVNPath.h"
#include ".\revisiongraph.h"
#include "SVNError.h"
#include "CachedLogInfo.h"
#include "RevisionIndex.h"
#include "CopyFollowingLogIterator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CSearchPathTree::DeLink()
{
	assert (parent);

	if (previous)
		previous->next = next;
	if (next)
		next->previous = previous;

	if (parent->firstChild == this)
		parent->firstChild = next;
	if (parent->lastChild == this)
		parent->lastChild = previous;

	parent = NULL;
}

void CSearchPathTree::Link (CSearchPathTree* newParent)
{
	assert (parent == NULL);
	assert (newParent != NULL);

	parent = newParent;

	previous = parent->lastChild;

	if (parent->firstChild == NULL)
		parent->firstChild = this;
	else
		parent->lastChild->next = this;

	parent->lastChild = this;
}

// construction / destruction

CSearchPathTree::CSearchPathTree (const CPathDictionary* dictionary)
	: path (dictionary, std::string())
	, startRevision ((revision_t)NO_REVISION)
	, lastEntry (NULL)
	, parent (NULL)
	, firstChild (NULL)
	, lastChild (NULL)
	, previous (NULL)
	, next (NULL)
{
}

CSearchPathTree::CSearchPathTree ( const CDictionaryBasedTempPath& path
								 , revision_t startrev
								 , CSearchPathTree* parent)
	: path (path)
	, startRevision (startrev)
	, lastEntry (NULL)
	, parent (NULL)
	, firstChild (NULL)
	, lastChild (NULL)
	, previous (NULL)
	, next (NULL)
{
	Link (parent);
}

CSearchPathTree::~CSearchPathTree()
{
	while (firstChild != NULL)
		delete firstChild;

	if (parent)
		DeLink();
}

// add a node for the given path and rev. to the tree

void CSearchPathTree::Insert ( const CDictionaryBasedTempPath& path
							 , revision_t startrev)
{
	assert (startrev != NO_REVISION);

	// exact match (will happen on root node only)?

	if (this->path == path)
	{
		startRevision = startrev;
		return;
	}

	// (partly or fully) overlap with an existing child?

	for (CSearchPathTree* child = firstChild; child != NULL; child = child->next)
	{
		CDictionaryBasedTempPath commonPath = child->path.GetCommonRoot (path);

		if (commonPath != this->path)
		{
			if (child->path == path)
			{
				// there is already a node for the exact same path
				// -> use it, if unused so far; append a new node otherwise

				if (child->startRevision == NO_REVISION)
					child->startRevision = startrev;
				else
					new CSearchPathTree (path, startrev, this);
			}
			else
			{
				if (child->path == commonPath)
				{
					// the path is a (true) sub-node of the child

					child->Insert (path, startrev);
				}
				else
				{
					// there is a common base path with this child
					// Note: no other child can have that.
					// ->factor out and insert new sub-child

					CSearchPathTree* commonParent 
						= new CSearchPathTree (commonPath, revision_t(NO_REVISION), this);

					child->DeLink();
					child->Link (commonParent);

					new CSearchPathTree (path, startrev, commonParent);
				}
			}

			return;
		}
	}

	// no overlap with any existing node
	// -> create a new child

	new CSearchPathTree (path, startrev, this);
}

bool CSearchPathTree::IsEmpty() const
{
	return (startRevision == NO_REVISION) && (firstChild == NULL);
}

void CSearchPathTree::Remove()
{
	startRevision = revision_t (NO_REVISION);
	lastEntry = NULL;

	CSearchPathTree* node = this;
	while (node->IsEmpty() && (node->parent != NULL))
	{
		CSearchPathTree* temp = node;
		node = node->parent;

		delete temp;
	}
}

// there is a new revision entry for this path

void CSearchPathTree::ChainEntries (CRevisionEntry* entry)
{
	if (lastEntry != NULL)
		lastEntry->next = entry;

	lastEntry = entry;
}

CRevisionGraph::CRevisionGraph(void) : m_bCancelled(FALSE)
	, m_FilterMinRev(-1)
	, m_FilterMaxRev(-1)
{
	memset (&m_ctx, 0, sizeof (m_ctx));
	parentpool = svn_pool_create(NULL);

	Err = svn_config_ensure(NULL, parentpool);
	pool = svn_pool_create (parentpool);
	// set up the configuration
	if (Err == 0)
		Err = svn_config_get_config (&(m_ctx.config), g_pConfigDir, pool);

	if (Err != 0)
	{
		::MessageBox(NULL, this->GetLastErrorMessage(), _T("TortoiseSVN"), MB_ICONERROR);
		svn_error_clear(Err);
		svn_pool_destroy (pool);
		svn_pool_destroy (parentpool);
		exit(-1);
	}

	// set up authentication
	m_prompt.Init(pool, &m_ctx);

	m_ctx.cancel_func = cancel;
	m_ctx.cancel_baton = this;

	//set up the SVN_SSH param
	CString tsvn_ssh = CRegString(_T("Software\\TortoiseSVN\\SSH"));
	if (tsvn_ssh.IsEmpty())
		tsvn_ssh = CPathUtils::GetAppDirectory() + _T("TortoisePlink.exe");
	tsvn_ssh.Replace('\\', '/');
	if (!tsvn_ssh.IsEmpty())
	{
		svn_config_t * cfg = (svn_config_t *)apr_hash_get (m_ctx.config, SVN_CONFIG_CATEGORY_CONFIG,
			APR_HASH_KEY_STRING);
		svn_config_set(cfg, SVN_CONFIG_SECTION_TUNNELS, "ssh", CUnicodeUtils::GetUTF8(tsvn_ssh));
	}
}

CRevisionGraph::~CRevisionGraph(void)
{
	svn_error_clear(Err);
	svn_pool_destroy (parentpool);

	ClearRevisionEntries();
}

void CRevisionGraph::ClearRevisionEntries()
{
	for (size_t i = 0, count = m_entryPtrs.size(); i < count; ++i)
		delete m_entryPtrs[i];

	m_entryPtrs.clear();
}

bool CRevisionGraph::SetFilter(svn_revnum_t minrev, svn_revnum_t maxrev, const CString& sPathFilter)
{
	m_FilterMinRev = minrev;
	m_FilterMaxRev = maxrev;
	m_filterpaths.clear();
	// the filtered paths are separated by an '*' char, since that char is illegal in paths and urls
	if (!sPathFilter.IsEmpty())
	{
		int pos = sPathFilter.Find('*');
		if (pos)
		{
			CString sTemp = sPathFilter;
			while (pos>=0)
			{
				m_filterpaths.insert((LPCWSTR)sTemp.Left(pos));
				sTemp = sTemp.Mid(pos+1);
				pos = sTemp.Find('*');
			}
			m_filterpaths.insert((LPCWSTR)sTemp);
		}
		else
			m_filterpaths.insert((LPCWSTR)sPathFilter);
	}
	return true;
}

BOOL CRevisionGraph::ProgressCallback(CString /*text*/, CString /*text2*/, DWORD /*done*/, DWORD /*total*/) {return TRUE;}

svn_error_t* CRevisionGraph::cancel(void *baton)
{
	CRevisionGraph * me = (CRevisionGraph *)baton;
	if (me->m_bCancelled)
	{
		CString temp;
		temp.LoadString(IDS_SVN_USERCANCELLED);
		return svn_error_create(SVN_ERR_CANCELLED, NULL, CUnicodeUtils::GetUTF8(temp));
	}
	return SVN_NO_ERROR;
}

// implement ILogReceiver

void CRevisionGraph::ReceiveLog ( LogChangedPathArray* changes
								, svn_revnum_t rev
								, const CString&
								, const apr_time_t&
								, const CString&)
{
    // we passed revs_only to Log()

    assert (changes == NULL);

	// update internal data

	if ((m_lHeadRevision < (revision_t)rev) || (m_lHeadRevision == NO_REVISION))
		m_lHeadRevision = rev;

	// update progress bar and check for user pressing "Cancel" somewhere

	static DWORD lastProgressCall = 0;
	if (lastProgressCall < GetTickCount() - 200)
	{
		lastProgressCall = GetTickCount();

		CString temp, temp2;
		temp.LoadString(IDS_REVGRAPH_PROGGETREVS);
		temp2.Format(IDS_REVGRAPH_PROGCURRENTREV, rev);
		if (!ProgressCallback (temp, temp2, m_lHeadRevision - rev, m_lHeadRevision))
		{
			m_bCancelled = TRUE;
			throw SVNError (cancel (this));
		}
	}
}

BOOL CRevisionGraph::FetchRevisionData(CString path)
{
	// set some text on the progress dialog, before we wait
	// for the log operation to start
	CString temp;
	temp.LoadString(IDS_REVGRAPH_PROGGETREVS);
	ProgressCallback(temp, _T(""), 0, 1);

	// prepare the path for Subversion
	SVN::preparePath(path);
	CStringA url = CUnicodeUtils::GetUTF8(path);

	svn_error_clear(Err);
	// convert a working copy path into an URL if necessary
	if (!svn_path_is_url(url))
	{
		//not an url, so get the URL from the working copy path first
		svn_wc_adm_access_t *adm_access;          
		const svn_wc_entry_t *entry;  
		const char * canontarget = svn_path_canonicalize(url, pool);
#pragma warning(push)
#pragma warning(disable: 4127)	// conditional expression is constant
			Err = svn_wc_adm_probe_open2 (&adm_access, NULL, canontarget,
				FALSE, 0, pool);
			if (Err) return FALSE;
			Err =  svn_wc_entry (&entry, canontarget, adm_access, FALSE, pool);
			if (Err) return FALSE;
			Err = svn_wc_adm_close (adm_access);
			if (Err) return FALSE;
#pragma warning(pop)

			url = entry ? entry->url : "";
	}
	url = CPathUtils::PathEscape(url);

	// we have to get the log from the repository root
	CTSVNPath urlpath;
	urlpath.SetFromSVN(url);

	SVN svn;
	m_sRepoRoot = svn.GetRepositoryRoot(urlpath);
	url = m_sRepoRoot;
	urlpath.SetFromSVN(url);

	if (m_sRepoRoot.IsEmpty())
	{
		Err = svn_error_dup(svn.Err);
		return FALSE;
	}

	m_lHeadRevision = (revision_t)NO_REVISION;
	try
	{
		CRegStdWORD useLogCache (_T("Software\\TortoiseSVN\\UseLogCache"), TRUE);
		CLogCachePool* caches = useLogCache != FALSE 
							  ? svn.GetLogCachePool() 
							  : NULL;

		svnQuery.reset (new CSVNLogQuery (&m_ctx, pool));
		query.reset (new CCacheLogQuery (caches, svnQuery.get()));

		query->Log ( CTSVNPathList (urlpath)
				   , SVNRev(SVNRev::REV_HEAD)
				   , SVNRev(SVNRev::REV_HEAD)
				   , SVNRev(0)
				   , 0
				   , false
				   , this
                   , true);
	}
	catch (SVNError& e)
	{
		Err = svn_error_create (e.GetCode(), NULL, e.GetMessage());
		return FALSE;
	}

	return TRUE;
}

BOOL CRevisionGraph::AnalyzeRevisionData(CString path, bool bShowAll /* = false */, bool /* bArrangeByPath = false */)
{
	svn_error_clear(Err);

	ClearRevisionEntries();
	m_maxurllength = 0;
	m_maxurl.Empty();
	m_maxRow = 0;
	m_maxColumn = 0;

	SVN::preparePath(path);
	CStringA url = CUnicodeUtils::GetUTF8(path);

	// convert a working copy path into an URL if necessary
	if (!svn_path_is_url(url))
	{
		//not an url, so get the URL from the working copy path first
		svn_wc_adm_access_t *adm_access;          
		const svn_wc_entry_t *entry;  
		const char * canontarget = svn_path_canonicalize(url, pool);
#pragma warning(push)
#pragma warning(disable: 4127)	// conditional expression is constant
		Err = svn_wc_adm_probe_open2 (&adm_access, NULL, canontarget,
			FALSE, 0, pool);
		if (Err) return FALSE;
		Err =  svn_wc_entry (&entry, canontarget, adm_access, FALSE, pool);
		if (Err) return FALSE;
		Err = svn_wc_adm_close (adm_access);
		if (Err) return FALSE;
#pragma warning(pop)

		url = entry ? entry->url : "";
	}

	url = CPathUtils::PathUnescape(url);
	url = url.Mid(CPathUtils::PathUnescape(m_sRepoRoot).GetLength());

	BuildForwardCopies();
	
	// in case our path was renamed and had a different name in the past,
	// we have to find out that name now, because we will analyze the data
	// from lower to higher revisions

	const CCachedLogInfo* cache = query->GetCache();
	const CPathDictionary* paths = &cache->GetLogInfo().GetPaths();
	CDictionaryBasedTempPath startPath (paths, (const char*)url);

	CCopyFollowingLogIterator iterator (cache, m_lHeadRevision, startPath);
	iterator.Retry();
	revision_t initialrev = m_lHeadRevision;

	while (   (iterator.GetRevision() > 0) 
		   && !iterator.EndOfPath()
		   && !iterator.DataIsMissing())
	{
		initialrev = iterator.GetRevision();
		iterator.Advance();
	}

	startPath = iterator.GetPath();

	AnalyzeRevisions (startPath, initialrev, bShowAll);
	ApplyForwardCopies();
	AssignCoordinates();
	Cleanup();

	return true;
}

inline bool AscendingFromRevision (const SCopyInfo* lhs, const SCopyInfo* rhs)
{
	return lhs->fromRevision < rhs->fromRevision;
}

inline bool AscendingToRevision (const SCopyInfo* lhs, const SCopyInfo* rhs)
{
	return lhs->toRevision < rhs->toRevision;
}

void CRevisionGraph::BuildForwardCopies()
{
	// iterate through all revisions and fill copyToRelation:
	// for every copy-from info found, add an entry

	const CCachedLogInfo* cache = query->GetCache();
	const CRevisionIndex& revisions = cache->GetRevisions();
	const CRevisionInfoContainer& revisionInfo = cache->GetLogInfo();

	// for all revisions ...

	for ( revision_t revision = revisions.GetFirstRevision()
		, last = revisions.GetLastRevision()
		; revision < last
		; ++revision)
	{
		// ... in the cache ...

		index_t index = revisions[revision];
		if (index != NO_INDEX)
		{
			// ... examine all changes ...

			for ( CRevisionInfoContainer::CChangesIterator 
					iter = revisionInfo.GetChangesBegin (index)
				, end = revisionInfo.GetChangesEnd (index)
				; iter != end
				; ++iter)
			{
				// ... and if it has a copy-from info ...

				if (iter->HasFromPath())
				{
					// ... add it to the list

					SCopyInfo* copyInfo = new SCopyInfo;
					copyInfo->fromRevision = iter->GetFromRevision();
					copyInfo->fromPathIndex = iter->GetFromPathID();
					copyInfo->toRevision = revision;
					copyInfo->toPathIndex = iter->GetPathID();

					copyInfo->sourceEntry = NULL;
					copyInfo->targetEntry = NULL;

					copyToRelation.push_back (copyInfo);
					copyFromRelation.push_back (copyInfo);
				}
			}
		}
	}

	// sort container by source revision and path

	std::sort (copyToRelation.begin(), copyToRelation.end(), &AscendingToRevision);
	std::sort (copyFromRelation.begin(), copyFromRelation.end(), &AscendingFromRevision);
}

void CRevisionGraph::AnalyzeRevisions ( const CDictionaryBasedTempPath& path
									  , revision_t startrev
									  , bool bShowAll)
{
	const CCachedLogInfo* cache = query->GetCache();
	const CRevisionIndex& revisions = cache->GetRevisions();
	const CRevisionInfoContainer& revisionInfo = cache->GetLogInfo();

	// initialize the paths we have to search for

	std::auto_ptr<CSearchPathTree> searchTree 
		(new CSearchPathTree (&revisionInfo.GetPaths()));
	searchTree->Insert (path, startrev);

	// the range of copy-to info that applies to the current revision

	std::vector<SCopyInfo*>::const_iterator firstFromCopy = copyFromRelation.begin();
	std::vector<SCopyInfo*>::const_iterator lastFromCopy = firstFromCopy;
	std::vector<SCopyInfo*>::const_iterator endFromCopy = copyFromRelation.end();

	while (   (firstFromCopy != endFromCopy) 
		   && ((*firstFromCopy)->fromRevision < startrev))
		++firstFromCopy;

	lastFromCopy = firstFromCopy;

	std::vector<SCopyInfo*>::const_iterator firstToCopy = copyToRelation.begin();
	std::vector<SCopyInfo*>::const_iterator lastToCopy = firstToCopy;
	std::vector<SCopyInfo*>::const_iterator endToCopy = copyToRelation.end();

	while (   (firstToCopy != endToCopy) 
		   && ((*firstToCopy)->toRevision < startrev))
		++firstToCopy;

	lastToCopy = firstToCopy;

	// collect nodes to draw ... revision by revision

	for (revision_t revision = startrev; revision <= m_lHeadRevision; ++revision)
	{
		index_t index = revisions[revision];
		if (index == NO_INDEX)
			continue;

		// update copy-to info interators

		firstToCopy = lastToCopy;
		while (   (lastToCopy != endToCopy) 
			   && ((*lastToCopy)->toRevision <= revision))
			++lastToCopy;

		firstFromCopy = lastFromCopy;
		while (   (lastFromCopy != endFromCopy) 
			   && ((*lastFromCopy)->fromRevision <= revision))
			++lastFromCopy;

		// we are looking for search paths that (may) overlap 
		// with the revisions' changes

		CDictionaryBasedPath basePath = revisionInfo.GetRootPath (index);
		if (!basePath.IsValid())
			continue;	// empty revision

		// collect search paths that have been deleted in this container
		// (delay potential node deletion until we finished tree traversal)

		std::vector<CSearchPathTree*> toRemove;

		// pre-order search-tree traversal

		CSearchPathTree* searchNode = searchTree.get();
		while (searchNode != NULL)
		{
			if (basePath.IsSameOrParentOf (searchNode->GetPath().GetBasePath()))
			{
				// maybe a hit -> match all changes against the whole sub-tree

				AnalyzeRevisions ( revision
								 , revisionInfo.GetChangesBegin (index)
								 , revisionInfo.GetChangesEnd (index)
								 , searchTree.get()
								 , searchNode
								 , firstFromCopy
								 , lastFromCopy
								 , firstToCopy
								 , lastToCopy
								 , bShowAll
								 , toRemove);
			}
			else
			{
				if (   (searchNode->GetFirstChild() != NULL)
					&& searchNode->GetPath().IsSameOrParentOf (basePath))
				{
					// the sub-nodes may be a match

					searchNode = searchNode->GetFirstChild();
					continue;
				}
			}

			// this sub-tree has fully been covered (or been no match at all)
			// -> to the next node

			while (   (searchNode->GetNext() == NULL)
				   && (searchNode->GetParent() != NULL))
				searchNode = searchNode->GetParent();

			searchNode = searchNode->GetNext();
		}

		// handle remaining copy-to entries
		// (some may have a fromRevision that does not touch the fromPath)

		AddRemainingCopies ( revision
						   , searchTree.get()
						   , firstFromCopy
						   , lastFromCopy);

		// remove deleted search paths

		for (size_t i = 0, count = toRemove.size(); i < count; ++i)
			toRemove[i]->Remove();
	}
}

void CRevisionGraph::AnalyzeRevisions ( revision_t revision
									  , CRevisionInfoContainer::CChangesIterator first
									  , CRevisionInfoContainer::CChangesIterator last
									  , CSearchPathTree* rootNode
									  , CSearchPathTree* startNode
									  , std::vector<SCopyInfo*>::const_iterator firstFromCopy
									  , std::vector<SCopyInfo*>::const_iterator lastFromCopy
									  , std::vector<SCopyInfo*>::const_iterator firstToCopy
									  , std::vector<SCopyInfo*>::const_iterator lastToCopy
									  , bool bShowAll
									  , std::vector<CSearchPathTree*>& toRemove)
{
	// cover the whole sub-tree

	CSearchPathTree* searchNode = startNode;
	do
	{
		// is this search path active?

		if (   (searchNode->GetStartRevision() != NO_REVISION)
			&& (searchNode->GetStartRevision() <= revision))
		{
			const CDictionaryBasedTempPath& path = searchNode->GetPath();

			// looking for the closet change that affected the path

			CRevisionInfoContainer::CChangesIterator lastMatch = last; 
			for ( CRevisionInfoContainer::CChangesIterator iter = first
				; iter != last
				; ++iter)
			{
				if (   iter->GetPath().IsSameOrParentOf (path.GetBasePath())
					&& (   (iter->GetAction() != CRevisionInfoContainer::ACTION_CHANGED)
						|| bShowAll))
				{
					lastMatch = iter;
				}
			}

			// has the path actually been touched?

			CRevisionEntry* newEntry = NULL;
			if (lastMatch != last)
			{
				int actionValue = lastMatch->GetAction();
				if (lastMatch->HasFromPath())
					++actionValue;

				CRevisionEntry::Action action 
					= static_cast<CRevisionEntry::Action>(actionValue);

				newEntry = new CRevisionEntry (path, revision, action);
				newEntry->index = m_entryPtrs.size();
				newEntry->realPath = lastMatch->GetPath();
				m_entryPtrs.push_back (newEntry);

				if (action == CRevisionEntry::deleted)
					toRemove.push_back (searchNode);
			}

			// add new paths (due to copy operations)

			for ( std::vector<SCopyInfo*>::const_iterator iter = firstFromCopy
				; iter != lastFromCopy
				; ++iter)
			{
				SCopyInfo* copy = *iter;
				if (searchNode->GetPath().IsSameOrChildOf (copy->fromPathIndex))
				{
					// add & schedule the new search path

					CDictionaryBasedTempPath targetPath 
						= path.ReplaceParent ( CDictionaryBasedPath ( path.GetDictionary()
																    , copy->fromPathIndex)
										     , CDictionaryBasedPath ( path.GetDictionary()
																    , copy->toPathIndex));

					rootNode->Insert (targetPath, copy->fromRevision);

					// add a revision entry for the source, if not yet available

					if (newEntry == NULL)
					{
						newEntry = new CRevisionEntry ( path
													  , revision
													  , CRevisionEntry::source);
						newEntry->realPath = CDictionaryBasedPath ( path.GetDictionary()
															      , copy->fromPathIndex);
						newEntry->index = m_entryPtrs.size();
						m_entryPtrs.push_back (newEntry);
					}

					// mark the copy source

					copy->sourceEntry = newEntry;
				}
			}

			// mark copy targets

			for ( std::vector<SCopyInfo*>::const_iterator iter = firstToCopy
				; (iter != lastToCopy) && (newEntry != NULL)
				; ++iter)
			{
				SCopyInfo* copy = *iter;
				if (searchNode->GetPath().IsSameOrChildOf (copy->toPathIndex))
				{
					// mark the copy target

					copy->targetEntry = newEntry;
				}
			}

			// link entries for the same search path

			if (newEntry)
				searchNode->ChainEntries (newEntry);
		}

		// select next node

		if (searchNode->GetFirstChild() != NULL)
		{
			searchNode = searchNode->GetFirstChild();
		}
		else
		{
			while (    (searchNode->GetNext() == NULL)
					&& (searchNode != startNode))
				searchNode = searchNode->GetParent();

			if (searchNode != startNode)
				searchNode = searchNode->GetNext();
		}
	}
	while (searchNode != startNode);
}

void CRevisionGraph::AddRemainingCopies ( revision_t revision
									    , CSearchPathTree* rootNode
									    , std::vector<SCopyInfo*>::const_iterator firstFromCopy
									    , std::vector<SCopyInfo*>::const_iterator lastFromCopy)
{
	for ( std::vector<SCopyInfo*>::const_iterator iter = firstFromCopy
		; iter != lastFromCopy
		; ++iter)
	{
		// skip those that already have a source revision entry

		SCopyInfo* copy = *iter;
		if (copy->sourceEntry != NULL)
			continue;

		// crawl the whole sub-tree for path matches

		CSearchPathTree* searchNode = rootNode;
		while (searchNode != NULL)
		{
			const CDictionaryBasedTempPath& path = searchNode->GetPath();

			// got this path copied?

			if (path.IsSameOrChildOf (copy->fromPathIndex))
			{
				CRevisionEntry*	newEntry 
					= new CRevisionEntry ( path
										 , revision
										 , CRevisionEntry::source);
				newEntry->realPath 
					= CDictionaryBasedPath ( path.GetDictionary()
									       , copy->fromPathIndex);
				newEntry->index = m_entryPtrs.size();
				m_entryPtrs.push_back (newEntry);

				// add & schedule the new search path

				CDictionaryBasedTempPath targetPath 
					= path.ReplaceParent ( CDictionaryBasedPath ( path.GetDictionary()
															    , copy->fromPathIndex)
									     , CDictionaryBasedPath ( path.GetDictionary()
															    , copy->toPathIndex));

				rootNode->Insert (targetPath, copy->fromRevision);

				// link with existing revision entries

				searchNode->ChainEntries (newEntry);

				// mark the copy source

				copy->sourceEntry = newEntry;
			}

			// select next node

			if (   (searchNode->GetFirstChild() != NULL)
				&& path.IsSameOrParentOf (copy->fromPathIndex))
			{
				searchNode = searchNode->GetFirstChild();
			}
			else
			{
				while (    (searchNode->GetNext() == NULL)
						&& (searchNode->GetParent() != NULL))
					searchNode = searchNode->GetParent();

				searchNode = searchNode->GetNext();
			}
		}
	}
}

void CRevisionGraph::ApplyForwardCopies()
{
	// add copy-to links between existing revision entries

	for ( std::vector<SCopyInfo*>::const_iterator iter = copyToRelation.begin()
		, end = copyToRelation.end()
		; iter != end
		; ++iter)
	{
		SCopyInfo* copyInfo = *iter;
		if ((copyInfo->sourceEntry != NULL) && (copyInfo->targetEntry != NULL))
			copyInfo->sourceEntry->copyTargets.push_back (copyInfo->targetEntry);

		delete copyInfo;
	}

	// now, all copy-to info has been deleted

	copyFromRelation.clear();
	copyToRelation.clear();
}

void CRevisionGraph::AssignColumns ( CRevisionEntry* start
								   , std::vector<int>& columnByRevision)
{
	// find larges level for the chain starting at "start"

	revision_t lastRevision = (revision_t)NO_REVISION;
	for (CRevisionEntry* entry = start; entry != NULL; entry = entry->next)
		lastRevision = entry->revision;

	int column = 0;
	for (revision_t revision = start->revision; revision <= lastRevision; ++revision)
		column = max (column, columnByRevision[revision]+1);

	// assign that level & collect branches

	std::vector<CRevisionEntry*> branches;
	for (CRevisionEntry* entry = start; entry != NULL; entry = entry->next)
	{
		entry->column = column;
		if (!entry->copyTargets.empty())
			branches.push_back (entry);
	}

	// block the level for the whole chain

	for (revision_t revision = start->revision; revision <= lastRevision; ++revision)
		columnByRevision[revision] = column;

	// follow the branches

	for ( std::vector<CRevisionEntry*>::reverse_iterator iter = branches.rbegin()
		, end = branches.rend()
		; iter != end
		; ++iter)
	{
		const std::vector<CRevisionEntry*>& targets = (*iter)->copyTargets;
		for (size_t i = 0, count = targets.size(); i < count; ++i)
			AssignColumns (targets[i], columnByRevision);
	}
}

void CRevisionGraph::AssignCoordinates()
{
	// the highest used column per revision

	std::vector<int> columnByRevision;
	columnByRevision.insert (columnByRevision.begin(), m_lHeadRevision, 0);

	AssignColumns (m_entryPtrs[0], columnByRevision);

	// assign rows

	int row = 0;
	revision_t lastRevision = 0;
	for (size_t i = 0, count = m_entryPtrs.size(); i < count; ++i)
	{
		CRevisionEntry * entry = m_entryPtrs[i];
		if (entry->revision > lastRevision)
		{
			lastRevision = entry->revision;
			++row;
		}
		
		entry->row = row;
	}
}

inline bool AscendingColumRow ( const CRevisionEntry* lhs
							  , const CRevisionEntry* rhs)
{
	return (lhs->column < rhs->column)
		|| (   (lhs->column == rhs->column)
		    && (lhs->row < rhs->row));
}

void CRevisionGraph::Cleanup()
{
	for (size_t i = 0, count = m_entryPtrs.size(); i < count; ++i)
	{
		// add the parent line to the target list

		CRevisionEntry * entry = m_entryPtrs[i];
		std::vector<CRevisionEntry*>& targets = entry->copyTargets;
		if (entry->next != NULL)
			targets.push_back (entry->next);

		// sort targets by level and revision

		sort (targets.begin(), targets.end(), &AscendingColumRow);
	}
}

bool CRevisionGraph::IsParentOrItself(const char * parent, const char * child)
{
	size_t len = strlen(parent);
	if (strncmp(parent, child, len)==0)
	{
		if ((child[len]=='/')||(child[len]==0))
			return true;
	}
	return false;
}

bool CRevisionGraph::IsParentOrItself(const wchar_t * parent, const wchar_t * child)
{
	size_t len = wcslen(parent);
	if (wcsncmp(parent, child, len)==0)
	{
		if ((child[len]=='/')||(child[len]==0))
			return true;
	}
	return false;
}

CString CRevisionGraph::GetLastErrorMessage()
{
	return SVN::GetErrorString(Err);
}
