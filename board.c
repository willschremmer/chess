#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"

char *FEN_DEFAULT = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

board *board_new(char *fen) {
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
      board_update_location(B, WHITE_PAWN, rank, file);
      break;
    case 'B':
      board_update_location(B, WHITE_BISHOP, rank, file);
      break;
    case 'N':
      board_update_location(B, WHITE_KNIGHT, rank, file);
      break;
    case 'R':
      board_update_location(B, WHITE_ROOK, rank, file);
      break;
    case 'Q':
      board_update_location(B, WHITE_QUEEN, rank, file);
      break;
    case 'K':
      board_update_location(B, WHITE_KING, rank, file);
      break;
    case 'p':
      board_update_location(B, BLACK_PAWN, rank, file);
      break;
    case 'b':
      board_update_location(B, BLACK_BISHOP, rank, file);
      break;
    case 'n':
      board_update_location(B, BLACK_KNIGHT, rank, file);
      break;
    case 'r':
      board_update_location(B, BLACK_ROOK, rank, file);
      break;
    case 'q':
      board_update_location(B, BLACK_QUEEN, rank, file);
      break;
    case 'k':
      board_update_location(B, BLACK_KING, rank, file);
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

  B->white_to_move = true;
  B->castle_K = true;
  B->castle_Q = true;
  B->castle_k = true;
  B->castle_q = true;
  B->enpassant = NULL;
  B->halfmove = 0;
  B->fullmove = 1;
  return B;
}

bool is_piece(Piece P) {
  return 0 <= P && P < 13;
}

bool is_board(board *B) {
  if (B != NULL && B->arr != NULL) {
    for (size_t i = 0; i < 64; i++) {
      if (!is_piece(B->arr[i])) {
        return false;
      }
    }
    return true;
  } else {
    return false;
  }
}

void board_free(board *B) {
  free(B->arr);
  free(B);
}

size_t index_from_rank_file(size_t rank, size_t file) {
  assert(1 <= rank && rank <= 8);
  assert(1 <= file && file <= 8);

  size_t row = rank - 1;
  size_t col = file - 1;

  return row*8 + col;
}

size_t rank_from_index(size_t index) {
  assert(0 <= index && index < 64);
  
  return index / 8;
}

size_t file_from_index(size_t index) {
  assert(0 <= index && index < 64);
  
  return index % 8;
}

Piece board_get_location(board *B, size_t rank, size_t file) {
  assert(is_board(B));

  size_t index = index_from_rank_file(rank, file);

  assert(is_piece(B->arr[index]));
  return B->arr[index];
}

void board_update_location(board *B, Piece piece, size_t rank, size_t file) {
  assert(is_board(B));
  assert(is_piece(piece));

  size_t index = index_from_rank_file(rank, file);
  B->arr[index] = piece;

  assert(is_board(B));
}

void piece_print(Piece P) {
  assert(is_piece(P));
  switch (P) {
  case EMPTY:
    printf("-");
    break;
  case WHITE_PAWN:
    printf("P");
    break;
  case WHITE_BISHOP:
    printf("B");
    break;
  case WHITE_KNIGHT:
    printf("N");
    break;
  case WHITE_ROOK:
    printf("R");
    break;
  case WHITE_QUEEN:
    printf("Q");
    break;
  case WHITE_KING:
    printf("K");
    break;
  case BLACK_PAWN:
    printf("p");
    break;
  case BLACK_BISHOP:
    printf("b");
    break;
  case BLACK_KNIGHT:
    printf("n");
    break;
  case BLACK_ROOK:
    printf("r");
    break;
  case BLACK_QUEEN:
    printf("q");
    break;
  case BLACK_KING:
    printf("k");
    break;
  default:
    printf("?");
    assert(false);
    break;
  }
}

void board_print(board *B) {
  assert(is_board(B));

  for (size_t rank = 1; rank < 9; rank++) {
    for (size_t file = 1; file < 9; file++) {
      piece_print(board_get_location(B, rank, file));
      printf(" ");
    }
    printf("\n");
  }
}
