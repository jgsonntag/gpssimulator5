#include "setupgui.h"


setupGui::setupGui(aircraft *myplane, QDateTime *mydatetime,
                   QWidget *parent, const char *name)
{

    // Set the local plane and date/time pointers to the parent's
    setupplane = myplane;
    setupdatetime = mydatetime;

    // Set up the default starting values
    QString lats,lons,hts;
    lats.setNum(setupplane->getLat());
    lons.setNum(setupplane->getLon());
    hts.setNum(setupplane->getAlt());

    // Set up the layout
    QWidget *dialog = new QWidget(this);
    QGridLayout *grid = new QGridLayout(this);

    // Create the component widgets of the dialog
    QLabel *startlatlabel = new QLabel("Enter starting latitude (dd.dd, -90 to 90)");
    startlat = new QLineEdit;
    startlat->setText(lats);
    QLabel *startlonlabel = new QLabel("Enter starting longitude (dd.dd, -180 to 360)");
    startlon = new QLineEdit;
    startlon->setText(lons);
    QLabel *starthtlabel = new QLabel("Enter starting altitude (ft, -9999 to 99999)");
    startht = new QLineEdit;
    startht->setText(hts);
    QLabel *datetimelabel = new QLabel("Enter start date/time");
    datetimeedit = new QDateTimeEdit(QDateTime::currentDateTimeUtc());
    datetimeedit->setMinimumDate(QDate::currentDate().addDays(-365));
    datetimeedit->setMaximumDate(QDate::currentDate().addDays(365));
    datetimeedit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    QPushButton *contbutton = new QPushButton("Continue");
    connect(contbutton,SIGNAL(clicked()),this,SLOT(slotTestInputs()));

    // Create the validators
    latval = new QDoubleValidator;
    latval->setRange(-90.0,90.0);
    latval->setDecimals(2);
    startlat->setValidator(latval);
    lonval = new QDoubleValidator;
    lonval->setRange(-180.0,360.0);
    lonval->setDecimals(2);
    startlon->setValidator(lonval);
    htval = new QDoubleValidator;
    htval->setRange(-9999,99999);
    htval->setDecimals(2);
    startht->setValidator(htval);

    // Populate the grid layout with the compoonents
    grid->addWidget(startlatlabel,1,1);
    grid->addWidget(startlat,1,2);
    grid->addWidget(startlonlabel,2,1);
    grid->addWidget(startlon,2,2);
    grid->addWidget(starthtlabel,3,1);
    grid->addWidget(startht,3,2);
    grid->addWidget(datetimelabel,4,1);
    grid->addWidget(datetimeedit,4,2);
    grid->addWidget(contbutton,5,1,1,-1);

    // Make it all happen
    dialog->setLayout(grid);
    dialog->show();

}


setupGui::~setupGui()
{
    delete startlat;
    delete startlon;
    delete startht;
    delete latval;
    delete lonval;
    delete htval;
}


void setupGui::slotTestInputs()
{
    int pos;
    QString stemp = startlat->text();
    bool latisvalid = (latval->validate(stemp,pos)==QValidator::Acceptable);
    stemp = startlon->text();
    bool lonisvalid = (lonval->validate(stemp,pos)==QValidator::Acceptable);
    if (latisvalid&&lonisvalid)
    {
      stemp = startlat->text();  setupplane->setLat(stemp.toDouble());
      stemp = startlon->text();  setupplane->setLon(stemp.toDouble());
      stemp = startht->text();  setupplane->setAlt(stemp.toDouble());
      QDateTime datetime1 = datetimeedit->dateTime();
      QDate date1 = datetime1.date();
      QTime time1 = datetime1.time();
      int hr = time1.hour();
      int min = time1.minute();
      int sec = time1.second();
      time1.setHMS(hr,min,sec,0);
      setupdatetime->setDate(date1);
      setupdatetime->setTime(time1);
      accept();
    }
    else
      reject();

}

