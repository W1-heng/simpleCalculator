#ifndef ADDANDSUBTRACT_H
#define ADDANDSUBTRACT_H
#include <string>
#include<QString>
struct node
{
    int num=0;//用字符类型，存储被算数每一位数字字符
    char dot='\0';
    node* next=nullptr;
    node* before = nullptr;
};
void delete_Node(node* head) ;
QString print(node* head, node* end, std::string num1, std::string num2);
bool num_Cp(node* head1, node* head2, node* division1, node* division2,bool& zero);
void create_Node(node*& head, const std::string& num1, node *& division);
node* add(node* division1, node* division2, node*& end);
node* sub(node* division1, node* division2, node*& end);
node* division1 = nullptr;
node* division2 = nullptr;//小数点节点指针
node* head1 = nullptr;
node* head2 = nullptr;//整个数的头指针
node* end = nullptr;   //结果链表尾节点指针
bool zero = false;
 node* newHead = nullptr;
#endif //ADDANDSUBTRACT_H
