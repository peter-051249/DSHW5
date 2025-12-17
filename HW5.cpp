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
    vector<int> idxs;  // 同 hp 的寶可夢編號

    node* left;
    node* right;

    node(int h, int id) {
        hp = h;
        idxs.push_back(id);
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

        status.clear();
        root = nullptr;

        // 準備title印出的東西
        string title;
        getline(file, title);

        stringstream ss(title);
        vector<string> titles;
        string temp;

        while (getline(ss, temp, '\t')) {
            titles.push_back(temp);
        }


        // 先讀整行，再切成13欄，再轉型
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

        root = nullptr;  // 建樹用
        for (int i = 0; i < status.size(); i++) {
            root = insertnode(root, status[i].hp, i);
        }

        // 幫忙debug
        // cout << status.size() << endl;
        // cout << status[6].name << " " << status[6].hp << " " << status[6].type1 << " " << status[6].type2 << status[6].total << endl;
        // cout << status[1].name << " " << status[1].hp << endl;

        // Print
        int size = status.size(); // status array size
        cout << titles[0] << "\t"
             << titles[1] << "\t"
             << titles[2] << "\t"
             << titles[5] << "\n";
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
            root->idxs.push_back(id);
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


    // task2 走訪BST，判斷大小，累計次數，收集結果
    // 需要在函式裡被修改，修改後外面要看得到 (&)
    // 指向一個node (*)
    // result 存所有符合條件的節點指標
    void Search(node* n, int left, int right, vector<node*>& result, int& count) {
        if (n == nullptr) {  // 空樹
            return;
        }

        else {
            count++;
            
            // 左子樹有可能有值
            if (n->hp > left) {
                // count++;
                Search(n->left, left, right, result, count);
            }

            // 範圍內，存入 result
            if (n->hp >= left && n->hp <= right) {
                result.push_back(n);
                // count++;
            }

            // 右子樹有可能的職
            if (n->hp < right) {
                // count++;
                Search(n->right, left, right, result, count);
            }

        }

    }

    
    void PrintTask2(vector<node*>& result, int count) {
        if (result.empty()) {
            cout << "No record was found in the specified range.\n";
            cout << "Number of visited nodes = " << count << endl;
            return;
        }
        
        // 暫時這樣print
        cout << "#\tID\tName\tType1\tTotal\tHP\tAttack\tDefense\n";
        
        int idx = 1;
        for (int i = result.size() - 1; i >= 0; i--) {
            node* n = result[i];
            
            for (int j = 0; j < n->idxs.size(); j++) {
                int id = n->idxs[j];
                
                const Pokemon& p = status[id];
                
                cout << "[" << idx++ << "]" << "\t"
                << p.ID << "\t"
                << p.name << "\t"
                << p.type1 << "\t"
                << p.total << "\t"
                << p.hp << "\t"
                << p.attack << "\t"
                << p.defence << "\n";
            }
        }
        cout << "Number of visited nodes = " << count << "\n";
    }

    // 幫助從 main 使用到 Search (private -> root)
    void SearchRange(int left, int right) {
        vector<node*> result;
        int count = 0;
        
        // 開始找
        Search(root, left, right, result, count);  // count 出來會是改過內容的 (int& count)
    
        // 印出結果
        PrintTask2(result, count);
    }
};





int main () {
    int comm;
    bool comm1 = false;
    string filenum;
    BST one;
    int range1;
    int range2; 

    while (1) {
        cout << "*** (^_^) Data Structure (^o^) ***\n";
        cout << "** Binary Search Tree on Pokemon *\n";
        cout << "* 0. QUIT                        *\n";
        cout << "* 1. Read a file to build HP BST *\n";
        cout << "* 2. Range search on HP field    *\n";
        cout << "* 3. Delete the min on HP field  *\n";
        cout << "* 4. Rebuild the balanced HP BST *\n";
        cout << "**********************************\n";
        cout << "Input a choice(0, 1, 2, 3, 4): ";  

        cin >> comm;

        if (comm == 0) {
            break; 
        }

        else if (comm == 1) {
            cout << "\nInput a file number [0: quit]: ";
            cin >> filenum;
            if (filenum == "0") {
                cout << "\n";
                continue;
            }
            one.ReadFile(filenum);
            cout << "\n";
            comm1 = true;
        }

        else if (comm == 2) {
            if (!comm1) {
                cout << "\n----- Execute Mission 1 first! -----\n" << endl;
                continue;
                // break;
            }
            cout << "Input a non-negative integer: ";
            cin >> range1;
            cout << "\nInput a non-negative integer: ";
            cin >> range2;
            one.SearchRange(range1, range2);
        }
    }
}
