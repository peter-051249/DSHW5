// 11327123 尤祖嫣, 11327126 黃圓譿
#include <iostream>
#include <stdbool.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
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


class BST {
  private:
    vector<Pokemon> status;  // 動態陣列，裡面每一個元素都是一個 struct Pokemon
    node* root;

  public:
    // 讀檔案進 vector<Pokemon>
    bool ReadFile(string filenum) {
        ifstream file("input"+ filenum + ".txt");
        
        if (!file) {
            cout << "\n### input" << filenum << ".txt does not exist! ###\n";
            return false;
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
        
        // Print
        int size = status.size(); // status array size
        cout << left
            << "\t" << titles[0] << "\t"
            << setw(19) << titles[1] << "\t"
            << setw(10) << titles[2] << "\t"
            << titles[5] << "\n";
        
        for (int i = 0; i < size; i++) {
            cout << "[" << right << setw(3) << i + 1 << "]\t";

            cout << left
                << status[i].ID << "\t"
                << setw(20) << status[i].name << "\t"
                << setw(10) << status[i].type1 << "\t"
                << setw(6) << status[i].hp << "\n";
        }
        int height = getHeight(root);
        cout << "HP tree height = " << height << "\n" << endl;
        return true;
    }

    node* insertnode(node* r, int hp, int id) {
        if (r == nullptr) {
            return new node(hp, id);  // 建立new node
        }

        if (hp < r->hp) {
            r->left = insertnode(r->left, hp, id);
        }

        else if (hp > r->hp) {
            r->right = insertnode(r->right, hp, id);
        }

        else {  // hp == root->hp
            r->idxs.push_back(id);
        }
        return r;
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

    int MaxHP(node* n) {
        if (n == nullptr) return 0;

        while (n->right != nullptr) {
            n = n->right;
        }
        return n->hp;
    }

    int getMaxHP() {
        return MaxHP(root);
    }
    // task2 走訪BST，判斷大小，累計次數，收集結果
    // 需要在函式裡被修改，修改後外面要看得到 (&)
    // 指向一個node (*)
    // result 存所有符合條件的節點指標
    void Search(node* n, int left, int right, vector<node*>& result, int& count) {
        // 找最大的hp，跟left and right比較
        if (n == nullptr) {  // 空樹
            return;
        }

        else {
            // 左子樹有可能有值
            if (n->hp > left) {
                Search(n->left, left, right, result, count);
                
            }
            
            // 範圍內，存入 result
            if (n->hp >= left && n->hp <= right) {
                result.push_back(n);
            }
            
            // 右子樹有可能的職
            if (n->hp < right) {  // 原本是n->hp < right
                Search(n->right, left, right, result, count);
            }
            count++;
            
        }

    }

    void PrintTask2(const vector<node*>& result, int count) {
        if (result.empty()) {
            cout << "No record was found in the specified range.\n";
            cout << "Number of visited nodes = " << count << "\n" << endl;
            return;
        }
        
        cout << "\t#\t"
            << left << setw(19) << "Name" << "\t"
            << left << setw(10) << "Type 1" << "\t"
            << "Total\tHP\tAttack\tDefense\n";

        
        int idx = 1;
        for (int i = result.size() - 1; i >= 0; i--) {
            node* n = result[i];
            
            for (int j = 0; j < n->idxs.size(); j++) {
                int id = n->idxs[j];
                
                const Pokemon& p = status[id];

                cout << "[" << right << setw(3) << idx++ << "]\t";

                cout << p.ID << "\t"
                    << left << setw(20) << p.name << "\t"
                    << setw(10) << p.type1 << "\t"
                    << setw(5)  << p.total << " \t"
                    << p.hp << "\t"
                    << p.attack << "\t"
                    << p.defence << "\n";
            }
        }
        cout << "Number of visited nodes = " << count << "\n" << endl;
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

    // 實際做 task 3 的
    node* DeleteMin(node* root, node* &deleted) {
        if (root == nullptr) {
            deleted = nullptr;
            return nullptr;
        }

        if (root->left == nullptr) {
            deleted = root;          // 記住被刪的節點
            return root->right;      // 不在這裡 delete
        }

        root->left = DeleteMin(root->left, deleted);
        return root;
    }


    node* DeleteMax(node* root, node* &deleted) {
        // 先判斷樹是不是空的
        if (root == nullptr) {
            deleted = nullptr;
            return nullptr;
        }

        if (root->right == nullptr) { 
            deleted = root;
            return root->left;
        }
        // 回傳刪掉的
        root->right = DeleteMax(root->right, deleted);
        return root;
    }

    // 用在 main 裡的
    void Delete_min() {
        if (root == nullptr) {
            cout << "\n----- Execute Mission 1 first! -----\n" << endl;
            return;
        }

        node* deleted = nullptr;
        root = DeleteMin(root, deleted);

        if (deleted != nullptr) {
            cout << "\t#\t"
                << left << setw(19) << "Name" << "\t"
                << left << setw(10) << "Type 1" << "\t"
                << "Total\tHP\tAttack\tDefense\tSp. Atk\tSp. Def\n";


            int idx = 1;
            for (int i = 0; i < deleted->idxs.size(); i++) {
                int id = deleted->idxs[i];   // status 的 index
                const Pokemon& p = status[id]; 
                cout << "[" << right << setw(3) << idx++ << "]\t"
                    << p.ID << "\t"
                    << left << setw(20) << p.name << "\t"
                    << setw(10) << p.type1 << "\t"
                    << setw(5)  << p.total << " \t"
                    << p.hp << "\t"
                    << p.attack << "\t"
                    << p.defence << "\t"
                    << setw(6) << p.sp_atk << "\t"
                    << p.sp_def << endl;
            }

            delete deleted;   // 這裡才真的釋放
        }

        int height = getHeight(root);
        cout << "HP tree height = " << height << "\n" << endl;
    }

    void Delete_max() {
        if (root == nullptr) {
            cout << "\n----- Execute Mission 1 first! -----\n" << endl;
            return;
        }

        node* deleted = nullptr;
        root = DeleteMax(root, deleted);

        if (deleted != nullptr) {
            cout << "\t#\t"
                << left << setw(19) << "Name" << "\t"
                << left << setw(10) << "Type 1" << "\t"
                << "Total\tHP\tAttack\tDefense\tSp. Atk\tSp. Def\n";

            int idx = 1;
            for (int i = 0; i < deleted->idxs.size(); i++) {
                int id = deleted->idxs[i];   // status 的 index
                const Pokemon& p = status[id]; 
                cout << "[" << right << setw(3) << idx++ << "]\t"
                    << p.ID << "\t"
                    << left << setw(20) << p.name << "\t"
                    << setw(10) << p.type1 << "\t"
                    << setw(5)  << p.total << " \t"
                    << p.hp << "\t"
                    << p.attack << "\t"
                    << p.defence << "\t"
                    << setw(6) << p.sp_atk << "\t"
                    << p.sp_def << endl;
            }
            delete deleted;   // 這裡才真的釋放
        }

        int height = getHeight(root);
        cout << "HP tree height = " << height << "\n" << endl;
    }


    void Task4() { // main 裡使用
        vector<node*>t_node;
        In_order_traval(root, t_node);
        root = bulid(t_node, 0, t_node.size() - 1);
        cout << endl;
        cout << "HP tree:" << endl;
        Print(root);
        cout << endl;
    }

    void In_order_traval(node* r, vector<node*>& t_node) {
        if (r == nullptr) {
            return;
        }

        In_order_traval(r -> left, t_node);

        t_node.push_back(r);

        In_order_traval(r -> right, t_node);
    }

    node *bulid(vector<node*>& t_node, int left, int right) {  //  建樹...
        if (left > right) {
            return nullptr; 
        }
        int mid = (left + right) / 2;
        node *p = t_node[mid];
        p -> left = bulid(t_node, left, mid - 1);
        p -> right = bulid(t_node, mid + 1, right);

        return p;
    }

    void Print(node *root) { 
        vector<node*> r;
        r.push_back(root);
        int level = 1;
        while (!r.empty()) {
            cout << "<level " << level << "> ";
            int s = r.size();  //  本層有多少節點
            for (int i = 0; i < s; i++) {  //  印hp
                node *p = r.front();  //  該層最左邊
                cout << "(" << p ->hp << ", ";
                
                for (int j = 0; j < p -> idxs.size();  j++) {
                    int x = p -> idxs[j];
                    cout << status[x].ID;
                    if (j < p -> idxs.size() - 1) {
                        cout << "|";
                    }
                }

                cout << ")";
                if (i < s - 1) {
                    cout << " ";
                }
                if (p -> left != nullptr) {
                    r.push_back(p -> left);
                }

                if (p -> right != nullptr) {
                    r.push_back(p -> right);
                }
                r.erase(r.begin());  //  刪掉第一個元素
            }
            level++;
            cout << endl;
        }
    }


};


int num(string size) {  // 字串變數字
  int n = size.size();  // 是幾位數? 1~5(最多到99999)
  int test = 0;  // 不是數字就等於1
  for (int i = 0; i < n; i++) {
    if (!isdigit(size[i])) {
      test = 1;
      break;
    }
  }

  if (test == 0) {  // 都是數字，把他們變成int
    int num = 0;
    for (int i = 0; i < n; i++) {
      num = (num * 10) + size[i] - '0';
    }
    return num;
  }
  return -1;  // 不是數字
}




int main () {
    string comm;
    bool comm1 = false;
    string filenum;
    BST bst; 
    string range1;
    string range2; 
    int count_3 = 0;

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

        if (comm == "0") {
            break; 
        }

        else if (comm == "1") {
            while (1) {
                cout << "\nInput a file number [0: quit]: ";
                cin >> filenum;
                if (filenum == "0") {
                    cout << "\n";
                    break;
                }
                if (bst.ReadFile(filenum)) {
                    break;
                }
            }
            comm1 = true;
        }

        else if (comm == "2") {
            int r1 = 0;
            int r2 = 0;
            if (!comm1) {
                cout << "\n----- Execute Mission 1 first! -----\n" << endl;
                continue;
            }
            cout << endl;
            int maxhp = bst.getMaxHP();
            int limit = maxhp * 2;
            while (1) {
                cout << "Input a non-negative integer: ";
                cin >> range1;
                r1 = num(range1);
                if (num(range1) == -1) {
                    cout << "\n### It is NOT a non-negative integer. ###\n";
                    cout << "Try again: \n";
                }

                // range problem
                else if (r1 > limit) {
                    cout << "\n### It is NOT in [0," << limit <<  "]. ###\n";
                    cout << "Try again: \n";
                }

                else {
                    break;
                }
            }

            cout << "\n";
            while (1) {
                cout << "Input a non-negative integer: ";
                cin >> range2;
                r2 = num(range2);
                if (num(range2) == -1) {
                    cout << "\n### It is NOT a non-negative integer. ###\n";
                    cout << "Try again: \n";
                }

                // range problem
                else if (r2 > limit) {
                    cout << "\n### It is NOT in [0," << limit <<  "]. ###\n";
                    cout << "Try again: \n";
                }

                else {
                    break;
                }
            }

            if (r1 > r2) {
                int temp;
                temp = r1;
                r1 = r2;
                r2 = temp;
            }


            bst.SearchRange(r1, r2);
        }

        else if (comm == "3") {
            if (!comm1) {
                cout << "\n----- Execute Mission 1 first! -----\n" << endl;
                continue;
            }

            count_3++;
            if (count_3 % 2 == 1) {  // 奇數次 刪hp最小
                cout << "\n";
                bst.Delete_min();
            }

            else if (count_3 % 2 == 0) {  // 偶數次 刪hp最大
                cout << "\n";
                bst.Delete_max();
            }
        }
    
        else if (comm == "4") {
            if (!comm1) {
                cout << "\n----- Execute Mission 1 first! -----\n" << endl;
                continue;
            }
            bst.Task4();
            count_3 = 0;
        }

        else {
            cout << "\nCommand does not exist!\n" << endl;
        }
    }
}
