#include "SubstringVector.h"

int main()
{
	Dna5String haystack("ACCTGAACCTAGTATGTACGTTTTT");
	std::pair<unsigned,unsigned> sub_indices (0,4);
	IndexedFragment subseq;
	std::cout << "This is the empty subseq " << subseq << std::endl;
	IndexedFragment subseq2(sub_indices, haystack);
	//std::cout << "This is the haystack " << haystack << std::endl;
	std::cout << "This is the defined subseq2 " << subseq2 << std::endl;
	std::pair<unsigned,unsigned> sub_indices3 (4,8);
	std::pair<unsigned,unsigned> sub_indices4 (8,12);
	IndexedFragment subseq3(sub_indices3, haystack);
	IndexedFragment subseq4(sub_indices4, haystack);
	ContextFragment whole_frag1(subseq, subseq2, subseq3); 
	ContextFragment whole_frag2(subseq2, subseq3, subseq4);
	std::cout<< "This is the whole fragment: " << whole_frag1 <<std::endl;
	ContextFragment whole_frag_copy;
	whole_frag_copy = whole_frag1;
	SubstringVector newSSvector;
	newSSvector.add_substring(whole_frag2);
	newSSvector.add_substring(whole_frag1);
	std::cout << newSSvector << std::endl;
	newSSvector.sort_by_index();
	std::cout << newSSvector << std::endl;
	std::cout << "Element 0 of the SubstringVector: " << newSSvector[0] << std::endl;
	newSSvector.annotated_print(); 	
	return 0;
}
