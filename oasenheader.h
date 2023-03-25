#ifndef OASENHEADER_H_INCLUDED
#define OASENHEADER_H_INCLUDED
#include <iostream>
#include <ctime> // rand
#include <cassert> // assert

const int MAX_X = 5;
const int MAX_Y = 5;
const int mob_spawn = 1; //enemy distance
const int h_l = 50; // store history array

using namespace std;

class Character {
private:
    int x_;
    int y_;
    int health_;
    int relics_;
    int regen_;
    int dmgtaken_;
    int rounds_;
    int item_;
    int str_;
    int dex_;
    int int_;

public:
    Character(int x, int y, int health, int relics, int regen, int dmgtaken, int rounds, int item, int str, int dex, int int_)
        : x_(x), y_(y), health_(health), relics_(relics), regen_(regen), dmgtaken_(dmgtaken), rounds_(rounds), item_(item),
          str_(str), dex_(dex), int_(int_) {}

    int getX() { return x_; }               //getter
    int getY() { return y_; }
    int getHealth() { return health_; }
    int getRelics() { return relics_; }
    int getRegen() { return regen_; }
    int getDmgTaken() { return dmgtaken_; }
    int getRounds() { return rounds_; }
    int getItem() { return item_; }
    int getStr() { return str_; }
    int getDex() { return dex_; }
    int getInt() { return int_; }

    void setX(int x) { x_ = x; }            //setter
    void setY(int y) { y_ = y; }
    void setHealth(int health) { health_ = health; }
    void setRelics(int relics) { relics_ = relics; }
    void setRegen(int regen) { regen_ = regen; }
    void setDmgTaken(int dmgTaken) { dmgtaken_ = dmgTaken; }
    void setRounds(int rounds) { rounds_ = rounds; }
    void setItem(int item) { item_ = item; }
    void setStr(int str) { str_ = str; }
    void setDex(int dex) { dex_ = dex; }
    void setInt(int int_) { int_ = int_; }
};

class Enemy {
private:
    int x_;
    int y_;
    int dmg_;
    char pastblock_;

public:
    Enemy() : dmg_(1) {}

    int getX() { return x_; }
    int getY() { return y_; }
    int getDmg() { return dmg_; }
    char getPastBlock() { return pastblock_; }

    void setX(int x) { x_ = x; }
    void setY(int y) { y_ = y; }
    void setDmg(int dmg) { dmg_ = dmg; }
    void setPastBlock(char pastBlock) { pastblock_ = pastBlock; }
};

class GameWorld {
private:
    Character player;
    Enemy monster;
    int maxrelic;
    char** matrix; // use pointer to pointer to allocate 2D array dynamically
    char stats[h_l];
    int index[h_l];
    int cnt;
    int level;
    int status;

public:
    GameWorld() :
        player(0, 0, 5, 0, 0 , 0, 0, 0, rand()%10 + 1, rand()%10 + 1, rand()%10 + 1),
        maxrelic(0), cnt(0), level(1), status(1)
        {
        matrix = new char*[MAX_X]; // allocate row pointers dynamically
        for (int i = 0; i < MAX_X; i++) {
            matrix[i] = new char[MAX_Y]; // allocate column elements dynamically
        }
        GenGrid();
        matrix[0][0] = 'P';

        assert(maxrelic > 0);
        assert(matrix[0][0] == 'P');

        SpawnMonster();
        cout << endl << "Total relics in the game: " << maxrelic << endl;
        cout << endl << "STR:" << player.getStr();
        cout << endl << "INT:" << player.getInt() ;
        cout << endl << "DEX:" << player.getDex() << endl;
    }

    void DisplayGame();
    void GenBlock(char &block);
    void GenGrid();
    int GenMonsterCords(int cord);
    void SpawnMonster();
    void PrintStats();
    void Statistics(char status);
    void PrintStatistics();
    void checkDangerBlock(char block, int playerstats);
    void findItem();
    void checkBlock(int x, int y);
    void movePlayer(int x, int y);
    void levelUp();
    bool checkMonster();
    void moveMonster();
    void dostuff();
    void playGame();

    ~GameWorld() {
        for (int i = 0; i < MAX_X; i++) {
            delete[] matrix[i]; // delete column elements
        }
        delete[] matrix; // delete row pointers
    }
};

#endif // OASENHEADER_H_INCLUDED
