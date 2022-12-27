#include <bits/stdc++.h>

bool CHANGED_BALANCE; //Проверка на изменение баланса при удалении
struct Room{
    char letter;
    int number;

    Room(){
        letter = 'A';
        number = 100;
    }

    Room(char let, int num){
        letter = let;
        number = num;
    }
};
//Создаём узел списка, который лежит в узле дерева
struct Node{
    Room* room;
    Node* next;
    Node* prev;

    Node(){
        room = nullptr;
        next = nullptr;
        prev = nullptr;
    }

    Node(char let, int num){
        room = new Room(let, num);
        next = nullptr;
        prev = nullptr;
    }

    ~Node() = default;
};
//Создаем узел дерева
struct TNode{
    int height;
    Node* list;
    TNode* left;
    TNode* right;

    TNode(){
        list = nullptr;
        left = nullptr;
        right = nullptr;
        height = 1;
    }

    TNode(char let, int num){
        list = new Node(let, num);
        left = nullptr;
        right = nullptr;
        height = 1;
    }

    ~TNode() = default;
};
//Получить букву аудитории из списка
char GetRoomLetter(Node* node){
    return node->room->letter;
}
//Получить номер аудитории из списка
int GetRoomNumber(Node* node){
    return node->room->number;
}
//Количество узлов в списке
int NodeNumber(Node* node){
    int number = 1;
    if (node->next == nullptr)
        return number;
    Node* current = node->next;
    while(current != nullptr){
        number++;
        current = current->next;
    }
    return number;
}
//Удалить из списка 1 узел
void DeleteFromList(TNode* node){
    Node* listnode = node->list;
    Node* temp = listnode->next;
    listnode->next = temp->next;
    if (temp->next != nullptr)
    temp->next->prev = listnode;
    delete(temp);
}
//Добавить в список 1 узел
void AddToList(Node* &node){
    Node* newone = new Node(GetRoomLetter(node), GetRoomNumber(node));
    if (node != nullptr) {
        node->prev = newone;
        newone->next = node;
    }
    node = newone;
}
//Высота узла дерева
int height(TNode* node)
{
    return (node == nullptr ? 0 : node->height);
}
//Баланс
int GetBalance(TNode* node)
{
    if(node == nullptr)
        return 0;
    else
        return height(node->left) - height(node->right);
}
//Правый поворот
TNode* RightRotate(TNode* &node)
{
    TNode* u = node->left;
    node->left = u->right;
    u->right = node;
    node->height = std::max(height(node->left), height(node->right))+1;
    u->height = std::max(height(u->left), (node->height))+1;
    return u;
}
//Левый поворот
TNode* LeftRotate(TNode* &node)
{
    TNode* u = node->right;
    node->right = u->left;
    u->left = node;
    node->height = std::max(height(node->left), height(node->right))+1;
    u->height = std::max(height(node->right), (node->height))+1 ;
    return u;
}
//Правый левый поворот
TNode* RightLeftRotate(TNode* &node)
{
    node->right = RightRotate(node->right);
    return LeftRotate(node);
}
//Левый правый поворот
TNode* LeftRightRotate(TNode* &node)
{
    node->left = LeftRotate(node->left);
    return RightRotate(node);
}
//Получить номер аудитории из дерева
int GetRoomNumber(TNode* p){
    return p->list->room->number;
}
//Получить Букву аудитории из дерева
char GetRoomLetter(TNode* p){
    return p->list->room->letter;
}
//Максимальный
TNode* Max(TNode* node){
    if (node == nullptr)
        return nullptr;
    if (node->right == nullptr)
        return node;
    TNode* max = node->right;
    while (max->right != nullptr)
        max = max->right;
    return max;
}
//Поиска узла дерева по аудитории
TNode* FindNode(TNode* root, char let, int num){
    if (root == nullptr) {
        std::cout << "Дерево пустое.\n";
        return nullptr;
    }
    TNode* current = root;
    while((current != nullptr) && (GetRoomNumber(current) != num || GetRoomLetter(current) != let)){
        if (GetRoomLetter(current) < let) current = current->left;
        else {
            if (GetRoomLetter(current) > let) current = current->right;
            else if (GetRoomLetter(current) == let) {
                if (GetRoomNumber(current) < num) current = current->left;
                else if (GetRoomNumber(current) > num) current = current->right;
            }
        }
    }
    if (current == nullptr) {
        std::cout << "Элемент не найден.\n";
        return nullptr;
    }
    return current;
}
//Добавить элемент в дерево
TNode* InsertInTree(TNode* root, char let, int num){
    auto newnode = new TNode(let, num);
    if (root == nullptr) return newnode;
    if ((let < GetRoomLetter(root))||
        (let == GetRoomLetter(root) && num < GetRoomNumber(root)))
        root->left = InsertInTree(root->left, let, num);
    else if ((let > GetRoomLetter(root))||
             (let == GetRoomLetter(root) && num > GetRoomNumber(root)))
        root->right = InsertInTree(root->right, let, num);
    else if (let == GetRoomLetter(root) && num == GetRoomNumber(root)) {
        AddToList(root->list);
    }
    root->height = 1 + std::max(height(root->left), height(root->right));
    int balanceFactor = GetBalance(root);
    if (balanceFactor > 1){
        if ((let < GetRoomLetter(root->left))||
            (let == GetRoomLetter(root->left) && num < GetRoomNumber(root->left))){
            return RightRotate(root);
        }
        else if ((let > GetRoomLetter(root))||
                 (let == GetRoomLetter(root) && num > GetRoomNumber(root))){
            return LeftRightRotate(root);
        }
    }
    if (balanceFactor < -1){
        if ((let > GetRoomLetter(root->right))||
            (let == GetRoomLetter(root->right) && num > GetRoomNumber(root->right))){
            return LeftRotate(root);
        }
        else if ((let < GetRoomLetter(root))||
                 (let == GetRoomLetter(root) && num < GetRoomNumber(root))){
            return RightLeftRotate(root);
        }
    }
    return root;
}
//Удалить элемент из дерева
TNode* DeleteFromTree(TNode* root, char let, int num){
    if (root == nullptr){
        std::cout << "Tree is empty.\n";
        return root;
    }
    CHANGED_BALANCE = true;
    if ((let < GetRoomLetter(root))||
        (let == GetRoomLetter(root) && num < GetRoomNumber(root)))
        root->left = DeleteFromTree(root->left, let, num);
    else if ((let > GetRoomLetter(root))||
        (let == GetRoomLetter(root) && num > GetRoomNumber(root)))
        root->right = DeleteFromTree(root->right, let, num);
    else{
        if (NodeNumber(root->list) > 1) {
            DeleteFromList(root);
            CHANGED_BALANCE = false;
        }
        if (CHANGED_BALANCE){
            if ((root->left == nullptr)||(root->right == nullptr)){
                TNode* temp = root->left ? root->left : root->right;
                if (temp == nullptr){
                    temp = root;
                    root = nullptr;
                }
                else
                    *temp = *root;
                delete(temp);
            }
            else{
                TNode* temp = Max(root->left);
                root->list = temp->list;
                root->left = DeleteFromTree(root->left, GetRoomLetter(temp), GetRoomNumber(temp));
            }
        }
    }
    if (root == nullptr)
        return root;

    root->height = 1 + std::max(height(root->left), height(root->right));
    int balanceFactor = GetBalance(root);
    if (balanceFactor > 1){
        if (GetBalance(root->left) >= 0){
            return RightRotate(root);
        }
        else {
            return LeftRightRotate(root);
        }
    }
    if (balanceFactor < -1){
        if (GetBalance(root->right) <= 0){
            return LeftRotate(root);
        }
        else {
            return RightLeftRotate(root);
        }
    }
    return root;
}
//Закинуть дерево в СТЕК
void ToStack(TNode* a, std::stack<std::pair<char, int>>* &astack){
    if (a != nullptr){
        ToStack(a->left, astack);
        astack->push(std::make_pair(GetRoomLetter(a), GetRoomNumber(a)));
        ToStack(a->right, astack);
    }

}
//Проверка на равенство
bool AreEqual(TNode* a, TNode* b){
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr ^ b == nullptr) return false;
    auto* astack = new std::stack<std::pair<char, int>>;
    auto* bstack = new std::stack<std::pair<char, int>>;
    ToStack(a, astack);
    ToStack(b, bstack);
    while (!astack->empty() && !bstack->empty()){
        if ((astack->top().first != bstack->top().first) || ((astack->top().second != bstack->top().second))) return false;
        astack->pop();
        bstack->pop();
    }
    return AreEqual(a->left, b->left)&&(AreEqual(a->right, b->right));
}
//Удалить дерево
void DeleteTree(TNode* node){
    if (node != nullptr) {
        if (node->left)
            DeleteTree(node->left);
        if (node->right)
            DeleteTree(node->right);
        delete(node->list);
        delete(node);
    }
}
//Печать
void PrintOnSide(int h, TNode* node){
    if (node != nullptr){
        PrintOnSide(h + 4, node->right);
        for (int i = 0; i < h; ++i) std::cout << " ";
        std::cout << GetRoomLetter(node) << GetRoomNumber(node) << " " << NodeNumber(node->list) << std::endl;
        PrintOnSide(h + 4, node->left);
    }
}

int main() {
    TNode* tree = nullptr; TNode* tree2 = nullptr;
    tree = InsertInTree(tree, 'A', 100);
    tree = InsertInTree(tree, 'B', 200);
    tree = InsertInTree(tree, 'A', 20);
    tree = InsertInTree(tree,'C',500);
    PrintOnSide(0,tree);
    //DeleteFromTree(tree,'C',500);
    std::cout << "\n\n";
    tree2 = InsertInTree(tree2, 'A', 100);
    tree2 = InsertInTree(tree2, 'B', 200);
    tree2 = InsertInTree(tree2, 'A', 20);
    PrintOnSide(0, tree2);
    std::cout << "\n\n" << AreEqual(tree,tree2);
    //DeleteTree(tree);
}