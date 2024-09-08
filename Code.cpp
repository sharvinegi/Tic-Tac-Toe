#include <iostream>
#include <ctime>
#include <cstdlib>  // For std::rand and std::srand

// Function prototypes
void drawBoard(const char *spaces);
void playerMove(char *spaces, char player);
void computerMove(char *spaces, char computer);
bool checkWinner(const char *spaces, char player, char computer);
bool checkTie(const char *spaces);
bool checkWinCondition(const char *spaces, char marker, int x, int y, int z);

// Main function
int main() {
    char spaces[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};  // Initialize the board
    char player = 'X';  // Player's marker
    char computer = 'O';  // Computer's marker
    bool running = true;

    drawBoard(spaces);

    while (running) {
        playerMove(spaces, player);
        drawBoard(spaces);

        // Check for winner or tie after player's move
        if (checkWinner(spaces, player, computer)) {
            running = false;
            break;
        } else if (checkTie(spaces)) {
            running = false;
            break;
        }

        computerMove(spaces, computer);
        drawBoard(spaces);

        // Check for winner or tie after computer's move
        if (checkWinner(spaces, player, computer)) {
            running = false;
            break;
        } else if (checkTie(spaces)) {
            running = false;
            break;
        }
    }

    std::cout << "Thanks for playing!\n";
    return 0;
}

// Function to draw the board
void drawBoard(const char *spaces) {
    std::cout << '\n';
    std::cout << "     |     |     " << '\n';
    std::cout << "  " << spaces[0] << "  |  " << spaces[1] << "  |  " << spaces[2] << "  " << '\n';
    std::cout << "_____|_____|_____" << '\n';
    std::cout << "     |     |     " << '\n';
    std::cout << "  " << spaces[3] << "  |  " << spaces[4] << "  |  " << spaces[5] << "  " << '\n';
    std::cout << "_____|_____|_____" << '\n';
    std::cout << "     |     |     " << '\n';
    std::cout << "  " << spaces[6] << "  |  " << spaces[7] << "  |  " << spaces[8] << "  " << '\n';
    std::cout << "     |     |     " << '\n';
    std::cout << '\n';
}

// Function to handle player's move
void playerMove(char *spaces, char player) {
    int number;
    do {
        std::cout << "Enter a spot to place a marker (1-9): ";
        std::cin >> number;
        number--;  // Convert to 0-based index
        if (number < 0 || number > 8) {
            std::cout << "Invalid input. Please enter a number between 1 and 9.\n";
        } else if (spaces[number] == ' ') {
            spaces[number] = player;
            break;
        } else {
            std::cout << "Spot already taken. Try again.\n";
        }
    } while (true);
}

// Function to check if there is a win condition
bool checkWinCondition(const char *spaces, char marker, int x, int y, int z) {
    return spaces[x] == marker && spaces[y] == marker && spaces[z] == marker;
}

// Function to handle computer's move with enhanced strategy
void computerMove(char *spaces, char computer) {
    char player = (computer == 'O') ? 'X' : 'O';
    const int winPatterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // Columns
        {0, 4, 8}, {2, 4, 6}  // Diagonals
    };

    int bestMove = -1;

    // 1. Check if the computer can win immediately
    for (const auto& pattern : winPatterns) {
        int emptySpot = -1;
        int countComputer = 0, countEmpty = 0;
        for (int i = 0; i < 3; i++) {
            if (spaces[pattern[i]] == computer) countComputer++;
            else if (spaces[pattern[i]] == ' ') {
                emptySpot = pattern[i];
                countEmpty++;
            }
        }
        if (countComputer == 2 && countEmpty == 1) {
            spaces[emptySpot] = computer;
            return;
        }
    }

    // 2. Block the player's winning move
    for (const auto& pattern : winPatterns) {
        int emptySpot = -1;
        int countPlayer = 0, countEmpty = 0;
        for (int i = 0; i < 3; i++) {
            if (spaces[pattern[i]] == player) countPlayer++;
            else if (spaces[pattern[i]] == ' ') {
                emptySpot = pattern[i];
                countEmpty++;
            }
        }
        if (countPlayer == 2 && countEmpty == 1) {
            spaces[emptySpot] = computer;
            return;
        }
    }

    // 3. Play the center if available
    if (spaces[4] == ' ') {
        spaces[4] = computer;
        return;
    }

    // 4. Play a corner if available
    const int corners[4] = {0, 2, 6, 8};
    for (int corner : corners) {
        if (spaces[corner] == ' ') {
            spaces[corner] = computer;
            return;
        }
    }

    // 5. Play a random move if no other move is found
    std::srand(static_cast<unsigned>(std::time(0)));  // Seed the random number generator
    while (true) {
        int number = std::rand() % 9;  // Random number between 0 and 8
        if (spaces[number] == ' ') {
            spaces[number] = computer;
            break;
        }
    }
}

// Function to check if there's a winner
bool checkWinner(const char *spaces, char player, char computer) {
    // Check rows, columns, and diagonals for a win
    const int winPatterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // Columns
        {0, 4, 8}, {2, 4, 6}  // Diagonals
    };

    for (const auto& pattern : winPatterns) {
        if (checkWinCondition(spaces, player, pattern[0], pattern[1], pattern[2])) {
            std::cout << "YOU WIN!\n";
            return true;
        } else if (checkWinCondition(spaces, computer, pattern[0], pattern[1], pattern[2])) {
            std::cout << "YOU LOSE!\n";
            return true;
        }
    }
    return false;
}

// Function to check if the game is a tie
bool checkTie(const char *spaces) {
    for (int i = 0; i < 9; i++) {
        if (spaces[i] == ' ') {
            return false;  // If any spot is empty, it's not a tie
        }
    }
    std::cout << "IT'S A TIE!\n";
    return true;
}
