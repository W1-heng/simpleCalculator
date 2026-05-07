
#include<string>
#include <QString>
using namespace std;
struct node
{
    int num=0;//用字符类型，存储被算数每一位数字字符
    char dot='\0';
    node* next=nullptr;
    node* before = nullptr;
};
void delete_Node(node* head)
{
    node* p = head;
    while (p != nullptr)
    {
        node* temp = p;
        p = p->next;
        delete temp;
        temp = nullptr;
    }
}
QString print(node* head, node* end, std::string num1, std::string num2)//打印要去除前导0！！！,注意是顺序输出
{
    QString result;
    int a = num1.find('.');
    int b = num2.find('.');//用来判断是不是小数,不是小数会返回-1
    node* tmp = head;
    node* head1 = head;
    node* tmp2 = nullptr;
    size_t total = 0;
    bool leading_Zero = true;
    bool flag = false;//标记整数部分结果是否为0

    while (tmp != nullptr)// 重新计算实际位数，保证在所有情况下末尾没有逗号
    // 并从低位（右侧）每三位插入逗号，输出为从高位到低位的顺序
    {
        if (tmp->num != 0) leading_Zero = false;//注意排除前导零对链表长度的影响
        if (!leading_Zero && tmp->dot != '.') total++;
        if ( tmp->dot == '.') break;//遇到小数点停止计数
        tmp = tmp->next;
    }

    if (!total)
    {
        //cout << 0;
         result +=QChar('0');
        flag = true;

    }//整数部分为零，即结果为零，直接打印零
    leading_Zero = true;//恢复前导0状态
    if (!flag)
    {
        while (head1 != nullptr && head1->dot != 46)//没遇到小数点就继续遍历整数部分
        {

            if (head1->num != 0)leading_Zero = false;
            if (!leading_Zero)
            {
                //cout << head1->num;
                result +=QChar(head1->num+48);
                // 打印后更新剩余位数并判断是否需要插入逗号：
                // 若剩余位数能被3整除且不为0，说明当前位置后需要逗号
                if (--total % 3 == 0 && total != 0)
                {
                    //cout << ',';
                     result +=QChar(',');
                }
            }
            head1 = head1->next;
        }//以上是打印整数部分
    }

    if (a != -1 || b != -1)//至少有一个是小数,说明需要处理小数部分
    {

        if (head1 == nullptr || head1->next == nullptr)
        {
            // 结果链表为空或没有小数节点，直接结束（不打印小数部分）
            //cout << endl;
            return result;
        }


        tmp2 = tmp->next;
        tmp = tmp->next;//跳过小数点节点，开始打印小数部分


        bool decimal_Zero = true;//判断小数部分是否全为0
        while (tmp != nullptr)
        {
            if (tmp->num != 0)
            {
                decimal_Zero = false;
                break;
            }
            tmp = tmp->next;
        }//第一遍遍历，判断小数部分是否全为0，打印小数点
        if (!decimal_Zero)//cout << '.';
              result +=QChar('.');
        // 在使用 end 之前确保 end 不为 nullptr
        if (end == nullptr)
        {
            // 没有尾节点说明没有小数可打印
            //cout << endl;
            return result;
        }

        while (end->num == 0 && !decimal_Zero)//从尾节点开始删除小数部分末尾的多余0
        {
            //用end标记最后一个非0节点
            // 这里要注意避免越过小数点：如果 end->before 为 nullptr 或 end 指向小数点之前，则停止
            if (end->before == nullptr) break;
            end = end->before;
            if (end->dot == '.') break; // 到达小数点则停止
        }
        // 再次检查 tmp2 和 end 是否有效，防止 tmp2 为 nullptr 或 end->next 访问越界
        while (tmp2 != nullptr && tmp2 != end->next && !decimal_Zero)//第二遍遍历，打印小数部分
        {
            //cout << tmp2->num;
             result +=QChar(tmp2->num+48);
            tmp2 = tmp2->next;
        }


    }
    //cout << endl;
    return result;
}
bool num_Cp(node* head1, node* head2, node* division1, node* division2,bool& zero)//比较哪个数更大，减法时要用
{
    node* tmp1 = division1;
    node* tmp2 = division2;//存储小数点位置，可作为整数比较的结束位置，也能为小数部分的比较提供开始位置
    while (division1->before != nullptr && division2->before != nullptr)//先比较整数部分长度
    {
        division1 = division1->before;
        division2 = division2->before;
    }
    if (division1->before == nullptr && division2->before != nullptr)//第一个数整数部分更短
        return false;
    if (division1->before != nullptr && division2->before == nullptr)
        return true;//第二个数整数部分更短
    while (head1 != tmp1)
    {
        if (head1->num > head2->num)
            return true;
        if (head1->num < head2->num)
            return false;
        else
        {
            head1 = head1->next;
            head2 = head2->next;
        }
    }
    while (tmp1->next != nullptr && tmp2->next != nullptr)//整数部分相等，比较小数部分
    {
        if (tmp1->next->num > tmp2->next->num)
            return true;
        if (tmp1->next->num < tmp2->next->num)
            return false;
        else
        {
            tmp1 = tmp1->next;
            tmp2 = tmp2->next;
        }
    }
    if (tmp1->next == nullptr && tmp2->next != nullptr)//第一个数小数部分更短
        return false;
    if (tmp1->next != nullptr && tmp2->next == nullptr)//第二个数小数部分更短
        return true;

    zero = true;//判断答案为零
    return true;//完全相等，返回true

}


void create_Node(node*& head, const std::string &num1,node *& division)//重载构造函数，生成只包含小数点和数字的链表，这里是顺序输入，先把整数和小数连成一条链表，在计算时再分开计算
{//记录好小数点位置的指针division

    head = nullptr;
    node* p = nullptr;
    int k = 0;
    int stlen = num1.length();
    // 从字符串末尾向前遍历，遇到数字则创建节点（跳过逗号和符号）
    // 从字符串末尾向前遍历，遇到数字则创建节点（跳过逗号和符号）
    if (head == nullptr)
    {
        if (num1[0] == '-')k = 1;


        head = new node;
        head->before = nullptr;
        p = head;
        p->next = nullptr;
        head->num = num1[k] - '0';

    }

    for (int i = k + 1; i < stlen; i++)
    {

        if (num1[i] != ',')//跳过逗号
        {
            if (num1[i] != '.')
            {
                node* q = new node;
                q->before = p;
                q->num = num1[i] - '0';
                p->next = q;
                p = q;
                p->next = nullptr;
            }
            if (num1[i] == '.')
            {
                node* q = new node;
                q->before = p;
                q->dot= num1[i];
                p->next = q;
                p = q;
                p->next = nullptr;
                division = p;//记录小数点节点位置
            }


        }
    }
    if (division == nullptr)
    {
        node* q = new node;
        q->before = p;
        q->dot = '.';
        p->next = q;
        p = q;
        p->next = nullptr;
        division = p;//没有小数点就创建一个小数点节点来对齐计算
    }

    return;
}



node* add(node* division1, node* division2, node*& end)
{
    node* p1 = division1;//指向第一个数的小数点
    node* p2 = division2;//指向第二个数的小数点
    node* p = nullptr;
    node* q = nullptr;

    while (p1->before != nullptr && p2->before != nullptr)//从尾节开始遍历，模拟加法的从低位对齐相加,计算整数部分
    {
        p1->before->num += p2->before->num;//直接把结果存在p1的节点中，节省空间.在计算中不用考虑p1比p2长的情况，因为天然的保留了p1的多余位
        p1 = p1->before;
        p2 = p2->before;

    }
    if (p1->before == nullptr)//处理两个链表长度不等的 情况
    {
        while (p2->before != nullptr)
        {

            q = new node;
            q->num = p2->before->num;
            p1->before = q;
            q->next = p1;
            q->before = nullptr;
            p1 = q;//更新迭代指针p1，继续向前遍历
            p2 = p2->before;
        }

    }
    p1 = division1;
    p2 = division2;//恢复指针位置回到小数点位置

    while (p1->next != nullptr && p2->next != nullptr)//从小数点开始遍历，模拟加法的从高位位对齐相加,计算小数部分
    {
        p1->next->num += p2->next->num;
        p1 = p1->next;
        p2 = p2->next;
    }
    if (p1->next == nullptr && p2->next != nullptr)//意味着第二个数比第一个数小数部分更多
    {

        while (p2->next != nullptr)
        {

            q = new node;
            q->num = p2->next->num;
            p1->next = q;
            q->before = p1;
            q->next = nullptr;
            p1 = q;//更新迭代指针p1，继续向后遍历
            p2 = p2->next;
        }

    }
    while (p1->next != nullptr)
    {
        p1 = p1->next;
    }//第一个数小数部分比第二个数小数部分长，手动把p1调到末尾节点，以赋值给end，否则end接收到的不是尾节点
    end = p1;//记录结果链表的尾节点位置
    //天然地把结果存在了division1链表中，现在需要处理进位问题，并且在计算小数部分时，已经把p1移动到了最后一位
    for (node* a = p1, *b = a->before; ; a = b, b = a->before)//双指针遍历链表，进行进位操作
    {
        // 如果 a 指向小数点，先安全地跨过小数点
        if (a->dot == '.')
        {
            // 确保可以安全移动到小数点前一位
            if (a->before == nullptr || b == nullptr)
                break; // 无更高位可处理，退出
            a = a->before;
            b = b->before;//最后一位是小数点，跳过小数点
        }


        if (b != nullptr && a->num >= 10)
        {
            if (b->dot != '.')
            {
                b->num++;
                a->num -= 10;
            }
            else // b 是小数点，需要将进位传到小数点前一位
            {
                // 若 b->before 为 nullptr，则先创建一个新的高位节点以接收进位
                if (b->before == nullptr)
                {
                    node* c = new node;
                    c->num = 1;
                    c->before = nullptr;
                    c->next = b;
                    b->before = c;
                }
                else
                {
                    b->before->num++;
                }
                a->num -= 10;
                // 跳过小数点继续向高位传播
                b = b->before;
                // a 也向前移动一位（跳过小数点时已在上面处理）
                if (a->before != nullptr)
                    a = a->before;
                else
                    a = b;
            }
        }

        // 处理最高位进位,并退出循环
        if (b == nullptr)
        {
            if (a->num >= 10)
            {
                node* c = new node;
                c->num = 1;
                a->num -= 10;
                a->before = c;
                c->next = a;
                c->before = nullptr;
                return c;//返回新的头节点
            }
            return a;//返回头节点
        }
    }


}


node* sub(node* division1, node* division2, node*& end)//减函数,head1减head2
{
    node* p1 = division1;//指向第一个数的小数点或最后一位
    node* p2 = division2;//指向第二个数的小数点或最后一位
    node* p = nullptr;
    node* q = nullptr;

    while (p1->before != nullptr && p2->before != nullptr)//从尾节开始遍历，模拟加法的从低位对齐相加,计算整数部分
    {
        // 若源为小数点则跳过对该节点的数值操作
        if (p2->before->dot != '.')
            p1->before->num -= p2->before->num;
        p1 = p1->before;
        p2 = p2->before;

    }

    p1 = division1;
    p2 = division2;//恢复指针位置回到小数点位置

    while (p1->next != nullptr && p2->next != nullptr)//从小数点开始遍历，模拟加法的从高位位对齐相加,计算小数部分
    {
        if (p1->next->dot != '.' && p2->next->dot != '.')
            p1->next->num -= p2->next->num;
        p1 = p1->next;
        p2 = p2->next;
    }
    if (p1->next == nullptr && p2->next != nullptr)//意味着第二个数比第一个数小数部分更多
    {

        while (p2->next != nullptr)
        {
            // 跳过小数点节点
            if (p2->next->dot == '.') { p2 = p2->next; continue; }

            q = new node;
            q->num = -(p2->next->num);
            p1->next = q;
            q->before = p1;
            q->next = nullptr;
            p1 = q;//更新迭代指针p1，继续向后遍历
            p2 = p2->next;
        }

    }
    while (p1->next != nullptr)
    {
        p1 = p1->next;
    }//第一个数小数部分比第二个数小数部分长，或p1因p2增加了节点，需要把p1调到末尾节点，以赋值给end，否则end接收到的不是尾节点
    end = p1;//记录结果链表的尾节点位置
    //天然地把结果存在了division1链表中，现在需要处理进位问题，并且在计算小数部分时，已经把p1移动到了最后一位
    for (node* a = p1, *b = a->before; ; a = b, b = a->before)//双指针遍历链表，进行进位操作
    {
        // 如果 a 指向小数点，先安全地跨过小数点
        if (a->dot == '.')
        {
            // 必须先检查 before 指针的有效性，避免空指针
            if (a->before == nullptr || b == nullptr)
                break;
            a = a->before;
            b = b->before;//最后一位是小数点，跳过小数点
        }

        // 只在 b 非空的情况下访问 b->num
        if (b != nullptr && a->num < 0)
        {
            if (b->dot != '.')
            {
                b->num--;
                a->num += 10;
            }
            else
            {
                b->before->num--;
                a->num += 10;
            }
        }

        if (b == nullptr)//处理最高位退位,并退出循环
        {

            return a;


        }
    }
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
