#include "spdlog/spdlog.h"

int main() {
	// make sure this stuff works
	spdlog::set_level( spdlog::level::debug );
	spdlog::set_pattern( "[%m.%d.%C - %I:%M:%S] [%^%l%$] %v" );

	spdlog::info( "Hello, world!" );
	spdlog::debug( "Hello, world!" );
	spdlog::warn( "Hello, world!" );
	spdlog::error( "Hello, world!" );
	spdlog::critical( "Hello, world!" );
}
