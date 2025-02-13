#include "video_player.hpp"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>

VideoPlayer::VideoPlayer() {
  mediaPlayer = new QMediaPlayer(this);
  videoWidget = new QVideoWidget(this);

  playButton = new QPushButton("Play", this);
  pauseButton = new QPushButton("Pause", this);
  stopButton = new QPushButton("Stop", this);
  openButton = new QPushButton("Open", this);

  connect(playButton, &QPushButton::clicked, this, &VideoPlayer::play);
  connect(pauseButton, &QPushButton::clicked, this, &VideoPlayer::pause);
  connect(stopButton, &QPushButton::clicked, this, &VideoPlayer::stop);
  connect(openButton, &QPushButton::clicked, this, &VideoPlayer::openFile);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(videoWidget);

  QHBoxLayout *controlLayout = new QHBoxLayout();
  controlLayout->addWidget(openButton);
  controlLayout->addWidget(playButton);
  controlLayout->addWidget(pauseButton);
  controlLayout->addWidget(stopButton);

  mainLayout->addLayout(controlLayout);

  mediaPlayer->setVideoOutput(videoWidget);
  setLayout(mainLayout);
}

void VideoPlayer::openFile() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Open Video File", "", "Video Files (*.mp4 *.avi *.mkv)");
  if (!fileName.isEmpty()) {
    mediaPlayer->setSource(QUrl::fromLocalFile(fileName));
  }
}

void VideoPlayer::play() { mediaPlayer->play(); }

void VideoPlayer::pause() { mediaPlayer->pause(); }

void VideoPlayer::stop() { mediaPlayer->stop(); }
