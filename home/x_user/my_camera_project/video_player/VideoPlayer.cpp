#include "VideoPlayer.h"
#include <QImage>
#include <QPixmap>

VideoPlayer::VideoPlayer(const QString &videoPath, QWidget *parent)
    : QWidget(parent), isPlaying(false), pipeline(nullptr), volumeElement(nullptr)
{
    videoLabel = new QLabel(this);
    videoLabel->setFixedSize(640, 360);

    playPauseButton = new QPushButton("Play", this);
    forwardButton = new QPushButton(">> 10s", this);
    backwardButton = new QPushButton("<< 10s", this);
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(backwardButton);
    controlLayout->addWidget(playPauseButton);
    controlLayout->addWidget(forwardButton);
    controlLayout->addWidget(new QLabel("Volume:", this));
    controlLayout->addWidget(volumeSlider);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(videoLabel);
    mainLayout->addLayout(controlLayout);
    setLayout(mainLayout);

    connect(playPauseButton, &QPushButton::clicked, this, &VideoPlayer::playPause);
    connect(&frameTimer, &QTimer::timeout, this, &VideoPlayer::updateFrame);
    connect(forwardButton, &QPushButton::clicked, this, &VideoPlayer::seekForward);
    connect(backwardButton, &QPushButton::clicked, this, &VideoPlayer::seekBackward);
    connect(volumeSlider, &QSlider::valueChanged, this, &VideoPlayer::volumeChanged);

    cap.open(videoPath.toStdString());
    if (!cap.isOpened()) {
        videoLabel->setText("Failed to open video.");
        return;
    }

    frameTimer.setInterval(1000 / cap.get(cv::CAP_PROP_FPS));

    gst_init(nullptr, nullptr);
    initGStreamerPipeline(videoPath);
}

VideoPlayer::~VideoPlayer()
{
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }
}

void VideoPlayer::initGStreamerPipeline(const QString &path)
{
    QString pipelineDesc = QString("filesrc location=\"%1\" ! decodebin name=d "
                                   "d. ! audioconvert ! volume name=vol volume=0.5 ! autoaudiosink")
                               .arg(path);

    GError *error = nullptr;
    pipeline = gst_parse_launch(pipelineDesc.toUtf8(), &error);
    if (error) {
        std::cout << "GStreamer pipeline error:" << error->message<< std::endl; 
        g_error_free(error);
        return;
    }

    volumeElement = gst_bin_get_by_name(GST_BIN(pipeline), "vol");
}

void VideoPlayer::playPause()
{
    if (!pipeline)
        return;

    if (isPlaying) {
        gst_element_set_state(pipeline, GST_STATE_PAUSED);
        frameTimer.stop();
        playPauseButton->setText("Play");
    } else {
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
        frameTimer.start();
        playPauseButton->setText("Pause");
    }

    isPlaying = !isPlaying;
}

void VideoPlayer::updateFrame()
{
    cv::Mat frame;
    if (!cap.read(frame)) {
        frameTimer.stop();
        return;
    }

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    videoLabel->setPixmap(QPixmap::fromImage(img).scaled(videoLabel->size(), Qt::KeepAspectRatio));
}

void VideoPlayer::seekForward()
{
    double pos = cap.get(cv::CAP_PROP_POS_MSEC);
    gstSeek(pos + 10000);
}

void VideoPlayer::seekBackward()
{
    double pos = cap.get(cv::CAP_PROP_POS_MSEC);
    gstSeek(qMax(pos - 10000, 0.0));
}

void VideoPlayer::gstSeek(qint64 milliseconds)
{
    cap.set(cv::CAP_PROP_POS_MSEC, milliseconds);
    gst_element_seek_simple(pipeline, GST_FORMAT_TIME,
                            GstSeekFlags(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT),
                            milliseconds * GST_MSECOND);
}

void VideoPlayer::volumeChanged(int volume)
{
    setGstVolume(volume / 100.0);
}

void VideoPlayer::setGstVolume(double volume)
{
    if (volumeElement)
        g_object_set(volumeElement, "volume", volume, nullptr);
}
