#include "StdAfx.h"
#include "SVNLogQuery.h"

svn_error_t* CSVNLogQuery::LogReceiver ( void* baton
									   , apr_hash_t* ch_paths
									   , svn_revnum_t revision
									   , const char* author
									   , const char* date
									   , const char* msg
									   , apr_pool_t* pool)
{
	// where to send the pre-processed informat

	ILogReceiver* receiver = reinterpret_cast<ILogReceiver*>(baton);

	// convert strings

	CString authorNative = CUnicodeUtils::GetUnicode (author);
	CString messageNative = CUnicodeUtils::GetUnicode (msg != NULL ? msg : "");

	// time stamp

	apr_time_t timeStamp = NULL;
	if (date && date[0])
		SVN_ERR (svn_time_from_cstring (&timeStamp, date, pool));

	// the individual changes

	std::auto_ptr<LogChangedPathArray> arChangedPaths (new LogChangedPathArray);
	try
	{
		if (ch_paths)
		{
			apr_array_header_t *sorted_paths 
				= svn_sort__hash (ch_paths, svn_sort_compare_items_as_paths, pool);

			for (int i = 0, count = sorted_paths->nelts; i < count; ++i)
			{
				// find the item in the hash

				std::auto_ptr<LogChangedPath> changedPath (new LogChangedPath);
				svn_sort__item_t *item = &(APR_ARRAY_IDX ( sorted_paths
														 , i
														 , svn_sort__item_t));

				// extract the path name

				const char *path = (const char *)item->key;
				changedPath->sPath = SVN::MakeUIUrlOrPath (path);

				// decode the action

				svn_log_changed_path_t *log_item 
					= (svn_log_changed_path_t *) apr_hash_get ( ch_paths
															  , item->key
															  , item->klen);
				static const char actionKeys[5] = "MRAD";
				const char* actionKey = strchr (actionKeys, log_item->action);

				changedPath->action = actionIndex == NULL 
									? 0
									: 1 << (actionKey - actionKeys);

				// decode copy-from info

				if (   log_item->copyfrom_path 
					&& SVN_IS_VALID_REVNUM (log_item->copyfrom_rev))
				{
					changedPath->lCopyFromRev = log_item->copyfrom_rev;
					changedPath->sCopyFromPath 
						= SVN::MakeUIUrlOrPath (log_item->copyfrom_path);
				}
				else
				{
					changedpath->lCopyFromRev = 0;
				}

				arChangedPaths->Add (changedpath.release());
			} 
		} 
	}
	catch (CMemoryException * e)
	{
		e->Delete();
	}
#pragma warning(push)
#pragma warning(disable: 4127)	// conditional expression is constant
	SVN_ERR (svn->cancel(baton));
#pragma warning(pop)

	// now, report the change

	try
	{
		receiver->ReceiveLog ( *arChangedPaths.get()
							 , revision
							 , authorNative
							 , timeStamp
							 , messageNative);
	}
	catch (...)
	{
		// we must not leak exceptions back into SVN
	}

	return NULL;
}

CSVNLogQuery::CSVNLogQuery(void)
{
}

CSVNLogQuery::~CSVNLogQuery(void)
{
}

void CSVNLogQuery::Log ( const CTSVNPathList& targets
					   , const SVNRev& peg_revision
					   , const SVNRev& start
					   , const SVNRev& end
					   , int limit
					   , bool strictNodeHistory
					   , ILogReceiver* receiver)
{
	SVNPool localpool (pool);
	svn_error_t *result = svn_client_log3 ( pathlist.MakePathArray (pool)
										  , peg_revision
										  , start
										  , end
										  , limit
										  , true
										  , strictNodeHistory ? TRUE : FALSE
										  , LogReceiver
										  , (void *)receiver
										  , context
										  , localpool);

	if (error != NULL)
		throw ESVNLogException (error);
}

