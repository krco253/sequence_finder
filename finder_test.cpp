#include <set>
#include <mutex>
#include <aio.h>

#include <seqan/index.h>

using namespace seqan;

int main ()
{
	Dna5String genome("CCCGAACCCGAACCCAAACCCAAACCCAAACCCAAACCCAAACCCAAACCCAAACCCAAACCCGGAGGGTTCCCAAGTCGCCTAAACCCGAAGGGTTTAGGATATTATTTCGTTTATTAGAATTGGATAATTATTTACCCCTGTTGGACAGGGGGGTTGCAGGGGTTAAATTAAGGTTTTTTATTATTTATGCGCCGTTTATTTGTTTACCCCCCCAAATATTATAAAAGCGCGTTCCATCCTCTTAGGAAAAGCGAAGCTTTTCCTTGTAAAAGTCGCTAGACTTTTACTATAAAAGTCGCTAGACTTTTATACCAATCTTTTAACAAAAAGCGTAGCTTTTTGTTGCCAATCTATTAAAAAAAGCGGAGCTTTTTTTAACTTTTTCTTTTTTTTTTTTTTTTCTTTTTTTTTTTTTTTTTTCTTTTTTTTTTTTTTTTTTTTTTATATATATTATTATTATTATT");

Index<Dna5String, BidirectionalIndex<FMIndex<> > >index(genome);

auto delegate = [](auto &iter, Dna5String const & needle, uint8_t errors)
{
	for (auto occ : getOccurrences(iter))
		std::cout << occ << std::endl;
};

Dna5String pattern("TTTTTTTTTTTTTTTTTT");
std::cout << "Hits with up to 2 errors: " << std::endl;
find<0,2>(delegate, index, pattern, HammingDistance());

return 0;

}

