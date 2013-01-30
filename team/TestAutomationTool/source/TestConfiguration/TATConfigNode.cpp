// [TERMS&CONDITIONS]

#include "TestConfiguration/TATConfigNode.h"


namespace Kinesis
{
namespace TestAutomationTool
{
namespace Backend
{

//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |  ATTRIBUTES INITIALIZATION |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################



//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |       CONSTRUCTORS		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

TATConfigNode::TATConfigNode() : m_type(ETATConfigNodeType::E_ROOT)
{
}


//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |		    METHODS			 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

TATNode::TNodeCollection TATConfigNode::GetChildrenByType(const ETATConfigNodeType& type) const
{
    TNodeCollection foundChildren;
    TNodeCollection::const_iterator it = m_children.begin();

    for(; it != m_children.end(); ++it)
        if(dynamic_cast<TATConfigNode*>(it->second)->GetType() == type)
            foundChildren.insert( TNodeNamePair(it->first, it->second) );

    return foundChildren;
}


//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |         PROPERTIES		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

ETATConfigNodeType TATConfigNode::GetType() const
{
    return m_type;
}

void TATConfigNode::SetType(const ETATConfigNodeType& type)
{
    m_type = type;
}


} //namespace Backend
} //namespace TestAutomationTool
} //namespace Kinesis