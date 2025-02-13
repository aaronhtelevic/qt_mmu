#pragma once

#include <QMediaPlayer>
#include <QPushButton>
#include <QVideoWidget>
#include <QWidget>

class VideoPlayer : public QWidget {
  Q_OBJECT
public:
  VideoPlayer();

private slots:
  void openFile();
  void play();
  void pause();
  void stop();

private:
  QMediaPlayer *mediaPlayer;
  QVideoWidget *videoWidget;
  QPushButton *playButton;
  QPushButton *pauseButton;
  QPushButton *stopButton;
  QPushButton *openButton;
};
