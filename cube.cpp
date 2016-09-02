#include "cube.h"

Cube::Cube(QObject *parent) :
    QObject(parent)
{
    //Make new QextSerialPort then close it
    port=new QextSerialPort(QextSerialPort::EventDriven,this);
    port->close();

    clearData(); //Clear Cube
}
Cube:: ~Cube()
{
}

void Cube::clearData()
{
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++){
             mySendCube[i][j]=0;
        }
}

void Cube::LedOn(int x, int y, int z)
{
    mySendCube[z][y] |= (1<<x);
}
void Cube::LedOff(int x, int y, int z)
{
    mySendCube[z][y] &= ~(1<<x);
}

//Serial port initialization, same as on led Cube
void Cube::initSerialPort(QString string)
{
    delete port;
    port= new QextSerialPort(string);
    port->setBaudRate(BAUD115200);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_2);
    //port->setQueryMode(QextSerialPort::Polling);
    port->close();
    if (port->open(QIODevice::ReadWrite) == true){
        qDebug("port is opened");
    }
    else{
        qDebug("port is not open");
    }
}

void Cube::paintCube(double* cubeData,int size) //Sends data to Cube
{
    if(size!=64){
        qDebug()<<"Size is different than 64";
        return;
    }

    //Prepare data
    for(int x=0;x<8;x++)
        for(int y=0;y<8;y++)
        {
           int value=cubeData[x*8+y]*8;
           if(value<0){
               value=0;
               qDebug("In cube class function print Cube, value lower than 0");
           }
           if(value>8){
               value=8;
               qDebug("In cube class function print Cube, value greater than 8");
           }

           setColumn(x,y,value);
        }
    //Send data
    if(port->isOpen())
        for(int i=0; i<8; i++)
            port->write(mySendCube[i],8);
    else
        qDebug("In cube class, printCube function. Port is not opened");
}

void Cube::setColumn(int x, int y, int z) //Sets whole column ON from Z=0 to Z=z
{
    for(int i=0;i<z;i++)
        LedOn(x,y,i);
    for(int i=z;i<8;i++)
        LedOff(x,y,i);
}
