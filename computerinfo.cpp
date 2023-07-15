#include "computerinfo.h"
#include "ui_computerinfo.h"



ComputerInfo::ComputerInfo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ComputerInfo)
{
    ui->setupUi(this);


    mainMenuBar            = new QMenuBar(this);
    mainMenu               = new QMenu("Info",this);
    actionShowInfoComputer = new QAction(tr("Computer Info"), this);

    mainMenu->addAction(actionShowInfoComputer);
    mainMenuBar->addMenu(mainMenu);

    this->setMenuBar(mainMenuBar);

    widgetComputerInfo = new QWidget(this, Qt::Window);
    widgetComputerInfo->hide();

    getSystemInfo();
    getNetworkInfo();
    getBiosInfo();
    getCPUInfo();
    getGPUInfo();
    //getGpuInfo();

    connect(actionShowInfoComputer,SIGNAL(triggered()),this, SLOT(slotShowComputerInfo()));
}

ComputerInfo::~ComputerInfo()
{
    delete ui;
}



void ComputerInfo::getSystemInfo()
{
    qDebug() << "System Info";

    qDebug() << "QSysInfo Class";
    qDebug() << "CPU Architecture : " << QSysInfo::currentCpuArchitecture().toLocal8Bit().constData();
    qDebug() << "Product Type :     " << QSysInfo::prettyProductName().     toLocal8Bit().constData();
    qDebug() << "Kernel Type :      " << QSysInfo::kernelType().            toLocal8Bit().constData();
    qDebug() << "Kernel Version :   " << QSysInfo::kernelVersion().         toLocal8Bit().constData();
    qDebug() << "Machine ID :       " << QSysInfo::machineHostName().       toLocal8Bit().constData();
    qDebug() << "-----------------------------------------------------------------";
}

void ComputerInfo::getNetworkInfo()
{
    qDebug() << "Network Info";
    //qDebug() << "Connected Network Informations";

    foreach(QNetworkInterface networkInterface, QNetworkInterface::allInterfaces())
    {
        if (networkInterface.flags().testFlag(QNetworkInterface::IsUp) && !networkInterface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            foreach (QNetworkAddressEntry entry, networkInterface.addressEntries())
            {
                if ( entry.ip().toString().contains(".")){
                    qDebug() << "Interface: " << networkInterface.name().            toLocal8Bit().constData() << " ";
                    qDebug() << "IP:        " << entry.ip().toString().              toLocal8Bit().constData() << " ";
                    qDebug() << "MAC:       " << networkInterface.hardwareAddress(). toLocal8Bit().constData();
                }
            }
        }
    }

    qDebug() << "-----------------------------------------------------------------";
}

QVector<QPair<QString,QString>> ComputerInfo::getBiosInfo()
{
    qDebug() << "BIOS info";
    //qDebug() << "System BIOS informations. ONLY FOR WINDOWS.";
    QMap<QString, QString> map;

    QVector<QPair<QString,QString>> pair;

    QSettings settings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\BIOS", QSettings::NativeFormat);
    qDebug() << "Base Manufacturer :   " << settings.value("BaseBoardManufacturer", "0"). toString().toLocal8Bit().constData();
    qDebug() << "Base Product :        " << settings.value("BaseBoardProduct", "0").      toString().toLocal8Bit().constData();
    qDebug() << "BIOS Vendor :         " << settings.value("BIOSVendor", "0").            toString().toLocal8Bit().constData();
    qDebug() << "BIOS Release Date :   " << settings.value("BIOSReleaseDate", "0").       toString().toLocal8Bit().constData();
    qDebug() << "System Manufacturer : " << settings.value("SystemManufacturer", "0").    toString().toLocal8Bit().constData();
    qDebug() << "Product Name :        " << settings.value("SystemProductName", "0").     toString().toLocal8Bit().constData();
    qDebug() << "System SKU :          " << settings.value("SystemSKU", "0").             toString().toLocal8Bit().constData();

    pair.push_back(QPair<QString,QString>("Base Manufacturer",   settings.value("BaseBoardManufacturer", "0").toString().toLocal8Bit().constData()));
    pair.push_back(QPair<QString,QString>("Base Product",        settings.value("BaseBoardProduct", "0").toString().toLocal8Bit().constData()));
    pair.push_back(QPair<QString,QString>("BIOS Vendor",         settings.value("BIOSVendor", "0").toString().toLocal8Bit().constData()));
    pair.push_back(QPair<QString,QString>("BIOS Release Date",   settings.value("BIOSReleaseDate", "0").toString().toLocal8Bit().constData()));
    pair.push_back(QPair<QString,QString>("System Manufacturer", settings.value("SystemManufacturer", "0").toString().toLocal8Bit().constData()));
    pair.push_back(QPair<QString,QString>("Product Name",        settings.value("SystemProductName", "0").toString().toLocal8Bit().constData()));
    pair.push_back(QPair<QString,QString>("System SKU",          settings.value("SystemSKU", "0").toString().toLocal8Bit().constData()));


    qDebug() << "-----------------------------------------------------------------";

    return pair;

}


//------CPU Name------//
void ComputerInfo::getCPUInfo()
{
    int CPUInfo[4] = {-1};
    char CPUBrandString[0x40];
    __cpuid(CPUInfo, 0x80000000);
    unsigned int nExIds = CPUInfo[0];

    memset(CPUBrandString, 0, sizeof(CPUBrandString));

    // Get the information associated with each extended ID.
    for (unsigned int i=0x80000000; i<=nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        // Interpret CPU brand string.
        if  (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }
    qDebug()<<QString(CPUBrandString);
    qDebug()<<"-------------------------------------";
}
//-------------------//

/////////////////////////////////////
/////////////////////////////////////
//-----Name video card----------------//

void ComputerInfo::getGPUInfo()
{
    DISPLAY_DEVICE displayDevice;
    displayDevice.cb = sizeof(DISPLAY_DEVICE);

    for (DWORD deviceIndex = 0; EnumDisplayDevices(NULL, deviceIndex, &displayDevice, 0); ++deviceIndex)
    {
        if (displayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
        {
            //qDebug() << "Primary Display Device Name:" << QString::fromWCharArray(displayDevice.DeviceName);
            qDebug() << "Video card name :" <<  QString::fromWCharArray(displayDevice.DeviceString);
            qDebug() << "-----------------------------";
            break;
        }
    }
}
//----------------------------------------//

//------Вывод информации о компьюетере в виджет-----//
void ComputerInfo::slotShowComputerInfo()
{
    QHBoxLayout* vLayout = new QHBoxLayout(widgetComputerInfo);
    QTableWidget* tableInfoWidget = new QTableWidget(this);
    tableInfoWidget->setColumnCount(2);

    for (const QPair<QString, QString>& item : getBiosInfo())
    {
        int tableInfoRowCount = tableInfoWidget->rowCount();
        tableInfoWidget->insertRow(tableInfoRowCount);
        tableInfoWidget->setItem(tableInfoRowCount, 0, new QTableWidgetItem(QString(item.first)));
        tableInfoWidget->setItem(tableInfoRowCount, 1, new QTableWidgetItem(item.second));
    }

    tableInfoWidget->resizeColumnsToContents();

    vLayout->addWidget(tableInfoWidget);
    widgetComputerInfo->setLayout(vLayout);
    widgetComputerInfo->show();
    //tableInfoWidget->setLayout(vLayout);


    //qDebug()<<"Раз два три";
}
//-------------------------------------//
