#include <windows.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "t3ai.hpp"

#define BOARD_ROWS  5
#define BOARD_COLS  5
#define BOARD_THRES 4

// shitty output function i wrote in like 5 min
void output_board( const t3_game_t& game, int16_t highlight = -1 ) {
	int16_t max_padding = std::log10( game.dims );
	int16_t board_width = ( max_padding + 3 + 1 ) * BOARD_COLS - 1;
	auto [ hrow, hcol ] = ~highlight ? t3_game_idx_2d( game, highlight ) : t3_pos2d_t { -1, -1 };

	for ( int16_t r = 0; r < game.rows; ++r ) {
		for ( int16_t c = 0; c < game.cols; ++c ) {
			int16_t index = t3_game_idx( game, r, c );
			int16_t padding = std::log10( index + 1 );

			if ( r == hrow && c == hcol ) {
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), FOREGROUND_INTENSITY | FOREGROUND_RED );
			}

			switch ( t3_game_get( game, index ) ) {
				case ST_EMPTY:
					for ( int16_t i = 0; i < max_padding - padding; ++i )
						std::cout << ' ';
					std::cout << ' ' << ( index + 1 ) << ' ';
					break;

				case ST_HAS_X:
					for ( int16_t i = 0; i < max_padding; ++i )
						std::cout << ' ';
					std::cout << " X ";
					break;

				case ST_HAS_O:
					for ( int16_t i = 0; i < max_padding; ++i )
						std::cout << ' ';
					std::cout << " O ";
					break;

				default:
					break;
			}

			if ( r == hrow && c == hcol ) {
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), FOREGROUND_INTENSITY | FOREGROUND_GREEN );
			}

			if ( c + 1 < game.cols ) {
				std::cout << '|';
			}
		}

		std::cout << '\n';
		for ( int16_t i =0; i < board_width; ++i )
			std::cout << '-';
		std::cout << '\n';
	}
}

int main() {
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), FOREGROUND_INTENSITY | FOREGROUND_GREEN );
	t3_game_t game = t3_game_create( BOARD_ROWS, BOARD_COLS, BOARD_THRES );
	t3_game_clear( game );

	bool user_turn = true;
	bool outputted = false;
	std::cout << "you are x\n";
	output_board( game );

	while ( true ) {
		int16_t index;
		if ( user_turn ) {
			std::cout << "choose > ";
			std::cin >> index, --index;

			if ( !t3_game_check_idx( game, index ) || t3_game_get( game, index ) ) {
				std::cout << "no\n";
				continue;
			}
		} else {
			std::cout << "thinking\n";
			index = t3_game_calc_best_move( game, ST_HAS_O );
		}

		t3_game_set( game, index, user_turn ? ST_HAS_X : ST_HAS_O );
		output_board( game, index );

		if ( t3_game_check_win( game, index ) ) {
			std::cout << ( user_turn ? 'x' : 'o' ) << " wins\n";
			std::cin >> index;
			break;
		}

		if ( t3_game_full( game ) ) {
			std::cout << "draw\n";
			std::cin >> index;
			break;
		}

		user_turn = !user_turn;
	}
}
