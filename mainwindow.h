#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class HengxuanCaculator;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_zero_clicked();

    void on_one_clicked();

    void on_two_clicked();

    void on_three_clicked();

    void on_four_clicked();

    void on_five_clicked();

    void on_six_clicked();

    void on_seven_clicked();

    void on_eight_clicked();

    void on_nine_clicked();

    void on_clear_clicked();

    void on_cancel_clicked();

    void on_decimalPoint_clicked();

    void on_subtract_clicked();

    void on_add_clicked();

    void on_mutiple_clicked();

    void on_division_clicked();

    void on_equal_clicked();

    void on_comma_clicked();

    void on_minus_clicked();

private:
    Ui::HengxuanCaculator *ui;
};
#endif // MAINWINDOW_H
