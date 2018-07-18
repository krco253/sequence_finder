#include "IndexedFragment.h"
//maybe there's something wrong with the object in your main of seq finder???
int main()
{
Dna5String whole_sequence("ATTTAGCCCGTTAAAGTGTGCATGTGTTTTTTTT");
std::pair<unsigned,unsigned> index(0,4);
IndexedFragment frag(index, whole_sequence);
unsigned test = 0;
test = frag.index_retrieve(2);
std::cout << test;
return 0;
}
