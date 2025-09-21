#include <array>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <thread>

using namespace std;

constexpr int ROW = 160;
constexpr int COL = 36;
constexpr int delayEachFrame = 100;

std::string_view alive = "█";
std::string_view blank = " ";

int iterationCount{0};
int populationCount{0};

void clearScreen() {
#if defined(_WIN32)
   system("cls");  // Windows
#elif defined(__linux__) || defined(__APPLE__)
system("clear");  // Linux/macOS
#else
#error "Unsupported platform"
#endif
}

template <typename T, size_t N, size_t M>
ostream& operator<<(ostream& os, array<array<T, M>, N>& matrix) {
   for (size_t i = 0; i < N; ++i) {
      os << '{';
      for (size_t j = 0; j < M; ++j) {
         os << matrix[i][j];

         if (j + 1 == M)
            os << '}';
         else
            os << ", ";
      }
      os << '\n';
   }

   return os;
}

template <size_t N, size_t M>
void printGame(const array<array<bool, M>, N>& matrix) {
   // Move cursor to top-left
   cout << "\033[H";
   // Top border
   cout << '+';
   for (size_t j = 0; j < M; ++j) cout << '-';
   cout << "+\n";

   // Each row with side borders
   for (size_t i = 0; i < N; ++i) {
      cout << '|';
      for (size_t j = 0; j < M; ++j) {
         if (matrix[i][j])
            cout << alive;
         else
            cout << blank;
      }
      cout << "|\n";
   }

   // Bottom border
   cout << '+';
   for (size_t j = 0; j < M; ++j) cout << '-';
   cout << "+\n";

   cout << "Current generation: " << iterationCount << '\n';
   cout << "Current population: " << populationCount << '\n';
}

template <typename T, size_t N, size_t M>
void initRandomMatrix2D(array<array<T, M>, N>& matrix) {
   mt19937 rng(random_device{}());
   uniform_int_distribution<int> dist(0, 9);

   for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
         if (matrix[i][j] = !dist(rng)) {
            populationCount++;
         }
      }
   }

   printGame(matrix);
}

template <size_t N, size_t M>
void countNeighbour(const array<array<bool, M>, N>& matrixInput,
                    array<array<int, M>, N>& matrixOutput) {
   for (int i = 0; i < N; ++i) {
      for (int j = 0; j < M; ++j) {
         int count{0};
         for (int di = -1; di <= 1; ++di) {
            for (int dj = -1; dj <= 1; ++dj) {
               // Center position not counted
               if (di == 0 && dj == 0) continue;

               int ni = (di + i + N) % N;
               int nj = (dj + j + M) % M;

               count += matrixInput[ni][nj];
            }
         }

         matrixOutput[i][j] = count;
      }
   }
}

template <size_t N, size_t M>
void playGame(array<array<bool, M>, N>& matrix2D) {
   array<array<int, M>, N> matrix2DInt{};
   array<array<bool, M>, N> matrix2DPrev{matrix2D};

   countNeighbour(matrix2D, matrix2DInt);

   for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
         if (matrix2DPrev[i][j] == false && matrix2DInt[i][j] == 3) {
            matrix2D[i][j] = true;
            populationCount++;
         }

         else if (matrix2DPrev[i][j] == true &&
                  (matrix2DInt[i][j] < 2 || matrix2DInt[i][j] > 3)) {
            matrix2D[i][j] = false;
            populationCount--;
         }
      }
   }

   iterationCount++;
}

template <size_t N, size_t M>
bool isExtinct(const array<array<bool, M>, N>& matrix) {
   bool allDead{true};

   for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
         if (matrix[i][j] == true) return false;
      }
   }

   return true;
}

// Conway's game of life, by John Horton Conway
int main() {
   clearScreen();
   array<array<bool, ROW>, COL> matrix2D{};

   initRandomMatrix2D(matrix2D);

   while (!isExtinct(matrix2D)) {
      playGame(matrix2D);
      printGame(matrix2D);

      std::this_thread::sleep_for(std::chrono::milliseconds(delayEachFrame));
   }

   cout << "Game has ended after " << iterationCount << " iteration.\n";

   return 0;
}
