#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QString>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFont>

#include <math.h>
#include "setupgui.h"
#include "genericserver.h"
#include "aircraft.h"

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT    

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();
    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *exitAct;
    QAction *aboutAct;
    QTimer *timer;
    QTime time1;
    QDate date1;
    double fracsecs;
    double mod05;
    GenericServer *server4060;
    GenericServer *server4061;
    GenericServer *server4062;
    GenericServer *server4063;
    QString string10;
    QString string11;
    QDateTime *datetime;
    aircraft *myplane;
    int wkn;
    double wks;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;
    QLabel *timelabel;
    QLabel *positionlabel;
    QString text;
    QLabel *label4060;
    QLabel *msg4060;
    QLabel *label4061;
    QLabel *msg4061;
    QLabel *label4062;
    QLabel *msg4062;
    QLabel *label4063;
    QLabel *msg4063;

private slots:
    void slotAbout();
    void slotUpdate();

};

#endif // MAINWINDOW_H
