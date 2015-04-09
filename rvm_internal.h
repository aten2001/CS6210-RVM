#ifndef RVMINTERNAL_H
#define RVMINTERNAL_H


#include <errno.h>
#include <map>
#include <list>
#include <fstream>
#include <sys/stat.h>
#include <errno.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <fstream>

using namespace std;

typedef int trans_t;

typedef struct log_t
{
	trans_t TransactionId;
	char* SegName;
	int Offset;
	int Size;
        char* Data;

}log;

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
	int isUnderTransaction;
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
