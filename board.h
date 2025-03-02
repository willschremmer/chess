#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

typedef enum {
  EMPTY,
  WHITE_PAWN,
  WHITE_BISHOP,
  WHITE_KNIGHT,
  WHITE_ROOK,
  WHITE_QUEEN,
  WHITE_KING,
  BLACK_PAWN,
  BLACK_BISHOP,
  BLACK_KNIGHT,
  BLACK_ROOK,
  BLACK_QUEEN,
  BLACK_KING,
} Piece;

typedef struct {
  Piece *arr;
  bool white_to_move;
  bool castle_K;
  bool castle_Q;
  bool castle_k;
  bool castle_q;
  char *enpassant; // this is a horrible way to store this but I don't feel
                   // like making it better right now since I'm not gonna be
                   // implementing en passant for a while lol
  size_t halfmove;
  size_t fullmove;
} board;

board *board_new();
void board_free(board *B);
void board_update_location(board *B, Piece P, size_t rank, size_t file);
Piece board_get_location(board *B, size_t rank, size_t file);
void piece_print(Piece P);
void board_print(board *B);

#endif
