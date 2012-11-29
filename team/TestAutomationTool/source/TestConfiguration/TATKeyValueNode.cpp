// [TERMS&CONDITIONS]

#include "TestConfiguration/TATKeyValueNode.h"


namespace Kinesis
{
namespace TestAutomationTool
{
namespace Backend
{

//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |       CONSTRUCTORS		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

TATKeyValueNode::TATKeyValueNode() : m_strValue(wxT(""))
{
}
	
TATKeyValueNode::TATKeyValueNode(const wxString& strName, const ETATConfigNodeType& eType, const wxString& strValue) :
                                                                                                m_strValue(strValue)
{
    m_strName = strName;
    m_type = eType;
}

//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |         DESTRUCTOR		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

TATKeyValueNode::~TATKeyValueNode()
{
}


//##################=======================================================##################
//##################			 ____________________________			   ##################
//##################			|							 |			   ##################
//##################		    |         PROPERTIES		 |			   ##################
//##################		   /|							 |\			   ##################
//##################			 \/\/\/\/\/\/\/\/\/\/\/\/\/\/			   ##################
//##################													   ##################
//##################=======================================================##################

wxString TATKeyValueNode::GetValue() const
{
    return m_strValue;
}

void TATKeyValueNode::SetValue(const wxString& strValue)
{
    m_strValue = strValue;
}

} //namespace Backend
} //namespace TestAutomationTool
} //namespace Kinesis
