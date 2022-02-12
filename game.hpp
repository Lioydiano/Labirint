#include <iostream>
#include <cstdlib>
#include "time.h"


struct Game {
    char matrix[20][50]; // Il campo di gioco
    unsigned points; // Punteggio del giocatore (unsigned = int ma senza i negativi)
    std::string name; // Nome del giocatore
    char skin; // Iniziale del giocatore, da usare come pedina
    int x; // Posizione in orizzontale della pedina
    int y; // Posizione in verticale della pedina
    bool emptyLine; // Una riga su due sarà stampata vuota, quando è true si stampa una riga vuota
    unsigned maxCloudWidth; // Massima larghezza delle nuvole
    unsigned bonus; // Una riga su Game.bonusFrequency potrebbe contenere un bonus
    unsigned bonusFrequency; // Frequenza dei bonus
    unsigned borderCounter; // Contatore per l'array della lunghezza del bordo sinistro
};
const int bordersLen = 48;
unsigned borders[bordersLen] = {
    7, 7,
    6, 6, 6,
    5, 5, 5, 5,
    4, 4, 4, 4, 4,
    3, 3, 3, 3,
    2, 2, 2,
    1, 1,
    0,
    1, 1,
    2, 2, 2,
    3, 3, 3, 3,
    4, 4, 4, 4, 4,
    5, 5, 5, 5,
    6, 6, 6,
    7, 7,
    8
};


void updateMatrix(Game &game_) {
    // Scaliamo tutto di una riga
    for (int i = 1; i < 20; i++) {
    	for (int j = 0; j < 50; j++)
        	game_.matrix[i-1][j] = game_.matrix[i][j];
    }

    // Trasformiamo la pedina vecchia in uno spazio
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 20; j++) {
            if (game_.matrix[j][i] == game_.skin) {
                game_.matrix[j][i] = ' ';
                break;
            }
        }
    }
    game_.matrix[game_.y][game_.x] = game_.skin;

    // Generiamo casualmente la nuova riga
    char newLine[50];
    for (int i = 0; i < 50; i++)
        newLine[i] = ' ';
    if (game_.emptyLine) {
        game_.emptyLine = false;
        int cloudBeginning = (rand() % (34-(borders[game_.borderCounter]))) + 15 + (borders[game_.borderCounter]);
        int cloudWidth = rand() % game_.maxCloudWidth;
        int cloudEnd = cloudBeginning + cloudWidth;
        for (int i = cloudBeginning; i < cloudEnd+1; i++)
            newLine[i] = '*';
    } else {
        game_.emptyLine = true;
    }
    if (game_.bonus == game_.bonusFrequency) {
        game_.bonus = 0;
        int bonus = rand() % 34 + 15; // Bonus in quelle posizioni (0-15) sarebbero comunque coperti dai bordi
        newLine[bonus] = '$';
    } else {
        game_.bonus++;
    }
    int endLeftBorder = (borders[game_.borderCounter])+15;
    int beginRightBorder = 49-(30-endLeftBorder);
    for (int i = 1; i < endLeftBorder; i++)
        newLine[i] = '-';
    for (int i = beginRightBorder; i < 49; i++)
        newLine[i] = '-';
    newLine[0] = '#';
    newLine[49] = '#';
    for (int i = 0; i < 50; i++)
    	game_.matrix[19][i] = newLine[i];
    
    // Border
    if (game_.borderCounter == bordersLen-1) {
        game_.borderCounter = 0;
    } else {
        game_.borderCounter++;
    }

    game_.points++;
}


void printMatrix(Game game_) {
    system("cls");
    
    // Stampo la matrice
    for (int i = 0; i < 50; i++)
        std::cout << '#';
    std::cout << std::endl;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 50; j++)
            std::cout << game_.matrix[i][j];
        std::cout << std::endl;
    }
    for (int i = 0; i < 50; i++)
        std::cout << '#';
    std::cout << std::endl << game_.points << std::endl;
}


void processMove(Game &game_, std::string input) {
    // Horizontal
    if (input == "s" or input == "S")
        game_.x--;
    if (input == "d" or input == "D")
        game_.x++;
    if (input == "ss" or input == "SS")
		game_.x -= 2;
    if (input == "dd" or input == "DD")
        game_.x += 2;
    // Vertical
    if (input == "a" or input == "A")
        game_.y--;
    if (input == "b" or input == "B")
        game_.y++;

    // Effetto pacman verticale
    if (game_.y <= 0)
        game_.y = 17;
    if (game_.y > 17)
        game_.y = 1;
}


bool checkMatrix(Game &game_) {
    if (game_.matrix[game_.y+1][game_.x] == '*')
        return true; // Il giocatore ha perso
    if (game_.matrix[game_.y+1][game_.x] == '-')
        return true; // Il giocatore ha perso
    if (game_.matrix[game_.y+1][game_.x] == '$')
        game_.points += 10;
    return false;
}