#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "raylib.h"

#define PIECE_COUNT 12

Texture2D pieceTextures[PIECE_COUNT+1]; // because obviously let's 1-index

void loadTextures() {
  const char *textureFiles[PIECE_COUNT+1] = {
    "DUMMY_VALUE",
    "icons/white_pawn.png", "icons/white_bishop.png", "icons/white_knight.png", 
    "icons/white_rook.png", "icons/white_queen.png", "icons/white_king.png",
    "icons/black_pawn.png", "icons/black_bishop.png", "icons/black_knight.png",
    "icons/black_rook.png", "icons/black_queen.png", "icons/black_king.png",
  };

  for (int i = 1; i < PIECE_COUNT+1; i++) {
    pieceTextures[i] = LoadTexture(textureFiles[i]);
  }
}

void unloadTextures() {
  for (int i = 0; i < PIECE_COUNT; i++) {
    UnloadTexture(pieceTextures[i]); 
  }
}

int main() {
  const int screenWidth = 1600;
  const int screenHeight = 900;

  const int boardPadding = 20;
  const int boardLeftOffset = (screenWidth - screenHeight) / 2;
  const int boardSize = screenHeight - 2*boardPadding;
  const int squareSize = boardSize/8;

  const Color POSSIBLE_MOVE_COLOR = { 218, 32, 60, 120 };

  int hoveredRank = -1;
  int hoveredFile = -1;

  Piece selectedPiece = EMPTY;
  int selectedRank = -1;
  int selectedFile = -1;

  InitWindow(screenWidth, screenHeight, "Chess asf");

  SetTargetFPS(60);

  board *B = boardNew(NULL);

  loadTextures();

  srand(time(NULL));

  // main game loop
  while (!WindowShouldClose()) {
    // update values
    // ------------------------------------------------------------------------
    hoveredRank = 8 - (GetMouseY() - boardPadding)/squareSize;
    if (GetMouseY() < boardPadding) hoveredRank -= 1;
    hoveredFile = 1 + (GetMouseX() - boardPadding - boardLeftOffset)/squareSize;
    if (GetMouseX() < boardPadding + boardLeftOffset) hoveredFile -= 1;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      if (selectedRank == hoveredRank && selectedFile == hoveredFile) {
        selectedRank = -1;
        selectedFile = -1;
        selectedPiece = EMPTY;
      } else if (selectedPiece == EMPTY) {
        selectedPiece = boardGetLocation(B, hoveredRank, hoveredFile);
        if (B->whiteToMove == pieceIsWhite(selectedPiece)) {
          selectedRank = hoveredRank;
          selectedFile = hoveredFile;
        } else {
          selectedPiece = EMPTY;
        }
      } else {
        move *M = moveNew(selectedRank, selectedFile,
                          hoveredRank, hoveredFile, B);
        if (isLegalMove(B, M)) {
          boardMove(B, M);
          selectedRank = -1;
          selectedFile = -1;
          selectedPiece = EMPTY;
        }
        moveFree(M);
      }
    }

    // make computer move
    if (!B->whiteToMove) {
      moveBank *allMoves = getPlayerMoves(B, false);
      int i = rand() % (allMoves->len);
      printf("Getting move %d (of %zu total)\n", i, allMoves->len);
      printMove(allMoves->arr[i]);
      boardMove(B, allMoves->arr[i]);
      moveBankFree(allMoves, true);
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

      DrawRectangleLines(left + 5, top + 5,
          squareSize - 10, squareSize - 10, BLACK);
    }

    if (1 <= selectedRank && selectedRank <= 8 &&
        1 <= selectedFile && selectedFile <= 8) {

      int left = boardLeftOffset + boardPadding
                 + squareSize*(selectedFile - 1);
      int top = boardPadding + squareSize*(8 - selectedRank);

      DrawRectangleLines(left + 5, top + 5, 
          squareSize - 10, squareSize - 10, BLACK);
      DrawRectangleLines(left + 10, top + 10, 
          squareSize - 20, squareSize - 20, BLACK);

      // this is horrible, evil code where I ignore all rules of everything :(
      moveBank *movesToDraw = getMoves(B, selectedRank, selectedFile);
      for (int i = 0; i < movesToDraw->len; i++) {
        move *curr = movesToDraw->arr[i];
        size_t rankHighlight = curr->endRank;
        size_t fileHighlight = curr->endFile;
        int left = boardLeftOffset + boardPadding
                   + squareSize*(fileHighlight - 1);
        int top = boardPadding + squareSize*(8 - rankHighlight);

        DrawRectangle(left, top, squareSize, squareSize, POSSIBLE_MOVE_COLOR);
      }
    }

    EndDrawing();
    // ------------------------------------------------------------------------
  }

  // de-initialization
  // --------------------------------------------------------------------------
  CloseWindow();
  boardFree(B);
  unloadTextures();
  // --------------------------------------------------------------------------

  return 0;
}

