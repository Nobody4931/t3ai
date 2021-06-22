# Get includes
set(Asio_INCLUDE_DIR "${PROJECT_DEP_FOLDER}/asio/asio/include")

# Get version
file(READ "${Asio_INCLUDE_DIR}/asio/version.hpp" _VERSION_H)

if (NOT _VERSION_H MATCHES "#define ASIO_VERSION ([0-9]+)")
	message(FATAL_ERROR "Cannot get Asio version from version.hpp")
endif ()

math(EXPR _PKG_VER_MAJOR "${CMAKE_MATCH_1} / 100000")
math(EXPR _PKG_VER_MINOR "${CMAKE_MATCH_1} / 100 % 1000")
math(EXPR _PKG_VER_PATCH "${CMAKE_MATCH_1} % 100")

string(CONCAT Asio_VERSION ${_PKG_VER_MAJOR}.${_PKG_VER_MINOR}.${_PKG_VER_PATCH})

# Handle find_package() call
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Asio
	REQUIRED_VARS Asio_INCLUDE_DIR
	VERSION_VAR Asio_VERSION)

mark_as_advanced(Asio_FOUND Asio_INCLUDE_DIR Asio_VERSION)

# Create library
if (Asio_FOUND AND NOT TARGET Asio::Asio)
	add_library(Asio::Asio INTERFACE IMPORTED)
	target_include_directories(Asio::Asio INTERFACE
		"${Asio_INCLUDE_DIR}")
endif ()
