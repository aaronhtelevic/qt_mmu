#include "pdf_viewer.hpp"
#include "video_player.hpp"
#include <QApplication>
#include <QFile>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QPainter>
#include <QPdfDocument>
#include <QPushButton>
#include <QScrollArea>
#include <QScroller>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QVideoSink>
#include <QWebEngineView>

class WebView : public QWidget {
public:
  WebView(const QString &url = "http://localhost:8085",
          QWidget *parent = nullptr)
      : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    QWebEngineView *webView = new QWebEngineView(this);
    webView->setUrl(QUrl(url));
    layout->addWidget(webView);
  }
};

class LargeList : public QWidget {
public:
  LargeList(QWidget *parent = nullptr) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *listWidget = new QWidget();
    QGridLayout *layout = new QGridLayout(listWidget);
    int screenWidth = listWidget->size().width() - 80;

    QFont font;
    font.setPointSize(30);

    QFile file(":/assets/delegates.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qDebug() << "Error: Could not open delegates.txt";
      return;
    }

    QTextStream in(&file);
    QPixmap pixmap(":/assets/avatar.png"); // Ensure correct path

    QPixmap scaledPixmap =
        pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    int i = 0;
    while (!in.atEnd()) {
      QString name = in.readLine().trimmed();
      if (name.isEmpty())
        continue;

      QWidget *itemWidget = new QWidget();
      itemWidget->setObjectName("itemWidget");
      itemWidget->setFixedWidth(screenWidth);
      QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);

      QLabel *textLabel = new QLabel(name, this);
      textLabel->setFont(font);
      textLabel->setFixedHeight(100);

      QLabel *imageLabel = new QLabel(this);
      imageLabel->setPixmap(scaledPixmap);
      itemLayout->addWidget(imageLabel);
      itemLayout->addWidget(textLabel);
      itemWidget->setLayout(itemLayout);

      layout->addWidget(itemWidget, i / 2, i % 2); // Two columns
      itemWidget->setStyleSheet("#itemWidget { border: 2px solid black; "
                                "background-color: white; padding: 5px; }");

      ++i;
    }
    file.close();

    listWidget->setLayout(layout);
    scrollArea->setWidget(listWidget);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);

    QScroller::grabGesture(scrollArea->viewport(), QScroller::TouchGesture);
  }
};

class MainWindow : public QMainWindow {
public:
  MainWindow(const QString &webViewUrl) {
    setWindowTitle("Qt C++ Multimedia App");
    setWindowState(Qt::WindowMaximized);
    QFont tabFont;
    tabFont.setPointSize(20);

    QTabWidget *tabs = new QTabWidget(this);
    tabs->setFont(tabFont);
    tabs->addTab(new LargeList(), "Large List");
    if (QFile::exists("sample.pdf"))
      tabs->addTab(new PdfViewer("sample.pdf"), "PDF Viewer");
    else
      tabs->addTab(new PdfViewer("/home/root/sample.pdf"), "PDF Viewer");

    tabs->addTab(new WebView(webViewUrl), "Web Browser");
    tabs->addTab(new VideoPlayer(), "Video Player");

    setCentralWidget(tabs);
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QString webViewUrl = (argc > 1) ? argv[1] : "http://localhost:8085";
  MainWindow mainWindow(webViewUrl);
  mainWindow.show();
  return app.exec();
}
