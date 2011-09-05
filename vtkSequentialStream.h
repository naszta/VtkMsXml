#ifndef vtkSequentialStreamH
#define vtkSequentialStreamH 1

#include <Objidl.h>
#include <istream>

class vtkSequentialStream : public ISequentialStream
{
public:
  vtkSequentialStream( std::istream &is );
  virtual ~vtkSequentialStream( void );

  // IUnknown
  virtual HRESULT __stdcall QueryInterface( const IID &riid, void ** ppvObject );
  virtual ULONG __stdcall AddRef( void );
  virtual ULONG __stdcall Release( void );

  // ISequentialStream
  virtual HRESULT __stdcall Read( void *pv, ULONG cb,  ULONG *pcbRead );
  virtual HRESULT __stdcall Write( void const *pv, ULONG cb, ULONG *pcbWritten );
  
protected:
  std::istream &is;
  CRITICAL_SECTION critical_section;
  ULONG ref_count;
};
#endif
