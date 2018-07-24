/*---------------------------------
| File: ContextFragment.cpp
| Class: ContextFragment
| Description: a container to store an IndexedFragment query sequence and its surrounding prefix/suffix
| Author: Kelsey Cole
-----------------------------------*/

#include "ContextFragment.h"


/*---------------------------------------------
| Constructors and Destructors
------------------------------------------------*/

ContextFragment::ContextFragment() { }

ContextFragment::ContextFragment(IndexedFragment prefix, IndexedFragment query, IndexedFragment suffix)
{
	this->prefix_seq = prefix;
	this->query_seq = query;
	this->suffix_seq = suffix;
}

ContextFragment::ContextFragment(const ContextFragment &copy)
{
	this->prefix_seq = copy.prefix_seq;
	this->query_seq = copy.query_seq;
	this->suffix_seq = copy.suffix_seq;
}

ContextFragment::~ContextFragment() { }

/*---------------------------------------------
| clear()
| Arguments: n/a
| Description: "Reset" a ContextFragment to empty
------------------------------------------------*/
void ContextFragment::clear()
{
	IndexedFragment empty_frag;
	this->prefix_seq = empty_frag;
	this->query_seq = empty_frag;
	this->suffix_seq = empty_frag;
}
/*---------------------------------------------
| isEmpty()
| Arguments: n/a
| Returns: boolean type (true if member variables prefix_seq, query_seq, and suffix_seq are all empty
------------------------------------------------*/
bool ContextFragment::isEmpty()
{
	IndexedFragment empty_frag;
	if ((this->prefix_seq == empty_frag) && (this->query_seq == empty_frag) && (this->suffix_seq == empty_frag))
		return true;
	else return false;
}
/*---------------------------------------------
| flatten()
| Arguments: n/a
| Returns: the coordinates of this ContextFragment in the reference genome, beginning at the first coordinate of the prefix_seq and ending at the last coordinate of the suffix_seq
------------------------------------------------*/
std::pair<unsigned, unsigned> ContextFragment::flatten()
{
	std::pair<unsigned,unsigned> flattened_indices;
	flattened_indices.first = ((this->prefix_seq).indices).first; 
	flattened_indices.second = ((this->suffix_seq).indices).second;
	return flattened_indices;
}
/*---------------------------------------------
| consolidate_frags()
| Arguments: 
| other - the ContextFragment to combine this ContextFragment with
| sequence - the reference genome
| Returns - the combined ContextFragment 
------------------------------------------------*/
ContextFragment ContextFragment::consolidate_frags(const ContextFragment &other, Dna5String &sequence)
{
	std::pair<unsigned,unsigned> query_indices;
	std::pair<unsigned, unsigned> prefix_indices;
	std::pair<unsigned, unsigned> suffix_indices;
	//conserve query indices
	if (((this->query_seq).indices).first <= ((other.query_seq).indices).first)
		query_indices.first = ((this->query_seq).indices).first;
	else 
		query_indices.first = ((other.query_seq).indices).first;
	if (((this->query_seq).indices).second >= ((other.query_seq).indices).second)
		query_indices.second = ((this->query_seq).indices).second;
	else
		query_indices.second = ((other.query_seq).indices).second;
	//create prefix and suffix indices
	//determine which prefix starts first
	if (((this->prefix_seq).indices).first <= ((other.prefix_seq).indices).first)
		prefix_indices.first = ((this->prefix_seq).indices).first;
	else
		prefix_indices.first = ((other.prefix_seq).indices).first;
	//make prefix continue up to query_seq
	prefix_indices.second = query_indices.first;
	//determine which suffix ends last
	if (((this->suffix_seq).indices).second >= ((other.suffix_seq).indices).second)
		suffix_indices.second = ((this->suffix_seq).indices).second;
	else
		suffix_indices.second = ((other.suffix_seq).indices).second;
	//make suffix start at end of query sequence
	suffix_indices.first = query_indices.second;
	
	//Create the new consolidated ContextFragment
	//Create indexedfragments
	IndexedFragment pre(prefix_indices, sequence);
	IndexedFragment que(query_indices, sequence);
	IndexedFragment suf(suffix_indices, sequence);

	ContextFragment consolidated_frags(pre, que, suf);
	
	return consolidated_frags; 
	
}
	

	
/*---------------------------------------------
| get_index
| Arguments: 
| which_index - which index to return (1 or 2)
| which_fragment - which fragment to return (prefix - 0, query - 1, suffix - 2)
------------------------------------------------*/
unsigned ContextFragment::get_index(unsigned which_index, unsigned which_fragment)
{
	unsigned index_req = 0;
	if ((which_index == 1) && (which_fragment == 0))
		 index_req = (this->prefix_seq).index_retrieve(which_index);
	else if ((which_index ==1) && (which_fragment ==1))
		index_req = (this->query_seq).index_retrieve(which_index);
	else if ((which_index == 1) && (which_fragment == 2))
		index_req =  (this->suffix_seq).index_retrieve(which_index);
	else if ((which_index ==2) && (which_fragment ==0))
		index_req = (this->prefix_seq).index_retrieve(which_index);
	else if ((which_index == 2) && (which_fragment ==1))
		index_req = (this->query_seq).index_retrieve(which_index);
	else if ((which_index == 2) && (which_fragment ==2))
		index_req = (this->suffix_seq).index_retrieve(which_index);
	return index_req;
}
/*---------------------------------------------
| print_query (in red)
| Arguments: n/a
| Output: this ContextFragment, with the query sequence in red
------------------------------------------------*/
void ContextFragment::print_query()
{
	std::cout << std::setw(8) << ((this->prefix_seq).indices).first << " " <<  this->prefix_seq << "\033[1;31m" <<  this->query_seq << "\033[0m" << this->suffix_seq << " " << ((this->suffix_seq).indices).second << std::endl;

}

/*---------------------------------------------
| Overloaded Operators
------------------------------------------------*/

ContextFragment ContextFragment::operator=(const ContextFragment &other)
{
	prefix_seq = other.prefix_seq;
	query_seq = other.query_seq;
	suffix_seq = other.suffix_seq;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const ContextFragment& cont_frag)
{
	os << cont_frag.prefix_seq << " " << cont_frag.query_seq << " " << cont_frag.suffix_seq;
	return os;
}


//boolean operators -- compare by first index of query_seq

bool operator<(const ContextFragment &first, const ContextFragment &last)
{
	if(first.query_seq < last.query_seq)
		return true;
	else return false;
}

bool operator>(const ContextFragment &first, const ContextFragment &last)
{
	if(first.query_seq > last.query_seq)
		return true;
	else return false;
}

bool operator==(const ContextFragment &first, const ContextFragment &last)
{
	if(first.query_seq == last.query_seq)
		return true;
	else return false;
}
