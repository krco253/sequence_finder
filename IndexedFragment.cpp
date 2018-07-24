/*---------------------------------
| File: IndexedFragment.cpp
| Class: IndexedFragment
| Description: a container to store a Seqan defined "infix" sequence 
| (which is a pointer to a subsequence on a reference genome) with its indices 
| Author: Kelsey Cole
-----------------------------------*/

#include "IndexedFragment.h"

/*---------------------------------------------
| Constructors and Destructors
------------------------------------------------*/

IndexedFragment::IndexedFragment()
{ 
	Dna5String empty_haystack = "";
	this->indices.first = 0;
	this->indices.second = 0;
	this->fragment = infix(empty_haystack, indices.first, indices.second);
}

IndexedFragment::IndexedFragment(std::pair<unsigned,unsigned> index_pair, Dna5String &haystack)
{
	this->indices = index_pair; //note that the indices behave like so: [x,y)
	this->fragment = infix(haystack, indices.first, indices.second);
}

IndexedFragment::IndexedFragment(const IndexedFragment& copy)
{
	this->indices = copy.indices;
	this->fragment = copy.fragment; 
}

IndexedFragment::~IndexedFragment()
{
	
}

/*---------------------------------------------
| index_retrieve
| Arguments: 
| index_num - which index to retrieve (1 - first, 2 - second)
| Returns: the requested index
-----------------------------------------------*/
unsigned IndexedFragment::index_retrieve(const unsigned &index_num) const
{
	unsigned index_value = 0;
	if(index_num == 1) 
		index_value = this->indices.first;
	else if (index_num == 2)
		index_value = this->indices.second;
	return index_value;
}

/*---------------------------------------------
| Overloaded Operators
------------------------------------------------*/

IndexedFragment &IndexedFragment::operator=(const IndexedFragment& other)
{
	if (this != &other)
	{
		this->indices = other.indices;
		this->fragment = other.fragment;
	}
	return *this;
}

std::ostream& operator<<(std::ostream& os, const IndexedFragment& ind_frag)
{
	os << (ind_frag).fragment;
	return os;
} 

//boolean operators -- compare by first index of member "indices"

bool operator<(const IndexedFragment &first, const IndexedFragment &last)
{
	if((first.indices).first < (last.indices).first)
		return true;
	else return false;
}

bool operator>(const IndexedFragment &first, const IndexedFragment &last)
{
	if((first.indices).first > (last.indices).first)
		return true;
	else return false;
}

bool operator==(const IndexedFragment &first, const IndexedFragment &last)
{
	if((first.indices).first == (last.indices).first)
		return true;
	else return false;
}
