#include "chessboard.h"

#include <iterator>
#include <string>
#include <iostream>
#include <cstdlib>

chesspeer::chessboard::chessboard () {
	std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	this->set_board(fen);
}

chesspeer::chessboard::chessboard (std::string fen) {
	this->set_board(fen);
}

void chesspeer::chessboard::set_board(std::string fen) {
	this->board.fill(' ');
	std::cout << "Reading the standard fen string..." << std::endl;
	std::array<char, 64>::iterator board_iter = this->board.begin();
	std::string::iterator fen_iter = fen.begin();
	int emptysquares;
	struct chesspeer::Movenode* position = new chesspeer::Movenode();

	// First store the board pieces
	for(board_iter; board_iter != this->board.end(); board_iter++) {
		if (*fen_iter == '/') {
			fen_iter++;
		}
		if (isdigit(*fen_iter)) {
			emptysquares = (*fen_iter) - 48; // Damn you ascii;
			board_iter = board_iter + emptysquares - 1;
			fen_iter++;
		}
		else {
			*board_iter = *fen_iter;
			fen_iter++;
		}
	}

	// Handle whos move is it anyway
	fen_iter++;
	position->color_to_move = *fen_iter;
	fen_iter++;

	// Handle castling rights
	fen_iter++;
	while (*fen_iter != ' ') {
		switch (*fen_iter) {
		case 'K': position->white_castle_kingside = true;
		case 'Q': position->white_castle_queenside = true;
		case 'k': position->black_castle_kingside = true;
		case 'q': position->black_castle_queenside = true;
		default: fen_iter++;
		}
	}

	// store en_passant squares
	fen_iter++;
	if (*fen_iter != '-') {
		position->en_passant = std::string(fen_iter, fen_iter + 2);
		fen_iter = fen_iter + 3;
	}
	else {
		position->en_passant = *fen_iter;
		fen_iter = fen_iter + 2;
	}

	// store total moves and moves since last capture
	auto start_digit = fen_iter;
	while (*fen_iter != ' ') {
		fen_iter++;
	}
	position->plys_since_capture = std::stoi(std::string(start_digit, fen_iter));

	fen_iter++;
	position->on_move = std::stoi(std::string(fen_iter, fen.end()));

	this->position = position;
}

void chesspeer::chessboard::show() {
	int counter = 0;
	for (auto iter = this->board.begin(); iter != this->board.end(); iter++) {
		if (counter % 8 == 0) {
			std::cout << std::endl;
		}
		if (*iter == ' ') {
			std::cout << char(254) << " ";
		}
		else {
			std::cout << *iter << " ";
		}
		counter++;
	}
	std::cout << std::endl;
	std::cout << this->position->color_to_move << " to move" << std::endl;
	std::cout << "on move " << this->position->on_move << std::endl;
}

std::string chesspeer::chessboard::get_fen() {
	std::string fen = "";
	int col_counter = 0;
	int empty_squares = 0;
	for (auto board_iter = this->board.begin(); board_iter != this->board.end(); board_iter++) {
		if ((col_counter % 8) == 0 && (col_counter != 0)) {
			if (empty_squares != 0) {
				fen += char(empty_squares + 48);
				empty_squares = 0;
			}
			fen.append("/");
		}
		if (*board_iter == ' ') {
			empty_squares++;
			col_counter++;
		}
		else {
			if (empty_squares != 0) {
				fen += char(empty_squares + 48);
				empty_squares = 0;
			}
			fen += *board_iter;
			col_counter++;
		}
	}
	if (empty_squares != 0) {
		fen += char(empty_squares + 48);
	}

	fen += ' ';
	fen += this->position->color_to_move;
	fen += ' ';

	if (this->position->white_castle_kingside == true) {
		fen += 'K';
	}
	if (this->position->white_castle_queenside == true) {
		fen += 'Q';
	}
	if (this->position->black_castle_kingside == true) {
		fen += 'k';
	}
	if (this->position->black_castle_queenside == true) {
		fen += 'q';
	}

	fen += ' ';
	fen += this->position->en_passant;
	fen += ' ';

	char nums[4];
	_itoa(this->position->plys_since_capture, nums, 10);
	fen.append(nums);
	fen += ' ';
	_itoa(this->position->on_move, nums, 10);
	fen.append(nums);

	return fen;
}

