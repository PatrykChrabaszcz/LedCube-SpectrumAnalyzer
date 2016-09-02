#include "core.h"

Core::Core(QObject *parent) :
    QObject(parent),
    listOfInputDevices(QAudioDeviceInfo::availableDevices(QAudio::AudioInput)),
    inputDevice(0),
    buffer(NumberOfDataBytes,0)
{
    setFormat();

    if(!QAudioDeviceInfo::defaultInputDevice().isFormatSupported(format))
        format=QAudioDeviceInfo::defaultInputDevice().nearestFormat(format);
    audioInput=new QAudioInput(QAudioDeviceInfo::defaultInputDevice(),format,this);

    timer=new QTimer(this);
    timer->setInterval(NotifyIntervalMs);
    connect (timer,SIGNAL(timeout()),this,SLOT(readToBuffer()));

    fallTimer=new QTimer(this);
    fallTimer->setInterval(50);
    connect(fallTimer,SIGNAL(timeout()),this,SLOT(fallData()));

    spectrumInputData= new double[NumberOfSamples];
    spectrumOutputDataFormated= new double[CubeSize];
    spectrumOutputDataUnformated= new double[NumberOfFrequencySamples];
    spectrumOutputDataToSend=new double [CubeSize];

    //FFTW3
    out_cpx = fftw_alloc_complex(NumberOfSamples);
    fft = fftw_plan_dft_r2c_1d(NumberOfSamples, spectrumInputData, out_cpx,FFTW_DESTROY_INPUT | FFTW_MEASURE);  //Setup fftw plan for fft
}
Core::~Core(){
    delete[] spectrumInputData;
    delete[] spectrumOutputDataFormated;
    delete[] spectrumOutputDataUnformated;
    delete[] spectrumOutputDataToSend;

    fftw_destroy_plan(fft);
    fftw_free(out_cpx);

}

void Core::playButtonPushed()
{
    if (audioInput) {
        if (QAudio::SuspendedState == state)
        {
           // audioInput->resume();
        }
    }

    inputDevice=audioInput->start();

    timer->start();
    fallTimer->start();
    //inputDevice->open(QIODevice::WriteOnly);
    qDebug()<<"Pushed";
}

void Core::setFormat()
{
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleRate(44100);
    format.setChannelCount(1);
}
void Core::setAudioInputDevice(int i)
{
    timer->stop();

    if(audioInput)
        delete audioInput;

    if(!listOfInputDevices[i].isFormatSupported(format))
        format=listOfInputDevices[i].nearestFormat(format);

    audioInput=new QAudioInput(listOfInputDevices[i],format,this);
    qDebug()<<"changed device ";
}

void Core::readToBuffer()
{
    inputDevice->read(buffer.data(),NumberOfDataBytes);
    inputDevice->readAll();
    int numBytes=format.sampleSize()/8;
    const char *ptr=buffer.constBegin();
    for(int i=0;i<NumberOfSamples;i++)
    {
        double x = 0.5 * (1 - qCos((2 * M_PI * i) / (NumberOfSamples - 1)));

        const qint16 pcmSample = *reinterpret_cast<const qint16*>(ptr);
        spectrumInputData[i]=x*(double)pcmSample/32768;
        for(int i=0;i<numBytes;i++)
            ptr++;
    }

    calculateSpectrum();
    prepareFormatedData();
    emit Paint(spectrumOutputDataToSend,CubeSize);
}
void Core::calculateSpectrum()
{
    fftw_execute(fft);

    float real;
    float imaginary;
    float amplitude;
    float magnitude;
    float frequency;

    int FSR=format.sampleRate();
    for(int i=0;i<NumberOfFrequencySamples;i++)
    {
        real      = out_cpx[i][0];//spectrumOutputData[i];
        imaginary = out_cpx[i][1];//spectrumOutputData[NumberOfFrequencySamples+i];

        frequency = float(i * FSR) / NumberOfSamples;

        magnitude = sqrt(real*real+imaginary*imaginary);

        amplitude = log(magnitude);

        spectrumOutputDataUnformated[i]=amplitude;
    }
}

void Core::prepareFormatedData(){
    int Scale=3;
    for(int i=0;i<CubeSize;i++)
        spectrumOutputDataFormated[i]=0;
    for(int j=0;j<CubeSize;j++){
        for(int i=j*NumberOfFrequencySamples/(2*CubeSize);i<(j+1)*NumberOfFrequencySamples/(2*CubeSize);i++){
            if(spectrumOutputDataUnformated[i]/Scale>spectrumOutputDataFormated[j])
                spectrumOutputDataFormated[j]=spectrumOutputDataUnformated[i]/Scale;
        }
    }
    for(int i=0;i<CubeSize;i++){
        if(spectrumOutputDataFormated[i]>1)
            spectrumOutputDataFormated[i]=1;
        if(spectrumOutputDataFormated[i]<0)
            spectrumOutputDataFormated=0;
    }
    for(int i=0;i<CubeSize;i++){
        if(spectrumOutputDataFormated[i]>spectrumOutputDataToSend[i])
            spectrumOutputDataToSend[i]=spectrumOutputDataFormated[i];
    }
}
void Core::fallData(){
    for(int i=0;i<CubeSize;i++){
        if(spectrumOutputDataFormated[i]<spectrumOutputDataToSend[i])
            spectrumOutputDataToSend[i] = spectrumOutputDataToSend[i]-0.1;
        if(spectrumOutputDataToSend[i]<0)
            spectrumOutputDataToSend[i]=0;
    }
}
