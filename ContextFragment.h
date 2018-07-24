/*---------------------------------
| File: ContextFragment.h
| Class: ContextFragment
| Description: a container to store an IndexedFragment query sequence and its surrounding prefix/suffix
| Author: Kelsey Cole
-----------------------------------*/

#include "IndexedFragment.h"

class ContextFragment
{
	friend class FragmentLibrary; //this class was inherently designed to be used for FragmentLibrary
	public:
		//Constructors and Destructors
		ContextFragment();
		ContextFragment(IndexedFragment prefix, IndexedFragment query, IndexedFragment suffix);
		ContextFragment(const ContextFragment &copy);
		~ContextFragment();

		//print_query(): print this ContextFragment, with member variable query_seq in red
		void print_query();
		//clear(): clear out this ContextFragment (set all member variables to empty)
		void clear();
		//isEmpty(): check if this ContextFragment is empty
		bool isEmpty();
		//flatten(): returned the "flattened" coordinates of this ContextFragment 
		std::pair<unsigned,unsigned> flatten();
		//consolidate_frags: combine fragments, conserving the query sequences
		ContextFragment consolidate_frags(const ContextFragment &other, Dna5String &sequence);
		//get_index: return the requested index of the query_seq
		unsigned get_index(unsigned which_index, unsigned which_fragment);
		
		//Overloaded Operators
		ContextFragment operator=(const ContextFragment& other);
		friend std::ostream& operator<<(std::ostream &os, const ContextFragment& cont_frag);

		//boolean operators -- compared by the first index of query_seq
		friend bool operator<(const ContextFragment &first, const ContextFragment &last);
		friend bool operator>(const ContextFragment &first, const ContextFragment &last);
		friend bool operator==(const ContextFragment &first, const ContextFragment &last);
	
	private:
		IndexedFragment prefix_seq; //sequence that precedes query_seq
		IndexedFragment query_seq; //main sequence
		IndexedFragment suffix_seq; //sequence that proceeds query_seq
}; 
