#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int NUM_SHIPS = 10; // ���������� ��������
const int NUM_DIRECTIONS = 4; //�-�� �����������
const int BOARD_SIZE = 10; //������ �����
const char SHIP_SYMBOL = '#'; //������ �������
const char HIT_SYMBOL = '*'; //������ ���������
const char MISS_SYMBOL = 'X'; //������ �������

//��������� ��������
struct Ship {
    int size; //������ (�-�� �����)
    bool vertical; //���������� (���������� ������������)
};

// ��������� ��� �������� ��������� ���������� ��������� ����������
struct LastHit {
    int x;
    int y;
    int direction; // 0 - �����, 1 - ����, 2 - �����, 3 - ������

    LastHit() : x(-1), y(-1), direction(-1) {}
};

// ��������� ��� �������� ��������� ���������� ��������� ������
struct PlayerLastHit {
    int x;
    int y;
    int direction; // 0 - �����, 1 - ����, 2 - �����, 3 - ������

    PlayerLastHit() : x(-1), y(-1), direction(-1) {}
};

//�������� �����
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = '.';
        }
    }
}

//����������� ��������
void displayBoard(const char playerBoard[BOARD_SIZE][BOARD_SIZE], const char computerBoard[BOARD_SIZE][BOARD_SIZE]) {
    cout << "\nYour board:\n";
    cout << "   Player's Board        Computer's Board\n";
    cout << "   ";
    for (int i = 0; i < BOARD_SIZE; ++i) {
        cout << "\033[1;33m" << i << " "; // ������ ���� ��� ���������� ��������
    }
    cout << "  ";
    for (int i = 0; i < BOARD_SIZE; ++i) {
        cout << "\033[1;33m" << i << " "; // ������
    }
    cout << "\033[0m\n"; // ����� �����

    for (int i = 0; i < BOARD_SIZE; ++i) {
        cout << "\033[1;33m" << i << "  "; // ������

        for (int j = 0; j < BOARD_SIZE; ++j) {
            char playerSymbol = playerBoard[i][j];
            char computerSymbol = computerBoard[i][j];

            // ����������� ������� ����� ������
            if (playerSymbol == SHIP_SYMBOL) {
                cout << "\033[1;34m" << playerSymbol << " "; // ����� ���� ��� �������� ������
            }
            else if (playerSymbol == HIT_SYMBOL) {
                cout << "\033[1;31m" << playerSymbol << " "; // ������� ���� ��� ��������� ������
            }
            else if (playerSymbol == MISS_SYMBOL) {
                cout << "\033[1;90m" << playerSymbol << " "; // ����� ���� ��� �������� ������
            }
            else {
                cout << playerSymbol << " ";
            }
            cout << "\033[0m"; // ����� �����
        }
        cout << "  ";

        for (int j = 0; j < BOARD_SIZE; ++j) {
            char computerSymbol = computerBoard[i][j];

            // ����������� ������� ����� ����������
            if (computerSymbol == HIT_SYMBOL) {
                cout << "\033[1;31m" << computerSymbol << " "; // �������
            }
            else if (computerSymbol == MISS_SYMBOL) {
                cout << "\033[1;90m" << computerSymbol << " "; // �����
            }
            else {
                cout << ". "; // ���������� ���������� ������ ���� ���������� ��� �����
            }
            cout << "\033[0m"; // ����� �����
        }
        cout << "\033[1;33m" << i << endl; // ������
    }
    cout << "\033[0m"; // ����� ����� 
}

//�������� �� ������������ ����������
bool isValidPlacement(const char playerBoard[BOARD_SIZE][BOARD_SIZE], int x, int y, int size, bool vertical) {
    if (vertical) {
        if (y + size > BOARD_SIZE) {
            return false; // ������� ������� �� ������� ����� �� ���������
        }
        // ��������� ������ ������, ������� ������ ������� �� ���������
        for (int i = y; i < y + size; ++i) {
            if (playerBoard[i][x] != '.') {
                return false; // ������ ��� ������ ������ ��������
            }
            if (x > 0 && playerBoard[i][x - 1] != '.') {
                return false; // ���� �������� ������� �����
            }
            if (x < BOARD_SIZE - 1 && playerBoard[i][x + 1] != '.') {
                return false; // ���� �������� ������� ������
            }
            if (i > 0 && playerBoard[i - 1][x] != '.') {
                return false; // ���� �������� ������� ������
            }
            if (i < BOARD_SIZE - 1 && playerBoard[i + 1][x] != '.') {
                return false; // ���� �������� ������� �����
            }
            if ((x > 0 && playerBoard[i - 1][x - 1] != '.') || (x < BOARD_SIZE - 1 && playerBoard[i - 1][x + 1] != '.')) {
                return false; // ���� �������� ������� �� ��������� ������
            }
            if ((x > 0 && playerBoard[i + 1][x - 1] != '.') || (x < BOARD_SIZE - 1 && playerBoard[i + 1][x + 1] != '.')) {
                return false; // ���� �������� ������� �� ��������� �����
            }
        }
    }
    else {
        if (x + size > BOARD_SIZE) {
            return false; // ������� ������� �� ������� ����� �� �����������
        }
        // ��������� ������ ������, ������� ������ ������� �� �����������
        for (int i = x; i < x + size; ++i) {
            if (playerBoard[y][i] != '.') {
                return false; // ������ ��� ������ ������ ��������
            }
            if (y > 0 && playerBoard[y - 1][i] != '.') {
                return false; // ���� �������� ������� ������
            }
            if (y < BOARD_SIZE - 1 && playerBoard[y + 1][i] != '.') {
                return false; // ���� �������� ������� �����
            }
            if (i > 0 && playerBoard[y][i - 1] != '.') {
                return false; // ���� �������� ������� �����
            }
            if (i < BOARD_SIZE - 1 && playerBoard[y][i + 1] != '.') {
                return false; // ���� �������� ������� ������
            }
            if ((y > 0 && playerBoard[y - 1][i - 1] != '.') || (y < BOARD_SIZE - 1 && playerBoard[y + 1][i - 1] != '.')) {
                return false; // ���� �������� ������� �� ��������� �����
            }
            if ((y > 0 && playerBoard[y - 1][i + 1] != '.') || (y < BOARD_SIZE - 1 && playerBoard[y + 1][i + 1] != '.')) {
                return false; // ���� �������� ������� �� ��������� ������
            }
        }
    }
    return true; // ���������� ������� ���������
}

//��������� ������� �� �����
void placeShip(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int size, bool vertical) {
    if (vertical) {
        for (int i = y; i < y + size; ++i) {
            board[i][x] = SHIP_SYMBOL;
        }
    }
    else {
        for (int i = x; i < x + size; ++i) {
            board[y][i] = SHIP_SYMBOL;
        }
    }
}

//������ ����������� �������� ������
void manuallyPlaceShips(char playerBoard[BOARD_SIZE][BOARD_SIZE], const Ship ships[], char computerBoard[BOARD_SIZE][BOARD_SIZE], int numShips) {
    for (int shipIndex = 0; shipIndex < numShips; ++shipIndex) {
        const auto& ship = ships[shipIndex];
        bool placed = false;
        while (!placed) {
            cout << "Enter the starting coordinates (x y) for a ship of size " << ship.size << ": ";
            int x, y;
            cin >> x >> y;

            cout << "Enter orientation (0 for horizontal, 1 for vertical): ";
            bool vertical;
            cin >> vertical;

            if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
                cout << "Invalid coordinates. Try again.\n";
                continue;
            }

            if (!isValidPlacement(playerBoard, x, y, ship.size, vertical)) {
                cout << "Invalid placement. Try again.\n";
                continue;
            }

            placeShip(playerBoard, x, y, ship.size, vertical);
            placed = true;

            // ������� ������� ���� ����� ���������� �������� �������
            cout << "Your board after placing ship " << shipIndex + 1 << ":\n";
            displayBoard(playerBoard, computerBoard);
        }
    }
}

//��������������� �������� ������
void randomlyPlaceShips(char board[BOARD_SIZE][BOARD_SIZE], const Ship ships[], int numShips) {
    for (int shipIndex = 0; shipIndex < numShips; ++shipIndex) {
        const Ship& ship = ships[shipIndex];
        bool placed = false;
        while (!placed) {
            int x = rand() % BOARD_SIZE;
            int y = rand() % BOARD_SIZE;
            bool vertical = rand() % 2 == 0;

            if (isValidPlacement(board, x, y, ship.size, vertical)) {
                placeShip(board, x, y, ship.size, vertical);
                placed = true;
            }
        }
    }
}

//�������������� ��� ����������
void computerMove(char playerBoard[BOARD_SIZE][BOARD_SIZE], int& playerRemainingCells, LastHit& lastHit, int& computerMoves, int& computerMovesGood, int& computerMovesBad) {
    int x = lastHit.x;
    int y = lastHit.y;

    // ������ ��� �������� ��������� ����������� ��������
    const int NUM_DIRECTIONS = 4;
    int directions[NUM_DIRECTIONS] = { 0, 1, 2, 3 };

    // ���������, ���� �� ������� ������ ���������� ���������
    int availableDirections[NUM_DIRECTIONS];
    int numAvailableDirections = 0;

    for (int i = 0; i < NUM_DIRECTIONS; ++i) {
        int dir = directions[i];
        int nx = x, ny = y;

        switch (dir) {
        case 0: // �����
            ny--;
            break;
        case 1: // ����
            ny++;
            break;
        case 2: // �����
            nx--;
            break;
        case 3: // ������
            nx++;
            break;
        }

        // ���������, ��� ����� ���������� � �������� ����� � ������ �� ���� ���������
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE &&
            playerBoard[ny][nx] != HIT_SYMBOL && playerBoard[ny][nx] != MISS_SYMBOL) {
            availableDirections[numAvailableDirections++] = dir;
        }
    }

    // �������� ��������� ��������� ����������� ��������
    if (numAvailableDirections > 0) {
        int randomDirectionIndex = rand() % numAvailableDirections;
        int randomDirection = availableDirections[randomDirectionIndex];

        switch (randomDirection) {
        case 0: // �����
            y--;
            break;
        case 1: // ����
            y++;
            break;
        case 2: // �����
            x--;
            break;
        case 3: // ������
            x++;
            break;
        }
    }
    else {
        // ���� ��� ��������� �����������, �������� ��������� ������
        do {
            x = rand() % BOARD_SIZE;
            y = rand() % BOARD_SIZE;
        } while (playerBoard[y][x] == HIT_SYMBOL || playerBoard[y][x] == MISS_SYMBOL);
    }

    if (playerBoard[y][x] == SHIP_SYMBOL) {
        playerBoard[y][x] = HIT_SYMBOL;
        cout << "\nComputer hit at (" << x << ", " << y << ")!\n";
        computerMoves++;
        computerMovesGood++;
        playerRemainingCells--; // ��������� ���������� ���������� ������ �������� ������
        lastHit.x = x;
        lastHit.y = y;
    }
    else {
        playerBoard[y][x] = MISS_SYMBOL;
        cout << "\nComputer missed at (" << x << ", " << y << ")!\n";
        computerMoves++;
        computerMovesBad++;
        lastHit.direction = (lastHit.direction + 1) % 4; // ��������� � ���������� �����������
    }
}

//�������������� ��� ������
void autoPlayerMove(char computerBoard[BOARD_SIZE][BOARD_SIZE], int& computerRemainingCells, PlayerLastHit& lastHit, int& playerMoves, int& playerMovesGood, int& playerMovesBad) {
    // ��������� ������� ������ ������� �������������� ���
    cout << "\nDo you want to make an automatic move? (Y/N): ";
    char autoChoice;
    cin >> autoChoice;

    if (autoChoice == 'Y' || autoChoice == 'y') {
        int x = lastHit.x;
        int y = lastHit.y;

        // ������ ��� �������� ��������� ����������� ��������
        const int NUM_DIRECTIONS = 4;
        int directions[NUM_DIRECTIONS] = { 0, 1, 2, 3 };

        // ���������, ���� �� ������� ������ ���������� ���������
        int availableDirections[NUM_DIRECTIONS];
        int numAvailableDirections = 0;

        for (int i = 0; i < NUM_DIRECTIONS; ++i) {
            int dir = directions[i];
            int nx = x, ny = y;

            switch (dir) {
            case 0: // �����
                ny--;
                break;
            case 1: // ����
                ny++;
                break;
            case 2: // �����
                nx--;
                break;
            case 3: // ������
                nx++;
                break;
            }

            // ���������, ��� ����� ���������� � �������� ����� � ������ �� ���� ���������
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE &&
                computerBoard[ny][nx] != HIT_SYMBOL && computerBoard[ny][nx] != MISS_SYMBOL) {
                availableDirections[numAvailableDirections++] = dir;
            }
        }

        // �������� ��������� ��������� ����������� ��������
        if (numAvailableDirections > 0) {
            int randomDirectionIndex = rand() % numAvailableDirections;
            int randomDirection = availableDirections[randomDirectionIndex];

            switch (randomDirection) {
            case 0: // �����
                y--;
                break;
            case 1: // ����
                y++;
                break;
            case 2: // �����
                x--;
                break;
            case 3: // ������
                x++;
                break;
            }
        }
        else {
            // ���� ��� ��������� �����������, �������� ��������� ������
            do {
                x = rand() % BOARD_SIZE;
                y = rand() % BOARD_SIZE;
            } while (computerBoard[y][x] == HIT_SYMBOL || computerBoard[y][x] == MISS_SYMBOL);
        }

        if (computerBoard[y][x] == SHIP_SYMBOL) {
            computerBoard[y][x] = HIT_SYMBOL;
            cout << "\nPlayer (with help) hit at (" << x << ", " << y << ")!\n";
            playerMoves++;
            playerMovesGood++;
            computerRemainingCells--; // ��������� ���������� ���������� ������ �������� ������
            lastHit.x = x;
            lastHit.y = y;
        }
        else {
            computerBoard[y][x] = MISS_SYMBOL;
            cout << "\nPlayer missed at (" << x << ", " << y << ")!\n";
            playerMoves++;
            playerMovesBad++;
            lastHit.direction = (lastHit.direction + 1) % 4; // ��������� � ���������� �����������
        }
    }
    else {
        // ����������� ���������� ��� ������� ����
        int x, y;
        bool validCoordinates = false;

        do {
            cout << "\nEnter coordinates to fire (x y): ";
            cin >> x >> y;

            if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
                cout << "Invalid coordinates. Try again.\n";
            }
            else {
                validCoordinates = true;
            }
        } while (!validCoordinates);

        if (computerBoard[y][x] == SHIP_SYMBOL) {
            computerBoard[y][x] = HIT_SYMBOL;
            cout << "\nPlayer hit at (" << x << ", " << y << ")!\n";
            playerMoves++;
            playerMovesGood++;
            computerRemainingCells--; // ��������� ���������� ���������� ������ �������� ����������
        }
        else {
            computerBoard[y][x] = MISS_SYMBOL;
            cout << "\nPlayer missed at (" << x << ", " << y << ")!\n";
            playerMoves++;
            playerMovesBad++;
        }
    }
}

//����������� ������� ����������� �������� ������
void setShips(char playerBoard[BOARD_SIZE][BOARD_SIZE], char computerBoard[BOARD_SIZE][BOARD_SIZE], Ship ships[NUM_SHIPS]) {
    cout << "Welcome to Battleship!\n";
    cout << "Do you want to manually place your ships? (Y/N): ";
    char placementChoice;
    cin >> placementChoice;

    if (placementChoice == 'Y' || placementChoice == 'y') {
        cout << "Place your ships:\n";
        manuallyPlaceShips(playerBoard, ships, computerBoard, NUM_SHIPS);
    }
    else {
        randomlyPlaceShips(playerBoard, ships, NUM_SHIPS);
    }
    randomlyPlaceShips(computerBoard, ships, NUM_SHIPS); // ����������� ������� ����������

    cout << "All ships placed. Let the game begin!\n";
}

//��������, �� �������� �� ����
bool doEndGame(int playerRemainingCells, int computerRemainingCells, bool& endGame) {
    // ��������� ������� ���������� ���� ����� ������� ����
    if (playerRemainingCells == 0) {
        cout << "\n\033[1;31mYou lost! Computer destroyed all your ships.\033[0m\n";
        endGame = true;
        return endGame;
    }
    else if (computerRemainingCells == 0) {
        cout << "\n\033[1;32mCongratulations! You destroyed all computer's ships.\033[0m\n";
        endGame = true;
        return endGame;
    }
}

void results(int& elapsedTime, int& playerMoves, int& playerMovesGood, int& playerMovesBad, int& computerMoves, int& computerMovesGood, int& computerMovesBad) {
    cout << "Elapsed time: " << elapsedTime << " seconds\n";
    cout << "Player moves: " << playerMoves << endl;
    cout << "Computer moves: " << computerMoves << endl;
    cout << "Player total moves " << playerMoves << ", where " << playerMovesGood << " hits and " << playerMovesBad << " miss." << endl;
    cout << "Computer total moves " << computerMoves << ", where " << computerMovesGood << " hits and " << computerMovesBad << " miss." << endl;

    ofstream outFile("game_results.txt", ios::app); // ��������� ���� ��� ������ (ios::app ��� ���������� � ����� �����)
    if (!outFile.is_open()) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    // �������� ������� ���� � �����
    time_t currentTime;
    char timeBuffer[26];
    time(&currentTime);
    ctime_s(timeBuffer, sizeof(timeBuffer), &currentTime);

    // ���������� ���������� ���� � ����
    outFile << "Game results (" << timeBuffer << "):" << endl;
    outFile << "Total player`s moves: " << playerMoves << endl;
    outFile << "Successful player`s moves: " << playerMovesGood << endl;
    outFile << "Failed player`s moves: " << playerMovesBad << endl;
    outFile << "Total computer`s moves: " << computerMoves << endl;
    outFile << "Successful computer`s moves: " << computerMovesGood << endl;
    outFile << "Failed computer`s moves: " << computerMovesBad << endl;


    outFile << "-----------------------------------" << endl;

    cout << "File with results has been updated." << endl;

    outFile.close(); // ��������� ����
}


int main() {
    srand(time(nullptr));

    // ������ �������
    time_t startTime = time(nullptr);

    int playerMoves = 0;
    int playerMovesGood = 0;
    int playerMovesBad = 0;
    int computerMoves = 0;
    int computerMovesGood = 0;
    int computerMovesBad = 0;

    int playerRemainingCells = 20;
    int computerRemainingCells = 20;

    bool endGame = false;

    char playerBoard[BOARD_SIZE][BOARD_SIZE];
    char computerBoard[BOARD_SIZE][BOARD_SIZE];

    initializeBoard(playerBoard);
    initializeBoard(computerBoard);

    Ship ships[NUM_SHIPS] = { {1, true}, {1, false}, {1, true}, {1, false},
                          {2, true}, {2, false}, {2, true},
                          {3, false}, {3, true},
                          {4, false} };

    setShips(playerBoard, computerBoard, ships);


    // ������������� ��������� ���������
    LastHit lastHit;
    PlayerLastHit playerLastHit;



    //�������� ���� ����
    while (!endGame) {
        //��������, �� �������� �� ����
        doEndGame(playerRemainingCells, computerRemainingCells, endGame);

        //����������� �����
        displayBoard(playerBoard, computerBoard);

        //�������� ������� ������� ����� ��� ��������� ����
        if (endGame) {
            time_t endTime = time(nullptr);
            int elapsedTime = difftime(endTime, startTime);
            results(elapsedTime, playerMoves, playerMovesGood, playerMovesBad, computerMoves, computerMovesGood, computerMovesBad);
            break;
        }

        //��� ������
        autoPlayerMove(computerBoard, computerRemainingCells, playerLastHit, playerMoves, playerMovesGood, playerMovesBad);
        
        // ��� ����������
        computerMove(playerBoard, playerRemainingCells, lastHit, computerMoves, computerMovesGood, computerMovesBad);
    }
    return 0;
}