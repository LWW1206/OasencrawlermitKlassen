#include <iostream>
#include <ctime> // rand
#include <cassert> // assert
#include "oasenheader.h"

void GameWorld::DisplayGame() {
    cout << endl;
    for(int x = 0; x < MAX_X; x++) {
        for(int y = 0; y < MAX_Y; y++) {
            if(y == 4) {
                cout << matrix[x][y] << endl;
            }
            else {
                cout << matrix[x][y] << ' ';
            }
        }
    }
}

void GameWorld::GenBlock(char &block) {
    int num = rand() % 100;
    if(num < 40) {
        block = '.'; // safe block
    }
    else if (num < 80) {
        int index = rand()%3;
        if (index == 0)
            block = 'S';
        else if(index == 1)
            block = 'D';
        else if (index == 2)
            block = 'I';
        //block = 'x'; // danger block
    }
    else if (num < 90) {
        maxrelic += 1;
        block = 'r'; // relic block
    }
    else if(num < 100) {
        block = 'f'; // fountain block
    }
}

void GameWorld::GenGrid() {
    do {
        for (int x = 0; x < MAX_X; x++) {
            for (int y = 0; y < MAX_Y; y++) {
                GenBlock(matrix[x][y]);
            }
        }
        if (matrix[player.getX()][player.getY()] == 'r') {
            maxrelic -= 1;
        }
    } while (maxrelic <= 0);

    assert(maxrelic > 0);
    //cout << maxrelic << endl;
}

int GameWorld::GenMonsterCords(int cord) {
    int num, diff;
    do {
        num = rand() % 5;
        diff = num - cord;
    } while(abs(diff) <= mob_spawn); // absolute value
    return num;
}

void GameWorld::SpawnMonster() {
    int monster_x = GenMonsterCords(player.getX());
    int monster_y = GenMonsterCords(player.getY());
    monster.setPastBlock(matrix[monster_x][monster_y]);
    monster.setX(monster_x);
    monster.setY(monster_y);
    matrix[monster.getX()][monster.getY()] = '!'; // monster
}

void GameWorld::PrintStats() {
    cout << "---------------------------------------" <<endl;
    cout << endl << "Level: " << level << endl;
    cout << "Rounds: " << player.getRounds() << endl;
    cout << "Total Relics collected " << player.getRelics() << "/" << maxrelic << endl;
    cout << "HP: " << player.getHealth() << endl;
    cout << "Monster-Dmg: " << monster.getDmg() << endl;
    string msg = player.getItem() == 0 ? "You own no potion" : "";
    if(player.getItem() == 1) msg = "You own a strength potion";
    else if(player.getItem() == 2) msg = "You own a dexterity potion";
    else if(player.getItem() == 3) msg = "You own a intelligence potion";
    cout << msg << endl;
}

void GameWorld::Statistics(char status) {
    stats[cnt] = status;
    index[cnt] = player.getRounds();
    cnt += 1;
}

void GameWorld::PrintStatistics() {
    cout << "---------------------------------------" << endl << "History of events: " << endl;
    for(int i = 0; i <= cnt; i++) {
        if(stats[i] == 'x')
            cout << "Round" << index[i] << ": Player has stepped on danger tile ... lost the challenge, and received dmg" << endl;
        else if(stats[i] == 'r')
            cout << "Round" << index[i] << ": Player has found a relic" << endl;
        else if(stats[i] == 'f')
            cout << "Round" << index[i] << ": Player has been healed" << endl;
        else if(stats[i] == 'm')
            cout << "Round" << index[i] << ": Player has been damaged by the monster" << endl;
        else if(stats[i] == 'l')
            cout << "Round" << index[i] << ": Player has leveled up!" << endl;
        else if(stats[i] == 'p')
            cout << "Round" << index[i] << ": Player has found a potion!" << endl;
        else if(stats[i] == 'u')
            cout << "Round" << index[i] << ": Player has used up a portion!" << endl;


    }
    cout << "Total Rounds " << player.getRounds() << endl;
    cout << "Total DMG Taken " << player.getDmgTaken() << endl;
    cout << "Total Healing Done " <<  player.getRegen() << endl;
}

void GameWorld::checkDangerBlock(char currBlock, int playerstats) {
    int num = rand()%10 + 1;
    cout << "The value of this Attribute-field: " << num << endl;
    if(playerstats <= num && player.getItem() == 1 && currBlock == 'S'){
        cout << "You used up ur Strength-Potion to raise your STR to max - You will take no damage" << endl;
        Statistics('u');
        player.setItem(0);
    }
    else if(playerstats <= num && player.getItem() == 2 && currBlock == 'D') {
        cout << "You used up ur Dexterity-Potion to raise your DEX to max - You will take no damage" << endl;
        Statistics('u');
        player.setItem(0);
    }
     else if(playerstats <= num && player.getItem() == 3 && currBlock == 'I') {
        cout << "You used up ur Intelligence-Potion to raise your INT to max - You will take no damage" << endl;
        Statistics('u');
        player.setItem(0);
     }
    else {
            if(playerstats <= num) {
                player.setHealth(player.getHealth() - 1);
                player.setDmgTaken(player.getDmgTaken() + 1);
                Statistics('x');
                cout << "You stepped on a danger tile and have taken dmg!" << endl;
                    if(player.getHealth() <= 0)
                        status = -1; //losing from losing too much hp
            }
            else
                cout << "You stepped on a danger tile, but bested the challenge - You will take no damage" << endl;
    }
}

void GameWorld::findItem() {
    int num = rand()%3 + 1;
    string msg = (player.getItem() == 0) ? "You have found a potion" : "You already have a potion, you cant take another" ;
    cout << msg << endl;
    if(player.getItem() == 0) {
        Statistics('p');
        player.setItem(num);
    }
}

void GameWorld::levelUp() {
    Statistics('l');
    level++;
    player.setRelics(0);
    maxrelic = 0;
    GenGrid();
    SpawnMonster();
    monster.setDmg(monster.getDmg() + 1);
    cout << "---------------------------------------" <<endl;
    cout << "You have leveled up!" << endl;
    cout << "The Map will randomely respawn and the Monster will deal: " << monster.getDmg() << " Damage now" << endl;
    cout << "---------------------------------------" <<endl;
    }

void GameWorld::checkBlock(int x, int y) {
    char currBlock = matrix[x][y];
    if(currBlock == '!'){
        currBlock = monster.getPastBlock(); 
    }
    switch(currBlock) {
    case '.':
        break;
    case 'S':
        checkDangerBlock(currBlock, player.getStr());
        break;
    case 'I':
        checkDangerBlock(currBlock, player.getInt());
        break;
    case 'D':
        checkDangerBlock(currBlock, player.getDex());
        break;
    case 'r':
        Statistics(currBlock);
        cout << "You have found a relic!" << endl;
        findItem();
        player.setRelics(player.getRelics() + 1);
        if(player.getRelics() == maxrelic) {
            if(level == 3)
                status = 0; //winning from getting all relics in all levels
            else if(level < 3) {
                levelUp();
            }
        }
        break;
    case 'f':
        Statistics(currBlock);
        findItem();
        player.setHealth(player.getHealth() + 1);
        player.setRegen(player.getRegen() + 1);
        cout << "You have found a well and regained 1HP" << endl;
        break;
    }
}

void GameWorld::movePlayer(int x, int y) {
    int pos_x = player.getX();
    int pos_y = player.getY();
    int new_pos_x = pos_x + x;
    int new_pos_y = pos_y + y;
    player.setX(new_pos_x);
    player.setY(new_pos_y);
    matrix[pos_x][pos_y] = '.';
    checkBlock(new_pos_x, new_pos_y);
    matrix[new_pos_x][new_pos_y] = 'P';
}

bool GameWorld::checkMonster() {
    if(monster.getX() - player.getX() == 0 && monster.getY() - player.getY() == 0)
        return true;
    return false;
}

void GameWorld::moveMonster() {
    int pos_x = monster.getX();
    int pos_y = monster.getY();
    int difference_x = monster.getX() - player.getX();
    int difference_y = monster.getY() - player.getY();
    bool checkagain = true;
    if(checkMonster() == true) {
        dostuff();
        checkagain = false;
        }
    else if(difference_x != 0) {
        if(difference_x > 0)
            monster.setX(monster.getX() - 1);
        else if(difference_x < 0)
            monster.setX(monster.getX() + 1);
    }
    else if(difference_y != 0) {
        if(difference_y > 0)
            monster.setY(monster.getY() - 1);
        else if(difference_y < 0)
            monster.setY(monster.getY() + 1);
    }
    if(checkagain == true) {
    matrix[pos_x][pos_y] = monster.getPastBlock(); //load past content of past block
    monster.setPastBlock(matrix[monster.getX()][monster.getY()]); //save content of next block
    matrix[monster.getX()][monster.getY()] = '!';  //monster moving to next block
    if(checkMonster() == true)
    dostuff();
    }
}

void GameWorld::dostuff() {
    player.setDmgTaken(player.getDmgTaken() + monster.getDmg());
    player.setHealth( player.getHealth() - monster.getDmg());
    cout << "You got caught by the monster and have received dmg..." << endl;

    if(player.getHealth() <= 0) {
            status = -1;
        }
        else {
            cout << "The monster will respawn at a random tile" << endl;
            Statistics('m');
            matrix[monster.getX()][monster.getY()] = 'P';
            SpawnMonster();
        }
}

void GameWorld::playGame() {
    do {
        DisplayGame();
        cout << "Please enter a direction: ";
        string s;
        cin >> s;
        char movement = s[0]; //will only evaluate first char even if you enter more
        if (movement != 'w' && movement != 'a' && movement != 's' && movement != 'd') {
            cout << "Please enter a valid movement (w, a, s, d)" << endl;
            continue;
        }
        else if(movement == 'w' && player.getX() == 0) {
            cout << "You are moving out of the map, please choose another direction!" << endl;
        }
        else if(movement == 'a' && player.getY() == 0) {
            cout << "You are moving out of the map, please choose another direction!" << endl;
        }
        else if(movement == 'd' && player.getY() == 4) {
            cout << "You are moving out of the map, please choose another direction!" << endl;
        }
        else if(movement == 's' && player.getX() == 4) {
            cout << "You are moving out of the map, please choose another direction!" << endl;
        }
        else {
        player.setRounds( player.getRounds() + 1);
        switch(movement) {
            case 'w':
                movePlayer(-1, 0);
                break;
            case 'a':
                movePlayer(0, -1);
                break;
            case 's':
                movePlayer(1, 0);
                break;
            case 'd':
                movePlayer(0, 1);
                break;
            }
    if(status == 1) {
        moveMonster();
        PrintStats();
    }
}
}while (status == 1);

if (status == 0)
    cout << endl << "You won! Congratulations!" << endl;
else if(status == -1)
    cout << endl << "You lost..." << endl;
}

int main()
{
    cout << " * Hello Player! Move with w - a - s - d!" << endl;
    cout << " * (.) = safe field " << endl <<" * (STR(S)), (INT(I)), (DEX(D)) = danger fields with certain Attributes" << endl << " * (f) = fountain to heal 1 HP!" << endl;
    cout << " * You will spawn with a random Number of each of the 3 Attributes, when your Attribute value is higher than the one of the field you will take no dmg, else : -1HP"<< endl;
    cout << " * You can find Potions at relics and wells" << endl;
    cout << " * If u step on a field with the attribute corresponding to your potion type && your own value is lower than that of the field -> the potion will activate and boost your value to the max for one round"<< endl;
    cout << " * You can only carry one Potion at a time"<< endl;
    cout << " * Collect all Relics(r) to level up/win (Max-LvL = 3)!"<< endl;
    cout << " * Lose at 0 HP! " << endl << " * !Warning: The monster(!) will chase and damage you if hit" << endl;
    cout << " * There are 3 levels available; the monster will hit you for +1 DMG which each level increase " << endl;
    srand(time(NULL));
    GameWorld newworld;
    newworld.playGame();
    newworld.PrintStatistics();
    return 0;
}
