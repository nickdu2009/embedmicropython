#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "MicroPythonEngine::micropython_engine" for configuration "Release"
set_property(TARGET MicroPythonEngine::micropython_engine APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MicroPythonEngine::micropython_engine PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libmicropython_engine.a"
  )

list(APPEND _cmake_import_check_targets MicroPythonEngine::micropython_engine )
list(APPEND _cmake_import_check_files_for_MicroPythonEngine::micropython_engine "${_IMPORT_PREFIX}/lib/libmicropython_engine.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
