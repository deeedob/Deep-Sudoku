#pragma once

#include <QObject>
#include <QPointer>
#include <QVideoSink>
#include <QQmlEngine>
#include <QTimer>
//#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>

class Producer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVideoSink* videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)
public:
    Producer(QObject *parent=nullptr);
    QVideoSink *videoSink() const;
    void setVideoSink(QVideoSink *newVideoSink);
    Q_INVOKABLE void start();

signals:
    void videoSinkChanged();

private:
    QPointer<QVideoSink> m_videoSink;
    void handleTimeout();
    QTimer m_timer;
};
