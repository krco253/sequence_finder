/*---------------------------------
| File: FragmentLibrary.h
| Class: FragmentLibrary
-----------------------------------*/

#include "ContextFragment.h" //ContextFragment
#include <list> //list
#include <algorithm> //sort
class FragmentLibrary
{
	public:
		//Constructors/Destructors
		FragmentLibrary();
		FragmentLibrary(std::list<ContextFragment> &substrings, ContextFragment &new_frag);
		FragmentLibrary(const FragmentLibrary &copy);
		~FragmentLibrary();
		//print list with all query sequences in red
		void annotated_print();
		//size() returns size of FragmentLibrary
		unsigned size();
		//Add substring at beginning of list
		void add_substring(const ContextFragment &new_frag);
		//Sort Vector by first index of each ContextFragment	
		void sort_by_index();
		//empty(): return true if FragmentLibrary is empty
		bool empty();
		//erase_substring: erase a substring from the FragmentLibrary
		std::list<ContextFragment>::iterator  erase_substring(std::list<ContextFragment>::iterator &index);

		//consolidate_sequences: consolidate overlapping sequences
		void consolidate_sequences(Dna5String &context_sequence); 
		//Overloaded Assignment Operators
		FragmentLibrary operator=(const FragmentLibrary & other);
		friend std::ostream& operator<<(std::ostream &os, const FragmentLibrary& sub_vec);
		
	private:
		std::list<ContextFragment> context_substrings;
};
