#include "vtkContentHandler.h"

#include "vtkXMLDataElement.h"

#include <Windows.h>

#include <vtksys/stl/vector>

using namespace vtksys_stl;

vtkContentHandler::vtkContentHandler( void )
  : RefCount(0), rootElem(0), workonElem(0)
{

}

vtkContentHandler::~vtkContentHandler( void )
{
  if ( this->rootElem != 0 )
  {
    this->rootElem->Delete();
    this->rootElem = 0;
  }
}

HRESULT __stdcall vtkContentHandler::QueryInterface( REFIID riid, void **ppvObject )
{
  if ( riid == __uuidof(ISAXContentHandler) )
  {
    *ppvObject = static_cast<void*>(this); 
    this->AddRef();
    return S_OK;
  }
  if (riid == IID_IUnknown)
  {
    *ppvObject = static_cast<void*>(this); 
    this->AddRef();
    return S_OK;
  }
  *ppvObject = 0;
  return E_NOINTERFACE;
}

ULONG __stdcall vtkContentHandler::AddRef( void )
{
  return InterlockedIncrement(&this->RefCount);
}

ULONG __stdcall vtkContentHandler::Release( void )
{
  ULONG nRefCount = InterlockedDecrement(&this->RefCount);
  if ( nRefCount == 0 ) delete this;
  return nRefCount;
}

HRESULT __stdcall vtkContentHandler::putDocumentLocator( ISAXLocator * pLocator)
{
  return S_OK;
}

HRESULT __stdcall vtkContentHandler::startDocument( void )
{
  return S_OK;
}

HRESULT __stdcall vtkContentHandler::endDocument( void )
{
  return S_OK;
}

HRESULT __stdcall vtkContentHandler::startPrefixMapping( const wchar_t *pwchPrefix, int cchPrefix, const wchar_t *pwchUri, int cchUri )
{
  return S_OK;
}

HRESULT __stdcall vtkContentHandler::endPrefixMapping( const wchar_t *pwchPrefix, int cchPrefix)
{
  return S_OK;
}

HRESULT __stdcall vtkContentHandler::startElement( const wchar_t *pwchNamespaceUri, int cchNamespaceUri, const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName, ISAXAttributes *pAttributes )
{
  vtkXMLDataElement * new_element = vtkXMLDataElement::New();
  vector<char> elemName( WideCharToMultiByte( CP_ACP, 0,  pwchLocalName, cchLocalName, NULL, NULL, NULL, NULL ) );
  WideCharToMultiByte( CP_ACP, 0,  pwchLocalName, cchLocalName, &elemName[0], elemName.size(), NULL, NULL );
  new_element->SetName( &elemName[0] );

  if ( this->workonElem != 0 )
  {
    this->workonElem->AddNestedElement( new_element );
    this->workonElem = new_element;
    new_element->Delete();
  }
  else
  {
    this->rootElem = this->workonElem = new_element;
  }
  this->character_arrays.push_back(wstring());

  int numOfAttribs = 0;
  if ( FAILED( pAttributes->getLength( &numOfAttribs ) ) )
    return S_OK;

  const wchar_t * wuri = 0, * wlocal = 0, * wqname = 0, * wvalue = 0;
  int iuri = 0, ilocal = 0, iqname = 0, ivalue = 0;

  vector<char> attrName, attrValue;

  for ( int i = 0; i < numOfAttribs; ++i )
  {
    pAttributes->getName( i, &wuri, &iuri, &wlocal, &ilocal, &wqname, &iqname );
    pAttributes->getValue( i, &wvalue, &ivalue );

    attrName.resize( WideCharToMultiByte( CP_ACP, 0, wlocal, ilocal, NULL, NULL, NULL, NULL ) );
    WideCharToMultiByte( CP_ACP, 0, wlocal, ilocal, &attrName[0], attrName.size(), NULL, NULL );

    attrValue.resize( WideCharToMultiByte( CP_ACP, 0, wvalue, ivalue, NULL, NULL, NULL, NULL ) );
    WideCharToMultiByte( CP_ACP, 0, wvalue, ivalue, &attrValue[0], attrValue.size(), NULL, NULL );

    this->workonElem->SetAttribute( &attrName[0], &attrValue[0] );
  }

  return S_OK;
}

HRESULT __stdcall vtkContentHandler::endElement( const wchar_t *pwchNamespaceUri, int cchNamespaceUri, const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName)
{
  if ( ! this->character_arrays.back().empty() )
  {
    wstring &string_obj = this->character_arrays.back();
    vector<char> tempstring( WideCharToMultiByte(CP_ACP, 0, string_obj.c_str(), string_obj.size(), NULL, NULL, NULL, NULL ) );
    WideCharToMultiByte(CP_ACP, 0, string_obj.c_str(), string_obj.size(), &tempstring[0], tempstring.size(), NULL, NULL );
    this->workonElem->SetCharacterData( &tempstring[0], tempstring.size() );
  }
  this->character_arrays.pop_back();


  vtkXMLDataElement * parent_elem = this->workonElem->GetParent();
  this->workonElem = parent_elem;
  return S_OK;
}

HRESULT __stdcall vtkContentHandler::characters( const wchar_t *pwchChars, int cchChars)
{
  wstring temp_string( pwchChars, pwchChars + cchChars );
  this->character_arrays.back() += temp_string;
  return S_OK;
}

HRESULT __stdcall vtkContentHandler::ignorableWhitespace( const wchar_t *pwchChars, int cchChars)
{
  return S_OK;
}

HRESULT __stdcall vtkContentHandler::processingInstruction( const wchar_t *pwchTarget, int cchTarget, const wchar_t *pwchData, int cchData)
{
  return S_OK;
}

HRESULT __stdcall vtkContentHandler::skippedEntity( const wchar_t *pwchName, int cchName)
{
  return S_OK;
}

vtkXMLDataElement * vtkContentHandler::GetRootElement( void )
{
  return this->rootElem;
}
