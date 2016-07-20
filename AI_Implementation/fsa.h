//
//
// Fixed allocator class
// Copyright 2001 Justin Heyes-Jones
//
// Description:
//
// A constant time 0(1) memory manager for objects of a specified type.
// Type is specified using a template class.
// Using GetFirst and GetNext it is possible to iterate through the elements
// one by one, and this would be the most common use for the class.
// I would suggest using this class when you want 0(1) add and delete and you don't do
// much searching, which would be 0(n). Structures such as binary trees can be used
// used instead to get 0(logn) access time.
//
//

#ifndef _FSA_H_
#define _FSA_H_

#include <string.h>
#include <stdio.h>

template<class USER_TYPE> class FixedSizeAllocator {
public:
	// CONSTANTS
	enum { FSA_DEFAULT_SIZE = 100 };
	struct FSA_ELEMENT {
		USER_TYPE UserType;
		FSA_ELEMENT *pPrev;
		FSA_ELEMENT *pNext;
	};
	// METHODS
	FixedSizeAllocator(unsigned int MaxElements = FSA_DEFAULT_SIZE) :
		m_pFirstUsed(NULL),
		m_maxElements(MaxElements) {
		// allocate enough memory for the max num of elements
		char *pMem = new char[m_MaxElements * sizeof(FSA_ELEMENT)];
		m_pMemory = (FSA_ELEMENT *)pMem;
		// set the free list first pointer
		m_pFirstFree = m_pMemory;
		// clear the memory
		memset(m_pMemory, 0, sizeof(FSA_ELEMENT) * m_MaxElements);
		// point at the first element
		FSA_ELEMENT *pMemory = m_pFirstFree;
		// set the double linked free list
		for (unsigned int i = 0; i < m_MaxElements; i++) {
			pElement->pPrev = pElement - 1;
			pElement->pNext = pElement + 1;
			pElement++;
		}
		// first element should have a null previous
		m_pFirstFree->pPrev = NULL;
		// last one shouldn't have another after it
		(pElement - 1)->pNext = NULL;
	}
	// FREE UP DAT MEMORY
	~FixedSizeAllocator() { delete[](char*)m_pMemory; }
	// Allocate a new USER_TYPE and return a pointer to it.
	USER_TYPE *alloc() {
		FSA_ELEMENT *pNewNode = NULL;
		if (!m_pFirstFree) return NULL;
		else {
			pNewNode = m_pFirstFree;
			m_pFirstFree = pNewNode->pNext;
			// if the new node points to another free node then change that nodes prev free pointer
			if (pNewNode->pNext) pNewNode->pNext->pPrev = NULL;
			pNewNode->pPrev = NULL; // allocated node always first in list
			if (m_pFirstUsed == NULL) {
				pNewNode->pNext = NULL; // no other nodes
			}
			else {
				m_pFirstUsed->pPrev = pNewNode; // insert this at the head of the used list
				pNewNode->pNext = m_pFirstUsed;
			}
			m_pFirstUsed = pNewNode;
		}
		return reinterpret_cast<USER_TYPE*>(pNewNode);
	}

	// free the given user type
	void free(USER_TYPE *user_data) {
		FSA_ELEMENT *pNode = reinterpret_cast<FSA_ELEMENT*>(user_data);
		if (pNode->pPrev) {
			pNode->pPrev->pNext = pNode->pNext;
		}
		else {
			m_pFirstUsed = pNode->pNext;
		}
		if (pNode->pNext) {
			pNode->pNext->pPrev = pNode->pPrev;
		}
		//add to free list
		if (m_pFirstFree == NULL) {
			m_pFirstFree = pNode;
			pNode->pPrev = NULL;
			pNode->pNext = NULL;
		}
		else {
			m_pFirstFree->pPrev = pNode;
			pNode->pNext = m_pFirstFree;
			m_pFirstFree = pNode;
		}
	}

	void Debug() {
		printf("Free list ");
		FSA_ELEMENT *p = m_pFirstFree;
		while (p) {
			printf("%x!%x ", p->pPrev, p->pNext);
			p = p->pNext;
		}
		printf("\n");
		printf("Used list ");
		p = m_pFirstUsed;
		while (p) {
			printf("%x!%x ", p->pPrev, p->pNext);
			p = p->pNext;
		}
		printf("\n");
	}

	// iterators
	USER_TYPE *GetFirst() { return reinterpret_cast<USER_TYPE*>(m_pFirstUsed); }
	USER_TYPE *GetNext() { return reinterpret_cast<USER_TYPE*>((reinterpret_cast<FSA_ELEMENT*>(node))->pNext); }
private:
	FSA_ELEMENT *m_pFirstFree;
	FSA_ELEMENT *m_pFirstUsed;
	unsigned int *m_MaxElements;
	FSA_ELEMENT *m_pMemory;
};

#endif // !_FSA_H_