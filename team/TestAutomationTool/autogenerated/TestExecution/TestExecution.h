///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TESTEXECUTION_H__
#define __TESTEXECUTION_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/treectrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace Kinesis
{
	namespace TestAutomationTool
	{
		namespace UI
		{
			#define ID_TEST_EXECUTION_DIALOG 1000
			
			///////////////////////////////////////////////////////////////////////////////
			/// Class TestExecutionBaseForm
			///////////////////////////////////////////////////////////////////////////////
			class TestExecutionBaseForm : public wxDialog 
			{
				private:
				
				protected:
					wxStaticText* m_txtLog;
					wxRichTextCtrl* m_rtbLog;
					wxStaticText* m_txtResultsTitle;
					wxTreeCtrl* m_treeResults;
					wxButton* m_btnStop;
					wxButton* m_btnRestart;
				
				public:
					
					TestExecutionBaseForm( wxWindow* parent, wxWindowID id = ID_TEST_EXECUTION_DIALOG, const wxString& title = _("TestExecutionTitle"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 560,721 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxDIALOG_NO_PARENT|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
					~TestExecutionBaseForm();
				
			};
			
		} // namespace UI
	} // namespace TestAutomationTool
} // namespace Kinesis

#endif //__TESTEXECUTION_H__
