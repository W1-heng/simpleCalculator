#include <iostream>
#include <string>
#include<QString>
using namespace std;

// 链表节点结构体：双向链表，存储单个数字位
struct Node1 {
    int digit;       // 存储0-9的单个数字
    Node1* next;      // 下一个节点指针
    Node1* prev;      // 上一个节点指针
};

// 大数结构体：分离整数和小数部分，方便运算
struct BigNumber {
    Node1* intHead;   // 整数部分头节点：存最低位（如123存3->2->1，intHead是3）
    Node1* intTail;   // 整数部分尾节点：存最高位（如123的intTail是1）
    Node1* decHead;   // 小数部分头节点：存最高位（如0.45存4->5，decHead是4）
    Node1* decTail;   // 小数部分尾节点：存最低位（如0.45的decTail是5）
    bool isNegative; // 符号位：true为负数，false为正数
    int intLen;      // 整数部分的长度
    int decLen;      // 小数部分的长度
};

// ==================== 基础工具函数 ====================
// 创建单个节点，初始化数字和指针
Node1* createNode(int d) {
    Node1* p = new Node1;

    p->digit = d;
    p->next = NULL;
    p->prev = NULL;
    return p;
}

// 初始化大数结构体，所有指针置空、长度置0、符号为正
void initBigNumber(BigNumber& num) {
    num.intHead = NULL;
    num.intTail = NULL;
    num.decHead = NULL;
    num.decTail = NULL;
    num.isNegative = false;
    num.intLen = 0;
    num.decLen = 0;
}

// 释放整条链表的内存
void clearList(Node1*& head) {
    while (head != NULL) {
        Node1* temp = head;
        head = head->next;
        delete temp;
    }
}

// 清空整个大数，释放内存并重新初始化
void clearBigNumber(BigNumber& num) {
    clearList(num.intHead);
    clearList(num.decHead);
    initBigNumber(num);
}

// 给整数部分尾部加节点（新增更高位，如3->2加1 → 3->2->1，对应数字123）
void appendIntTail(BigNumber& num, int d) {
    Node1* p = createNode(d);
    if (num.intTail == NULL) { // 整数部分为空，头尾都指向新节点
        num.intHead = p;
        num.intTail = p;
    }
    else { // 加到尾部
        p->prev = num.intTail;
        num.intTail->next = p;
        num.intTail = p;
    }
    num.intLen++;
}

// 给整数部分头部加节点（新增更低位，如3->2加4 → 4->3->2，对应数字234）
void appendIntHead(BigNumber& num, int d) {
    Node1* p = createNode(d);
    if (num.intHead == NULL) { // 整数部分为空
        num.intHead = p;
        num.intTail = p;
    }
    else { // 加到头部
        p->next = num.intHead;
        num.intHead->prev = p;
        num.intHead = p;
    }
    num.intLen++;
}

// 给小数部分尾部加节点（新增更低小数位，如4->5加6 → 4->5->6，对应0.456）
void appendDecTail(BigNumber& num, int d) {
    Node1* p = createNode(d);
    if (num.decTail == NULL) { // 小数部分为空
        num.decHead = p;
        num.decTail = p;
    }
    else { // 加到尾部
        p->prev = num.decTail;
        num.decTail->next = p;
        num.decTail = p;
    }
    num.decLen++;
}

// 给小数部分头部加节点（新增更高小数位，如4->5加3 → 3->4->5，对应0.345）
void appendDecHead(BigNumber& num, int d) {
    Node1* p = createNode(d);
    if (num.decHead == NULL) { // 小数部分为空
        num.decHead = p;
        num.decTail = p;
    }
    else { // 加到头部
        p->next = num.decHead;
        num.decHead->prev = p;
        num.decHead = p;
    }
    num.decLen++;
}

// 去掉输入字符串中的千分位逗号
void removeCommas(string& str) {
    string res;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] != ',') res += str[i];
    }
    str = res;
}

// ==================== 核心标准化函数：彻底去除无意义0 ====================
// 标准化大数：删除整数前导零、小数后导零，处理-0、空值等非法情况
void standardizeBigNumber(BigNumber& num) {
    // 1. 处理整数部分前导零（整数部分低位在前，intTail是最高位）
    while (num.intLen > 1 && num.intTail != NULL && num.intTail->digit == 0) {
        Node1* temp = num.intTail;
        num.intTail = num.intTail->prev;
        if (num.intTail != NULL) num.intTail->next = NULL;
        else num.intHead = NULL;
        delete temp;
        num.intLen--;
    }

    // 2. 处理小数部分后导零（小数部分高位在前，decTail是最低位）
    while (num.decLen > 0 && num.decTail != NULL && num.decTail->digit == 0) {
        Node1* temp = num.decTail;
        num.decTail = num.decTail->prev;
        if (num.decTail != NULL) num.decTail->next = NULL;
        else num.decHead = NULL;
        delete temp;
        num.decLen--;
    }

    // 3. 特殊情况：整数部分为空，补一个0
    if (num.intLen == 0) appendIntTail(num, 0);

    // 4. 特殊情况：数值为0时，强制符号为正，避免出现-0
    if (num.intLen == 1 && num.intHead->digit == 0 && num.decLen == 0) {
        num.isNegative = false;
    }
}

// ==================== 字符串解析函数 ====================
// 将输入的数字字符串解析为BigNumber结构体
void parse(BigNumber& num, string str) {
    initBigNumber(num);
    removeCommas(str); // 先去掉所有逗号

    int start = 0;
    // 处理符号位
    if (str[0] == '-') {
        num.isNegative = true;
        start = 1;
    }
    else if (str[0] == '+') {
        start = 1;
    }

    // 查找小数点位置
    int dotPos = -1;
    for (int i = start; i < str.size(); i++) {
        if (str[i] == '.') {
            dotPos = i;
            break;
        }
    }

    // 处理整数部分：从后往前读，倒序存入链表（低位在前，方便运算）
    int intEnd = (dotPos == -1) ? str.size() - 1 : dotPos - 1;
    for (int i = intEnd; i >= start; i--) {
        appendIntTail(num, str[i] - '0');
    }

    // 处理小数部分：从前往后读，正序存入链表（高位在前，符合小数逻辑）
    if (dotPos != -1) {
        for (int i = dotPos + 1; i < str.size(); i++) {
            appendDecTail(num, str[i] - '0');
        }
    }

    // 解析完成后标准化，去除无意义0
    standardizeBigNumber(num);
}

// ==================== 辅助判断函数 ====================
// 判断大数是否为0
bool isZero(BigNumber& num) {
    return (num.intLen == 1 && num.intHead->digit == 0 && num.decLen == 0);
}

// 判断除数是否小于1e-6（绝对值<0.000001）
bool isDivisorTooSmall(BigNumber& num) {
    // 整数部分非0，肯定>=1，不小于1e-6
    if (!(num.intLen == 1 && num.intHead->digit == 0)) return false;
    // 整数部分为0，检查小数前6位是否全为0
    Node1* p = num.decHead;
    int count = 0;
    while (p != NULL && count < 6) {
        if (p->digit != 0) return false;
        p = p->next;
        count++;
    }
    return true; // 前6位全0，小于1e-6
}

// ==================== 除法核心辅助函数 ====================
// 比较两个正整数链表（低位在前）：1(a>b)、0(a==b)、-1(a<b)
int compareInt(Node1* aHead, Node1* bHead) {
    // 先比较长度
    int lenA = 0, lenB = 0;
    Node1* p = aHead;
    while (p != NULL) { lenA++; p = p->next; }
    p = bHead;
    while (p != NULL) { lenB++; p = p->next; }
    if (lenA != lenB) return lenA > lenB ? 1 : -1;

    // 长度相同，从高位到低位（尾节点到头节点）比较
    Node1* aTail = aHead;
    while (aTail->next != NULL) aTail = aTail->next;
    Node1* bTail = bHead;
    while (bTail->next != NULL) bTail = bTail->next;

    while (aTail != NULL && bTail != NULL) {
        if (aTail->digit != bTail->digit) return aTail->digit > bTail->digit ? 1 : -1;
        aTail = aTail->prev;
        bTail = bTail->prev;
    }
    return 0;
}

// 正整数链表（低位在前）乘以个位数(0-9)，返回新链表头
Node1* multiplyIntByDigit(Node1* head, int d) {
    if (d == 0) return createNode(0);
    Node1* resHead = NULL;
    Node1* resTail = NULL;
    int carry = 0;
    Node1* p = head;

    while (p != NULL || carry > 0) {
        int num = carry;
        if (p != NULL) { num += p->digit * d; p = p->next; }
        Node1* newNode = createNode(num % 10);
        carry = num / 10;
        // 追加到结果尾部
        if (resHead == NULL) { resHead = newNode; resTail = newNode; }
        else { resTail->next = newNode; newNode->prev = resTail; resTail = newNode; }
    }
    return resHead;
}

// 正整数链表减法：a - b（要求a>=b），返回新链表头（低位在前）
Node1* subtractInt(Node1* aHead, Node1* bHead) {
    Node1* resHead = NULL;
    Node1* resTail = NULL;
    int borrow = 0;
    Node1* pa = aHead;
    Node1* pb = bHead;

    while (pa != NULL) {
        int numA = pa->digit - borrow;
        int numB = (pb != NULL) ? pb->digit : 0;
        borrow = 0;
        if (numA < numB) { numA += 10; borrow = 1; }

        Node1* newNode = createNode(numA - numB);
        if (resHead == NULL) { resHead = newNode; resTail = newNode; }
        else { resTail->next = newNode; newNode->prev = resTail; resTail = newNode; }

        pa = pa->next;
        if (pb != NULL) pb = pb->next;
    }

    // 去掉结果的前导零（尾部的零）
    while (resTail != resHead && resTail->digit == 0) {
        Node1* temp = resTail;
        resTail = resTail->prev;
        resTail->next = NULL;
        delete temp;
    }
    return resHead;
}

// ==================== 核心运算函数 ====================
// 高精度除法：a / b，结果存入res，error标记是否出错
// ========== 核心修改：固定保留10位小数，四舍五入 ==========
void divide(BigNumber& a, BigNumber& b, BigNumber& res, bool& error) {
    initBigNumber(res);
    error = false;

    // 1. 除数合法性检查
    if (isZero(b) || isDivisorTooSmall(b)) {
        error = true;
        return;
    }

    // 2. 处理符号：异或运算，符号不同结果为负
    res.isNegative = (a.isNegative != b.isNegative);

    // 3. 被除数为0，结果直接为0
    if (isZero(a)) {
        appendIntTail(res, 0);
        standardizeBigNumber(res);
        return;
    }

    // 4. 把被除数、除数转为纯整数链表，记录小数位数
    int m = a.decLen; // 被除数小数位数
    int n = b.decLen; // 除数小数位数

    // 构建被除数纯整数链表A（低位在前）：整数+小数部分合并
    Node1* A = NULL;
    Node1* ATail = NULL;
    Node1* p = a.decTail; // 小数部分倒序加入（转为低位在前）
    while (p != NULL) {
        Node1* newNode = createNode(p->digit);
        if (A == NULL) { A = newNode; ATail = newNode; }
        else { ATail->next = newNode; newNode->prev = ATail; ATail = newNode; }
        p = p->prev;
    }
    p = a.intHead; // 整数部分直接加入（本来就是低位在前）
    while (p != NULL) {
        Node1* newNode = createNode(p->digit);
        if (A == NULL) { A = newNode; ATail = newNode; }
        else { ATail->next = newNode; newNode->prev = ATail; ATail = newNode; }
        p = p->next;
    }

    // 构建除数纯整数链表B（低位在前）
    Node1* B = NULL;
    Node1* BTail = NULL;
    p = b.decTail;
    while (p != NULL) {
        Node1* newNode = createNode(p->digit);
        if (B == NULL) { B = newNode; BTail = newNode; }
        else { BTail->next = newNode; newNode->prev = BTail; BTail = newNode; }
        p = p->prev;
    }
    p = b.intHead;
    while (p != NULL) {
        Node1* newNode = createNode(p->digit);
        if (B == NULL) { B = newNode; BTail = newNode; }
        else { BTail->next = newNode; newNode->prev = BTail; BTail = newNode; }
        p = p->next;
    }

    // 商的小数点偏移量：a/b = (A/B) * 10^(n-m)
    int shift = n - m;

    // 5. 模拟手工除法，计算A/B，商存到数组（高位到低位）
    // 把A转为高位在前的数组，方便逐位取数
    int lenA = 0;
    p = A;
    while (p != NULL) { lenA++; p = p->next; }
    int* arrA = new int[lenA];
    p = ATail;
    for (int i = 0; i < lenA; i++) { arrA[i] = p->digit; p = p->prev; }

    Node1* remainder = createNode(0); // 余数（低位在前）
    int quotient[10000] = { 0 };       // 商的每一位（高位到低位）
    int qIdx = 0;                     // 商的索引
    int intPartLen = 0;               // 商的整数部分长度

    // 计算整数部分
    for (int i = 0; i < lenA; i++) {
        // 余数 = 余数*10 + 当前位
        Node1* newRem = createNode(0);
        newRem->next = remainder;
        remainder->prev = newRem;
        remainder = newRem;
        remainder->digit = arrA[i];

        // 去掉余数的前导零
        Node1* remTail = remainder;
        while (remTail->next != NULL) remTail = remTail->next;
        while (remTail != remainder && remTail->digit == 0) {
            Node1* temp = remTail;
            remTail = remTail->prev;
            remTail->next = NULL;
            delete temp;
        }

        // 试商：找最大的q，使得 B*q <= 余数
        int q = 0;
        for (int trial = 9; trial >= 1; trial--) {
            Node1* prod = multiplyIntByDigit(B, trial);
            if (compareInt(remainder, prod) >= 0) {
                q = trial;
                clearList(prod);
                break;
            }
            clearList(prod);
        }

        quotient[qIdx++] = q;
        intPartLen++;

        // 更新余数
        Node1* prod = multiplyIntByDigit(B, q);
        Node1* newRemainder = subtractInt(remainder, prod);
        clearList(remainder);
        clearList(prod);
        remainder = newRemainder;
    }

    // ========== 修改1：计算11位小数（10位有效位+1位四舍五入位） ==========
    for (int i = 0; i < 11; i++) {
        if (remainder->digit == 0 && remainder->next == NULL) {
            quotient[qIdx++] = 0;
            continue;
        }
        // 余数*10
        Node1* newRem = createNode(0);
        newRem->next = remainder;
        remainder->prev = newRem;
        remainder = newRem;

        // 试商
        int q = 0;
        for (int trial = 9; trial >= 1; trial--) {
            Node1* prod = multiplyIntByDigit(B, trial);
            if (compareInt(remainder, prod) >= 0) {
                q = trial;
                clearList(prod);
                break;
            }
            clearList(prod);
        }

        quotient[qIdx++] = q;

        // 更新余数
        Node1* prod = multiplyIntByDigit(B, q);
        Node1* newRemainder = subtractInt(remainder, prod);
        clearList(remainder);
        clearList(prod);
        remainder = newRemainder;
    }

    // 6. 处理小数点偏移，拆分整数和小数部分
    int newIntLen = intPartLen + shift; // 新的整数部分长度

    // 处理整数部分
    if (newIntLen > 0) {
        for (int i = 0; i < newIntLen; i++) {
            int d = (i < qIdx) ? quotient[i] : 0;
            appendIntHead(res, d); // 高位到低位，头插转为低位在前
        }
    }
    else {
        appendIntTail(res, 0); // 整数部分为0
    }

    // ========== 修改2：小数位数组，11位用于四舍五入 ==========
    int decStart = newIntLen;
    int decDigits[11] = { 0 };
    for (int i = 0; i < 11; i++) {
        int pos = decStart + i;
        if (pos < 0) decDigits[i] = 0;
        else if (pos < qIdx) decDigits[i] = quotient[pos];
        else decDigits[i] = 0;
    }

    // ========== 修改3：四舍五入逻辑，第11位判断，进位到第10位 ==========
    int carry = 0;
    if (decDigits[10] >= 5) {
        carry = 1;
        // 从第10位（索引9）往前进位
        for (int i = 9; i >= 0 && carry; i--) {
            decDigits[i] += carry;
            if (decDigits[i] >= 10) {
                decDigits[i] -= 10;
                carry = 1;
            }
            else {
                carry = 0;
            }
        }
        // 进位到整数部分
        if (carry) {
            Node1* pInt = res.intHead;
            while (pInt != NULL && carry) {
                pInt->digit += carry;
                if (pInt->digit >= 10) {
                    pInt->digit -= 10;
                    carry = 1;
                }
                else {
                    carry = 0;
                }
                pInt = pInt->next;
            }
            if (carry) appendIntTail(res, 1);
        }
    }

    // ========== 修改4：把前10位小数全部存入结果 ==========
    for (int i = 0; i < 10; i++) {
        appendDecTail(res, decDigits[i]);
    }

    // 运算完成后标准化，彻底去除无意义0（只删末尾的0，保留有效位）
    standardizeBigNumber(res);

    // 释放临时内存
    clearList(A);
    clearList(B);
    clearList(remainder);
    delete[] arrA;
}

// 高精度乘法：a * b，结果存入res
void multiply(BigNumber& a, BigNumber& b, BigNumber& res) {
    initBigNumber(res);
    // 处理符号
    res.isNegative = (a.isNegative != b.isNegative);
    // 有一个为0，结果为0
    if (isZero(a) || isZero(b)) {
        appendIntTail(res, 0);
        standardizeBigNumber(res);
        return;
    }

    // 把两个数转为纯整数数组（低位在前）
    int totalLenA = a.intLen + a.decLen;
    int totalLenB = b.intLen + b.decLen;
    int maxLen = totalLenA + totalLenB + 2;
    int* temp = new int[maxLen](); // 乘法结果数组，初始化为0

    // ==================== 核心修复开始：正确提取a的所有位（低位在前） ====================
    int* arrA = new int[totalLenA];
    int* tempA = new int[totalLenA]; // 临时存纯整数的高位到低位
    int idx = 0;
    Node1* p = a.intTail; // 1. 先取整数部分：从高位(intTail)到低位(intHead)
    while (p != NULL) {
        tempA[idx++] = p->digit;
        p = p->prev;
    }
    p = a.decHead; // 2. 再取小数部分：从高位(decHead)到低位(decTail)
    while (p != NULL) {
        tempA[idx++] = p->digit;
        p = p->next;
    }
    // 3. 反转tempA，得到低位在前的arrA
    for (int i = 0; i < totalLenA; i++) {
        arrA[i] = tempA[totalLenA - 1 - i];
    }
    delete[] tempA;
    // ==================== 核心修复结束 ====================

    // ==================== 核心修复开始：正确提取b的所有位（低位在前） ====================
    int* arrB = new int[totalLenB];
    int* tempB = new int[totalLenB]; // 临时存纯整数的高位到低位
    idx = 0;
    p = b.intTail; // 1. 先取整数部分：从高位(intTail)到低位(intHead)
    while (p != NULL) {
        tempB[idx++] = p->digit;
        p = p->prev;
    }
    p = b.decHead; // 2. 再取小数部分：从高位(decHead)到低位(decTail)
    while (p != NULL) {
        tempB[idx++] = p->digit;
        p = p->next;
    }
    // 3. 反转tempB，得到低位在前的arrB
    for (int i = 0; i < totalLenB; i++) {
        arrB[i] = tempB[totalLenB - 1 - i];
    }
    delete[] tempB;
    // ==================== 核心修复结束 ====================

    // 乘法核心运算
    for (int i = 0; i < totalLenA; i++) {
        for (int j = 0; j < totalLenB; j++) {
            temp[i + j] += arrA[i] * arrB[j];
        }
    }

    // 处理进位
    int carry = 0;
    for (int i = 0; i < maxLen - 1; i++) {
        int sum = temp[i] + carry;
        temp[i] = sum % 10;
        carry = sum / 10;
    }

    // 结果的小数位数 = 两个数的小数位数之和
    int decPos = a.decLen + b.decLen;

    // 处理整数部分
    bool intStart = false;
    for (int i = maxLen - 1; i >= decPos; i--) {
        if (!intStart && temp[i] == 0) continue;
        intStart = true;
        appendIntHead(res, temp[i]);
    }
    if (!intStart) appendIntTail(res, 0);

    // 处理小数部分
    for (int i = decPos - 1; i >= 0; i--) appendDecTail(res, temp[i]);

    // 运算完成后标准化，彻底去除无意义0
    standardizeBigNumber(res);

    // 释放内存
    delete[] temp;
    delete[] arrA;
    delete[] arrB;
}

// ==================== 输出函数 ====================
// 按题目要求格式化输出大数：整数部分每三位加逗号，小数部分无逗号
QString printBigNumber(BigNumber& num) {
    // 输出负号：负数且非0才输出
    QString result;
    if (num.isNegative && !isZero(num))result+='-';

    // 提取整数部分（高位在前）
    int* intDigits = new int[num.intLen];
    Node1* p = num.intTail;
    for (int i = 0; i < num.intLen; i++) {
        intDigits[i] = p->digit;
        p = p->prev;
    }

    // 输出整数部分，每三位加逗号
    for (int i = 0; i < num.intLen; i++) {
        result+= char(intDigits[i]+48);
        int remain = num.intLen - i - 1;
        if (remain % 3 == 0 && i != num.intLen - 1) result+= ',';
    }

    // 输出小数部分：只有小数长度>0时才输出小数点和小数
    if (num.decLen > 0) {
        result+='.';
        p = num.decHead;
        while (p != NULL) {
            result+=char(p->digit+48);
            p = p->next;
        }
    }

    delete[] intDigits;
    return result;
}
