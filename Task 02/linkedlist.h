#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct LinkedListNode 
{
	void* pData;
	struct LinkedListNode* pNext;

} LinkedListNode;


typedef struct LinkedList 
{
	LinkedListNode* pHead;
	LinkedListNode* pTail;
	int len;
} LinkedList;

typedef void (*listFunc)(void*);

LinkedList* createLinkedList();
void insertLast(LinkedList* list, void* pData);
void freeLinkedList(LinkedList* list, listFunc funcPtr);
void printLinkedList(LinkedList* list, listFunc funcPtr);
void debugLinkedList(LinkedList* list, char* prefix);

#endif
