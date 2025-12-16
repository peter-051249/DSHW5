#include <iostream>
#include <stdbool.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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
    int sp_atk;
    int sp_def;
    int speed;
    int gen;
    bool legend;
} Pokemon;



typedef struct node {
    int hp;
    vector<int> ids;  // 同 hp 的寶可夢編號

    node* left;
    node* right;

    node(int h, int id) {
        hp = h;
        ids.push_back(id);
        left = nullptr;
        right = nullptr;
    }
} node;


// 用來建立樹
class BST {
  private:
    vector<Pokemon> status;  // 動態陣列，裡面每一個元素都是一個 struct Pokemon
    node* root;

  public:
    // 讀檔案進 vector<Pokemon>
    void ReadFile(string filenum) {
        ifstream file("input"+ filenum + ".txt");
        if (!file) {
            cout << "### input" << filenum << ".txt does not exist! ###\n";
            return;
        }

        string title;
        getline(file, title);  // 把第一行讀掉
        
        // 先讀整行，再切成13欄
        // 再轉型
        string line;
        Pokemon p; 
        while (getline(file, line)) {
            stringstream s(line);
            string data;

            getline(s, data, '\t');  // 從 s 裡讀到下一個 tab 為止，把這段文字放進data
            p.ID = stoi(data);
            getline(s, data, '\t');
            p.name = data;
            getline(s, data, '\t');
            p.type1 = data;
            getline(s, data, '\t');  // 如果type2 沒東西，這裡會存成空字串
            p.type2 = data;
            getline(s, data, '\t');
            p.total = stoi(data);
            getline(s, data, '\t');
            p.hp = stoi(data);    
            getline(s, data, '\t');
            p.attack = stoi(data);
            getline(s, data, '\t');
            p.defence = stoi(data);
            getline(s, data, '\t');
            p.sp_atk = stoi(data);
            getline(s, data, '\t');
            p.sp_def = stoi(data);
            getline(s, data, '\t');
            p.speed = stoi(data);
            getline(s, data, '\t');
            p.gen = stoi(data);
            getline(s, data, '\t');
            p.legend = (data == "True");

            status.push_back(p);

        }

        root = nullptr;  // 建樹
        for (int i = 0; i < status.size(); i++) {
            root = insertnode(root, status[i].hp, status[i].ID);
        }


        // 幫忙debug
        // cout << status.size() << endl;
        // cout << status[6].name << " " << status[6].hp << " " << status[6].type1 << " " << status[6].type2 << status[6].total << endl;
        // cout << status[1].name << " " << status[1].hp << endl;

        // Print
        int size = status.size(); // status array size
        for (int i = 0; i < size; i++) {
            cout << status[i].ID << "\t";
            cout << status[i].name << "\t";
            cout << status[i].type1 << "\t";
            // if (status[i].type2 == "") {  // type2 沒東西，空出一個欄位給他(但排版看起來怪怪的)
            //     cout << "\t";
            // }
            cout << status[i].hp << "\n";
        }

        int height = getHeight(root);
        cout << "BST height = " << height << endl;

    }

    node* insertnode(node* root, int hp, int id) {
        if (root == nullptr) {
            return new node(hp, id);  // 建立new noode
        }

        if (hp < root->hp) {
            root->left = insertnode(root->left, hp, id);
        }

        else if (hp > root->hp) {
            root->right = insertnode(root->right, hp, id);
        }

        else {  // hp == root->hp
            root->ids.push_back(id);
        }
        return root;
    }


    int getHeight(node* root) {
        if (root == nullptr) {
            return 0;
        }

        int l_height = getHeight(root->left);
        int r_height = getHeight(root->right);
        

        if (l_height > r_height) {
            return l_height + 1;
        }
        else {
            return r_height + 1;
        }
    }

};





int main () {
    int comm;
    string filenum;
    BST one;

    while (1) {
        // cout << "*** (^_^) Data Structure (^o^) ***\n";
        // cout << "\n";

        cout << "input command number: ";

        cin >> comm;

        if (comm == 0) {
            break; 
        }

        else if (comm == 1) {
            cout << "Input a file number: ";
            cin >> filenum;
            one.ReadFile(filenum);
        }
    }
}
