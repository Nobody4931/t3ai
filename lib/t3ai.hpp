/*
 * t3ai.hpp - tic tac toe ai header
 *
 * look, i would use a smaller datatype because there's
 * no way in hell a tic tac toe board would ever have
 * over 256 slots, but 8 bytes is a char so fuck c++
 *
 */

#pragma once
#ifndef HEADER_T3AI_H
#define HEADER_T3AI_H

#include <cstdint>
#include <cstring>

#define MINIMAX_PREDICT_DEPTH 4

/* slot state enums */
enum t3_state_t : uint16_t {
	ST_EMPTY,
	ST_HAS_X,
	ST_HAS_O,
	ST_INVERT
};

inline t3_state_t t3_state_invert( t3_state_t state ) {
	return static_cast<t3_state_t>( ST_INVERT - state );
}

/* game position class */
struct t3_pos2d_t {
	int16_t row;
	int16_t col;
};

/* game class */
struct t3_game_t {
	t3_state_t* board;
	int16_t rows;
	int16_t cols;
	int16_t dims;
	int16_t threshold;
};

// check index
inline bool t3_game_check_idx( const t3_game_t& game, int16_t index ) {
	return ( index > -1 && index < game.dims );
}

inline bool t3_game_check_idx( const t3_game_t& game, int16_t row, int16_t col ) {
	return ( row > -1 && row < game.rows ) && ( col > -1 && col < game.cols );
}

// get index
inline int16_t t3_game_idx( const t3_game_t& game, int16_t row, int16_t col ) {
	return game.cols * row + col;
}

inline t3_pos2d_t t3_game_idx_2d( const t3_game_t& game, int16_t index ) {
	int16_t row = index / game.cols;
	int16_t col = index - game.cols * row;
	return { row, col };
}

// get slot state
inline t3_state_t t3_game_get( const t3_game_t& game, int16_t index ) {
	return game.board[ index ];
}

inline t3_state_t t3_game_get( const t3_game_t& game, int16_t row, int16_t col ) {
	return game.board[ t3_game_idx( game, row, col ) ];
}

// set slot state
inline void t3_game_set( t3_game_t& game, int16_t index, t3_state_t state ) {
	game.board[ index ] = state;
}

inline void t3_game_set( t3_game_t& game, int16_t row, int16_t col, t3_state_t state ) {
	game.board[ t3_game_idx( game, row, col ) ] = state;
}

// check if board is full
inline bool t3_game_full( const t3_game_t& game ) {
	bool full = 1;

	for ( int16_t i = 0; i < game.dims; ++i ) if ( !t3_game_get( game, i ) ) {
		full = 0;
		break;
	}

	return full;
}

// check winner
t3_state_t t3_game_check_win( const t3_game_t& game, int16_t index );

inline t3_state_t t3_game_check_win( const t3_game_t& game, int16_t row, int16_t col ) {
	return t3_game_check_win( game, t3_game_idx( game, row, col ) );
}

// create board
inline t3_game_t t3_game_create( int16_t rows, int16_t cols, int16_t threshold ) {
	int16_t dims = rows * cols;

	return {
		.board = new t3_state_t[ dims ],
		.rows = rows,
		.cols = cols,
		.dims = dims,
		.threshold = threshold
	};
}

// clone board
inline t3_game_t t3_game_clone( const t3_game_t& target ) {
	t3_game_t clone {
		.board = new t3_state_t[ target.dims ],
		.rows = target.rows,
		.cols = target.cols,
		.dims = target.dims,
		.threshold = target.threshold
	};

	for ( int16_t i = 0; i < clone.dims; ++i ) {
		t3_game_set( clone, i, t3_game_get( target, i ) );
	}

	return clone;
}

// clear board
inline void t3_game_clear( t3_game_t& game ) {
	std::memset( game.board, ST_EMPTY, sizeof( game.board[0] ) * game.dims );
}

// destroy board
inline void t3_game_destroy( t3_game_t& game ) {
	delete[] game.board;

	std::memset( &game, 0, sizeof( t3_game_t ) );
}

/* next best move class */
struct t3_best_move_t {
	int32_t score;
	int16_t move;
};

// calculate next best move
extern t3_state_t t3_ai_player;
extern t3_state_t t3_opp_player;

t3_best_move_t t3_game_calc_best_move(
		t3_game_t& game,
		int16_t depth,
		int32_t alpha,
		int32_t beta,
		bool maximizing,
		int16_t last = -1 );

inline int16_t t3_game_calc_best_move( t3_game_t& game, t3_state_t turn ) {
	t3_ai_player = turn;
	t3_opp_player = t3_state_invert( turn );

	return t3_game_calc_best_move( game, MINIMAX_PREDICT_DEPTH, INT32_MIN, INT32_MAX, true ).move;
}

#endif
