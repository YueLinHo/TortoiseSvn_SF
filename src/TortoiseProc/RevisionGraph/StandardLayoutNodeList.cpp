#include "StdAfx.h"
#include "resource.h"
#include "StandardLayoutNodeList.h"
#include "VisibleGraphNode.h"
#include "CachedLogInfo.h"
#include "SVN.h"
#include "UnicodeUtils.h"

/// utilities

index_t CStandardLayoutNodeList::GetStyle 
	(const CVisibleGraphNode* node) const
{
	CNodeClassification classification = node->GetClassification();

	if (classification.Is (CNodeClassification::IS_ADDED))
		return ILayoutNodeList::SNode::STYLE_ADDED;
	else if (classification.Is (CNodeClassification::IS_DELETED))
		return ILayoutNodeList::SNode::STYLE_DELETED;
	else if (classification.Is (CNodeClassification::IS_RENAMED))
		return ILayoutNodeList::SNode::STYLE_RENAMED;
	else if (classification.Is (CNodeClassification::IS_LAST))
		return ILayoutNodeList::SNode::STYLE_LAST;
	else
		return ILayoutNodeList::SNode::STYLE_DEFAULT;
}

DWORD CStandardLayoutNodeList::GetStyleFlags 
	(const CVisibleGraphNode* node) const
{
	return 0;
}

// construction

CStandardLayoutNodeList::CStandardLayoutNodeList 
    ( const std::vector<CStandardLayoutNodeInfo>& nodes
    , const CCachedLogInfo* cache)
    : cache (cache)
    , nodes (nodes)
{
}

// implement ILayoutItemList

index_t CStandardLayoutNodeList::GetCount() const
{
    return static_cast<index_t>(nodes.size());
}

CString CStandardLayoutNodeList::GetToolTip (index_t index) const
{
    CString strTipText;

    const CRevisionIndex& revisions = cache->GetRevisions();
    const CRevisionInfoContainer& revisionInfo = cache->GetLogInfo();

    const CVisibleGraphNode* node = nodes[index].node;

    // find the revision in our cache. 
    // May not be present if this is the WC / HEAD revision.

    revision_t revision = node->GetRevision();
    index_t revisionIndex = revisions [revision];
    if (revisionIndex == NO_INDEX)
        return strTipText;

    // get standard revprops

	TCHAR date[SVN_DATE_BUFFER];
	apr_time_t timeStamp = revisionInfo.GetTimeStamp (revisionIndex);
	SVN::formatDate(date, timeStamp);

    CString realPath 
        = CUnicodeUtils::StdGetUnicode 
            (node->GetRealPath().GetPath()).c_str();
    CString author 
        = CUnicodeUtils::StdGetUnicode 
            (revisionInfo.GetAuthor (revisionIndex)).c_str();
    CString comment 
        = CUnicodeUtils::StdGetUnicode 
            (revisionInfo.GetComment (revisionIndex)).c_str();

    // construct the tooltip

    if (node->GetFirstTag() == NULL)
    {
	    strTipText.Format ( IDS_REVGRAPH_BOXTOOLTIP
                          , revision, (LPCTSTR)realPath, (LPCTSTR)author
                          , date, (LPCTSTR)comment);
    }
    else
    {
        CString tags;
        for ( const CVisibleGraphNode::CFoldedTag* tag = node->GetFirstTag()
            ; tag != NULL
            ; tag = tag->GetNext())
        {
            UINT format = tag->IsAlias()
                        ? tag->IsDeleted()
                            ? IDS_REVGRAPH_TAGALIASDELETED
                            : IDS_REVGRAPH_TAGALIAS
                        : tag->IsDeleted()
                            ? IDS_REVGRAPH_TAGDELETED
                            : IDS_REVGRAPH_TAG;

            CString tagInfo;
            tagInfo.Format ( format
                           , CUnicodeUtils::StdGetUnicode 
                                (tag->GetTag()->GetPath().GetPath()).c_str());

            tags +=   _T("\r\n")
                    + CString (' ', static_cast<int>(tag->GetDepth()) * 6) 
                    + tagInfo;
        }

	    strTipText.Format ( IDS_REVGRAPH_BOXTOOLTIP_TAGGED
                          , revision, (LPCTSTR)realPath, (LPCTSTR)author
                          , date, (LPCTSTR)tags, (LPCTSTR)comment);
    }

    // ready

    return strTipText;
}

index_t CStandardLayoutNodeList::GetFirstVisible (const CRect& viewRect) const
{
    return GetNextVisible (static_cast<index_t>(-1), viewRect);
}

index_t CStandardLayoutNodeList::GetNextVisible ( index_t prev
                                                , const CRect& viewRect) const
{
    for (size_t i = prev+1, count = nodes.size(); i < count; ++i)
        if (FALSE != CRect().IntersectRect (nodes[i].rect, viewRect))
            return static_cast<index_t>(i);

    return static_cast<index_t>(NO_INDEX);
}

index_t CStandardLayoutNodeList::GetAt (const CPoint& point, long delta) const
{
    for (size_t i = 0, count = nodes.size(); i < count; ++i)
    {
        const CRect& rect = nodes[i].rect;
        if (   (rect.top - point.y <= delta)
            && (rect.left - point.x <= delta)
            && (point.y - rect.bottom <= delta)
            && (point.x - rect.right <= delta))
        {
            return static_cast<index_t>(i);
        }
    }

    return static_cast<index_t>(NO_INDEX);
}

// implement ILayoutNodeList

CStandardLayoutNodeList::SNode 
CStandardLayoutNodeList::GetNode (index_t index) const
{
    SNode result;

	const CVisibleGraphNode* node = nodes[index].node;

    result.rect = nodes[index].rect;
    result.node = node;
    result.style = GetStyle (node);
    result.styleFlags = GetStyleFlags (node);

    return result;
}
