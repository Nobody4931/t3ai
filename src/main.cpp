/**
 * main.cpp
 *
 * currently testing dependencies while
 * adding them via git submodules
 *
 * fmt api docs: https://fmt.dev/
 * spdlog api docs: https://github.com/gabime/spdlog/wiki/1.-QuickStart
 * json api docs: https://nlohmann.github.io/json
 *
 */

#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main() {
	// make sure this stuff works
	spdlog::set_level( spdlog::level::debug );
	spdlog::set_pattern( "[%m.%d.%C - %I:%M:%S] [%^%l%$] %v" );

	const char* random_json_str = R"(
	{
		"value_1": "sample text",
		"value_2": 69,
		"value_3": 4.20,
		"value_4": true,
		"value_5": null,
		"value_6": {
			"subvalue_1": [
				"str",
				123,
				4.56,
				false,
				null
			]
		}
	}
	)";

	auto random_json = json::parse( random_json_str );
	for ( auto& item : random_json.items() ) {
		spdlog::debug( "Key: {}", item.key() );
		spdlog::debug( "Typ: {}", item.value().type_name() );
		spdlog::debug( "Val: {}", item.value().dump() );
		spdlog::debug( "==========================" );
	}
}
