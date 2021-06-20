# Get includes
set(Spdlog_INCLUDE_DIR "${PROJECT_DEP_FOLDER}/spdlog/include")

# Get version
file(READ "${Spdlog_INCLUDE_DIR}/spdlog/version.h" _VERSION_H)

if (NOT _VERSION_H MATCHES "#define SPDLOG_VER_MAJOR ([0-9]+)")
	message(FATAL_ERROR "Cannot get Spdlog major version from version.h")
endif ()

math(EXPR _PKG_VER_MAJOR ${CMAKE_MATCH_1})

if (NOT _VERSION_H MATCHES "#define SPDLOG_VER_MINOR ([0-9]+)")
	message(FATAL_ERROR "Cannot get Spdlog minor version from version.h")
endif ()

math(EXPR _PKG_VER_MINOR ${CMAKE_MATCH_1})

if (NOT _VERSION_H MATCHES "#define SPDLOG_VER_PATCH ([0-9]+)")
	message(FATAL_ERROR "Cannot get Spdlog patch version from version.h")
endif ()

math(EXPR _PKG_VER_PATCH ${CMAKE_MATCH_1})


string(CONCAT Spdlog_VERSION ${_PKG_VER_MAJOR}.${_PKG_VER_MINOR}.${_PKG_VER_PATCH})

# Handle find_package() call
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Spdlog
	REQUIRED_VARS Spdlog_INCLUDE_DIR
	VERSION_VAR Spdlog_VERSION)

mark_as_advanced(Spdlog_FOUND Spdlog_INCLUDE_DIR Spdlog_VERSION)

# Create library
if (Spdlog_FOUND AND NOT TARGET Spdlog::Spdlog)
	add_library(Spdlog::Spdlog INTERFACE IMPORTED)
	target_include_directories(Spdlog::Spdlog INTERFACE
		"${Spdlog_INCLUDE_DIR}")
endif ()
