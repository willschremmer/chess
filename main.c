#include <stdio.h>
#include "board.h"

int main() {
  board *B = board_new(NULL);

  printf("Starting board:\n");
  board_print(B);
  
  printf("\nMake move: e4\n");
  move *M = move_new(2, 5, 4, 5, B);
  board_move(B, M);
  board_print(B);

  board_free(B);

  return 0;
}

