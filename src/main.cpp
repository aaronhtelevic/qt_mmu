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
  WebView(QWidget *parent = nullptr) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    QWebEngineView *webView = new QWebEngineView(this);
    webView->setUrl(QUrl("http://localhost:8085"));
    layout->addWidget(webView);
  }
};

class PdfViewer : public QWidget {
public:
  PdfViewer(const QString &pdfPath, QWidget *parent = nullptr)
      : QWidget(parent), currentPage(0) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    pdfDocument = new QPdfDocument(this);
    pdfDocument->load(pdfPath);

    pdfLabel = new QLabel(this);
    layout->addWidget(pdfLabel);

    QHBoxLayout *navLayout = new QHBoxLayout();
    prevButton = new QPushButton("Previous", this);
    nextButton = new QPushButton("Next", this);
    connect(prevButton, &QPushButton::clicked, this,
            &PdfViewer::showPreviousPage);
    connect(nextButton, &QPushButton::clicked, this, &PdfViewer::showNextPage);
    navLayout->addWidget(prevButton);
    navLayout->addWidget(nextButton);

    layout->addLayout(navLayout);

    updatePage();
    updateButtons();
  }

private slots:
  void showPreviousPage() {
    if (currentPage > 0) {
      --currentPage;
      updatePage();
      updateButtons();
    }
  }

  void showNextPage() {
    if (currentPage < pdfDocument->pageCount() - 1) {
      ++currentPage;
      updatePage();
      updateButtons();
    }
  }

private:
  void updatePage() {
    QImage image = pdfDocument->render(
        currentPage, pdfDocument->pagePointSize(currentPage).toSize());
    pdfLabel->setPixmap(QPixmap::fromImage(image));
  }

  void updateButtons() {
    prevButton->setEnabled(currentPage > 0);
    nextButton->setEnabled(currentPage < pdfDocument->pageCount() - 1);
  }

  QPdfDocument *pdfDocument;
  QLabel *pdfLabel;
  QPushButton *prevButton;
  QPushButton *nextButton;
  int currentPage;
};

class LargeList : public QWidget {
public:
  LargeList(QWidget *parent = nullptr) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *listWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(listWidget);

    QFont font;
    font.setPointSize(30);
    for (int i = 0; i < 1000; ++i) {
      QLabel *label = new QLabel(QString("Item %1").arg(i), this);
      label->setFont(font);
      label->setFixedHeight(100);
      layout->addWidget(label);
    }

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
  MainWindow() {
    setWindowTitle("Qt C++ Multimedia App");
    // should be fullscreen
    setWindowState(Qt::WindowMaximized);
    QFont tabFont;
    tabFont.setPointSize(20);

    QTabWidget *tabs = new QTabWidget(this);
    tabs->setFont(tabFont);
    tabs->addTab(new WebView(), "Web Browser");
    // first check if sample.pdf exists in current directory, then try
    // /home/root
    if (QFile::exists("sample.pdf"))
      tabs->addTab(new PdfViewer("sample.pdf"), "PDF Viewer");
    else
      tabs->addTab(new PdfViewer("/home/root/sample.pdf"), "PDF Viewer");

    tabs->addTab(new LargeList(), "Large List");
    tabs->addTab(new VideoPlayer(), "Video Player");

    setCentralWidget(tabs);
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow mainWindow;
  mainWindow.show();
  return app.exec();
}
