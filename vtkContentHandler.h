#ifndef vtkContentHandlerH
#define vtkContentHandlerH 1

#include <msxml6.h>

#include <vtksys/stl/list>
#include <vtksys/stl/string>

class vtkXMLDataElement;

class vtkContentHandler : public ISAXContentHandler
{
public:
  vtkContentHandler( void );
  virtual ~vtkContentHandler( void );
  virtual HRESULT __stdcall QueryInterface( REFIID riid, void **ppvObject );
  virtual ULONG __stdcall AddRef( void );
  virtual ULONG __stdcall Release( void );

  virtual HRESULT __stdcall putDocumentLocator( ISAXLocator * pLocator);
  virtual HRESULT __stdcall startDocument( void );
  virtual HRESULT __stdcall endDocument( void );
  virtual HRESULT __stdcall startPrefixMapping( const wchar_t *pwchPrefix, int cchPrefix, const wchar_t *pwchUri, int cchUri );
  virtual HRESULT __stdcall endPrefixMapping( const wchar_t *pwchPrefix, int cchPrefix);
  virtual HRESULT __stdcall startElement( const wchar_t *pwchNamespaceUri, int cchNamespaceUri, const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName, ISAXAttributes *pAttributes );
  virtual HRESULT __stdcall endElement( const wchar_t *pwchNamespaceUri, int cchNamespaceUri, const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName);
  virtual HRESULT __stdcall characters( const wchar_t *pwchChars, int cchChars);
  virtual HRESULT __stdcall ignorableWhitespace( const wchar_t *pwchChars, int cchChars);
  virtual HRESULT __stdcall processingInstruction( const wchar_t *pwchTarget, int cchTarget, const wchar_t *pwchData, int cchData);
  virtual HRESULT __stdcall skippedEntity( const wchar_t *pwchName, int cchName);

  virtual vtkXMLDataElement * GetRootElement( void );
protected:
  ULONG RefCount;
  vtkXMLDataElement * rootElem;
  vtkXMLDataElement * workonElem;
  std::list<std::wstring> character_arrays;
};

#endif
