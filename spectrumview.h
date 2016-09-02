#ifndef SPECTRUMVIEW_H
#define SPECTRUMVIEW_H

#include <QWidget>

class SpectrumView : public QWidget
{
    Q_OBJECT
public:
    explicit SpectrumView(QWidget *parent = 0);

    void paintEvent(QPaintEvent *event);
signals:

public slots:
    void updateSpectrumData(double *data,int size);
private:
    double *spectrumData;

};

#endif // SPECTRUMVIEW_H
