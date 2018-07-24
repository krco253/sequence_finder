/*---------------------------------
| File: FragmentLibrary.h
| Class: FragmentLibrary
| Description: a library to store ContextFragments from the same genome
| Author: Kelsey Cole
-----------------------------------*/

#include "ContextFragment.h" //ContextFragment
#include <map> //map
#include <algorithm> //sort

class FragmentLibrary
{
	public:
		//Constructors/Destructors
		FragmentLibrary();
		FragmentLibrary(const FragmentLibrary &copy);
		~FragmentLibrary();
		//annotated_print(): print member variable context_substrings with all query sequences in red
		void annotated_print();
		//size(): return size of member variable context_substrings
		unsigned size();
		//empty(): return true if FragmentLibrary is empty
		bool empty();
		//erase_substring: erase a substring from member variable context_substrings
		void erase_substring(std::pair<unsigned, unsigned> &fragment_key);
		//consolidate_sequences: consolidate overlapping sequences 
		void consolidate_sequences(Dna5String &context_sequence); 
		//insert: insert element into member variable context_substrings
		void insert(std::pair<unsigned,unsigned> &fragment_key, ContextFragment fragment_object); 
		//begin(): return iterator to beginning of map
		std::map<std::pair<unsigned,unsigned>, ContextFragment>::iterator begin();
		//Overloaded Assignment Operators
		FragmentLibrary operator=(const FragmentLibrary & other);
		friend std::ostream& operator<<(std::ostream &os, const FragmentLibrary& sub_vec);
		
	private:
		//store ContextFragments in a map with their coordinates as their key
		std::map<std::pair<unsigned,unsigned>, ContextFragment> context_substrings;
		//the name of the common reference genome
		CharString reference_name;
};
