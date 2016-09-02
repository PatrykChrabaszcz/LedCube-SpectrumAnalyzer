#ifndef CUBE_H
#define CUBE_H

//Handling Serial Port and Cube Printing

#include <qextserialport.h>
#include <qtimer.h>
#include <QDebug>

class Cube : public QObject
{
    Q_OBJECT
public:
    explicit Cube(QObject *parent = 0);
    ~Cube();

    void initSerialPort(QString);

public slots:
    void paintCube(double *cubeData, int size);
private:
    void clearData();
    QextSerialPort* port; //Serial port for sending data to Led Cube

    char mySendCube[8][8]; //Representation of cube in memory

    void setColumn(int x, int y, int z); //Sets whole column ON from Z=0 to Z=z
    void LedOn(int x,int y, int z);
    void LedOff(int x,int y,int z);
};

#endif // CUBE_H
