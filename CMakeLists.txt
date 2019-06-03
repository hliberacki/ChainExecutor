cmake_minimum_required (VERSION 3.0)
project(ChainExecutor LANGUAGES CXX)

include(GNUInstallDirs)

set(library_name ChainExecutor)
add_library(${library_name} INTERFACE)
  8 target_include_directories(${library_name} INTERFACE
  9         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
 10   $<INSTALL_INTERFACE:include>)

install(TARGETS swissknife EXPORT ${library_name}Config
 13   ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
 14   LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
 15   RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}) # For Windows
 16
 17 install(DIRECTORY include/
 18         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
 19         FILES_MATCHING PATTERN "*.hpp*")
 20
 21 install(EXPORT ${library_name}Config DESTINATION share/${library_name}/cmake)
 22
 23 export(TARGETS ChainExecutor FILE ${library_name}Config.cmake)
