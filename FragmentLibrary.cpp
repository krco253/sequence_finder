/*---------------------------------
| File: FragmentLibrary.cpp
| Class: FragmentLibrary
-----------------------------------*/

#include "FragmentLibrary.h"

/*---------------------------------------------
| Constructors and Destructors
------------------------------------------------*/

FragmentLibrary::FragmentLibrary()
{ 
	
}

FragmentLibrary::FragmentLibrary(std::list<ContextFragment> &substrings, ContextFragment &new_frag)
{
	substrings.push_back(new_frag);
} 

FragmentLibrary::FragmentLibrary(const FragmentLibrary &copy)
{
	this->context_substrings= copy.context_substrings;
}

FragmentLibrary::~FragmentLibrary() { }


/*---------------------------------------------
| annotated_print
------------------------------------------------*/
void FragmentLibrary::annotated_print()
{
	std::list<ContextFragment>::iterator it; 
	for(it = (this->context_substrings).begin(); it != (this->context_substrings).end(); it++)
	{
		it->print_query();
	}
}

/*---------------------------------------------
| empty()
------------------------------------------------*/
bool FragmentLibrary::empty() 
{
	//returns true if the FragmentLibrary is empty
	//else returns false
	if(this->context_substrings.empty())
		return true;
	else 
		return false;	
}

/*---------------------------------------------
| erase_substring
-----------------------------------------------*/
std::list<ContextFragment>::iterator FragmentLibrary::erase_substring(std::list<ContextFragment>::iterator &index)
{
	auto next = std::next(index,1);
	(this->context_substrings).erase(index);
	return next;
}

/*---------------------------------------------
| add_substring
------------------------------------------------*/

void FragmentLibrary::add_substring(const ContextFragment &new_frag)
{
	(this->context_substrings).push_back(new_frag);
}

/*---------------------------------------------
| consolidate_sequences
------------------------------------------------*/
void FragmentLibrary::consolidate_sequences(Dna5String &context_sequence)
{
	std::list<ContextFragment>::iterator it = (this->context_substrings).begin();
	std::list<ContextFragment>::iterator last =(this->context_substrings).end(); 
	auto nx = std::next(it,1);

	while  (nx != last)
	{
		if (((*nx).get_index(1,0) <= (*it).get_index(1,0)) && ((*it).get_index(1,0) <= (*nx).get_index(2,2)) && ((*nx).get_index(2,2) <= (*it).get_index(2,2)))
		{
			ContextFragment temp;
			temp = it->consolidate_frags((*nx), context_sequence);
			this->add_substring(temp);
			it = this->erase_substring(it);
			nx = this->erase_substring(nx);
		}
		else if(((*it).get_index(1,0) <= (*nx).get_index(1,0)) && ((*nx).get_index(1,0) <= (*it).get_index(2,2)) && ((*it).get_index(2,2) <= (*nx).get_index(2,2)))
		{
			ContextFragment temp;
			temp = it->consolidate_frags((*nx), context_sequence);
			this->add_substring(temp);
			it = this->erase_substring(it);
			nx = this->erase_substring(nx);
		}
		else
		{
			it++;
			nx++;
		}
			
	}
	
}

/*---------------------------------------------
| sort_by_index
------------------------------------------------*/

void FragmentLibrary::sort_by_index()
{
	(this->context_substrings).sort();	
}
/*----------------------------------------------
|	size()
------------------------------------------------*/
unsigned FragmentLibrary::size()
{
	unsigned size = (this->context_substrings).size();
	return size;
}
/*---------------------------------------------
| Overloaded Operators
------------------------------------------------*/
FragmentLibrary FragmentLibrary::operator=(const FragmentLibrary& other)
{	
	context_substrings = other.context_substrings;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const FragmentLibrary& sub_vec)
{
	for(std::list<ContextFragment>::const_iterator it = (sub_vec.context_substrings).begin(); it != (sub_vec.context_substrings).end(); it++)
	{
		os << *it << std::endl; 
	}	
	return os;
}


