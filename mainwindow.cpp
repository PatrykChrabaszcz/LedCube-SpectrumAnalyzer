#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    core = new Core(this);
    cube = new Cube(this);
    ui->setupUi(this);

    QAudioDeviceInfo device;
    foreach (device, core->listOfInputDevices)
        ui->comboBoxDevices->addItem(device.deviceName(),QVariant::fromValue(device));

    connect(ui->pushButtonPlay,SIGNAL(clicked()),this->core,SLOT(playButtonPushed()));
    connect(ui->comboBoxDevices,SIGNAL(currentIndexChanged(int)),this->core,SLOT(setAudioInputDevice(int)));
    connect(this->core,SIGNAL(Paint(double*,int)),ui->widgetSpectrumView,SLOT(updateSpectrumData(double*,int)));
    connect(ui->pushButtonOpenPort,SIGNAL(clicked()),this,SLOT(serialPortButtonPushed()));
    connect(this->core,SIGNAL(Paint(double*,int)),this->cube,SLOT(paintCube(double*,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::serialPortButtonPushed()
{
    cube->initSerialPort(ui->lineEditPortURL->text());
}
