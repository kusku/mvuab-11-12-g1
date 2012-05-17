
#pragma once

#ifndef _A_STAR_H
#define _A_STAR_H

#include <list>

#include "Base.h"

class CWayPoint;

class GraphNode
{
	private:
		float		m_G;
		float		m_H;
		CWayPoint*	m_WP;
		GraphNode*	m_ParentNode;

	public:
		GraphNode();
		~GraphNode();

		//Gets
		float		GetG			() const		{ return m_G; }
		float		GetH			() const		{ return m_H; }
		CWayPoint*	GetWP			() const		{ return m_WP; }
		float		GetF			() const		{ return m_G + m_H; }
		GraphNode*	GetParentNode	()				{ return m_ParentNode; }

		//Sets
		void		SetG			(float val)			{ m_G = val; }
		void		SetH			(float val)			{ m_H = val; }
		void		SetWP			(CWayPoint* val)	{ m_WP = val; }
		void		SetParentNode	(GraphNode* val)		{ m_ParentNode = val; }
		
		bool		CompareNode		(GraphNode* val);
		bool		operator==		(const GraphNode& b);
};

class GraphNodeList
{
	typedef std::list<GraphNode*> GraphList;
	typedef GraphList::iterator GraphListIt;

	private:
		GraphList	m_List;

		//Sort
		struct SortGraphNode : public std::binary_function<const GraphNode*, const GraphNode*, bool>
		{
			bool operator()(const GraphNode* left, const GraphNode* right) const
			{
				return (left->GetF() < right->GetF());
			}
		};
		SortGraphNode			m_SortGraphNode;

	public:
		GraphNodeList();
		~GraphNodeList();

		GraphNode*				PopFront	();

		void					PushBack	(GraphNode* node);

		int32					IndexOf		(GraphNode* node);

		void					Sort		();

		bool					IsEmpty		();

		GraphNode*				At			( uint32 index );

		bool					RemoveAt	( uint32 index );
};


#endif