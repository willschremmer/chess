#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "raylib.h"

char *FEN_DEFAULT = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

board *boardNew(char *fen) {
  if (fen == NULL) {
    fen = FEN_DEFAULT;
  }
  board *B = malloc(sizeof(board));
  B->arr = calloc(64, sizeof(Piece));

  // loop through fen to initialize the board
  size_t rank = 8;
  size_t file = 1;
  
  for (int i = 0; i < strlen(fen); i++) {
    char curr = fen[i];
    switch (curr) {
    case 'P':
      boardUpdateLocation(B, WHITE_PAWN, rank, file);
      break;
    case 'B':
      boardUpdateLocation(B, WHITE_BISHOP, rank, file);
      break;
    case 'N':
      boardUpdateLocation(B, WHITE_KNIGHT, rank, file);
      break;
    case 'R':
      boardUpdateLocation(B, WHITE_ROOK, rank, file);
      break;
    case 'Q':
      boardUpdateLocation(B, WHITE_QUEEN, rank, file);
      break;
    case 'K':
      boardUpdateLocation(B, WHITE_KING, rank, file);
      break;
    case 'p':
      boardUpdateLocation(B, BLACK_PAWN, rank, file);
      break;
    case 'b':
      boardUpdateLocation(B, BLACK_BISHOP, rank, file);
      break;
    case 'n':
      boardUpdateLocation(B, BLACK_KNIGHT, rank, file);
      break;
    case 'r':
      boardUpdateLocation(B, BLACK_ROOK, rank, file);
      break;
    case 'q':
      boardUpdateLocation(B, BLACK_QUEEN, rank, file);
      break;
    case 'k':
      boardUpdateLocation(B, BLACK_KING, rank, file);
      break;
    case '/':
      file = 0; // set this to 0 since it's gonna be += 1 later lol
      rank -= 1;
      break;
    case ' ':
      i = strlen(fen); // TEMPORARY: abort if you get to the part of the fen
                       // that I haven't dealt with lol
      break;
    default:
      if (isdigit(curr)) {
        size_t offset = curr - '0';
        file += offset;
      }
      break;
    }
    file += 1;
  }

  B->whiteToMove = true;
  B->castle_K = true;
  B->castle_Q = true;
  B->castle_k = true;
  B->castle_q = true;
  B->enPassant = NULL;
  B->halfmove = 0;
  B->fullmove = 1;
  return B;
}

bool isPiece(Piece P) {
  return 0 <= P && P < 13;
}

bool isBoard(board *B) {
  if (B != NULL && B->arr != NULL) {
    for (size_t i = 0; i < 64; i++) {
      if (!isPiece(B->arr[i])) {
        return false;
      }
    }
    return true;
  } else {
    return false;
  }
}

bool isMove(move *M) {
  return 1 <= M->startRank && M->startRank <= 8
      && 1 <= M->startFile && M->startFile <= 8
      && 1 <= M->endRank && M->endRank <= 8
      && 1 <= M->endFile && M->endFile <= 8
      && (!M->isCapture || isPiece(M->victim));
}

move *moveNew(size_t startRank, size_t startFile, size_t endRank,
               size_t endFile, board *B) {
  bool isCapture = false;
  Piece victim = boardGetLocation(B, endRank, endFile);
  if (victim != EMPTY) {
    isCapture = true;
  }
  move *M = malloc(sizeof(move));
  M->startRank = startRank;
  M->startFile = startFile;
  M->endRank = endRank;
  M->endFile = endFile;
  M->isCapture = isCapture;
  if (isCapture) {
    M->victim = victim;
  }
  assert(isMove(M));
  return M;
}

void boardFree(board *B) {
  free(B->arr);
  free(B);
}

void moveFree(move *M) {
  free(M);
}

size_t indexFromRankFile(size_t rank, size_t file) {
  assert(1 <= rank && rank <= 8);
  assert(1 <= file && file <= 8);

  size_t row = rank - 1;
  size_t col = file - 1;

  return row*8 + col;
}

size_t rankFromIndex(size_t index) {
  assert(0 <= index && index < 64);
  
  return index / 8;
}

size_t fileFromIndex(size_t index) {
  assert(0 <= index && index < 64);
  
  return index % 8;
}

Piece boardGetLocation(board *B, size_t rank, size_t file) {
  assert(isBoard(B));

  size_t index = indexFromRankFile(rank, file);

  assert(isPiece(B->arr[index]));
  return B->arr[index];
}

void boardUpdateLocation(board *B, Piece piece, size_t rank, size_t file) {
  assert(isBoard(B));
  assert(isPiece(piece));

  size_t index = indexFromRankFile(rank, file);
  B->arr[index] = piece;

  assert(isBoard(B));
}

char pieceChar(Piece P) {
  assert(isPiece(P));
  switch (P) {
  case EMPTY:
    return '-';
    break;
  case WHITE_PAWN:
    return 'P';
    break;
  case WHITE_BISHOP:
    return 'B';
    break;
  case WHITE_KNIGHT:
    return 'N';
    break;
  case WHITE_ROOK:
    return 'R';
    break;
  case WHITE_QUEEN:
    return 'Q';
    break;
  case WHITE_KING:
    return 'K';
    break;
  case BLACK_PAWN:
    return 'p';
    break;
  case BLACK_BISHOP:
    return 'b';
    break;
  case BLACK_KNIGHT:
    return 'n';
    break;
  case BLACK_ROOK:
    return 'r';
    break;
  case BLACK_QUEEN:
    return 'q';
    break;
  case BLACK_KING:
    return 'k';
    break;
  default:
    return '?';
    assert(false);
    break;
  }
}

void piecePrint(Piece P) {
  assert(isPiece(P));

  printf("%c", pieceChar(P));
}

void boardPrint(board *B) {
  assert(isBoard(B));
  printf("+---------------+\n");
  for (size_t rank = 8; rank > 0; rank--) {
    printf("|");
    for (size_t file = 1; file < 9; file++) {
      piecePrint(boardGetLocation(B, rank, file));
      if (file != 8) printf(" ");
    }
    printf("|\n");
  }
  printf("+---------------+\n");
}

void boardDraw(board *B, int left, int top, int squareSize) {
  assert(isBoard(B));
  assert(squareSize > 0);

  for (size_t file = 1; file < 9; file++) {
    for (size_t rank = 1; rank < 9; rank++) {
      int x = (file-1) * squareSize + left;
      int y = (8-rank) * squareSize + top;

      Color fill;

      if ((file + rank) % 2 == 1) {
        fill = BEIGE;
      } else {
        fill = BROWN;
      }

      DrawRectangle(x, y, squareSize, squareSize, fill);

      if (boardGetLocation(B, rank, file) != EMPTY) {
        char *pieceName = malloc(2*sizeof(char));
        pieceName[0] = pieceChar(boardGetLocation(B, rank, file));
        pieceName[1] = '\0';
        DrawText(pieceName, x + squareSize/4, y + squareSize/4, squareSize/2, BLACK);
        free(pieceName);
      }
    }
  }
}

void boardMove(board *B, move *M) {
  assert(isBoard(B));
  assert(isMove(M));

  Piece mover = boardGetLocation(B, M->startRank, M->startFile);

  boardUpdateLocation(B, EMPTY, M->startRank, M->startFile);
  boardUpdateLocation(B, mover, M->endRank, M->endFile);

  assert(isBoard(B));
} 

void boardUndo(board *B, move *M) {
  assert(isBoard(B));
  assert(isMove(M));
  assert(boardGetLocation(B, M->startRank, M->startFile) == EMPTY);

  Piece mover = boardGetLocation(B, M->endRank, M->endFile);
  Piece oldPiece;

  if (M->isCapture) {
    oldPiece = M->victim;
  } else {
    oldPiece = EMPTY;
  }
  
  boardUpdateLocation(B, oldPiece, M->endRank, M->endFile);
  boardUpdateLocation(B, mover, M->startRank, M->startFile);

  assert(isBoard(B));
}
