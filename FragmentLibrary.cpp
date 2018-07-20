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
	std::map<std::pair<unsigned,unsigned>, ContextFragment>::iterator it; 
	for(it = (this->context_substrings).begin(); it != (this->context_substrings).end(); it++)
	{
		(it->second).print_query();
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
| begin()
------------------------------------------------*/
std::map<std::pair<unsigned,unsigned>, ContextFragment>::iterator FragmentLibrary::begin()
{
	return (this->context_substrings).begin();
}
 /*---------------------------------------------
| insert
------------------------------------------------*/
void FragmentLibrary::insert(std::pair<unsigned,unsigned> &fragment_key, ContextFragment fragment_object)
{
	(this->context_substrings).insert(std::make_pair(fragment_key, fragment_object));
}
/*---------------------------------------------
| erase_substring
------------------------------------------------*/

void FragmentLibrary::erase_substring(std::pair<unsigned, unsigned> &fragment_key)
{
	(this->context_substrings).erase(fragment_key);
}

/*---------------------------------------------
| consolidate_sequences
------------------------------------------------*/
void FragmentLibrary::consolidate_sequences(Dna5String &context_sequence)
{
	std::map<std::pair<unsigned,unsigned>, ContextFragment>::iterator last = (this->context_substrings).end(); 
	last--;
	std::map<std::pair<unsigned,unsigned>, ContextFragment>::iterator it=(this->context_substrings).begin();	
	while(it != last)	
	{
		std::map<std::pair<unsigned,unsigned>, ContextFragment>::iterator nx = std::next(it, 1);

		if (it->second == nx->second)
		{	
			it = last;
			break;
		}
		else if ((((nx)->second).get_index(1,0) <= ((it)->second).get_index(1,0)) && (((it)->second).get_index(1,0) <= ((nx)->second).get_index(2,2)) && (((nx)->second).get_index(2,2) <= ((it)->second).get_index(2,2)))
		{
			ContextFragment temp;
			temp = (it->second).consolidate_frags((nx)->second, context_sequence);
			std::pair<unsigned, unsigned> temp_indices;
			std::pair<unsigned, unsigned> this_indices;
			std::pair<unsigned, unsigned> nx_indices;
			temp_indices = temp.flatten();
			this_indices = ((it)->second).flatten();
			nx_indices = ((nx)->second).flatten();
			this->erase_substring(this_indices);
			this->erase_substring(nx_indices);	
			this->insert(temp_indices, temp);
			it =  (this->context_substrings).find(temp_indices);
		}
		else if ((((it)->second).get_index(1,0) <= ((nx)->second).get_index(1,0)) && (((nx)->second).get_index(1,0) <= ((it)->second).get_index(2,2)) && (((it)->second).get_index(2,2) <= ((nx)->second).get_index(2,2)))
		{
			ContextFragment temp;
			temp = (it->second).consolidate_frags((nx)->second, context_sequence);
			std::pair<unsigned, unsigned> temp_indices;
			std::pair<unsigned, unsigned> this_indices;
			std::pair<unsigned, unsigned> nx_indices;
			temp_indices = temp.flatten();
			this_indices = ((it)->second).flatten();
			nx_indices = ((nx)->second).flatten();
			this->erase_substring(this_indices);
			this->erase_substring(nx_indices);
			this->insert(temp_indices, temp);
			it = (this->context_substrings).find(temp_indices);
		}
		else
			it++;
		
	
	} 
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
	std::map<std::pair<unsigned,unsigned>, ContextFragment>::const_iterator it = (sub_vec.context_substrings).begin();
	std::map<std::pair<unsigned,unsigned>, ContextFragment>::const_iterator last = (sub_vec.context_substrings).end();
	while(it != last)
	{
		os << it->second << std::endl; 
		it++;
	}	
	return os;
}


