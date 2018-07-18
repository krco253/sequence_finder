/*-----------------------------------------------------------
Name: sequence_finder
Author: Kelsey Cole
Purpose: Search for a sequence with fuzzy matching
Usage: compile with flags g++ -isystem ~/devel/seqan/include 
-std=c++14 -O3 -DNDEBUG -W -Wall -pedantic -fopenmp
-lpthread -ggdb -o sequence_finder sequence_finder.cpp -lrt 
-Wno-unused-parameter -Wno-sequence-point
TODO: -line up query column 
      -look for statistics for T content next to MoTeR relics
      - correct consolidate_sequences
      - output results in a way that can be pipelined (for Harrison)
      - compare results with FH against the paper
      - eventually extend to also search for 5' end of MoTER and consolidate whole results
-----------------------------------------------------------*/

#include <mutex> 
#include <seqan/arg_parse.h> //parsing arguments from the command line
#include <seqan/file.h>
#include <seqan/seq_io.h> //reading input files/writing input files
#include "FragmentLibrary.h"
#include <aio.h>

using namespace seqan;

#define ENABLE_SEQAN_DEBUG = 1

typedef Infix<Dna5String>::Type SString; //a type that stores subsequences of Dna5Strings
typedef std::pair <SString, SString> SStringPair; //a pair of SStrings
typedef std::pair <SString, SStringPair> ContextPair; //a pair of a SString and SStringPair, later used to store a match and its context

/*--------------------------------------------------------------
| Function: find_MoTERs
| Input: Empty StringSets 
| "Returns": The essential MoTER sequences and extended sequences 
| to search for
---------------------------------------------------------------*/

void find_MoTERs(StringSet<Dna5String> &moter_patterns_central, StringSet<Dna5String> &moter_patterns_extended)
{
	//moter_patterns_central represents "essential" MoTER sequences
	//moter_patterns_extended represents "extended" sequences to search for

	Dna5String moter_relic1("AATTAAACCCTAA"); 
	Dna5String moter_relic3("ATTAAACCCTAACCCTAA");
	Dna5String moter_relic4("TATAATAAAGCGCGAATTAAAA");
	Dna5String moter_relic5("AATTAAACCCTAACCCTAACCCTAA");	
	Dna5String moter_relic2("TTAGCAAATAAGCTTAGAATATAATAAAGCGCGAATTAAAA");
	Dna5String moter_relic6("TCTATTTTATTTGTACGACAAAACCCTTAGCAAATAAGCTTAGAATATAATAAA");
	Dna5String moter_relic7("ACGGTTTTATTTTCCCGTTTGTTTTTTCTATTTTATTTGTACGACAAAACC");	
	
	appendValue(moter_patterns_central, moter_relic1);
	appendValue(moter_patterns_central, moter_relic2);
	appendValue(moter_patterns_central, moter_relic3);
	appendValue(moter_patterns_extended, moter_relic4);
	appendValue(moter_patterns_extended, moter_relic5);
	appendValue(moter_patterns_extended, moter_relic6);
	appendValue(moter_patterns_extended, moter_relic7);
}

/*------------------------------------------------------------
|
|			  Main Function              
|
------------------------------------------------------------*/

int main(int argc, char const ** argv) 
{
	//Set Up ArgumentParser
	ArgumentParser parser("sequence_finder");

	addOption(parser, ArgParseOption("s", "haystack", "Provide path for sequence to search (.fasta)", ArgParseArgument::INPUT_FILE, "IN"));	
	addOption(parser, ArgParseOption("n", "needle", "Provide path for sequence to search for (.fasta). Default is MoTER relic", ArgParseArgument::INPUT_FILE, "IN")); 
	addOption(parser, ArgParseOption("t", "needle_tip", "Short string to look for typed out on command line. Default is MoTER relic", ArgParseArgument::STRING, "TEXT"));
	addOption(parser, ArgParseOption("e", "errors", "Max number of errors allowed. Default is 2", ArgParseArgument::INTEGER, "INT")); 
	addOption(parser, ArgParseOption("m", "minimum_match", "Minimum length of nucleotide sequence to consider a \"match\". Default is no minimum.", ArgParseArgument::INTEGER, "INT"));
	//TODO
//	addOption(parser, ArgParseOption("m", "moter_setting", "Choose from: \"begin\", \"end\", i
	//Parse command line arguments
	ArgumentParser::ParseResult res = parse(parser, argc, argv);
	
	//if parsing was not successful exit with code 1 if there were errors
	if (res != ArgumentParser::PARSE_OK)
		return res == ArgumentParser::PARSE_ERROR;
	//extract and store option value
	int errors;
	unsigned min_match;	
	CharString haystackFileName, needleFileName;
	Dna5String query;
	
	getOptionValue(haystackFileName, parser, "haystack");
	getOptionValue(needleFileName, parser, "needle");
	getOptionValue(query, parser, "needle_tip");
	getOptionValue(errors, parser, "errors");
	getOptionValue(min_match, parser, "minimum_match");
	
	//Open input file
	StringSet<CharString> h_ids;
	StringSet<Dna5String> h_seqs;
		
	SeqFileIn seqFileIn1(toCString(haystackFileName));
	//Reads records at the same time
	try
	{
		readRecords(h_ids, h_seqs, seqFileIn1);
	}
	catch (Exception const &e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
	}
	
//	std::cout << lengthSum(h_ids) << std::endl;
//	std::cout << "Position 0: " << value(h_seqs, 0) << '\n';
//	std::cout << "Position 15: " <<value(h_seqs, 0) << '\n';
		
	//declare containers to store needle file
	CharString n_id;
	Dna5String n_seq;

	//open needle file
	if(isSet(parser, "needle"))
	{
		SeqFileIn seqFileIn2(toCString(needleFileName));
 		try
		{
			readRecord(n_id, n_seq, seqFileIn2);
		}
		catch (Exception const &e)
		{
			std::cout << "ERROR: " << e.what() << std::endl;
		}
	}
	//cout the sequence to make sure it read in correctly
	//std::cout << "Position 0: " << value(h_seqs, 0) << '\n';
	
	//Default: Search for MoTER sequence

	Dna5String whole_seq;
	whole_seq = concat(h_seqs);

	Index<Dna5String, BidirectionalIndex<FMIndex<> > > index(whole_seq);
	

	std::mutex mtx;
	FragmentLibrary hits;
	SString temp_sstring;
	auto delegateParallel = [&hits, &mtx, &whole_seq](auto &iter, Dna5String const & needle, uint8_t errors)
	{
		std::lock_guard<std::mutex> lck(mtx);
		for (auto occ : getOccurrences(iter)) //function getOccurences returns the starting index of the hit
		{
			unsigned point = occ;
			if((point > 40) && ((point+length(needle)+41 != length(whole_seq))))
			{
				std::pair<unsigned, unsigned> q_point(point, point+length(needle));
				std::pair<unsigned, unsigned> pre_point(point-41,point);
				std::pair<unsigned, unsigned> suff_point(point+length(needle), point+length(needle)+41);
				IndexedFragment temp_ind_frag1(q_point, whole_seq);
				IndexedFragment temp_ind_frag0(pre_point, whole_seq);
				IndexedFragment temp_ind_frag2(suff_point, whole_seq);
				ContextFragment temp_cont(temp_ind_frag0, temp_ind_frag1, temp_ind_frag2);			
				hits.add_substring(temp_cont);
			}
			else if((point <= 40) && (point+length(needle)+81 != length(whole_seq)))
			{
				std::pair<unsigned, unsigned> q_point(point, point+length(needle));
				std::pair<unsigned, unsigned> suff_point(point+length(needle), point+length(needle)+81);
				IndexedFragment temp_ind_frag1(q_point, whole_seq);
				IndexedFragment temp_ind_frag2(suff_point, whole_seq);
				IndexedFragment temp_ind_frag0;
				ContextFragment temp_cont(temp_ind_frag0, temp_ind_frag1, temp_ind_frag2);			
				hits.add_substring(temp_cont);
				
			} 
			else if(point >= 80)
			{
				std::pair<unsigned, unsigned> q_point(point, point+length(needle));
				std::pair<unsigned, unsigned> pre_point(point-81, point);
				IndexedFragment temp_ind_frag1(q_point, whole_seq);
				IndexedFragment temp_ind_frag0(pre_point, whole_seq);
				IndexedFragment temp_ind_frag2;
				ContextFragment temp_cont(temp_ind_frag0, temp_ind_frag1, temp_ind_frag2);			
				hits.add_substring(temp_cont);
	
			}
			else
			{
				std::pair<unsigned, unsigned> q_point (point, point+length(needle));
			
				IndexedFragment temp_ind_frag1(q_point, whole_seq);
				IndexedFragment temp_ind_frag0, temp_ind_frag2;
				ContextFragment temp_cont(temp_ind_frag0, temp_ind_frag1, temp_ind_frag2);			
				hits.add_substring(temp_cont);

			}
		}
		
	}; 
	//Search for hits

	if((isSet(parser, "needle")) || (isSet(parser, "needle_tip"))) //if needle was specified
	{	
		int mismatches = 0;
		if(isSet(parser, "errors"))
			mismatches = errors;
		else
			mismatches = 2;	
		std::cout << "Looking for hits with up to " << mismatches << " errors: " << std::endl;
		
		StringSet<Dna5String> search_patterns;
	
		if (isSet(parser, "needle"))
		{	search_patterns = n_seq;}
 		else if (isSet(parser, "needle_tip"))
		{	appendValue(search_patterns, query);}


		//check for number of mismatches

		if (mismatches ==1) 
		{
			find<0,1>(delegateParallel, index, search_patterns, EditDistance());
		}
		else if(mismatches ==2)
		{
			find<0,2>(delegateParallel, index, search_patterns, EditDistance());
	
		}
		else if(mismatches ==3)
		{
			find<0,3>(delegateParallel, index, search_patterns, EditDistance());
	
		}
		else if (mismatches ==4)
		{
			find<0,4>(delegateParallel, index, search_patterns, EditDistance());
		
		}

	}
	else //Search for moter relics
	{
		std::cout << "Beginning search for MoTER relics" << std::endl;
		StringSet<Dna5String> essential_seqs;
		StringSet<Dna5String> extended_seqs;
		find_MoTERs(essential_seqs, extended_seqs);
		std::cout << "Searching for essential hits. . ." << std::endl;
		find<0,1>(delegateParallel, index, essential_seqs, EditDistance());
		std::cout << hits.size() << " essential hits found!" << std::endl;
		std::cout << "Searching for extended hits. . ." << std::endl;
		find<0,3>(delegateParallel, index, extended_seqs, EditDistance());
	}
	
	//check if there were hits

	if (hits.empty()) 
	{
		std::cout << "There were no hits :(" << std::endl;	
		return 0;
	}
	
	//Consolidate hits

	std::cout <<  "Consolidating hits . . ." << std::endl;
	
	hits.annotated_print();	

//	hits.consolidate_sequences(whole_seq);

	//Prepare hits for printing	
	
//	if(!isSet(parser, "minimum_match"))
//		min_match = 1;

	//Print hits
//	hits.sort_by_index();
	std::cout << std::endl;
//
	hits.annotated_print();	

//	std::cout << "There were " << hits.size() << " consolidated hits" << std::endl;

	return 0;	
}