#pragma once
#include <QObject>
#include <QCamera>
#include <QImageCapture>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QThread>
#include <QThreadPool>
#include <opencv2/opencv.hpp>

class QCvAlgorithm : public QObject
{
    Q_OBJECT

public:
    QString inputFileName; // set this and start
    void doDft();
    void doCanny();
    void doGray();
    void doNone();

signals:
    void errorOccured(QString msg);
    void imageProcessed(QString fname);

};

class QCvImageProcessor : public QObject
{
Q_OBJECT
public:
    explicit QCvImageProcessor(QObject *parent = 0);
    ~QCvImageProcessor();

    Q_INVOKABLE void setCamera(QVariant v);
    Q_INVOKABLE void capture();
    Q_INVOKABLE void setAlgorithm(QString alg);

    Q_INVOKABLE void processEvents()
    {
        qApp->processEvents();
    }

signals:
    void errorOccured(QString msg);
    void imageProcessed(QString fname);
    void processStarted();

private:
    QCamera *camera;
    QImageCapture *imageCapture;

    void processSavedImage(const QString &fname);

    QCvAlgorithm *cvAlgorithm;
    QThread *workerThread;

    QString algorithm;
};