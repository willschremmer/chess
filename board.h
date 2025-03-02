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
  bool whiteToMove;
  bool castle_K;
  bool castle_Q;
  bool castle_k;
  bool castle_q;
  char *enPassant; // this is a horrible way to store this but I don't feel
                   // like making it better right now since I'm not gonna be
                   // implementing en passant for a while lol
  size_t halfmove;
  size_t fullmove;
} board;

typedef struct {
  size_t startRank;
  size_t startFile;
  size_t endRank;
  size_t endFile;
  bool isCapture;
  Piece victim; // only matters if is_capture is true
} move;

board *boardNew();
void boardFree(board *B);
void boardUpdateLocation(board *B, Piece P, size_t rank, size_t file);
Piece boardGetLocation(board *B, size_t rank, size_t file);
void piecePrint(Piece P);
void boardPrint(board *B);
move *moveNew();
void boardMove(board *B, move *M);
void boardUndo(board *B, move *M);
void boardDraw(board *B, int left, int top, int squareSize);
void moveFree(move *M);

#endif
