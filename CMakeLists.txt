CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

PROJECT(VtkMsXml)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

SET(LIBRARY_OUTPUT_PATH ${VTK_BINARY_DIR}/bin CACHE PATH
    "Single output directory for building all libraries.")
SET(EXECUTABLE_OUTPUT_PATH ${VTK_BINARY_DIR}/bin CACHE PATH
    "Single output directory for building all executables.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

SET(VtkMsXmlSrcs
  vtkMsXmlParser.cxx
  vtkContentHandler.cxx
  vtkSequentialStream.cxx
)

SET(VtkMsXmlHdrs
  vtkMsXmlParser.h
  vtkContentHandler.h
  vtkSequentialStream.h
  vtkMsXmlExport.h
)

ADD_LIBRARY(VtkMsXml SHARED ${VtkMsXmlSrcs} ${VtkMsXmlHdrs})

TARGET_LINK_LIBRARIES(VtkMsXml
  vtkCommon
  msxml6.lib
)
