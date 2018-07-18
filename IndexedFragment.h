/*---------------------------------------------
| File: IndexedFragment.h
| Class: IndexedFragment
------------------------------------------------*/

#include <utility> //pair
#include <iostream> //cout 
#include <aio.h> //for asynchrous search
#include <iterator>
#include <stdlib.h>

//seqan includes
#include <seqan/stream.h>
#include <seqan/sequence.h>
#include <seqan/basic.h>
#include <seqan/index.h>

using namespace seqan;

typedef Infix<Dna5String>::Type SString; //a type that creates pointers to subsequences of Dna5Strings

class IndexedFragment
{
	friend class ContextFragment;
	friend class SubstringVector;
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
		std::pair <unsigned,unsigned> indices;
		SString fragment;
};
