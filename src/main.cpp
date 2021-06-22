/**
 * main.cpp
 *
 * currently testing dependencies while
 * adding them via git submodules
 *
 * fmt api docs: https://fmt.dev/
 * spdlog api docs: https://github.com/gabime/spdlog/wiki/1.-QuickStart
 * json api docs: https://nlohmann.github.io/json
 * asio api docs: https://think-async.com/Asio/asio-1.18.2/doc/
 *
 */

#define ASIO_STANDALONE

#include <cstring>
#include <vector>

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <asio.hpp>

inline void check_asio_error( const asio::error_code& error, const char* message ) {
	if ( error ) {
		spdlog::error( message, error.message() );
		throw asio::system_error( error );
	}
}

int main() {
	// make sure this stuff works
	asio::io_context context;
	asio::error_code error;

	asio::ip::tcp::resolver resolver( context );
	asio::ip::tcp::socket socket( context );

	socket.connect( resolver.resolve( "httpbin.org", "80" )->endpoint(), error );
	check_asio_error( error, "Failed to connect: {}" );
	spdlog::info( "Connected to httpbin.org" );

	const char* payload =
		"GET /ip HTTP/1.1\r\n"
		"Host: httpbin.org\r\n"
		"Connection: close\r\n"
		"\r\n";
	const size_t payload_s = strlen( payload );
	const size_t buffer_s = 1024;
	char buffer[ buffer_s ];

	socket.write_some( asio::buffer( payload, payload_s ), error );
	check_asio_error( error, "Failed to send payload: {}" );
	spdlog::info( "Sent payload to /ip" );

	socket.wait( socket.wait_read );

	while ( true ) {
		size_t bytes = socket.read_some( asio::buffer( buffer, buffer_s ), error );
		if ( error == asio::error::eof ) break;
		else check_asio_error( error, "An error occurred: {}" );

		spdlog::info( "Received {} bytes:\n{:.{}}", bytes, buffer, bytes );
	}
}
