#include <bits/stdc++.h>
#include <ctime>
using namespace std;

namespace sudoku {

void print_board(char board[81]) {
  for (int j = 0; j < 81; j++) {
    cout << static_cast<int>(board[j]);
    if (j % 9 == 8) {
      cout << '\n';
    }
  }
  cout << '\n';
}

bool dfs(int i, char board[81], bool row[9][10], bool col[9][10],
         bool reg[9][10]) {
  if (i == 81) {
    return true;
  }
  bool res = false;
  if (board[i] == 0) {
    int row_index = i / 9;
    int col_index = i % 9;
    int reg_index = row_index / 3 * 3 + col_index / 3;
    for (int j = 1; j <= 9; j++) {
      if (row[row_index][j] == false && col[col_index][j] == false &&
          reg[reg_index][j] == false) {
        board[i] = j;
        row[row_index][j] = true;
        col[col_index][j] = true;
        reg[reg_index][j] = true;
        if (dfs(i + 1, board, row, col, reg) == true) {
          return true;
        }
        row[row_index][j] = false;
        col[col_index][j] = false;
        reg[reg_index][j] = false;
        board[i] = 0;
      }
    }
  } else {
    return dfs(i + 1, board, row, col, reg);
  }
  return false;
}

char* solve(const char board[81]) {
  bool row[9][10];
  bool col[9][10];
  bool reg[9][10];
  char* board_cpy = new char[81];
  memcpy(board_cpy, board, sizeof(char) * 81);
  memset(row, 0, sizeof(row));
  memset(col, 0, sizeof(col));
  memset(reg, 0, sizeof(reg));
  for (int i = 0; i < 81; i++) {
    if (board[i] != 0) {
      int row_index = i / 9;
      int col_index = i % 9;
      int reg_index = row_index / 3 * 3 + col_index / 3;
      row[row_index][board[i]] = true;
      col[col_index][board[i]] = true;
      reg[reg_index][board[i]] = true;
    }
  }
  dfs(0, board_cpy, row, col, reg);
  return board_cpy;
}

}  // namespace sudoku
