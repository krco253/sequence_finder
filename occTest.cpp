#include <seqan/stream.h>
#include <seqan/sequence.h>
#include <seqan/basic.h>
#include <seqan/index.h>
#include <aio.h> //for asynchrous search

using namespace seqan;

int main()
{
 
Dna5String example_seq("GAGGCCACTCGCAGGATTAAGTCAATAAGTTAATGGCGTGGGGTTATGGTATGGGGGTTCTCGCCCACAGTGACCTCATCGGTGCATTTCCTCATCGTAGGCGGAACGGTAGACACAAGGCATGATGTCAAATCGCGACTCCAATCCCAAGGTCGCAAGCCTATATAGGAACCCGCTTATGCCCTCTAATCCCGGACAGACCCCAAATATGGCATAGCTGGTTGGGGGTACCTACTAGGCACAGCCGGAAGCA");

std::cout << example_seq << std::endl;

Index<Dna5String, BidirectionalIndex<FMIndex<> > > index(example_seq);
auto delegate = [&example_seq](auto &iter, Dna5String const & needle, uint8_t errors)
{
	for (auto occ : getOccurrences(iter))
	{
		Infix<Dna5String>::Type inf = infix(example_seq, occ, occ + length(needle));
		Infix<Dna5String>::Type inf0 = infix(example_seq, occ-20, occ);
		Infix<Dna5String>::Type inf2 = infix(example_seq, occ+length(needle), occ+length(needle)+11); 
		std::cout << occ-20 << " " << inf0 << " " << occ << " " << inf << " " << occ + length(needle) << " " << inf2 << " " << occ+length(needle) + 11 << std::endl;
	}
};
std::cout << "Hits with up to 2 errors: " << std::endl;
std::cout << "Original pattern: " << std::endl;

Dna5String pattern("GGGGTTAT");
std::cout << pattern << std::endl;

find<0,2>(delegate, index, pattern, EditDistance());

return 0;
}
