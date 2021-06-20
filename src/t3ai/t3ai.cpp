/*
 * t3ai.cpp - tic tac toe ai source
 */

#include <iostream>
#include <algorithm>
#include "t3ai.hpp"

/**
 * check winner
 *
 * could you believe this function
 * used to be 150 lines long?
 *
 */

#define MATCH_CONSECUTIVE_POS( game, row, col, match, out, cont ) \
if ( (cont) && t3_game_check_idx( (game), (row), (col) ) && t3_game_get( (game), (row), (col) ) == (match) ) { \
	++(out); \
} else { \
	(cont) = false; \
}

t3_state_t t3_game_check_win( const t3_game_t& game, int16_t index ) {
	auto [ row, col ] = t3_game_idx_2d( game, index );
	t3_state_t match = t3_game_get( game, index );

	bool continue_vert_1 = true;
	bool continue_vert_2 = true;

	bool continue_horz_1 = true;
	bool continue_horz_2 = true;

	bool continue_d045_1 = true;
	bool continue_d045_2 = true;

	bool continue_d315_1 = true;
	bool continue_d315_2 = true;

	uint16_t consecutive_vert = 1;
	uint16_t consecutive_horz = 1;
	uint16_t consecutive_d045 = 1;
	uint16_t consecutive_d315 = 1;

	for ( int16_t i = 1; i < game.threshold; ++i ) {
		MATCH_CONSECUTIVE_POS( game, row + i, col, match, consecutive_vert, continue_vert_1 );
		MATCH_CONSECUTIVE_POS( game, row - i, col, match, consecutive_vert, continue_vert_2 );

		MATCH_CONSECUTIVE_POS( game, row, col + i, match, consecutive_horz, continue_horz_1 );
		MATCH_CONSECUTIVE_POS( game, row, col - i, match, consecutive_horz, continue_horz_2 );

		MATCH_CONSECUTIVE_POS( game, row - i, col + i, match, consecutive_d045, continue_d045_1 );
		MATCH_CONSECUTIVE_POS( game, row + i, col - i, match, consecutive_d045, continue_d045_2 );

		MATCH_CONSECUTIVE_POS( game, row + i, col + i, match, consecutive_d315, continue_d315_1 );
		MATCH_CONSECUTIVE_POS( game, row - i, col - i, match, consecutive_d315, continue_d315_2 );
	}

	return ( consecutive_vert >= game.threshold ||
			consecutive_horz >= game.threshold ||
			consecutive_d045 >= game.threshold ||
			consecutive_d315 >= game.threshold )
		? match
		: ST_EMPTY;
}

/**
 * get best next move
 *
 * jesus christ this code makes
 * me want to cough up blood
 *
 * checks some number of moves into the future using
 * the minimax algorithm with alpha beta pruning, then
 * calculates a score for it using a fitness function
 *
 * see: https://www.youtube.com/watch?v=l-hh51ncgDI
 * see: https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-4-alpha-beta-pruning
 * see: https://en.wikipedia.org/wiki/Fitness_function
 *
 */

#define STATE_INC_VAR( state, x_var, o_var ) \
if ( state == ST_HAS_X ) \
	++(x_var); \
else if ( state == ST_HAS_O ) \
	++(o_var)

#define STATE_DEC_VAR( state, x_var, o_var ) \
if ( state == ST_HAS_X ) \
	--(x_var); \
else if ( state == ST_HAS_O ) \
	--(o_var)

t3_state_t t3_ai_player;
t3_state_t t3_opp_player;

static inline int32_t pow4( int32_t n ) { // binary exponentiation but i was too lazy to write it out
	int32_t n2 = n * n;
	return n2 * n2;
}

static inline int32_t calculate_fitness( int16_t count_x, int16_t count_o ) {
	if ( count_x > 0 && count_o == 0 )
		return ( t3_ai_player == count_x ? 1 : -1 ) * pow4( count_x );
	else if ( count_x == 0 && count_o > 0 )
		return ( t3_ai_player == count_o ? 1 : -1 ) * pow4( count_o );
	return 0;
}

// terribly inefficient i know
// fuck off
static int32_t calculate_fitness( const t3_game_t& game ) {
	int32_t fitness = 0;

	// rows
	if ( game.threshold <= game.cols ) {
		for ( int16_t i = 0; i < game.rows; ++i ) {
			int16_t count_x = 0;
			int16_t count_o = 0;
			int16_t offset = 0;

			for ( ; offset < game.threshold; ++offset ) {
				t3_state_t state = t3_game_get( game, i, offset );
				STATE_INC_VAR( state, count_x, count_o );
			}

			do {
				fitness += calculate_fitness( count_x, count_o );

				t3_state_t state = t3_game_get( game, i, offset - game.threshold );
				STATE_DEC_VAR( state, count_x, count_o );

				state = offset < game.cols ? t3_game_get( game, i, offset ) : ST_EMPTY;
				STATE_INC_VAR( state, count_x, count_o );

				++offset;
			} while ( offset <= game.cols );
		}
	}

	// columns
	if ( game.threshold <= game.rows ) {
		for ( int16_t i = 0; i < game.cols; ++i ) {
			int16_t count_x = 0;
			int16_t count_o = 0;
			int16_t offset = 0;

			for ( ; offset < game.threshold; ++offset ) {
				t3_state_t state = t3_game_get( game, offset, i );
				STATE_INC_VAR( state, count_x, count_o );
			}

			do {
				fitness += calculate_fitness( count_x, count_o );

				t3_state_t state = t3_game_get( game, offset - game.threshold, i );
				STATE_DEC_VAR( state, count_x, count_o );

				state = offset < game.rows ? t3_game_get( game, offset, i ) : ST_EMPTY;
				STATE_INC_VAR( state, count_x, count_o );

				++offset;
			} while ( offset <= game.rows );
		}
	}

	// diagonal y = -x
	if ( game.threshold <= game.rows && game.threshold <= game.cols ) {
		for ( int16_t i = 0; i + game.threshold <= game.cols; ++i ) {
			int16_t count_x = 0;
			int16_t count_o = 0;
			int16_t offset = 0;

			for ( ; offset < game.threshold; ++offset ) {
				t3_state_t state = t3_game_get( game, offset, i + offset );
				STATE_INC_VAR( state, count_x, count_o );
			}

			do {
				fitness += calculate_fitness( count_x, count_o );

				t3_state_t state = t3_game_get( game, offset - game.threshold, i + offset - game.threshold );
				STATE_DEC_VAR( state, count_x, count_o );

				state = t3_game_get( game, offset, i + offset );
				STATE_INC_VAR( state, count_x, count_o );

				++offset;
			} while ( offset <= game.rows && i + offset <= game.cols );
		}
	}

	// diagonal y = x
	if ( game.threshold <= game.rows && game.threshold <= game.cols ) {
		for ( int16_t i = game.cols - 1; i - game.threshold >= -1; --i ) {
			int16_t count_x = 0;
			int16_t count_o = 0;
			int16_t offset = 0;

			for ( ; offset < game.threshold; ++offset ) {
				t3_state_t state = t3_game_get( game, offset, i - offset );
				STATE_INC_VAR( state, count_x, count_o );
			}

			do {
				fitness += calculate_fitness( count_x, count_o );

				t3_state_t state = t3_game_get( game, offset - game.threshold, i - offset - game.threshold );
				STATE_DEC_VAR( state, count_x, count_o );

				state = t3_game_get( game, offset, i - offset );
				STATE_INC_VAR( state, count_x, count_o );

				++offset;
			} while ( offset <= game.rows && i - offset >= -1 );
		}
	}

	return fitness;
}

t3_best_move_t t3_game_calc_best_move(
		t3_game_t& game,
		int16_t depth,
		int32_t alpha,
		int32_t beta,
		bool maximizing,
		int16_t last )
{
	int32_t best_score = maximizing ? INT32_MIN : INT32_MAX;
	int16_t best_move = -1;

	// if game has ended, calculate score of path
	t3_state_t winner = ~last ? t3_game_check_win( game, last ) : ST_EMPTY;
	if ( ( ~last && winner ) || t3_game_full( game ) ) {
		//best_score = depth * ( !winner ? 0 : winner == t3_ai_player ? 1 : -1 );
		best_score = ( INT32_MAX - MINIMAX_PREDICT_DEPTH - 5 + depth ) * ( !winner ? 0 : winner == t3_ai_player ? 1 : -1 );
		return { best_score, best_move };
	}

	// if depth has been reached, calculate fitness of path
	if ( depth == 0 ) {
		best_score = calculate_fitness( game );
		return { best_score, best_move }; // ...
	}

	// not a base case bullshit
	for ( int16_t i = 0; i < game.dims; ++i ) if ( !t3_game_get( game, i ) ) {
		t3_game_set( game, i, maximizing ? t3_ai_player : t3_opp_player );

		int32_t score = t3_game_calc_best_move( game, depth - 1, alpha, beta, !maximizing, i ).score;
		if ( maximizing ) {
			if ( best_score < score ) {
				best_score = score;
				best_move = i;

				alpha = std::max( alpha, best_score );
				if ( beta <= alpha ) {
					t3_game_set( game, i, ST_EMPTY );
					break;
				}
			}
		} else {
			if ( best_score > score ) {
				best_score = score;
				best_move = i;

				beta = std::min( beta, best_score );
				if ( beta <= alpha ) {
					t3_game_set( game, i, ST_EMPTY );
					break;
				}
			}
		}

		t3_game_set( game, i, ST_EMPTY );
	}

	return { best_score, best_move };
}
