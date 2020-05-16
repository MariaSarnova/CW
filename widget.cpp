#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <windows.h>
#include <TlHelp32.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    enumProcess();
    connect(this,&Widget::refreshProcTab,this,&Widget::enumProcess);

    ui->processTab->setColumnWidth(0,250);

}

uint Widget::getPid()
{
    int row = ui->processTab->currentRow();
    uint pid = ui->processTab->item(row,1)->text().toUInt();
    return pid;
}


void Widget::clearProcTab()
{
    int rowCount = ui->processTab->rowCount();

    for( int i = 0; i < rowCount; i++ )
    {
        ui->processTab->removeRow(0);
    }
}


void Widget::enumProcess()
{
    clearProcTab();

    HANDLE snapHandele = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);

    if( INVALID_HANDLE_VALUE == snapHandele)
    {
        qDebug() << "CreateToolhelp32Snapshot error" ;
        return;
    }

    PROCESSENTRY32 entry = {0};
    entry.dwSize = sizeof(entry);

    BOOL ret = Process32First(snapHandele,&entry);
    int i = 0;

    while (ret) {
        QString exeFile = QString::fromWCharArray(entry.szExeFile);
        ui->processTab->insertRow(i);
        ui->processTab->setItem(i,0,new QTableWidgetItem(exeFile));
        ui->processTab->setItem(i,1,new QTableWidgetItem(QString("%1").arg(entry.th32ProcessID)));
        ui->processTab->setItem(i,2, new QTableWidgetItem(QString("%1").arg(entry.cntThreads)));
        i++;
        ret = Process32Next(snapHandele,&entry);
    }
    CloseHandle(snapHandele);
}
Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_5_clicked()
{

    this->close();
}


void Widget::on_pushButton_clicked()
{

    int row = ui->processTab->currentRow();
    uint pid = getPid();

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);

    if( hProc == NULL)
    {
        qDebug() << "OpenProcess error ";
        return;
    }
    BOOL ret = TerminateProcess(hProc,0);

    if(ret == FALSE)
    {
        qDebug() << "TerminateProcess error ";
        return ;
    }

    ui->processTab->removeRow(row);
    CloseHandle(hProc);
}


void Widget::on_pushButton_2_clicked()
{
    uint pid = getPid();
    HANDLE snapHandele = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,NULL);

    if( INVALID_HANDLE_VALUE == snapHandele)
    {
        qDebug() << "CreateToolhelp32Snapshot error" ;
        return;
    }
    THREADENTRY32 entry = {0};
    entry.dwSize = sizeof(entry);
    BOOL ret = Thread32First(snapHandele,&entry);
    while( ret )
    {
        if( entry.th32OwnerProcessID == pid)
        {

            HANDLE tHandle = OpenThread(THREAD_ALL_ACCESS,FALSE,entry.th32ThreadID);
            if( tHandle == NULL)
            {
                qDebug() << "OpenThread error，threadId = " << entry.th32ThreadID;
            }
            else
            {
                DWORD ret = SuspendThread(tHandle);
                if( ret == -1)
                {
                    qDebug() << "SuspendThread error";
                }else
                {
                    qDebug() << "SuspendThread success";
                }
                CloseHandle(tHandle);
            }
        }
        ret = Thread32Next(snapHandele,&entry);
    }
    CloseHandle(snapHandele);
}


void Widget::on_pushButton_3_clicked()
{
    uint pid = getPid();
    HANDLE snapHandele = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,NULL);

    if( INVALID_HANDLE_VALUE == snapHandele)
    {
        qDebug() << "CreateToolhelp32Snapshot error" ;
        return;
    }

    THREADENTRY32 entry = {0};
    entry.dwSize = sizeof(entry);
    BOOL ret = Thread32First(snapHandele,&entry);

    while( ret )
    {
        if( entry.th32OwnerProcessID == pid)
        {

            HANDLE tHandle = OpenThread(THREAD_ALL_ACCESS,FALSE,entry.th32ThreadID);

            if( tHandle == NULL)
            {
                qDebug() << "OpenThread error，threadId = " << entry.th32ThreadID;
            }

            else
            {
                DWORD ret = ResumeThread(tHandle);

                if( ret == -1)
                {
                    qDebug() << "SuspendThread error";
                }
                else
                {
                    qDebug() << "ResumeThread success";
                }
                CloseHandle(tHandle);
            }
        }
        ret = Thread32Next(snapHandele,&entry);
    }
}


void Widget::on_pushButton_7_clicked()
{

    enumProcess();
}


