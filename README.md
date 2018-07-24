# sequence_finder

sequence_finder is a C++ project that utilizes the Seqan template library to find approximate matches for DNA query sequences. 

## Getting Started



### Prerequisites
- Can only be used on Linux OS
- To use sequence_finder, Seqan 2.4.0 is required. For instructions on how to download the Seqan library, visit: https://seqan.readthedocs.io/en/seqan-v2.4.0/Infrastructure/Use/CustomBuildSystem.html 

### Installing

1. Download this repository:
```
git clone https://github.com/krco253/sequence_finder.git
```
2. Compile with gcc, with the following flags:  g++ -isystem <path/to/seqan>
-std=c++14 -O3 -DNDEBUG -W -Wall -pedantic -fopenmp
-lpthread -ggdb -o sequence_finder sequence_finder.cpp FragmentLibrary.cpp ContextFragment.cpp IndexedFragment.cpp -lrt 
-Wno-unused-parameter -Wno-sequence-point

3. View sequence_finder help page like so: 
```
./sequence_finder -h
```

### Usage
1. This program must be given the path to a .fasta file to search in order to work.
2. By default, this program looks for MoTeR relics and the surrounding sequence. To use the program in its default state: 
```
./sequence_finder -s <path/to/your/FASTA/file>
```
3. To use this program to search for a short string: 
```
./sequence_finder -s <path/to/your/FASTA/file> -t <"your DNA string"> -e <max number of errors>
```
This program only supports up to 4 errors.
4. For other commands, check out the help page.

## Authors

* **Kelsey Cole** 


## Acknowledgments

* Used Seqan C++ Template Library, version 2.4.0. Check out their docs: https://seqan.readthedocs.io/en/seqan-v2.4.0/ 
