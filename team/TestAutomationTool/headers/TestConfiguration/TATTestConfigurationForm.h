// [TERMS&CONDITIONS]

#ifndef __TATTESTCONFIGURATIONFORM__
#define __TATTESTCONFIGURATIONFORM__

#include "TestConfiguration/TestConfiguration.h"
#include "TestConfiguration/TATTestAutomationToolConfiguration.h"

using Kinesis::TestAutomationTool::UI::TestConfigurationBaseForm;
using Kinesis::TestAutomationTool::Backend::TATTestAutomationToolConfiguration;

namespace Kinesis
{
namespace TestAutomationTool
{
namespace UI
{

// Forward declarations
class TestExecutionBaseForm;


/// <summary>
/// Dialog that lets the user configure the testing process before executing it.
/// </summary>
class TATTestConfigurationForm : public TestConfigurationBaseForm
{
	// CONSTRUCTORS
	// ---------------
public:

	/// <summary>
	/// Default constructor.
	/// </summary>
	TATTestConfigurationForm();


	// DESTRUCTOR
	// ---------------
public:

	/// <summary>
	/// Destructor.
	/// </summary>
	virtual ~TATTestConfigurationForm();


	// METHODS
	// ---------------
public:

    /// <summary>
	/// DOC
	/// </summary>
    void ShowExecutionWindow();


    // EVENT HANDLERS
	// ---------------
public:

    /// <summary>
	/// Event handler called when the dialog is initialized.
	/// </summary>
    /// <param name="event">The event argument. Not used.</param>
    virtual void OnInitDialog( wxInitDialogEvent& event );

    /// <summary>
	/// Event handler called when the dialog is about to be closed.
	/// </summary>
    /// <param name="event">The event argument. Not used.</param>
    virtual void OnDialogClose( wxCloseEvent& event );

    /// <summary>
	/// Event handler called when the user checks one of the items in the compilation configuration list.
	/// </summary>
    /// <param name="event">The event argument.</param>
	virtual void OnCompilationConfigurationCheckListBoxToggled( wxCommandEvent& event );

    /// <summary>
	/// Event handler called when the user selects one of the items in the flag combination list.
	/// </summary>
    /// <param name="event">The event argument.</param>
	virtual void OnFlagCombinationsCheckListBoxSelected( wxCommandEvent& event );

    /// <summary>
	/// Event handler called when the user checks one of the items in the flag combination list.
	/// </summary>
    /// <param name="event">The event argument.</param>
	virtual void OnFlagCombinationsCheckListBoxToggled( wxCommandEvent& event );

    /// <summary>
	/// Event handler called when the user clicks on the button that navigates to the Editor.
	/// </summary>
    /// <param name="event">The event argument.</param>
	virtual void OnEditorButtonClick( wxCommandEvent& event );

    /// <summary>
	/// Event handler called when the user clicks on the button that navigates to the Test Execution dialog.
	/// </summary>
    /// <param name="event">The event argument.</param>
	virtual void OnLaunchButtonClick( wxCommandEvent& event );


	// PROPERTIES
	// ---------------
public:


	// ATTRIBUTES
	// ---------------
protected:

    /// <summary>
	/// Backend objects. Manages the business logic behind the user interface.
	/// </summary>	
    TATTestAutomationToolConfiguration m_backend;

    /// <summary>
	/// DOC
	/// </summary>	
    TestExecutionBaseForm* m_pExecutionForm;
};

} //namespace UI
} //namespace TestAutomationTool
} //namespace Kinesis

#endif // __TATTESTCONFIGURATIONFORM__
