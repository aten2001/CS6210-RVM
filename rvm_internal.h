#ifndef RVMINTERNAL_H
#define RVMINTERNAL_H


#include <map>
#include <list>
#include <fstream>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

typedef int trans_t;

typedef struct region_t{
	void* SegBase;
	int Offset;
	int Size;
	char* Undo;
} region;


typedef struct segment_t{
        const char* SegName;
	void* SegBase;
	size_t SegSize;
	int locked;
	trans_t TransactionId;
} segment;

typedef struct transaction_t{
	trans_t TransactionId;
	int No_Segments;
	void** SegBases;
	list<region*> Region_List;
} transaction;
	

typedef struct rvmInternal{
	char* Directory;
	list<segment*> Segment_Map;
	trans_t Working_Transaction;
	list<transaction*> Transaction_List;
}rvm_type;


typedef rvm_type * rvm_t;
#endif
