
#include "AStar.h"
#include "WayPoint.h"

GraphNode::GraphNode()
	: m_G(0.0f)
	, m_H(0.0f)
	, m_WP(NULL)
	, m_ParentNode(NULL)
{

}

GraphNode::~GraphNode()
{

}

bool GraphNode::operator==( const GraphNode& b )
{
	if(this->GetWP() == NULL || b.GetWP() == NULL)
	{
		return false;
	}

	return (this->GetWP()->GetName().compare(b.GetWP()->GetName()) == 0);
}

bool GraphNode::CompareNode( GraphNode* val )
{
	assert(val);

	return (this->GetWP()->GetName().compare(val->GetWP()->GetName()) == 0);
}

GraphNodeList::GraphNodeList()
{

}

GraphNodeList::~GraphNodeList()
{
	GraphListIt it = m_List.begin();
	GraphListIt itEnd = m_List.end();

	for (uint32 i = 0; it != itEnd; ++it, ++i)
	{
		GraphNode* node = (*it);

		CHECKED_DELETE(node);
	}

	m_List.clear();
}

GraphNode* GraphNodeList::PopFront()
{
	if(m_List.size() == 0)
	{
		return NULL;
	}

	GraphNode* node = (*m_List.begin());
	m_List.pop_front();

	return node;
}

void GraphNodeList::Sort()
{
	m_List.sort(m_SortGraphNode);
}

void GraphNodeList::PushBack( GraphNode* node )
{
	m_List.push_back(node);
}

bool GraphNodeList::IsEmpty()
{
	return (m_List.size() == 0);
}

int32 GraphNodeList::IndexOf( GraphNode* node )
{
	GraphListIt it = m_List.begin();
	GraphListIt itEnd = m_List.end();

	for (uint32 i = 0; it != itEnd; ++it, ++i)
	{
		if((*it)->CompareNode(node))
		{
			return i;
		}
	}

	return -1;
}

GraphNode* GraphNodeList::At( uint32 index )
{
	GraphNode* node = NULL;

	GraphListIt it = m_List.begin();
	GraphListIt itEnd = m_List.end();

	for (uint32 i = 0; it != itEnd && i < index; ++it, ++i);

	if(it != itEnd)
	{
		node = (*it);
	}

	return node;
}

bool GraphNodeList::RemoveAt( uint32 index )
{
	GraphListIt it = m_List.begin();
	GraphListIt itEnd = m_List.end();

	for (uint32 i = 0; it != itEnd && i < index; ++it, ++i);

	if(it != itEnd)
	{
		GraphNode* gn = (*it);
		m_List.erase(it);

		CHECKED_DELETE(gn);

		return true;
	}

	return false;
}