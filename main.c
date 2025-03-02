#include <stdio.h>
#include "board.h"
#include "raylib.h"

int main() {
  const int screenWidth = 1600;
  const int screenHeight = 900;

  const int boardPadding = 20;
  const int boardLeftOffset = (screenWidth - screenHeight) / 2;
  const int boardSize = screenHeight - 2*boardPadding;
  const int squareSize = boardSize/8;

  int hoveredRank = -1;
  int hoveredFile = -1;

  Piece selectedPiece = EMPTY;
  int selectedRank = -1;
  int selectedFile = -1;

  InitWindow(screenWidth, screenHeight, "Chess asf");

  SetTargetFPS(60);

  board *B = boardNew(NULL);

  // main game loop
  while (!WindowShouldClose()) {
    // update values
    // ------------------------------------------------------------------------
    hoveredRank = 8 - (GetMouseY() - boardPadding)/squareSize;
    if (GetMouseY() < boardPadding) hoveredRank -= 1;
    hoveredFile = 1 + (GetMouseX() - boardPadding - boardLeftOffset)/squareSize;
    if (GetMouseX() < boardPadding + boardLeftOffset) hoveredFile -= 1;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      if (selectedPiece == EMPTY) {
        selectedRank = hoveredRank;
        selectedFile = hoveredFile;
        selectedPiece = boardGetLocation(B, selectedRank, selectedFile);
      } else {
        move *M = moveNew(selectedRank, selectedFile,
            hoveredRank, hoveredFile, B);
        boardMove(B, M);
        moveFree(M);
        selectedRank = -1;
        selectedFile = -1;
        selectedPiece = EMPTY;
      }
    }
    // ------------------------------------------------------------------------

    // draw
    // ------------------------------------------------------------------------
    BeginDrawing();
      
    ClearBackground(WHITE);
    boardDraw(B, boardLeftOffset + boardPadding, boardPadding, squareSize);
    
    if (1 <= hoveredRank && hoveredRank <= 8 &&
        1 <= hoveredFile && hoveredFile <= 8) {

      int left = boardLeftOffset + boardPadding + squareSize*(hoveredFile - 1);
      int top = boardPadding + squareSize*(8 - hoveredRank);

      DrawRectangleLines(left + 5, top + 5, squareSize - 10, squareSize - 10, BLACK);
    }

    if (1 <= selectedRank && selectedRank <= 8 &&
        1 <= selectedFile && selectedFile <= 8) {

      int left = boardLeftOffset + boardPadding + squareSize*(selectedFile - 1);
      int top = boardPadding + squareSize*(8 - selectedRank);

      DrawRectangleLines(left + 5, top + 5, squareSize - 10, squareSize - 10, BLACK);
      DrawRectangleLines(left + 10, top + 10, squareSize - 20, squareSize - 20, BLACK);
    }

    EndDrawing();
    // ------------------------------------------------------------------------
  }

  // de-initialization
  // --------------------------------------------------------------------------
  CloseWindow();
  boardFree(B);
  // --------------------------------------------------------------------------

  return 0;
}

