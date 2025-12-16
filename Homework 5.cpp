#include <iostream>
#include <stdbool.h>
using namespace std;


typedef struct Pokemon {
    int ID;
    string name;
    string type1;
    string type2;
    int total;
    int hp;
    int attack;
    int defence;
    int spAttack;
    int spDefence;
    int speed;
    int gen;
    bool legend;
} Pokemon;







int main () {
    int comm;
    while (1) {
        cout << "*** (^_^) Data Structure (^o^) ***\n";
        cout << "\n";



        cin >> comm;

        if (comm == 0) {
            break; // 暫時
        }

        else if (comm == 1) {

        }
    }
 

}
