#include <stdio.h>
#include "linkedlist.h"
#include <stdlib.h>
#include <assert.h>

LinkedList* createLinkedList()
{
	LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));

	list->pHead = NULL;
	list->pTail = NULL;
	list->len = 0;

	return list;
}

void insertLast(LinkedList* list, void* pData)
{
	LinkedListNode* pNew = (LinkedListNode*) malloc(sizeof(LinkedListNode));
	pNew->pData = pData;
	pNew->pNext = NULL;

	if (list->pTail != NULL)
	{
		list->pTail->pNext = pNew;
	}

	list->pTail = pNew;


	if (list->pHead == NULL)
	{
		list->pHead = pNew;
	}

	(list->len)++;	
}

void freeLinkedList(LinkedList* list, listFunc funcPtr)
{
	LinkedListNode* pTemp = NULL;
	LinkedListNode* pCur = list->pHead;
	while (pCur != NULL)
	{
		/* Clean the data inside a node */
		(*funcPtr)(pCur->pData);

		
		pTemp = pCur;
		pCur = pCur->pNext;
		
		/* free the previos node */
		free(pTemp);
		
		(list->len)--;		
	}

	
	list->pHead = NULL;
	list->pTail = NULL;
	assert(list->len == 0);	
	
	free(list);
}

void printLinkedList(LinkedList* list, listFunc funcPtr)
{
	LinkedListNode* pCur = list->pHead;
	while (pCur != NULL)
	{
		(*funcPtr)(pCur->pData);
		pCur = pCur->pNext;
	}
}


