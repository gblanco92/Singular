/****************************************
*  Computer Algebra System SINGULAR     *
****************************************/
/* $Id: mminit.cc,v 1.12 1998-05-27 17:14:09 Singular Exp $ */
/*
* ABSTRACT: init of memory management
*/

#define _POSIX_SOURCE 1

#include <stdlib.h>
#include <string.h>
#define __USE_MISC
#include <unistd.h>
#include <sys/types.h>
#include "mod2.h"
#include "mmemory.h"
#include "mmprivat.h"
extern "C" { /* begin of "C" */
#include <gmp.h>

#ifdef ALIGN_8
mcb mm_normList[MAXLIST]=
{ NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL};
;
mcb mm_tmpList[MAXLIST]=
{ NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL};
#ifdef MM_STAT
int mm_active[MAXLIST+1]=
{ 0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0};
int mm_max[MAXLIST+2]=
{ 0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0};
#endif
#else
mcb mm_normList[MAXLIST]=
{ NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL};
mcb mm_tmpList[MAXLIST]=
{ NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL,NULL,NULL,
  NULL,NULL};
#ifdef MM_STAT
int mm_active[MAXLIST+1]=
{ 0,0,0,0,
  0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0};
int mm_max[MAXLIST+2]=
{ 0,0,0,0,
  0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0};
#endif
#endif


mcb *mm_theList=mm_normList;

#ifdef MPOLYPART
void * mm_maxAddr=NULL;
void * mm_minAddr=NULL;
mcb mm_spec=NULL;
void *mm_spec_part=NULL;
#endif

#ifdef MDEBUG

DBMCB mm_theDBused={NULL,NULL,0,0,NULL,0,0,NULL};
DBMCB mm_theDBfree={NULL,NULL,0,0,NULL,0,0,NULL};
DBMCB mm_tmpDBused={NULL,NULL,0,0,NULL,0,0,NULL};
DBMCB mm_tmpDBfree={NULL,NULL,0,0,NULL,0,0,NULL};
DBMCB mm_normDBused={NULL,NULL,0,0,NULL,0,0,NULL};
DBMCB mm_normDBfree={NULL,NULL,0,0,NULL,0,0,NULL};
void * mm_maxAddr=NULL;
void * mm_minAddr=NULL;

#endif /* MDEBUG */

status_t mm_status = MM_NORMAL;

int mm_bytesUsed=0;
int mm_bytesReal=0;
int mm_bytesTmp=0;
int mm_bytesNorm=0;

int mm_specIndex=0;
size_t mm_specSize = mm_mcbSizes[0];

int mm_printMark=102400;
} /* end of "C" */

void* operator new ( size_t size )
{
#ifdef MDEBUG
  return mmDBAlloc( size, "new",0);
#else
  return AllocL( size );
#endif
}

void operator delete ( void* block )
{
#ifdef MDEBUG
  mmDBFree( block, "delete",0);
#else
  FreeL( block );
#endif
}

#ifndef HAVE_SMALLGMP
#ifdef MDEBUG
void * mgAllocBlock( size_t t)
{
  return mmDBAllocBlock(t,"gmp",0);
}
void mgFreeBlock( void* a, size_t t)
{
  mmDBFreeBlock(a,t,"gmp",0);
}
void * mgReallocBlock( void* a, size_t t1, size_t t2)
{
  return mmDBReallocBlock(a,t1,t2,"gmp",0);
}
#endif
#endif

#ifdef HAVE_SBRK
#include <unistd.h>
unsigned long mm_SbrkInit = (unsigned long)sbrk(0);
#endif

int mmInit();
static int mmIsInitialized=mmInit();

int mmInit( void )
{
#ifndef HAVE_SMALLGMP
  if(mmIsInitialized==0)
  {
#ifndef MDEBUG
    mp_set_memory_functions(mmAllocBlock,mmReallocBlock,mmFreeBlock);
#else
    mp_set_memory_functions(mgAllocBlock,mgReallocBlock,mgFreeBlock);
#endif
  }
#endif
  mmIsInitialized=1;
  return 1;
}
