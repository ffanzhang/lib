#include <bits/stdc++.h>

#include <ctime>
using namespace std;

namespace sudoku {

class SudokuSolver {
 public:
  char board[81];
  virtual void Init() = 0;
  virtual void Set(const char board[81]) = 0;
  virtual bool HasUniqueSolution() = 0;
  virtual char* SolveOne() = 0;
  bool Solved() {
    if (board == nullptr) {
      return false;
    }
    for (int j = 0; j < 9; j++) {
      int sum = 0;
      for (int i = 0; i < 9; i++) {
        sum += board[j * 9 + i];
      }
      if (sum != 45) {
        return false;
      }
    }
    for (int i = 0; i < 9; i++) {
      int sum = 0;
      for (int j = 0; j < 9; j++) {
        sum += board[j * 9 + i];
      }
      if (sum != 45) {
        return false;
      }
    }
    for (int k = 0; k < 9; k++) {
      int row_start = k / 3 * 3;
      int col_start = k % 3 * 3;
      int sum = 0;
      for (int j = row_start; j < row_start + 3; j++) {
        for (int i = col_start; i < col_start + 3; i++) {
          sum += board[j * 9 + i];
        }
      }
      if (sum != 45) {
        return false;
      }
    }
    return true;
  }
  void PrintBoard() {
    for (int j = 0; j < 81; j++) {
      cout << static_cast<int>(board[j]);
      if (j % 9 == 8) {
        cout << '\n';
      }
    }
    cout << '\n';
  }
  string to_string() {
    string s;
    for (int i = 0; i < 81; i++) {
      s += (char)(board[i] + '0');
    }
    return s;
  }
};

struct BacktrackingSolver : public SudokuSolver {
  const int candidate_mask = ((1 << 10) - 1) - 1;
  char row_index[81];
  char col_index[81];
  char reg_index[81];
  int row[9], col[9], reg[9];
  int num[513];

  void Init() override {
    for (char i = 1; i <= 9; i++) {
      num[1 << i] = i;
    }
    for (char i = 0; i < 81; i++) {
      row_index[i] = i / 9;
      col_index[i] = i % 9;
      reg_index[i] = row_index[i] / 3 * 3 + col_index[i] / 3;
    }
  }

  void Set(const char other[81]) override {
    memcpy(board, other, 81 * sizeof(char));
    memset(row, 0xff, sizeof(row));
    memset(col, 0xff, sizeof(col));
    memset(reg, 0xff, sizeof(reg));
    for (int i = 0; i < 81; i++) {
      int mask = 1 << board[i];
      row[row_index[i]] ^= mask;
      col[col_index[i]] ^= mask;
      reg[reg_index[i]] ^= mask;
    }
  }

  bool HasUniqueSolution() override { return dfs(0, true, false) == 1; }

  char* SolveOne() override {
    if (dfs(0)) {
      return board;
    } else {
      return nullptr;
    }
  }

  long long dfs(char i, bool countMode = false, bool exploreAll = false) {
    long long ct = 0;
    if (i == 81) {
      return 1;
    }
    if (board[i] == 0) {
      int candidates = row[row_index[i]] & col[col_index[i]] &
                       reg[reg_index[i]] & candidate_mask;
      while (candidates) {
        int bit = candidates & (~(candidates - 1));
        candidates &= (candidates - 1);
        board[i] = num[bit];
        row[row_index[i]] ^= bit;
        col[col_index[i]] ^= bit;
        reg[reg_index[i]] ^= bit;
        if (countMode) {
          ct += dfs(i + 1, countMode, exploreAll);
          if (ct > 1 && !exploreAll) {
            return ct;
          }
        } else {
          if (dfs(i + 1, countMode, exploreAll)) {
            return 1;
          }
        }
        reg[reg_index[i]] |= bit;
        col[col_index[i]] |= bit;
        row[row_index[i]] |= bit;
        board[i] = 0;
      }
    } else {
      return dfs(i + 1, countMode, exploreAll);
    }
    return ct;
  }
};

struct MetaData {
  const int candidate_mask = ((1 << 10) - 1) - 1;
  char row_index[81];
  char col_index[81];
  char reg_index[81];
  int row[9], col[9], reg[9];
  int num[513];

  MetaData() {
    for (char i = 1; i <= 9; i++) {
      num[1 << i] = i;
    }
    for (char i = 0; i < 81; i++) {
      row_index[i] = i / 9;
      col_index[i] = i % 9;
      reg_index[i] = row_index[i] / 3 * 3 + col_index[i] / 3;
    }
    memset(row, 0xff, sizeof(row));
    memset(col, 0xff, sizeof(col));
    memset(reg, 0xff, sizeof(reg));
  }

  void SetMask(char i, int mask) {
    row[row_index[i]] |= mask;
    col[col_index[i]] |= mask;
    reg[reg_index[i]] |= mask;
  }

  void UnsetMask(char i, int mask) {
    row[row_index[i]] ^= mask;
    col[col_index[i]] ^= mask;
    reg[reg_index[i]] ^= mask;
  }

  int GetCandidates(int i) {
    return row[row_index[i]] & col[col_index[i]] & reg[reg_index[i]] &
           candidate_mask;
  }

  MetaData operator=(const MetaData& other) {
    memcpy((void*)row, (void*)other.row, sizeof(row));
    memcpy((void*)col, (void*)other.col, sizeof(col));
    memcpy((void*)reg, (void*)other.reg, sizeof(reg));
    return *this;
  }
};

MetaData dummy;

struct PriorityIndex {
  char index;
  MetaData& metadata = dummy;
  PriorityIndex(const PriorityIndex& other) {
    index = other.index;
    metadata = other.metadata;
  }
  PriorityIndex(char index, MetaData& metadata)
      : index(index), metadata(metadata) {}
  bool operator<(const PriorityIndex& other) const;
  PriorityIndex& operator=(const PriorityIndex& other) {
    index = other.index;
    metadata = other.metadata;
    return *this;
  }
};

struct HeapSolver : public SudokuSolver {
  MetaData metadata;
  int i = 0;
  std::vector<PriorityIndex> eyes;

  void Init() override {
    // pass
  }

  void Set(const char other[81]) override {
    eyes.reserve(81);
    memcpy(board, other, 81 * sizeof(char));
    metadata = MetaData();
    for (int i = 0; i < 81; i++) {
      int mask = 1 << board[i];
      metadata.UnsetMask(i, mask);
      if (board[i] == 0) {
        eyes.push_back(PriorityIndex(i, metadata));
      }
    }
    make_heap(eyes.begin(), eyes.end());
  }

  char nextI() {
    if (eyes.empty()) {
      return 81;
    }
    if (__builtin_popcount(metadata.GetCandidates(eyes.front().index)) == 0) {
      return -1;
    }
    return eyes.front().index;
  }

  bool HasUniqueSolution() override { return dfs(0, true, false) == 1; }

  char* SolveOne() override {
    if (dfs(nextI())) {
      return board;
    } else {
      return nullptr;
    }
  }

  long long dfs(char i, bool countMode = false, bool exploreAll = false) {
    long long ct = 0;
    if (i == -1) {
      return 0;
    }
    if (i == 81) {
      return 1;
    }
    if (board[i] == 0) {
      int candidates = metadata.GetCandidates(i);
      bool popped = false;
      if (eyes.size()) {
        pop_heap(eyes.begin(), eyes.end());
        eyes.pop_back();
        popped = true;
      }
      while (candidates) {
        int bit = candidates & (~(candidates - 1));
        candidates &= (candidates - 1);
        board[i] = metadata.num[bit];
        metadata.UnsetMask(i, bit);
        if (countMode) {
          ct += dfs(nextI(), countMode, exploreAll);
          if (ct > 1 && !exploreAll) {
            return ct;
          }
        } else {
          if (dfs(nextI(), countMode, exploreAll)) {
            return 1;
          }
        }
        metadata.SetMask(i, bit);
        board[i] = 0;
      }
      if (popped) {
        eyes.push_back(PriorityIndex(i, metadata));
        push_heap(eyes.begin(), eyes.end());
      }
    } else {
      return dfs(nextI(), countMode, exploreAll);
    }
    return ct;
  }
};

bool PriorityIndex::operator<(const PriorityIndex& other) const {
  return __builtin_popcount(metadata.GetCandidates(index)) >
         __builtin_popcount(metadata.GetCandidates(other.index));
}

}  // namespace sudoku
