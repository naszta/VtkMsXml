#include "vtkMsXmlParser.h"

#include "vtkContentHandler.h"
#include "vtkSequentialStream.h"
#include "vtkXMLDataElement.h"

#include <Windows.h>
#include <msxml6.h>

#include <vtksys/ios/fstream>
using namespace vtksys_stl;

vtkXMLDataElement * vtkMsXmlParser::ParseFile( const char * filename )
{
  ifstream file( filename );
  if ( ! file.is_open() )
    return 0;

  vtkXMLDataElement * temp = ParseStream( file );
  file.close();
  return temp;
}

vtkXMLDataElement * vtkMsXmlParser::ParseFile( const wchar_t * filename )
{
  ifstream file( filename );
  if ( ! file.is_open() )
    return 0;

  vtkXMLDataElement * temp = ParseStream( file );
  file.close();
  return temp;
}

vtkXMLDataElement * vtkMsXmlParser::ParseStream( vtksys_ios::istream &is )
{
  if ( FAILED(CoInitialize(NULL)) )
    return 0;

  ISAXXMLReader * reader = 0;
  if ( FAILED( CoCreateInstance( __uuidof(SAXXMLReader60), NULL, CLSCTX_ALL, __uuidof(ISAXXMLReader), (void**) &reader ) ) )
  {
    CoUninitialize();
    return 0;
  }

  vtkContentHandler * cont_handler = new vtkContentHandler;
  ISequentialStream * my_stream = new vtkSequentialStream(is);
    
  cont_handler->AddRef();
  my_stream->AddRef();

  if ( FAILED( reader->putContentHandler( cont_handler ) ) )
  {
    my_stream->Release();
    cont_handler->Release();
    reader->Release();
    return 0;
  }

  VARIANT var;
  var.vt = VT_UNKNOWN;
  var.punkVal = my_stream;
  VARIANT_BOOL success = FALSE;

  bool value = SUCCEEDED( reader->parse( var ) );

  vtkXMLDataElement * return_value = cont_handler->GetRootElement();
  if ( return_value != 0 )
    return_value->Register(NULL);

  my_stream->Release();
  cont_handler->Release();
  reader->Release();
  return return_value;
}
