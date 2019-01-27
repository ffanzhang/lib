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

int row[9];
int col[9];
int reg[9];
unordered_map<int, int> num;
int candidate_mask = ((1 << 10) - 1) - 1;

bool dfs(int i, char board[81]) {
  if (i == 81) {
    return true;
  }
  bool res = false;
  if (board[i] == 0) {
    int row_index = i / 9;
    int col_index = i % 9;
    int reg_index = row_index / 3 * 3 + col_index / 3;
    int candidates = (~row[row_index]) & (~col[col_index]) & (~reg[reg_index]) &
                     candidate_mask;
    while (candidates) {
      int bit = candidates & (~(candidates - 1));
      board[i] = num[bit];
      row[row_index] |= bit;
      col[col_index] |= bit;
      reg[reg_index] |= bit;
      if (dfs(i + 1, board) == true) {
        return true;
      }
      row[row_index] &= (~bit);
      col[col_index] &= (~bit);
      reg[reg_index] &= (~bit);
      board[i] = 0;
      candidates &= (candidates - 1);
    }
  } else {
    return dfs(i + 1, board);
  }
  return false;
}

char* solve(const char board[81]) {
  char* board_cpy = new char[81];
  memcpy(board_cpy, board, sizeof(char) * 81);
  memset(row, 0, sizeof(row));
  memset(col, 0, sizeof(col));
  memset(reg, 0, sizeof(reg));
  for (int i = 1; i <= 9; i++) {
    num[1 << i] = i;
  }
  for (int i = 0; i < 81; i++) {
    if (board[i] != 0) {
      int row_index = i / 9;
      int col_index = i % 9;
      int reg_index = row_index / 3 * 3 + col_index / 3;
      int mask = 1 << board[i];
      row[row_index] |= mask;
      col[col_index] |= mask;
      reg[reg_index] |= mask;
    }
  }
  dfs(0, board_cpy);
  return board_cpy;
}

}  // namespace sudoku
