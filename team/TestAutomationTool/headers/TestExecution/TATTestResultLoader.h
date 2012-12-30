// [TERMS&CONDITIONS]

#ifndef __TATTESTRESULTLOADER__
#define __TATTESTRESULTLOADER__

#include "TestExecution/ITATTestResultLoader.h"


namespace Kinesis
{
namespace TestAutomationTool
{
namespace Backend
{

class TATTestResultNode;
template<typename T> class TATXmlReader;
    
/// <summary>
/// Test result loader implementation for extracting test results from an XML file generated by 
/// Boost's Unit Test Framework.
/// </summary>
class TATTestResultLoader : public ITATTestResultLoader
{
    // CONSTRUCTORS
    // ---------------
public:

    /// <summary>
    /// Default constructor.
    /// </summary>
    TATTestResultLoader();


    // DESTRUCTOR
    // ---------------
public:

    /// <summary>
    /// Destructor.
    /// </summary>
    ~TATTestResultLoader();


	// METHODS
	// ---------------
public:

    //! @copydoc ITATTestResultLoader::Load()
    virtual void Load(const wxString &strSource);

    //! @copydoc ITATTestResultLoader::Clear()
    virtual void Clear();

protected:

    /// <summary>
	/// Releases all the resources.
	/// </summary>
    void Destroy();

    /// <summary>
    /// Parses XML data to a test result tree.
    /// </summary>
    /// <remarks>
    /// The format of the XML has to be the same the Boost's Unit Test Framework uses.
    /// </remarks>
    /// <param name="strXmlData">The XML data.</param>
    /// <param name="xmlReader">A component to read the XML data.</param>
    /// <returns>
    /// The root of the genereated test result tree.
    /// </returns>
    TATTestResultNode* ParseXmlToTree(const wxString &strXmlData, TATXmlReader<std::string> &xmlReader);


    // PROPERTIES
    // ---------------
public:

    //! @copydoc ITATTestResultLoader::GetTestResultTree()
    virtual TATTestResultNode* GetTestResultTree() const;


    // ATTRIBUTES
    // ---------------
protected:

    /// <summary>
    /// The generated test result tree.
    /// </summary>
    TATTestResultNode* m_pTestResultTree;
};

} //namespace Backend
} //namespace TestAutomationTool
} //namespace Kinesis

#endif // __TATTESTRESULTLOADER__
