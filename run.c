#include <sys/types.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include "run.h"
#include "util.h"
//start
void *base = 0;
p_meta head=NULL;
p_meta tail=NULL;
struct rlimit limit;

p_meta find_meta(p_meta *last, size_t size) {
  p_meta index = (p_meta)malloc(sizeof(struct metadata));
  p_meta result = (p_meta)malloc(sizeof(struct metadata));

  switch(fit_flag){
    case FIRST_FIT:
    {
      //FIRST FIT CODE
			while(index!=NULL)
			{
				if(index->free)
				{
					if(index->size >= size) break;
				}
				*last=index;
				index=index->next;
			}
			result=index;
    }
    break;

    case BEST_FIT:
    {
      //BEST_FIT CODE
			p_meta best=index;
			while(index!=NULL)
			{
				if(index->free)
				{
					if(index->size >= size)
					{
						if(best->size >= index->size) best=index;
					}
				}
				*last=index;
				index=index->next;
			}
			result=best;
    }
    break;

    case WORST_FIT:
    {
      //WORST_FIT CODE
			p_meta worst=index;
			while(index!=NULL)
			{
				if(index->free)
				{
					if(index->size >= size)
					{
						if(worst->size <= index->size) worst=index;
					}
				}
				*last=index;
				index=index->next;
			}
			result=worst;
    }
    break;

  }
  return result;
}

void init_meta(p_meta* new, size_t size)
{
	*new=sbrk(META_SIZE);
	(*new)->free=0;
	(*new)->size=size;
	head=*new;
	(*new)->next=base;
	(*new)->prev=head;
}

void *m_malloc(size_t size) {
	size=size-(size+3)%4;
	p_meta new=(p_meta)malloc(sizeof(struct metadata));
	p_meta temp=(p_meta)malloc(sizeof(struct metadata));
	int len = size + META_SIZE;
	getrlimit(RLIMIT_DATA,&limit);
	if(head==NULL) init_meta(&new, size);
	if(new!=NULL)
	{
		if(new->size > len)
		{
			temp=new+len;
			new->next=temp;
			temp->prev=new;
			temp->free=1;
			temp->size=size-META_SIZE;
		}
		new->free=0;
		new->size=size;
	}
	else
	{
		new=sbrk(META_SIZE);
		new->prev=tail;
		new->next=base;
		new->size=size;
		new->free=0;
	}
	return new;
}

void m_free(void *ptr) {

}

void* m_realloc(void* ptr, size_t size)
{

}
