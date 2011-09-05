#ifndef vtkMsXmlParserH
#define vtkMsXmlParserH 1

#include "vtkMsXmlExport.h"

#include <vtksys/ios/iostream>

class vtkXMLDataElement;

class vtkMsXml_EXPORT vtkMsXmlParser
{
public:
  static vtkXMLDataElement * ParseFile( const char * filename );
  static vtkXMLDataElement * ParseFile( const wchar_t * filename );
  static vtkXMLDataElement * ParseStream( vtksys_ios::istream &is );
};

#endif
