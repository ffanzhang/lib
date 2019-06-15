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

const int candidate_mask = ((1 << 10) - 1) - 1;
static char static_board[82];
char row_index[82];
char col_index[82];
char reg_index[82];
int row[9];
int col[9];
int reg[9];
int num[513];

bool dfs(char i) {
  if (i == 81) {
    return true;
  }
  if (static_board[i] == 0) {
    int candidates = (~row[row_index[i]]) & (~col[col_index[i]]) &
                     (~reg[reg_index[i]]) & candidate_mask;
    while (candidates) {
      int bit = candidates & (~(candidates - 1));
      candidates &= (candidates - 1);
      static_board[i] = num[bit];
      row[row_index[i]] |= bit;
      col[col_index[i]] |= bit;
      reg[reg_index[i]] |= bit;
      if (dfs(i + 1) == true) {
        return true;
      }
      reg[reg_index[i]] &= (~bit);
      col[col_index[i]] &= (~bit);
      row[row_index[i]] &= (~bit);
      static_board[i] = 0;
    }
  } else {
    return dfs(i + 1);
  }
  return false;
}

char* solve(const char board[81]) {
  memcpy(static_board, board, sizeof(static_board));
  memset(row, 0, sizeof(row));
  memset(col, 0, sizeof(col));
  memset(reg, 0, sizeof(reg));
  memset(num, 0, sizeof(num));
  for (char i = 1; i <= 9; i++) {
    num[1 << i] = i;
  }
  for (int i = 0; i < 81; i++) {
    row_index[i] = i / 9;
    col_index[i] = i % 9;
    reg_index[i] = row_index[i] / 3 * 3 + col_index[i] / 3;
    int mask = 1 << static_board[i];
    row[row_index[i]] |= mask;
    col[col_index[i]] |= mask;
    reg[reg_index[i]] |= mask;
  }
  dfs(0);
  return static_board;
}

}  // namespace sudoku
