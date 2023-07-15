#ifndef COMPUTERINFO_H
#define COMPUTERINFO_H

#include <QMainWindow>
#include <QtGlobal>
#include <QDebug>
#include <QSysInfo>
#include <QStorageInfo>
#include <QNetworkInterface>
#include <QSettings>
#include <QProcess>


#include <windows.h>
#include <intrin.h>


#include <QMenuBar>
#include <QMenu>
#include <QAction>

//----Для вывода текста информации о компьютере----//
//#include <QGraphicsScene>
//#include <QGraphicsView>
//#include <QGraphicsTextItem>
#include <QLabel>
#include <QHBoxLayout>
#include <QTableWidget>
//-------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class ComputerInfo; }
QT_END_NAMESPACE

class ComputerInfo : public QMainWindow
{
    Q_OBJECT

public:
    ComputerInfo(QWidget *parent = nullptr);
    ~ComputerInfo();


//--------INFO COMPUTER--------//
private:
    void getSystemInfo();
    void getNetworkInfo();
    QVector<QPair<QString,QString>> getBiosInfo();
    void getCPUInfo();
    void getGPUInfo();

private slots:
    void slotShowComputerInfo();
//-----------------------------//
private:
    QMenuBar* mainMenuBar;
    QMenu*    mainMenu;
    QAction*  actionShowInfoComputer;

    QWidget* widgetComputerInfo;


    Ui::ComputerInfo *ui;
};
#endif // COMPUTERINFO_H
