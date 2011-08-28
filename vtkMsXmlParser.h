#ifndef vtkMsXmlParserH
#define vtkMsXmlParserH 1

#include "vtkMsXmlExport.h"

class vtkXMLDataElement;

class vtkMsXml_EXPORT vtkMsXmlParser
{
public:
  static vtkXMLDataElement * ParseFile( const char * filename );
  static vtkXMLDataElement * ParseFile( const wchar_t * filename );
};

#endif
