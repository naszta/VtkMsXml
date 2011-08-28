#include "vtkMsXmlParser.h"

#include "vtkContentHandler.h"
#include "vtkXMLDataElement.h"

#include <Windows.h>
#include <msxml6.h>

#include <vtksys/stl/vector>
#include <vtksys/stl/string>
#include <vtksys/stl/algorithm>

using namespace vtksys_stl;

vtkXMLDataElement * vtkMsXmlParser::ParseFile( const char * filename )
{
  if ( ( filename == 0 ) || ( filename[0] == 0 ) )
    return 0;

  int size = MultiByteToWideChar( CP_ACP, 0, filename, -1, NULL, NULL );
  if ( size <= 0 )
    return 0;

  vector<wchar_t> wfilename( size );
  MultiByteToWideChar( CP_ACP, 0, filename, -1, &wfilename[0], size );
  return ParseFile( &wfilename[0] );
}

vtkXMLDataElement * vtkMsXmlParser::ParseFile( const wchar_t * filename )
{
  if ( ( filename == 0 ) || ( filename[0] == 0 ) )
    return 0;

  wstring wfilename( filename );
  replace( wfilename.begin(), wfilename.end(), L'/', L'\\' );
  wfilename = L"file://" + wfilename;

  if ( FAILED(CoInitialize(NULL)) )
    return 0;

  ISAXXMLReader * reader = 0;

  if ( FAILED( CoCreateInstance( __uuidof(SAXXMLReader60), NULL, CLSCTX_ALL, __uuidof(ISAXXMLReader), (void**) &reader ) ) )
  {
    CoUninitialize();
    return 0;
  }

  vtkContentHandler * cont_handler = new vtkContentHandler;
  cont_handler->AddRef();

  if ( FAILED( reader->putContentHandler( cont_handler ) ) )
  {
    cont_handler->Release();
    reader->Release();
    CoUninitialize();
    return 0;
  }

  if ( FAILED( reader->parseURL( wfilename.c_str() ) ) )
  {
    cont_handler->Release();
    reader->Release();
    CoUninitialize();
    return 0;
  }

  vtkXMLDataElement * return_value = cont_handler->GetRootElement();
  if ( return_value != 0 )
    return_value->Register(NULL);

  cont_handler->Release();
  reader->Release();
  CoUninitialize();
  return return_value;
}
