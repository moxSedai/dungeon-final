#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include "NPC.h"
#include "Enemy.h"

using namespace std;

template<typename offence, typename defence>
double calculateDamage(offence offensive, defence defensive) {
    srand(time(0));
    double randValue = (((double) rand() / (RAND_MAX)) + 1) * 20;
    double damage = (randValue * offensive->getLevel() * offensive->getBaseDamage()/100 * offensive->getRace().raceModifier) /(defensive->getRace().raceModifier);
    return damage;
}

Player newPlayer() {
    string name;
    Race race;
    string weapon;
    int health;
    cout << "What is your name? ";
    getline(cin, name);
    cout << "What is your race? ";
    race = chooseRace();
    cout << endl << "What is your weapon of choice? ";
    getline(cin, weapon);

    Player player = Player(name, race, weapon);

    cout << endl << endl << endl << endl << endl << endl;
    return player;
}

void enemyDefeated(Player *player, Enemy *enemy) {
    player->setExperience(player->getExperience() +  enemy->getExperience());
    cout << endl << "You gain " << enemy->getExperience() << " EXP!" << endl;
}

void battle(Player *player, NPC *party[3], int NPC_NUM) {
    srand(time(0));
    int enemyCount = rand() % 4 + 1;
    int selection;
    double damage;
    double damagedHealth;
    Enemy *enemies[4] = {};
    for (int i = 0; i< enemyCount; i++) {
        enemies[i] = new Enemy({"kobold", "claws", 1}, 1);
    }
    while (true) {
        //bATTLE hAPPENS

        //Player always goes first

        cout << player->getName() << "[" << player->getHealth() << "/" << player->getMaxHealth() << "]" << "       ";
        for (int i = 0; i < NPC_NUM; i++) {
            cout << party[i]->getName() << "[" << party[i]->getHealth() << "/" << party[i]->getMaxHealth() << "]        ";
            if (i == 0) cout << endl;
        }
        cout << endl;
        cout << "------ENEMIES------" << endl;
        for(int i = 0; i < enemyCount; i++) {
            cout << "[" << i << "]    " << enemies[i]->getRace().raceName << "      HP[" << fixed << setprecision(3) << enemies[i]->getHealth() << "/" << enemies[i]->getMaxHealth() << "]" << endl;
        }
        cout << endl << "Choose which enemy to attack: ";
        cin >> selection;

        cout << endl << endl << endl;

        damage = calculateDamage(player, enemies[selection]);
        damagedHealth = enemies[selection]->getHealth();
        damagedHealth -= damage;
        enemies[selection]->setHealth(damagedHealth);
        cout << endl << endl << "You did " << fixed << setprecision(3) << damage << " damage to the " << enemies[selection]->getRace().raceName << " with your " << player->getWeapon() << "!" << endl;
        if (damagedHealth <= 0) {
            cout << endl << "You have killed the " << enemies[selection]->getRace().raceName << "!" << endl;
            enemyDefeated(player, enemies[selection]);

            for (int i = selection; i< enemyCount; i++) {
                enemies[i] = enemies[i+1];
            }
            enemyCount--;
        }

        if (enemyCount == 0) {
            cout << endl << endl << "All enemies are dead!  You win!" << endl;
            return;
        }

        //NPC turns

        for(int i = 0; i < NPC_NUM; i++) {
            selection = rand() % enemyCount;
            damage = calculateDamage(party[i], enemies[selection]);
            damagedHealth = enemies[selection]->getHealth();
            damagedHealth -= damage;
            enemies[selection]->setHealth(damagedHealth);
            cout << endl << party[i]->getName() << " did " << damage << " damage to the " << enemies[selection]->getRace().raceName << " with their " << party[i]->getRace().defaultWeapon << "!" << endl;
            if (damagedHealth <= 0) {
                cout << endl << "You have killed the " << enemies[selection]->getRace().raceName << "!" << endl;
                enemyDefeated(player, enemies[selection]);

                for (int i = selection; i< enemyCount; i++) {
                    enemies[i] = enemies[i+1];
                }
                enemyCount--;
            }
            if (enemyCount == 0) {
                cout << endl << endl << "All enemies are dead!  You win!" << endl;
                return;
            }
        }

        // Enemy turns

        for (int i = 0; i < enemyCount; i++) {
            if (NPC_NUM > 0) {
                selection = rand() % (NPC_NUM);
                if (selection != (NPC_NUM)) {
                    damage = calculateDamage(enemies[i], party[selection]);
                    damagedHealth = party[selection]->getHealth();
                    damagedHealth -= damage;
                    party[selection]->setHealth(damagedHealth);

                    cout << "The " << enemies[i]->getRace().raceName << " did " << damage << " damage to " << party[selection]->getName() << " with their " << enemies[i]->getRace().defaultWeapon << "!" << endl;

                    if (damagedHealth <= 0) {
                        cout << endl << party[selection]->getName() << " has been critically injured!  They choose to stay behind to heal up." << endl << "They may come back later." << endl;
                        for (int i = selection; i< NPC_NUM; i++) {
                            party[i] = party[i+1];
                        }
                        NPC_NUM--;
                    }
                }
                else if (selection == NPC_NUM) {
                    damage = calculateDamage(enemies[i], player);
                    damagedHealth = player->getHealth();
                    damagedHealth -= damage;
                    player->setHealth(damagedHealth);

                    cout << "The " << enemies[i]->getRace().raceName << " did " << damage << " damage to " << player->getName() << " with their " << enemies[i]->getRace().defaultWeapon << "!" << endl;

                    if (damagedHealth <= 0) {
                        cout << endl << "You have been killed.  Game over.  Press enter to exit" << endl;
                        cin.ignore();
                        exit(0);
                    }
                }
            }
            damage = calculateDamage(enemies[i], player);
            damagedHealth = player->getHealth();
            damagedHealth -= damage;
            player->setHealth(damagedHealth);

            cout << "The " << enemies[i]->getRace().raceName << " did " << damage << " damage to " << player->getName() << " with their " << enemies[i]->getRace().defaultWeapon << "!" << endl;

            if (damagedHealth <= 0) {
                cout << endl << "You have been killed.  Game over.  Press enter to exit" << endl;
                cin.ignore();
                exit(0);
            }
        }
    }
}


int main()
{
    NPC *CurrentNPCS[3];
    int NPC_NUM = 3;

    CurrentNPCS[0] = new NPC("Jeff", {"human", "sword", 0.1}, 1);
    CurrentNPCS[1] = new NPC("Potato", {"pseudodragon", "psionics", 0.1}, 1);
    CurrentNPCS[2] = new NPC("OOLAJAHEASCA", {"kobold", "claws", 0.1}, 1);

    Player player = newPlayer();

    Player *playerPtr = &player;

    battle(playerPtr, CurrentNPCS, NPC_NUM);
    return 0;
}


