#pragma once

// required includes

#include "DictionaryBasedTempPath.h"
#include "NodeClassification.h"

/**
 * Efficiently classifies a path as "trunk", "branches" or "tags".
 * For every class, a list of wildcard patterns separed by semicolon 
 * can be specified. Example "tags;qa ?;qa ?.*". String comparison
 * is case-insensitive for latin chars ('a' .. 'z').
 * A given path may be assigned to multiple classes.
 */

class CPathClassificator
{
private:

    class CWildCardPattern
    {
    private:

        /// pattern to match against

        std::string pattern;

        /// algorithm selection

        bool hasWildCards;

        /// different matching algorithms

        bool WildCardMatch (const char* s, const char* pattern) const;
        bool StraightMatch (const char* s, size_t length) const;

        /// construction utility

        void NormalizePattern();

    public:

        /// construction

        CWildCardPattern (const std::string& pattern);

        /// match s against the pattern

        bool Match (const char* s, size_t length) const;
    };

    /// the patterns to detect trunk, branches and tags

    typedef std::vector<CWildCardPattern> TPatterns;

    TPatterns trunkPatterns;
    TPatterns branchesPatterns;
    TPatterns tagsPatterns;

    /// path classification

    std::vector<unsigned char> pathElementClassification;
    std::vector<unsigned char> pathClassification;

    /// construction utilities

    TPatterns ParsePatterns (const std::string& patterns) const;

    bool Matches ( TPatterns::const_iterator firstPattern
                 , TPatterns::const_iterator lastPattern
                 , const char* element
                 , size_t length) const;
    void ClassifyPathElements ( const LogCache::CStringDictionary& elements
                              , const TPatterns& patterns
                              , unsigned char classification);
    void ClassifyPathElements (const LogCache::CStringDictionary& elements);
    void ClassifyPaths (const LogCache::CPathDictionary& paths);

public:

    /// construction / destuction

    CPathClassificator ( const LogCache::CPathDictionary& paths
                       , const std::string& trunkPatterns = "trunk"
                       , const std::string& branchesPatterns = "branches"
                       , const std::string& tagsPatterns = "tags");
    ~CPathClassificator(void);

    /// get the classification for a given path

    unsigned char GetClassification (const LogCache::index_t pathID) const;
    unsigned char operator[](const LogCache::CDictionaryBasedPath& path) const;

    unsigned char GetClassification (const LogCache::CDictionaryBasedTempPath& path) const;
    unsigned char operator[](const LogCache::CDictionaryBasedTempPath& path) const;
};

/// get the classification for a given path

inline unsigned char 
CPathClassificator::GetClassification (const LogCache::index_t pathID) const
{
    return pathClassification[pathID];
}

inline unsigned char 
CPathClassificator::operator[](const LogCache::CDictionaryBasedPath& path) const
{
    return GetClassification (path.GetIndex());
}

inline unsigned char 
CPathClassificator::operator[](const LogCache::CDictionaryBasedTempPath& path) const
{
    return GetClassification (path);
}
