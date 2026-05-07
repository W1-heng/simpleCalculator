#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "addAndSubtract.h"
#include "multiplyAndDivision.h"
#include <string>
#include<QString>

std::string num1,num2;
std::string symbol;
QString result;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HengxuanCaculator)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_zero_clicked()
{
    ui->lineEdit1->setText(ui->lineEdit1->text()+"0");
}


void MainWindow::on_one_clicked()
{
     ui->lineEdit1->setText(ui->lineEdit1->text()+"1");
}


void MainWindow::on_two_clicked()
{
    ui->lineEdit1->setText(ui->lineEdit1->text()+"2");
}



void MainWindow::on_three_clicked()
{
     ui->lineEdit1->setText(ui->lineEdit1->text()+"3");
}




void MainWindow::on_four_clicked()
{
     ui->lineEdit1->setText(ui->lineEdit1->text()+"4");
}



void MainWindow::on_five_clicked()
{
     ui->lineEdit1->setText(ui->lineEdit1->text()+"5");
}



void MainWindow::on_six_clicked()
{
     ui->lineEdit1->setText(ui->lineEdit1->text()+"6");
}


void MainWindow::on_seven_clicked()
{
     ui->lineEdit1->setText(ui->lineEdit1->text()+"7");
}


void MainWindow::on_eight_clicked()
{
     ui->lineEdit1->setText(ui->lineEdit1->text()+"8");
}


void MainWindow::on_nine_clicked()
{
     ui->lineEdit1->setText(ui->lineEdit1->text()+"9");
}

void MainWindow::on_decimalPoint_clicked()
{
     ui->lineEdit1->setText(ui->lineEdit1->text()+".");
}

void MainWindow::on_clear_clicked()
{
    ui->lineEdit1->clear();
}

void MainWindow::on_cancel_clicked()
{
    QString str = ui->lineEdit1->text();
    str.chop(1);//chop，砍的意思，从后往前删除几个字符
    ui->lineEdit1->setText(str);
}

void MainWindow::on_add_clicked()
{
    num1=ui->lineEdit1->text().toStdString();  
    symbol='+';
    ui->lineEdit1->clear();
}


void MainWindow::on_subtract_clicked()
{
    num1=ui->lineEdit1->text().toStdString();
    symbol='-';
    ui->lineEdit1->clear();
}


void MainWindow::on_mutiple_clicked()
{
    num1=ui->lineEdit1->text().toStdString();
    symbol='*';
    ui->lineEdit1->clear();
}


void MainWindow::on_division_clicked()
{
    num1=ui->lineEdit1->text().toStdString();
    symbol='/';
    ui->lineEdit1->clear();
}
void MainWindow::on_comma_clicked()
{
     ui->lineEdit1->setText(ui->lineEdit1->text()+",");
}
void MainWindow::on_minus_clicked()
{
    ui->lineEdit1->setText(ui->lineEdit1->text()+"-");
}
void MainWindow::on_equal_clicked()
{
    num2=ui->lineEdit1->text().toStdString();
    node* division1 = nullptr;
    node* division2 = nullptr;//小数点节点指针
    node* head1 = nullptr;
    node* head2 = nullptr;//整个数的头指针
    node* end = nullptr;   //结果链表尾节点指针
    bool zero = false;
    node* newHead = nullptr;

    create_Node(head1,  num1, division1);
    create_Node(head2, num2, division2);

    ui->lineEdit1->clear();
    if (symbol[0] == '+')//处理两正数相加或两负数相加的情况
    {

        if (num1[0] != '-' && num2[0] != '-')
        {
            newHead = add(division1, division2, end);
            result = print(newHead, end, num1, num2);

            ui->lineEdit1->setText(ui->lineEdit1->text()+result);


        }
        if (num1[0] == '-' && num2[0] == '-')
        {
            newHead = add(division1, division2, end);
            ui->lineEdit1->setText(QString('-'));
            result =print(newHead, end, num1, num2);
            ui->lineEdit1->setText(ui->lineEdit1->text()+result);
        }
        if (num1[0] != '-' && num2[0] == '-')
        {
            if (num_Cp(head1, head2, division1, division2, zero))
            {
                newHead = sub(division1, division2, end);
               result = print(newHead, end, num1, num2);
                ui->lineEdit1->setText(ui->lineEdit1->text()+result);
            }
            else
            {
                newHead = sub(division2, division1, end);
                ui->lineEdit1->setText(QString('-'));
               result = print(newHead, end, num1, num2);
                 ui->lineEdit1->setText(ui->lineEdit1->text()+result);
            }
        }
        if (num1[0] == '-' && num2[0] != '-')
        {
            if (num_Cp(head1, head2, division1, division2, zero))
            {
                if (!zero) ui->lineEdit1->setText(QString('-'));
                newHead = sub(division1, division2, end);
                result =print(newHead, end, num1, num2);
                ui->lineEdit1->setText(ui->lineEdit1->text()+result);
            }
            else
            {
                newHead = sub(division2, division1, end);
                result =print(newHead, end, num1, num2);
                ui->lineEdit1->setText(ui->lineEdit1->text()+result);
            }
        }

    }

    if (symbol[0] == '-')
    {
        if (num1[0] != '-' && num2[0] != '-')
        {
            if (num_Cp(head1, head2, division1, division2, zero))
            {
                newHead = sub(division1, division2, end);
               result = print(newHead, end, num1, num2);
                ui->lineEdit1->setText(ui->lineEdit1->text()+result);
            }
            else
            {
                newHead = sub(division2, division1, end);
               ui->lineEdit1->setText(QString('-'));
                result =print(newHead, end, num1, num2);

                ui->lineEdit1->setText(ui->lineEdit1->text()+result);
            }
        }
        if (num1[0] == '-' && num2[0] == '-')
        {

            if (num_Cp(head1, head2, division1, division2, zero))
            {
                newHead = sub(division1, division2, end);
                if (!zero)ui->lineEdit1->setText(QString('-'));
               result = print(newHead, end, num1, num2);
                ui->lineEdit1->setText(ui->lineEdit1->text()+result);
            }
            else
            {
                newHead = sub(division2, division1, end);
                result =print(newHead, end, num1, num2);
                 ui->lineEdit1->setText(ui->lineEdit1->text()+result);
            }
        }
        if (num1[0] != '-' && num2[0] == '-')
        {
            newHead = add(division1, division2, end);
            result =print(newHead, end, num1, num2);
            ui->lineEdit1->setText(ui->lineEdit1->text()+result);
        }
        if (num1[0] == '-' && num2[0] != '-')
        {
            newHead = add(division2, division1, end);
            ui->lineEdit1->setText(QString('-'));
            result = print(newHead, end, num1, num2);

           ui->lineEdit1->setText(ui->lineEdit1->text()+result);
        }
    }

        if (head1->before == nullptr && head1->next == nullptr)
        {
            delete_Node(head2);
            head1 = nullptr;
        }
        if (head2->before == nullptr && head2->next == nullptr)
        {
            delete_Node(head1);
            head2 = nullptr;
        }

        if(symbol[0]=='*'||symbol[0]=='/')
        {
        parse(a, num1);
        parse(b, num2);

        if (symbol[0] == '*') {
            multiply(a, b, res);
            result=printBigNumber(res);
            ui->lineEdit1->setText(ui->lineEdit1->text()+result);
        }
        else if (symbol[0] == '/') {
                    bool error = false;
                    divide(a, b, res, error);
                    result=printBigNumber(res);
                    ui->lineEdit1->setText(ui->lineEdit1->text()+result);

        }

        clearBigNumber(a);
        clearBigNumber(b);
        clearBigNumber(res);
        }


}







