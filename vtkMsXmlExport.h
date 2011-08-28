#ifndef vtkMsXmlExportH
#define vtkMsXmlExportH 1

#ifdef WIN32
# ifdef VtkMsXml_EXPORTS
#  define vtkMsXml_EXPORT __declspec( dllexport )
# else
#  define vtkMsXml_EXPORT __declspec( dllimport )
# endif
#else
# define vtkMsXml_EXPORT
#endif

#endif
