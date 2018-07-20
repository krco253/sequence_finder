/*---------------------------------
| File: ContextFragment.h
| Class: ContextFragment
-----------------------------------*/

#include "IndexedFragment.h"

class ContextFragment
{
	friend class SubstringVector;
	public:
		//Constructors and Destructors
		ContextFragment();
		ContextFragment(IndexedFragment prefix, IndexedFragment query, IndexedFragment suffix);
		ContextFragment(const ContextFragment &copy);
		~ContextFragment();

		//print the query sequence in red
		void print_query();
		//clear out contextfragment
		void clear();
		//check if a contextfragment is empty
		bool isEmpty();
		//"flatten" the ContextFragment into one IndexedFragment
		std::pair<unsigned,unsigned> flatten();
		//combine fragments, conserving the query sequences
		ContextFragment consolidate_frags(const ContextFragment &other, Dna5String &sequence);
		//return the requested index of the query_seq
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
