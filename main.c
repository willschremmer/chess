#include <stdio.h>
#include "board.h"
#include "raylib.h"

int main() {
  const int screenWidth = 1600;
  const int screenHeight = 900;

  InitWindow(screenWidth, screenHeight, "Chess asf");

  SetTargetFPS(60);

  board *B = board_new(NULL);


  // main game loop
  while (!WindowShouldClose()) {
    // draw
    // ------------------------------------------------------------------------
    BeginDrawing();
      
      ClearBackground(WHITE);
      board_draw(B, screenWidth, screenHeight);

    EndDrawing();
    // ------------------------------------------------------------------------
  }

  // de-initialization
  // --------------------------------------------------------------------------
  CloseWindow();
  board_free(B);
  // --------------------------------------------------------------------------

  return 0;
}

