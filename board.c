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

bool isMove(move *M) {
  return 1 <= M->startRank && M->startRank <= 8
      && 1 <= M->startFile && M->startFile <= 8
      && 1 <= M->endRank && M->endRank <= 8
      && 1 <= M->endFile && M->endFile <= 8
      && (!M->isCapture || isPiece(M->victim));
}

// this is so poorly written sorry lol
bool isMoveBank(moveBank *X) {
  if (X != NULL) {
    if (X->piece == EMPTY) {
      return X->arr != NULL;
    } else {
      if (X->arr == NULL) {
        return false;
      } else {
        for (int i = 0; i < X->len; i++) {
          if (!isMove(X->arr[i])) return false;
        }
        return true;
      }
    }
  } else {
    return false;
  }
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
  assert(B->whiteToMove == pieceIsWhite(mover));

  boardUpdateLocation(B, EMPTY, M->startRank, M->startFile);
  boardUpdateLocation(B, mover, M->endRank, M->endFile);
  B->whiteToMove = !B->whiteToMove;

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
  B->whiteToMove = !B->whiteToMove;

  assert(isBoard(B));
}

moveBank *moveBankNew(Piece piece) {
  moveBank *X = malloc(sizeof(moveBank));
  X->piece = piece;
  X->len = 0;
  X->arr = malloc(64*sizeof(move*));

  return X;
}

void moveBankAdd(moveBank *X, move *M) {
  X->arr[X->len] = M;
  X->len++;
}

Piece pieceMakeWhite(Piece P) {
  if (P <= 6) return P;
  return P - 6;
}

bool pieceIsWhite(Piece P) {
  return 1 <= P && P <= 6;
}

bool pieceIsBlack(Piece P) {
  return 7 <= P && P <= 12;
}

bool piecesSameColor(Piece P1, Piece P2) {
  assert(P1 != EMPTY);
  assert(P2 != EMPTY);

  return (pieceIsWhite(P1) && pieceIsWhite(P2))
      || (pieceIsBlack(P1) && pieceIsBlack(P2));
}

moveBank *getMoves(board *B, size_t rank, size_t file) {
  assert(1 <= rank && rank <= 8);
  assert(1 <= file && file <= 8);

  Piece mover = boardGetLocation(B, rank, file);
  moveBank *bank = moveBankNew(mover);

  switch (pieceMakeWhite(mover)) {
  case EMPTY:
    assert(false); // never should be getting moves of empty square
    break;
  case WHITE_PAWN:
    int d;
    int homeRank;
    if (pieceIsWhite(mover)) {
      d = 1;
      homeRank = 2;
    } else {
      assert(pieceIsBlack(mover));
      d = -1;
      homeRank = 7;
    }
    Piece front = boardGetLocation(B, rank + d, file);
    if (front == EMPTY) {
      move *stepForward = moveNew(rank, file, rank + d, file, B);
      moveBankAdd(bank, stepForward);
      if (rank == homeRank) {
        Piece doubleFront = boardGetLocation(B, rank + 2*d, file);
        if (doubleFront == EMPTY) {
          move *stepTwo = moveNew(rank, file, rank + 2*d, file, B);
          moveBankAdd(bank, stepTwo);
        }
      }
    }
    if (rank != 1) {
      Piece leftDiag = boardGetLocation(B, rank + d, file - 1);
      if (leftDiag != EMPTY && !piecesSameColor(mover, leftDiag)) {
        move *captureLeft = moveNew(rank, file, rank + d, file - 1, B);
        moveBankAdd(bank, captureLeft);
      }
    }
    if (rank != 8) {
      Piece rightDiag = boardGetLocation(B, rank + d, file + 1);
      if (rightDiag != EMPTY && !piecesSameColor(mover, rightDiag)) {
        move *captureRight = moveNew(rank, file, rank + d, file + 1, B);
        moveBankAdd(bank, captureRight);
      }
    }
    break;
  case WHITE_KING:
    for (int dRank = -1; dRank < 2; dRank++) {
      for (int dFile = -1; dFile < 2; dFile++) {
        if (dRank == 0 && dFile == 0) continue;
        if (rank + dRank < 1 || rank + dRank > 8
            || file + dFile < 1 || file + dFile > 8) {
          continue;
        }
        Piece target = boardGetLocation(B, rank + dRank, file + dFile);
        if (target == EMPTY || !piecesSameColor(mover, target)) {
          move *goThere = moveNew(rank, file, rank + dRank, file + dFile, B);
          moveBankAdd(bank, goThere);
        }
      }
    }
    break;
  case WHITE_KNIGHT:
    for (int negRank = -1; negRank < 2; negRank += 2) {
      for (int negFile = -1; negFile < 2; negFile += 2) {
        for (int swap = 0; swap < 2; swap++) {
          int dRank = 1 * negRank;
          int dFile = 2 * negFile;
          if (swap) {
            int dummy = dRank;
            dRank = dFile;
            dFile = dummy;
          }
          if (rank + dRank < 1 || rank + dRank > 8
              || file + dFile < 1 || file + dFile > 8) {
            continue;
          }
          Piece target = boardGetLocation(B, rank + dRank, file + dFile);
          if (target == EMPTY || !piecesSameColor(mover, target)) {
            move *goThere = moveNew(rank, file, rank + dRank, file + dFile, B);
            moveBankAdd(bank, goThere);
          }
        }
      }
    }
    break;
  case WHITE_BISHOP:
    for (int rankSign = -1; rankSign < 2; rankSign += 2) {
      for (int fileSign = -1; fileSign < 2; fileSign += 2) {
        int distance = 1;
        while (true) {
          int dRank = rankSign * distance;
          int dFile = fileSign * distance;
          if (rank + dRank < 1 || rank + dRank > 8
              || file + dFile < 1 || file + dFile > 8) {
            break;
          }
          Piece target = boardGetLocation(B, rank + dRank, file + dFile);
          if (target == EMPTY) {
            move *goThere = moveNew(rank, file, rank + dRank, file + dFile, B);
            moveBankAdd(bank, goThere);
            distance++;
          } else if (!piecesSameColor(mover, target)) {
            move *goThere = moveNew(rank, file, rank + dRank, file + dFile, B);
            moveBankAdd(bank, goThere);
            break;
          } else break;
        }
      }
    }
    break;
  case WHITE_ROOK:
    for (int rankSign = -1; rankSign < 2; rankSign += 1) {
      for (int fileSign = -1; fileSign < 2; fileSign += 1) {
        if (rankSign != 0 && fileSign != 0) continue;
        if (rankSign == 0 && fileSign == 0) continue;
        int distance = 1;
        while (true) {
          int dRank = rankSign * distance;
          int dFile = fileSign * distance;
          if (rank + dRank < 1 || rank + dRank > 8
              || file + dFile < 1 || file + dFile > 8) {
            break;
          }
          Piece target = boardGetLocation(B, rank + dRank, file + dFile);
          if (target == EMPTY) {
            move *goThere = moveNew(rank, file, rank + dRank, file + dFile, B);
            moveBankAdd(bank, goThere);
            distance++;
          } else if (!piecesSameColor(mover, target)) {
            move *goThere = moveNew(rank, file, rank + dRank, file + dFile, B);
            moveBankAdd(bank, goThere);
            break;
          } else break;
        }
      }
    }
    break;
  case WHITE_QUEEN:
    for (int rankSign = -1; rankSign < 2; rankSign += 1) {
      for (int fileSign = -1; fileSign < 2; fileSign += 1) {
        if (rankSign == 0 && fileSign == 0) continue;
        int distance = 1;
        while (true) {
          int dRank = rankSign * distance;
          int dFile = fileSign * distance;
          if (rank + dRank < 1 || rank + dRank > 8
              || file + dFile < 1 || file + dFile > 8) {
            break;
          }
          Piece target = boardGetLocation(B, rank + dRank, file + dFile);
          if (target == EMPTY) {
            move *goThere = moveNew(rank, file, rank + dRank, file + dFile, B);
            moveBankAdd(bank, goThere);
            distance++;
          } else if (!piecesSameColor(mover, target)) {
            move *goThere = moveNew(rank, file, rank + dRank, file + dFile, B);
            moveBankAdd(bank, goThere);
            break;
          } else break;
        }
      }
    }
    break;
  default:
    assert(false);
    break;
  }

  return bank;
}

bool moveEqual(move *M1, move *M2) {
  return M1->startRank == M2->startRank
      && M1->startFile == M2->startFile
      && M1->endRank   == M2->endRank
      && M1->endFile   == M2->endFile
      && ((M1->isCapture && M2->isCapture
           && M1->victim == M2->victim)
          || (!M1->isCapture && !M2->isCapture));
}

bool moveBankContains(moveBank *X, move *M) {
  for (int i = 0; i < X->len; i++) {
    if (moveEqual(X->arr[i], M)) return true;
  }
  return false;
}

bool isLegalMove(board *B, move *M) {
  moveBank *legalMoves = getMoves(B, M->startRank, M->startFile);
  bool res = moveBankContains(legalMoves, M);
  moveBankFree(legalMoves);
  return res;
}

void moveBankFree(moveBank *X) {
  for (int i = 0; i < X->len; i++) {
    moveFree(X->arr[i]);
  }
  free(X->arr);
  free(X);
}
