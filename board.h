#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include "raylib.h"

#define PIECE_COUNT 12

extern Texture2D pieceTextures[PIECE_COUNT+1];

// TYPES
// ----------------------------------------------------------------------------
typedef enum { // TYPE: piece
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

typedef struct { // TYPE: board
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

typedef struct { // TYPE: move
  size_t startRank;
  size_t startFile;
  size_t endRank;
  size_t endFile;
  bool isCapture;
  Piece victim; // only matters if is_capture is true
} move;

typedef struct { // TYPE: moveBank
  Piece piece; // set to EMPTY iff storing for multiple pieces
  size_t len; // length of arr
  move **arr; // this is not the final way i want to do this so this is just
              // hacked together and arr is just initialized to having 
              // length 64 for every piece lolz
} moveBank;
// ============================================================================

// FUNCTIONS
// ----------------------------------------------------------------------------
bool piecesSameColor(Piece P1, Piece P2);
bool isLegalMove(board *B, move *M);
bool moveBankContains(moveBank *X, move *M);
bool moveEqual(move *M1, move *M2);
void moveBankFree(moveBank *X);
char pieceChar(Piece P);
char *pieceToFileName(Piece P);


board *boardNew();
/* REQUIRES: true
 * ENSURES: boardNew() != NULL
 */

void boardFree(board *B);
/* REQUIRES: B != NULL
 * ENSURES: B is freed
 */

void boardUpdateLocation(board *B, Piece P, size_t rank, size_t file);
/* REQUIRES: B != NULL && 1 <= rank, file <= 8
 * ENSURES: After calling, `B` will have piece `P` at rank `rank` and 
 *          file `file`
 */

Piece boardGetLocation(board *B, size_t rank, size_t file);
/* REQUIRES: B != NULL && 1 <= rank, file <= 8
 * ENSURES: returns the Piece at rank `rank` and file `file` in B
 */

Piece pieceMakeWhite(Piece P);
/* REQUIRES: true
 * ENSURES: if P is black, turn it white
 */

bool pieceIsWhite(Piece P);
/* REQUIRES: true
 * ENSURES: return whether P is white
 */

bool pieceIsBlack(Piece P);
/* REQUIRES: true
 * ENSURES: return whether P is black
 */


void piecePrint(Piece P);
/* REQUIRES: true
 * ENSURES: prints the character corresponding to P
 */

void boardPrint(board *B);
/* REQUIRES: B != NULL
 * ENSURES: prints every piece in B in a grid
 */

move *moveNew(size_t startRank, size_t startFile, size_t endRank,
              size_t endFile, board *B);
/* REQUIRES: B != NULL && 1 <= rank, file <= 8
 * ENSURES: \result != NULL, \result is a move corresponding to moving the
 *          piece at (startRank, startFile) in B to (endRank, endFile)
 */

void boardMove(board *B, move *M);
/* REQUIRES: B != NULL && M != NULL
 * ENSURES: updates B to correspond to making the move M
 */

void boardUndo(board *B, move *M);
/* REQUIRES: B != NULL && M != NULL
 * ENSURES: updates B to correspond to undoing the move M
 */

void boardDraw(board *B, int left, int top, int squareSize);
/* REQUIRES: B != NULL && squareSize > 0;
 * ENSURES: draws the board and pieces on the screen
 */

void moveFree(move *M);
/* REQUIRES: M != NULL
 * ENSURES: M is freed
 */

moveBank *moveBankNew(Piece piece);
/* REQURES: true
 * ENSURES: \result != NULL, \result is an empty collection of `move`s
 */

void moveBankAdd(moveBank *X, move *M);
/* REQUIRES: X != NULL && M != NULL
 * ENSURES: X is updated with M now added
 */

moveBank *getMoves(board *B, size_t rank, size_t file);
/* REQUIRES: B != NULL && 1 <= rank, file <= 8
 * ENSURES: \result != NULL, \result is a `moveBank` of all legal moves made
 *          by the piece at (rank, file) in B
 */
// ============================================================================

#endif
