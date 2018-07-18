#include "FragmentLibrary.h"
int main()
{
Dna5String genome("ACCTTAGTGACGTATTGCTTGCCCGGTAACTTTCCGTTAA");
std::pair<unsigned,unsigned> pair0(0,4);
std::pair<unsigned,unsigned> pair1(4,8);
std::pair<unsigned,unsigned> pair2(8,12);
std::pair<unsigned,unsigned> pair01(12,16);
std::pair<unsigned,unsigned> pair02(16,20);
std::pair<unsigned,unsigned> pair13(20,26);
IndexedFragment frag0(pair0, genome);
IndexedFragment frag1(pair1, genome);
IndexedFragment frag2(pair2, genome);
IndexedFragment frag01(pair01, genome);
IndexedFragment frag02(pair02, genome);
IndexedFragment frag13(pair13, genome);
ContextFragment context1(frag0, frag1, frag2);
ContextFragment context2(frag2, frag01, frag02);
ContextFragment context3(frag01, frag02, frag13);
FragmentLibrary test_vector;
test_vector.add_substring(context1);
test_vector.add_substring(context2);
test_vector.add_substring(context3);
std::cout << genome << std::endl;
std::cout << "Before consolidation: " << std::endl;
test_vector.annotated_print();
test_vector.consolidate_sequences(genome);
std::cout << "After consolidation: " << std::endl;
test_vector.annotated_print();
return 0;
}
