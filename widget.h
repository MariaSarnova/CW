#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    // 枚举进程
    void enumProcess();
    // 获取进程表选中的进程ID
    uint getPid();
    // 提升权限


    // 清空进程表
    void clearProcTab();
    // 清空DLL表
    void clearDLLTab();
signals:
    // 刷新进程表
    void refreshProcTab();
private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();



    void on_pushButton_7_clicked();



private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
