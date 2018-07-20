/*---------------------------------
| File: FragmentLibrary.h
| Class: FragmentLibrary
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
		//print list with all query sequences in red
		void annotated_print();
		//size() returns size of FragmentLibrary
		unsigned size();
		//Sort Vector by first index of each ContextFragment	
		void sort_by_index();
		//empty(): return true if FragmentLibrary is empty
		bool empty();
		//erase_substring: erase a substring from the FragmentLibrary
		void erase_substring(std::pair<unsigned, unsigned> &fragment_key);

		//consolidate_sequences: consolidate overlapping sequences
		void consolidate_sequences(Dna5String &context_sequence); 
		//insert element 
		void insert(std::pair<unsigned,unsigned> &fragment_key, ContextFragment fragment_object); 
		//return iterator to beginning of map
		std::map<std::pair<unsigned,unsigned>, ContextFragment>::iterator begin();
		//Overloaded Assignment Operators
		FragmentLibrary operator=(const FragmentLibrary & other);
		friend std::ostream& operator<<(std::ostream &os, const FragmentLibrary& sub_vec);
		
	private:
		std::map<std::pair<unsigned,unsigned>, ContextFragment> context_substrings;
};
