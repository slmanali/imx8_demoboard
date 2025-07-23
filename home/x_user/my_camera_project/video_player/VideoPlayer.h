#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>

#include <opencv2/opencv.hpp>
#include <gst/gst.h>

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(const QString &videoPath, QWidget *parent = nullptr);
    ~VideoPlayer();

private slots:
    void playPause();
    void updateFrame();
    void seekForward();
    void seekBackward();
    void volumeChanged(int volume);

private:
    void initGStreamerPipeline(const QString &path);
    void setGstVolume(double volume);
    void gstSeek(qint64 milliseconds);

    QLabel *videoLabel;
    QPushButton *playPauseButton;
    QPushButton *forwardButton;
    QPushButton *backwardButton;
    QSlider *volumeSlider;

    cv::VideoCapture cap;
    QTimer frameTimer;

    bool isPlaying;

    GstElement *pipeline;
    GstElement *volumeElement;
};

#endif // VIDEOPLAYER_H
