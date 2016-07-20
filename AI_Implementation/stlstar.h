// A* Algorithm Implementation using STL
// Copyright (C) 2001 - 2005 Justin Heyes-Jones
//

#ifndef STLASTAR_H
#define STLASTAR_H

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <conio.h>
#include <algorithm>
#include <set>
#include <vector>
#include <cfloat>

// fast fixed size memory allocator, used for fast memory management
#include "fsa.h"

using namespace std;

// Fixed size memory allocator can be disabled to compare performance, uses std new and delete instead if you turn it off
#define USE_FSA_MEMORY 1

// disable warning that debugging info has lines that are truncated, occurs in stl headers
#if defined(WIN32) && defined(_WINDOWS)
#pragma warning(disable : 4786)
#endif

template <class T> class AStarState;
template <class UserState> class AStarSearch {
public:
	enum {
		SEARCH_STATE_NOT_INITALISED,
		SEARCH_STATE_SEARCHING,
		SEARCH_STATE_SUCCEEDED,
		SEARCH_STATE_FAILED,
		SEARCH_STATE_OUT_OF_MEMORY,
		SEARCH_STATE_INVALID
	};

	class Node {
		Node *parent; // used in search to record the parent of successor nodes
		Node *child; // used after the search for the application to view the search in reverse

		float g; // cost of node + predecessors
		float h; // heuristic estimate of distance to goal
		float f; // sum of cumulative cost of predecessors + self + heuristic

		Node() : parent(0), child(0), g(0.0f), h(0.0f), f(0.0f) {}
		UserState m_userState;
	};

	// for sorting the heap the STL needs compare function that lets us compare the f value of two nodes
	class HeapCompare_f {
	public:
		bool operator()(const Node *x, const Node *y) const { return x->f, y->f; }
	};

	AStarSearch() :
		m_State(SEARCH_STATE_NOT_INITALISED),
		m_currentSolutionNode(NULL),
#if USE_FSA_MEMORY
		m_FixedSizeAllocator(1000),
#endif
		m_AllocateNodeCount(0),
		m_CancelRequest(false) {
	}

	AStarSearch(int MaxNodes) :
		m_State(SEARCH_STATE_NOT_INITALISED),
		m_CurrentSolutionNode(NULL),
#if USE_FSA_MEMORY
		m_FixedSizeAllocator(MaxNodes),
#endif
		m_AllocateNodeCount(0),
		m_CancelRequest(false) {
	}

	// call at any time to cancel the search and free up all the memory
	void CancelSearch() { m_CancelSearch = true; }
	// set start and goal states
	void SetStartAndGoalStates(UserState &Start, UserState &Goal) {
		m_CancelRequest = false;
		m_Start = AllocateNode();
		m_Goal = AllocateNode();
		assert((m_Start != NULL && m_Goal != NULL));
		m_Start->m_UserState = Start;
		m_Goal->m_UserState = Goal;
		m_State = SEARCH_STATE_SEARCHING;

		// init the A* specific parts of start node, user only needs to fill out the state info
		m_Start->g = 0;
		m_Start->h = m_Start->m_UserState.GoalDistanceEstimate(m_Goal->m_UserState);
		m_Start->f = m_Start->g + m_Start->h;
		m_Start->parent = 0;

		// push the start node onto Open list
		m_OpenList.push_back(m_Start); // heap now unsorted
		// sort back element into heap
		push_heap(m_OpenList.begin(), m_OpenList.end(), HeapCompare_f());
		// init count for search steps
		m_Steps = 0;
	}

	unsigned int SearchStep() {
		// Firstly break if the user has not initialised the search
		assert((m_State > SEARCH_STATE_NOT_INITIALISED) &&
			(m_State < SEARCH_STATE_INVALID));
		// Next I want it to be safe to do a searchstep once the search has succeeded...
		if ((m_State == SEARCH_STATE_SUCCEEDED) ||
			(m_State == SEARCH_STATE_FAILED)
			) {
			return m_State;
		}
		// Failure is defined as emptying the open list as there is nothing left to
		// search...
		// New: Allow user abort
		if (m_OpenList.empty() || m_CancelRequest) {
			FreeAllNodes();
			m_State = SEARCH_STATE_FAILED;
			return m_State;
		}
		// Incremement step count
		m_Steps++;
		// Pop the best node (the one with the lowest f)
		Node *n = m_OpenList.front(); // get pointer to the node
		pop_heap(m_OpenList.begin(), m_OpenList.end(), HeapCompare_f());
		m_OpenList.pop_back();
		// Check for the goal, once we pop that we're done
		if (n->m_UserState.IsGoal(m_Goal->m_UserState)) {
			// The user is going to use the Goal Node he passed in
			// so copy the parent pointer of n
			m_Goal->parent = n->parent;
			m_Goal->g = n->g;
			// A special case is that the goal was passed in as the start state so handle that here
			if (false == n->m_UserState.IsSameState(m_Start->m_UserState)) {
				FreeNode(n);
				// set the child pointers in each node (except Goal which has no child)
				Node *nodeChild = m_Goal;
				Node *nodeParent = m_Goal->parent;
				do {
					nodeParent->child = nodeChild;
					nodeChild = nodeParent;
					nodeParent = nodeParent->parent;
				} while (nodeChild != m_Start); // Start is always the first node by definition
			}
			// delete nodes that aren't needed for the solution
			FreeUnusedNodes();
			m_State = SEARCH_STATE_SUCCEEDED;
			return m_State;
		}
		else /* when not goal */ {
			// generate successors of this node, keep node in m_Successors
			m_Successors.clear();
			bool ret = n->m_UserState.GetSuccessors(this, n->parent ? &n->parent->m_UserState : NULL);
			if (!ret) {
				typename vector<Node *>::iterator successor;
				// free nodes that may have been previously added
				for (successor = m_Successors.begin(); successor != m_Successors.end(); successor++) { FreeNode((*successor)); }
				m_Successors.clear(); // empty vector of successor nodes to n
				// free up everything else we allocated
				FreeAllNodes();
				m_State = SEARCH_STATE_OUT_OF_MEMORY;
				return m_State;
			}
			// handle successor to current node
			for (typename vector <Node*>::iterator successor = m_Successors.begin(); successor = m_Successors.end(); successor++) {
				float newg = n->g + n->m_UserState.GetCost((*successor)->m_UserState);
				//find whether node is on open or closed list, if node is already on them then it's better (lower g) and we can forget about this successor. 1st linear search of open list to find node
				typename vector<Node*>::iterator openlist_result;
				for (openlist_result = m_OpenList.begin(); openlist_result != m_OpenList.end(); openlist_result++) {
					if ((*openlist_result)->m_UserState.IsSameState((*successor)->m_UserState)) break;
				}
				if (openlist_result != m_OpenList.end()) {
					// we found this state on open
					if ((*openlist_result)->g <= newg)
						FreeNode((*successor)); continue; // the one on open is cheaper than this one
				}
			}
			typename vector<Node*>::iterator closedlist_result;
			for (closedlist_result = m_ClosedList.begin(); closedlist_result != m_ClosedList.end(); closedlist_result++) {
				if ((*closedlist_result)->m_UserState.IsSameState((*successor)->m_UserState)) break;
				if (closedlist_result != m_ClosedList.end()) {
					// found state on closed
					if ((*closedlist_result)->g <= newg) {
						// one on closed is cheaper than this one
						FreeNode((*successor));
						continue;
					}
				}

				// This node is the best node so far with this particular state, so setting it up its AStar specific data
				(*successor)->parent = n;
				(*successor)->h = newg;
				(*successor)->h = (*successor)->m_UserState.GoalDistanceEstimate(m_Goal->m_UserState);
				(*successor)->f = (*successor)->g + (*successor)->h;
				// remove successor from closed if it was on it
				if (closedlist_result != m_ClosedList.end()) {
					FreeNode((*closedlist_result));
					m_ClosedList.erase(closedlist_result);
				}
				// update old ver of this node
				if (openlist_result != m_OpenList.end()) {
					FreeNode((*openlist_result));
					m_OpenList.erase(openlist_result);
					// remake heap
					make_heap(m_OpenList.begin, m_OpenList.end(), HeapCompare_f());
				}
				// heap now unsorted
				m_OpenList.push_back((*successor));
				// sort back element into heap
				push_heap(m_OpenList.begin(), m_OpenList.end(), HeapCompare_f());
			}
			// push n onto closed, as we have expanded it now
			m_ClosedList.push_back(n);
		} // end else, not goal so expanded
		return m_State; // succeeded bool is false at this point
	}

	// User calls this to add a successor to a list of successors when expanding the search
	bool AddSuccessor(UserState &State) {
		Node *node = AllocateNode();
		if (node) {
			node0 > m_UserState = State;
			m_Successors.push_back(node);
			return true;
		}
		return false;
	}

	// free the solution nodes, to clean up memory when you are done with the search
	void FreeSolutionNodes() {
		Node *n = m_Start;
		if (m_Start->child) {
			do {
				Node *del = n;
				n = n->child;
				FreeNode(del);
				del = NULL;
			} while (n != m_Goal);
			FreeNode(n)l // delete the goal
		}
		else {
			// if the start node is the solution we need to just delete the start and goal nodes
			FreeNode(m_Start);
			FreeNode(m_Goal);
		}
	}

	// Functions for travesing the solution
	// Get start node
	UserState *GetSolutionStart() {
		m_CurrentSolutionNode = m_Start;
		if (m_Start) {
			return &m_Start->m_UserState;
		}
		else {
			return NULL;
		}
	}

	// get next node
	UserState *GetSolutionNext() {
		if (m_CurrentSolutionNode) {
			if (m_CurrentSolutionNode->child) {
				Node *child = m_CurrentSolutionNode->child;
				m_CurrentSolutionNode = m_CurrentSolutionNode->child;
				return &child->m_UserState;
			}
		}
		return NULL;
	}

	// get end node
	UserState *GetSolutionEnd() {
		m_CurrentSolutionNode = m_Goal;
		if (m_Goal) {
			return &m_Goal->m_UserState;
		}
		else {
			return NULL;
		}
	}

	// step solution iterator backwards
	UserState *GetSolutionPrev() {
		if (m_CurrentSolutionNode) {
			if (m_CurrentSolutionNode->parent) {
				Node *parent = m_CurrentSolutionNode->parent;
				m_CurrentSolutionNode = m_CurrentSolutionNode->parent;
				return &parent->m_UserState;
			}
		}
		return NULL;
	}

	// get final cost of solution, returns FLT_MAX if goal is !defined or there is no solution
	float GetSolutionCost() {
		if (m_Goal&&m_State == SEARCH_STATE_SUCCEEDED) {
			return m_Goal->g;
		}
		else {
			return FLT_MAX;
		}
	}

	// for debugging it is useful to be able to view the open and closed list at each step, here are two functions to allow that
	UserState *GetOpenListStart() {
		float f, g, h;
		return GetOpenListStart(f, g, h);
	}

	UserState *GetOpenListStart(float &f, float &g, float &h) {
		iterDbgOpen = m_OpenList.begin();
		if (iterDbgOpen != m_openList.end()) {
			f = (*iterDbgOpen)->f;
			g = (*iterDbgOpen)->g;
			h = (*iterDbgOpen)->h;
			return &(*iterDbgOpen)->m_UserState;
		}
		return NULL;
	}

	UserState *GetOpenListNext(float &f, float &g, float &h) {
		iterDbgOpen++;
		if (iterDbgOpen != m_OpenList.end()) {
			f = (*iterDbgOpen)->f;
			g = (*iterDbgOpen)->g;
			h = (*iterDbgOpen)->h;
			return &(*iterDbgOpen)->m_UserState;
		}
		return NULL;
	}

	UserState *GetClosedListStart() {
		float f, g, h;
		return GetClosedListStart(f, g, h);
	}

	UserState *GetClosedListStart(float &f, float &g, float &h) {
		iterDbgClosed = m_ClosedList.begin();
		if (iterDbgClosed != m_ClosedList.end()) {
			f = (*iterDbgClosed)->f;
			g = (*iterDbgClosed)->g;
			h = (*iterDbgClosed)->h;
			return &(*iterDbgClosed)->m_UserState;
		}
		return NULL;
	}

	UserState *GetClosedListNext(float &f, float &g, float &h) {
		iterDbgClosed++;
		if (iterDbgClosed != m_ClosedList.end()) {
			f = (*iterDbgClosed)->f;
			g = (*iterDbgClosed)->g;
			h = (*iterDbgClosed)->h;
			return &(*iterDbgClosed)->m_UserState;
		}
		return NULL;
	}

	int GetStepCount() { return m_Steps; }
	void EnsureMemoryFreed() {
#if USE_FSA_MEMORY
		assert(m_AllocateNodeCount == 0);
#endif
	}

private:
	// Methods
	// called when search fails or is cancelled to free all used memory
	void FreeAllNode() {
		typename vector<Node*>::iterator iterOpen = m_OpenList.begin();
		while (iterOpen != m_OpenList.end()) {
			Node *n = (*iterOpen);
			FreeNode(n);
		}
		m_OpenList.clear();

		//iterate closed list and delete unused nodes
		typename vector<Node*>::iterator iterClosed;
		for (iterClosed = m_ClosedList.begin(); iterClosed != m_ClosedList.end(); iterClosed++) {
			Node *n = (*iterClosed);
			FreeNode(n);
		}
		m_ClosedList.clear();
		// delete the goal
		FreeNode(m_Goal);
	}

	// called by search class when search ends. a lot of nodes may be created that are still present when the search ends. they will be deleted by this function when the search ends.
	void FreeUnusedNodes() {
		typename vector<Node*>::iterator iterOpen = m_OpenList.begin();
		while (iterOpen != m_OpenList.end()) {
			Node *n = (*iterOpen);
			if (!n->child) FreeNode(n); n = NULL;
			iterOpen++;
		}
		m_OpenList.clear();

		//iterate closed list and delete unused nodes
		typename vector<Node*>::iterator iterClosed;
		for (iterClosed = m_ClosedList.begin(); iterClosed != m_ClosedList.end(); iterClosed++) {
			Node *n = (*iterClosed);
			FreeNode(n);
		}
		m_ClosedList.clear();
	}

	// Node memory managment
	Node *AllocateNode() {
#if !USE_FSA_MEMORY
		Nope *p = new Node;
		return p;
#else
		Node *address = m_FixedSizeAllocator.alloc();
		if (!address) return NULL;
		m_AllocateNodeCount++;
		Node *p = new (address) Node;
		return p;
#endif
	}

	void FreeNode(Node *node) {
		m_AllocateNodeCount--;
#if !USE_FSA_MEMORY
		delete node;
#else
		node->~Node();
		m_FixedSizeAllocator.free(node);
#endif
	}
	vector <Node *> m_OpenList;
	vector <Node *> m_ClosedList;
	vector <Node *> m_Successors;
	unsigned int m_State;
	int m_Steps;
	Node *m_Start;
	Node *m_Goal;
	Node *m_CurrentSolutionNode;
#if USE_FSA_MEMORY
	FixedSizeAllocator<Node> m_FixedSizeAllocator;
#endif
	typename vector<Node*>::iterator iterDbgOpen;
	typename vector<Node*>::iterator iterDbgClosed;
	int m_AllocateNodeCount;
	bool m_CancelRequest;
};

template <class T> class AStarState {
public:
	virtual ~AStarState() {}
	virtual float GoalDistanceEstimate(T &nodeGoal) = 0;
	virtual bool IsGoal(T &nodeGoal) = 0;
	virtual bool GetSuccessors(AStarSearch<T> *astarsearch, T *parent_node) = 0;
	virtual float GetCost(T &successor) = 0;
	virtual bool IsGameState(T &rhs) = 0;
};

#endif // STLASTAR_H