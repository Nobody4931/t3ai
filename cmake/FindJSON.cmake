# Get includes
set(JSON_INCLUDE_DIR "${PROJECT_DEP_FOLDER}/json/include")

# Get version
file(READ "${JSON_INCLUDE_DIR}/nlohmann/json.hpp" _VERSION_H)

if (NOT _VERSION_H MATCHES "version ([0-9]+).([0-9]+).([0-9]+)")
	message(FATAL_ERROR "Cannot get JSON version from json.hpp")
endif ()

math(EXPR _PKG_VER_MAJOR ${CMAKE_MATCH_1})
math(EXPR _PKG_VER_MINOR ${CMAKE_MATCH_2})
math(EXPR _PKG_VER_PATCH ${CMAKE_MATCH_3})

string(CONCAT JSON_VERSION ${_PKG_VER_MAJOR}.${_PKG_VER_MINOR}.${_PKG_VER_PATCH})

# Handle find_package() call
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JSON
	REQUIRED_VARS JSON_INCLUDE_DIR
	VERSION_VAR JSON_VERSION)

mark_as_advanced(JSON_FOUND JSON_INCLUDE_DIR JSON_VERSION)

# Create library
if (JSON_FOUND AND NOT TARGET JSON::JSON)
	add_library(JSON::JSON INTERFACE IMPORTED)
	target_include_directories(JSON::JSON INTERFACE
		"${JSON_INCLUDE_DIR}")
endif ()
