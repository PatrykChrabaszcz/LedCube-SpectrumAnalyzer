#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QAudioInput>
#include <QBuffer>
#include <QDebug>
#include <math.h>
#include <QTimer>
#include <qmath.h>
#include <fftw3.h>

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

// Number of audio samples used to calculate the frequency spectrum
const int PowerOfTwo=10;
const int NumberOfSamples=pow(2,PowerOfTwo);
const int NumberOfDataBytes=NumberOfSamples*2;
const int NumberOfFrequencySamples=NumberOfSamples/2;
const int NotifyIntervalMs=25;
const int CubeSize=64;

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);
    ~Core();
    QList<QAudioDeviceInfo> listOfInputDevices;

signals:
    void Paint(double *frame, int size);

public slots:

    void setAudioInputDevice(int i);
    void playButtonPushed();
    void fallData();

private slots:
    void readToBuffer();

private:

     //AUDIO
     QByteArray buffer;
     QAudioInput *audioInput;
     QAudioFormat format;
     QAudio::State state;
     QIODevice *inputDevice;
     QTimer *timer;
     QTimer *fallTimer;

     //SPECTRUM
     double *spectrumInputData;
     double *spectrumOutputDataFormated;
     double *spectrumOutputDataUnformated;
     double *spectrumOutputDataToSend;

     //FFTW3
     fftw_complex *out_cpx;
     fftw_plan fft;

     void setFormat();
     void calculateSpectrum();
     void prepareFormatedData();
};

#endif // CORE_H
