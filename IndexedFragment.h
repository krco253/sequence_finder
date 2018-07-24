/*---------------------------------
| File: IndexedFragment.h
| Class: IndexedFragment
| Description: a container to store a Seqan defined "infix" sequence (which is a pointer to a subsequence on a reference genome) with its indices 
| Author: Kelsey Cole
-----------------------------------*/

#include <utility> //pair
#include <iostream> //cout 
#include <iterator>
#include <stdlib.h>

//seqan includes
#include <seqan/stream.h>
#include <seqan/sequence.h>
#include <seqan/basic.h>
#include <seqan/index.h>

using namespace seqan;

typedef Infix<Dna5String>::Type SString; //a type that stores subsequences of Dna5Strings
typedef std::pair <SString, SString> SStringPair; //a pair of SStrings
typedef std::pair <SString, SStringPair> ContextPair; //a pair of a SString and SStringPair, later used to store a match and its context

class IndexedFragment
{
	friend class ContextFragment; //this class is inherently designed to be used for ContextFragment and FragmentLibrary
	friend class FragmentLibrary;
	public:
		//Constructors and Destructors
		IndexedFragment();
		IndexedFragment(std::pair<unsigned, unsigned> index_pair, Dna5String &haystack);
		IndexedFragment(const IndexedFragment &copy);
		~IndexedFragment();

		IndexedFragment &operator=(const IndexedFragment& other);
		//retrieve index
		unsigned index_retrieve(const unsigned& index_num) const;
		friend std::ostream& operator<<(std::ostream& os, const IndexedFragment& ind_frag);
		//Compare IndexedFragments by first index
		friend bool operator<(const IndexedFragment &first, const IndexedFragment &last);
		friend bool operator>(const IndexedFragment &first, const IndexedFragment &last);
		friend bool operator==(const IndexedFragment &first, const IndexedFragment &last);
	
	private:
		std::pair <unsigned,unsigned> indices; //indices of the sequence
		SString fragment; //the sequence itself
};
