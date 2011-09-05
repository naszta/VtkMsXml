#include "vtkSequentialStream.h"

vtkSequentialStream::vtkSequentialStream( vtksys_ios::istream &is )
  : is(is), ref_count(0)
{
  InitializeCriticalSection( &this->critical_section );
}

vtkSequentialStream::~vtkSequentialStream( void )
{
  DeleteCriticalSection( &this->critical_section );
}

// IUnknown
HRESULT __stdcall vtkSequentialStream::QueryInterface( const IID &riid, void ** ppvObject )
{
  if ( riid == IID_ISequentialStream )
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

ULONG __stdcall vtkSequentialStream::AddRef( void )
{
  return InterlockedIncrement(&this->ref_count);
}

ULONG __stdcall vtkSequentialStream::Release( void )
{
  ULONG nRefCount = InterlockedDecrement(&this->ref_count);
  if ( nRefCount == 0 ) delete this;
  return nRefCount;
}

// ISequentialStream
HRESULT __stdcall vtkSequentialStream::Read( void *pv, ULONG cb, ULONG *pcbRead )
{
  EnterCriticalSection( &this->critical_section );
  this->is.read( reinterpret_cast<char*>(pv), cb );
  *pcbRead = static_cast<ULONG>( this->is.gcount() );
  LeaveCriticalSection( &this->critical_section );
  return S_OK;
}

HRESULT __stdcall vtkSequentialStream::Write( void const *pv, ULONG cb, ULONG *pcbWritten )
{
  *pcbWritten = cb;
  return S_OK;
}
