#include "mainwindow.h"
#include "utility.h"
#include "ui_mainwindow.h"

#define LAT0 76.5
#define LON0 291.2
#define ALT0 1500
#define CRS0 90
#define SPEED0 250
#define ROC0 0


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qApp->setStyleSheet("QGroupBox { background: white; font: bold }");

    // Setup the aircraft state vector
    myplane = new aircraft(LAT0,LON0,ALT0,CRS0,SPEED0,ROC0);
    datetime = new QDateTime();

    // Get starting info from user
    setupGui *setup = new setupGui(myplane,datetime,this);
    while (setup->exec() == QDialog::Rejected)
    {
        QMessageBox msgBox;
        msgBox.setText("Invalid input");
        msgBox.exec();
    }

    // Create the network servers
    server4060 = new GenericServer(this,4060);  // 2 Hz message10
    server4061 = new GenericServer(this,4061);  // 10 Hz message10
    server4062 = new GenericServer(this,4062);  // 2 Hz message11
    server4063 = new GenericServer(this,4063);  // 10 Hz message11

    // Start the timer
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slotUpdate()));
    timer->start(100);

    // Build the menu bar
    createActions();
    createMenus();

    // Window management
    setWindowTitle("GPSSimulator5");
    setMinimumSize(160,160);
    resize(530,680);
    QWidget *window = new QWidget(this);

    // Build the controls part of the GUI
    QSpinBox *hdgselector = new QSpinBox();
    hdgselector->setMinimum(0);
    hdgselector->setMaximum(359);
    hdgselector->setSingleStep(1);
    hdgselector->setValue(CRS0);
    hdgselector->setSuffix(" deg true");
    hdgselector->setWrapping(true);
    connect(hdgselector,SIGNAL(valueChanged(int)),myplane,SLOT(slotSetCrs(int)));
    QSpinBox *gsselector = new QSpinBox();
    gsselector->setMinimum(0);
    gsselector->setMaximum(4000);
    gsselector->setSingleStep(10);
    gsselector->setValue(SPEED0);
    gsselector->setSuffix(" knots");
    connect(gsselector,SIGNAL(valueChanged(int)),myplane,SLOT(slotSetSpeed(int)));
    QSpinBox *rocselector = new QSpinBox();
    rocselector->setMinimum(-5000);
    rocselector->setMaximum(5000);
    rocselector->setSingleStep(100);
    rocselector->setValue(ROC0);
    rocselector->setSuffix(" fpm");
    connect(rocselector,SIGNAL(valueChanged(int)),myplane,SLOT(slotSetRoc(int)));
    QHBoxLayout *controlslayout = new QHBoxLayout;
    QGroupBox *controlsbox = new QGroupBox("Heading, groundspeed and rate-of-climb");
    //controlsbox->setFlat(false);
    //controlsbox->setStyleSheet("title: subcontrol-position: top left");
    //controlsbox->setStyleSheet("border: 1px solid gray; margin-top: 2ex");
    controlslayout->addWidget(hdgselector);
    controlslayout->addWidget(gsselector);
    controlslayout->addWidget(rocselector);
    controlsbox->setLayout(controlslayout);

    // Build the text part of the GUI
    timelabel = new QLabel();
    positionlabel = new QLabel();
    QVBoxLayout *statelayout = new QVBoxLayout;
    QGroupBox *statebox = new QGroupBox("Current state");
    //statebox->setFlat(false);
    statelayout->addWidget(timelabel);
    statelayout->addWidget(positionlabel);
    statebox->setLayout(statelayout);

    // Build the network portion of the GUI
    QFont outfont("Courier",8);
    label4060 = new QLabel();
    msg4060 = new QLabel();
    msg4060->setFont(outfont);
    label4061 = new QLabel();
    msg4061 = new QLabel();
    msg4061->setFont(outfont);
    label4062 = new QLabel();
    msg4062 = new QLabel();
    msg4062->setFont(outfont);
    label4063 = new QLabel();
    msg4063 = new QLabel();
    msg4063->setFont(outfont);
    QVBoxLayout *networklayout = new QVBoxLayout;
    QGroupBox *networkbox = new QGroupBox("Network servers");
    networklayout->addWidget(label4060);
    networklayout->addWidget(msg4060);
    networklayout->addWidget(label4061);
    networklayout->addWidget(msg4061);
    networklayout->addWidget(label4062);
    networklayout->addWidget(msg4062);
    networklayout->addWidget(label4063);
    networklayout->addWidget(msg4063);
    networkbox->setLayout(networklayout);

    // Finalize the overall GUI
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(controlsbox);
    layout->addWidget(statebox);
    layout->addWidget(networkbox);
    window->setLayout(layout);
    setCentralWidget(window);

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction(aboutAct);

}


void MainWindow::createActions()
{

    exitAct = new QAction("Exit",this);
    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));

    aboutAct = new QAction("About",this);
    connect(aboutAct,SIGNAL(triggered()),this,SLOT(slotAbout()));

}


void MainWindow::slotAbout()
{
    QMessageBox msgBox;
    msgBox.setText("GPSSimulator5\n"
                   "Qt5-Based Network GPS Server Simulator\n"
                   "Author: John G. Sonntag\n"
                   "Released: 27 March 2018");
    msgBox.exec();
}


void MainWindow::slotUpdate()
{

    // Increment the timetag;
    *datetime = datetime->addMSecs(100);
    date1 = datetime->date();
    time1 = datetime->time();

    // Update the aircraft state
    myplane->update(0.1);

    // Format the timetags
    fracsecs = double(time1.msec()/1000.0);
    year = date1.year();
    month = date1.month();
    day = date1.day();
    hour = time1.hour();
    minute = time1.minute();
    second = time1.second()+fracsecs;
    kal2gps(year,month,day,hour,minute,second,&wkn,&wks);

    // Format the messages
    string10.sprintf("10,%8.1lf,%10.6lf,%10.6lf,%8.2lf,%8.2lf,%6.1lf,%5.0lf\n",
                     wks,myplane->getLat(),myplane->getLon(),myplane->getAlt(),
                     myplane->getCrs(),myplane->getSpeed(),myplane->getRoc());
    string11.sprintf("11,%04d%02d%02d,%02d%02d%04.1lf,%10.6lf,%10.6lf,%8.2lf,",
                   year,month,day,hour,minute,second,myplane->getLat(),
                   myplane->getLon(),myplane->getAlt());
    text.sprintf("%8.2lf,%6.1lf,%5.0lf\n",myplane->getCrs(),myplane->getSpeed(),
                 myplane->getRoc());
    string11.append(text);

    // Output the 10 Hz messages
    server4061->sendText(string10);
    msg4061->setText(string10);
    server4063->sendText(string11);
    msg4063->setText(string11);

    // Output the 2 Hz messages on the even half-sec
    mod05 = fabs(fmod(fracsecs*10.0,5));
    if (mod05<0.01)
    {
        server4060->sendText(string10);
        msg4060->setText(string10);
        server4062->sendText(string11);
        msg4062->setText(string11);
    }

    // Update the GUI
    text.sprintf("UTC date and time: %04d-%02d-%02d %02d:%02d:%4.1lf",
                 year,month,day,hour,minute,second);
    timelabel->setText(text);
    text.sprintf("Latitude: %8.4lf deg\nLongitude: %8.4lf deg\nAltitude: %5.0lf ft",
                 myplane->getLat(),myplane->getLon(),myplane->getAlt());
    positionlabel->setText(text);
    text.sprintf("Serving on port 4060\nActive network listeners: %02d\nCurrent broadcast text:\n",
                 server4060->numActiveConnections());
    label4060->setText(text);
    text.sprintf("Serving on port 4061\nActive network listeners: %02d\nCurrent broadcast text:\n",
                 server4061->numActiveConnections());
    label4061->setText(text);
    text.sprintf("Serving on port 4062\nActive network listeners: %02d\nCurrent broadcast text:\n",
                 server4062->numActiveConnections());
    label4062->setText(text);
    text.sprintf("Serving on port 4063\nActive network listeners: %02d\nCurrent broadcast text:\n",
                 server4063->numActiveConnections());
    label4063->setText(text);

    //qDebug() << server4040->numActiveConnections()
    //         << server4041->numActiveConnections();
    //qDebug() << myplane->getCrs() << myplane->getSpeed() << myplane->getRoc();

}

