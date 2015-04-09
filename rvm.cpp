#include "rvm.h"

char * redoLogFile;  
map<int,rvm_t> transIdToRvmMap;
map<void*,const char *> segBaseToSegNameMap;

char* getAbsFilePath(char * directory,const char * segName)
{
      char* filePath = (char*)malloc(strlen(directory)+strlen(segName)+strlen("/")+1);
      strcpy(filePath,directory);
      strcat(filePath,"/");
      strcat(filePath,segName);
      return filePath;
}

rvm_t rvm_init(const char *directory)
{
        if(mkdir(directory,S_IRWXU|S_IRWXG|S_IRWXO)==-1)
        {
            if(errno != EEXIST)
            {
            	printf("ERROR: Unable to create a directory.");
                exit(0);
            }            
        }
        rvm_t rvm = new rvm_type;
        if(rvm == NULL)
        {
        	printf("ERROR: Can't allocate memory for RVM.");
                exit(0);
        }
        rvm->Directory = (char*)malloc(strlen(directory)+1);  
	if(rvm->Directory == NULL)
        {
        	printf("ERROR: Can't allocate memory for RVM Directory");
                exit(0);
        }
        rvm->Working_Transaction = 1;
        strncpy(rvm->Directory,directory,strlen(directory));
	redoLogFile = getAbsFilePath(rvm->Directory, "redo.log");
	return rvm;
}

void* rvm_map(rvm_t rvm, const char *segname, int size_to_create)
{
      	FILE* fd;
      	void* SegBase=NULL;
   	/*if ( !exists(rvm->Directory))
	{
		printf("ERROR: No Backing Directory found. Run rvm_init first.\n");
		exit(1);
	}*/
       	for(list<segment*>::iterator segmentListItr = rvm->Segment_Map.begin(); segmentListItr!=rvm->Segment_Map.end(); segmentListItr++)
       	{
                if((*segmentListItr)->SegName==segname)
                {
                  	printf("ERROR: Segment Already Mapped. Attempt to map the same segment detected.\n"); 
                  	exit(1);
                }
         }
	
      	char* filePath = getAbsFilePath(rvm->Directory,segname); 
      	fd = fopen(filePath,"ab+"); 
      	if (fd == NULL) 
      	{
      		printf("ERROR: Unable to open file to read %d  %s \n",errno,filePath);
      		exit(1);
      	}
      	fseek(fd,0,SEEK_END); 
     	long fsize = ftell(fd);
     	if(size_to_create < fsize)
	{
      		size_to_create=fsize;
	}
	fseek(fd,0,SEEK_SET);
     	SegBase = (char*)malloc(size_to_create);
	if(SegBase == NULL)
	{
		printf("Failure in allocation of memory for segBase in rvm_map.\n");
		exit(1);
	}
     	if(fsize!=0)
	{
     		fread(SegBase,fsize,1,fd);
	} 
     	fclose(fd);
      	free(filePath);
      	segBaseToSegNameMap[SegBase]=segname;
	segment* newSegment = (segment*)malloc(sizeof(segment));
        if(NULL == newSegment)
        {
        	printf("Memory allocation failed at __createNewSegmentNode__\n");
                exit(0);
        }
        newSegment->SegName = segname;
        newSegment->SegBase = SegBase;
        newSegment->SegSize = size_to_create;
        newSegment->isUnderTransaction = 0;
        newSegment->TransactionId = 0;

      	rvm->Segment_Map.push_back(newSegment);
      	return SegBase;
}
