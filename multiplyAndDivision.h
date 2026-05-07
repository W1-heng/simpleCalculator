#ifndef MULTIPLYANDDIVISION_H
#define MULTIPLYANDDIVISION_H

#include <string>
#include<QString>
using namespace std;

// 链表节点结构体：双向链表，存储单个数字位
struct Node1 {
    int digit;       // 存储0-9的单个数字
    Node1* next;      // 下一个节点指针
    Node1* prev;      // 上一个节点指针
};
struct BigNumber {
    Node1* intHead;   // 整数部分头节点：存最低位（如123存3->2->1，intHead是3）
    Node1* intTail;   // 整数部分尾节点：存最高位（如123的intTail是1）
    Node1* decHead;   // 小数部分头节点：存最高位（如0.45存4->5，decHead是4）
    Node1* decTail;   // 小数部分尾节点：存最低位（如0.45的decTail是5）
    bool isNegative; // 符号位：true为负数，false为正数
    int intLen;      // 整数部分的长度
    int decLen;      // 小数部分的长度
};
BigNumber a, b, res;
Node1* createNode(int d);
void initBigNumber(BigNumber& num);
void clearList(Node1*& head);
void clearBigNumber(BigNumber& num);
void appendIntTail(BigNumber& num, int d);
void appendIntHead(BigNumber& num, int d);
void appendDecTail(BigNumber& num, int d);
void appendDecHead(BigNumber& num, int d);
void removeCommas(string& str);
void standardizeBigNumber(BigNumber& num);
void parse(BigNumber& num, string str);
bool isZero(BigNumber& num);
bool isDivisorTooSmall(BigNumber& num);
int compareInt(Node1* aHead, Node1* bHead);
Node1* multiplyIntByDigit(Node1* head, int d);
Node1* subtractInt(Node1* aHead, Node1* bHead);
void divide(BigNumber& a, BigNumber& b, BigNumber& res, bool& error);
void multiply(BigNumber& a, BigNumber& b, BigNumber& res);
QString printBigNumber(BigNumber& num);
#endif // MULTIPLYANDDIVISION_H
