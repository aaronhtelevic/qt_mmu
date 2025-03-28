#include "pdf_viewer.hpp"
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <chrono>

PdfViewer::PdfViewer(const QString &pdfPath, QWidget *parent)
    : QWidget(parent), currentPage(0), zoomFactor(1.0) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  setLayout(layout);

  pdfDocument = new QPdfDocument(this);
  pdfDocument->load(pdfPath);

  pdfLabel = new QLabel(this);
  QScrollArea *scrollArea = new QScrollArea(this);
  pdfLabel = new QLabel();
  scrollArea->setWidget(pdfLabel);
  scrollArea->setWidgetResizable(true);
  layout->addWidget(scrollArea);

  QHBoxLayout *navLayout = new QHBoxLayout();
  prevButton = new QPushButton("Previous", this);
  nextButton = new QPushButton("Next", this);
  zoomInButton = new QPushButton("Zoom In", this);
  zoomOutButton = new QPushButton("Zoom Out", this);
  autoScrollButton = new QPushButton("Auto Scroll", this);

  connect(prevButton, &QPushButton::clicked, this,
          &PdfViewer::showPreviousPage);
  connect(nextButton, &QPushButton::clicked, this, &PdfViewer::showNextPage);
  connect(zoomInButton, &QPushButton::clicked, this, &PdfViewer::zoomIn);
  connect(zoomOutButton, &QPushButton::clicked, this, &PdfViewer::zoomOut);
  connect(autoScrollButton, &QPushButton::clicked, this,
          &PdfViewer::startAutoScroll);

  navLayout->addWidget(prevButton);
  navLayout->addWidget(nextButton);
  navLayout->addWidget(zoomInButton);
  navLayout->addWidget(zoomOutButton);
  navLayout->addWidget(autoScrollButton);

  layout->addLayout(navLayout);

  updatePage();
  updateButtons();
}

void PdfViewer::startAutoScroll() {
  // go to first page
  currentPage = 0;
  updatePage();
  autoScroll();
}

void PdfViewer::autoScroll() {
  if (currentPage < pdfDocument->pageCount() - 1) {
    auto startTime = std::chrono::high_resolution_clock::now();
    connect(this, &PdfViewer::pageRendered, this, [this, startTime]() {
      auto endTime = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                          endTime - startTime)
                          .count();
      qDebug() << "Page rendered in" << duration << "ms";

      disconnect(this, &PdfViewer::pageRendered, nullptr, nullptr);
      showNextPage();
      autoScroll();
    });
    updatePage();
  }
}
void PdfViewer::showPreviousPage() {
  if (currentPage > 0) {
    --currentPage;
    updatePage();
    updateButtons();
  }
}

void PdfViewer::showNextPage() {
  if (currentPage < pdfDocument->pageCount() - 1) {
    ++currentPage;
    updatePage();
    updateButtons();
  }
}

void PdfViewer::zoomIn() {
  zoomFactor *= 1.2;
  updatePage();
}

void PdfViewer::zoomOut() {
  zoomFactor /= 1.2;
  updatePage();
}

void PdfViewer::updatePage() {
  QSize originalSize = pdfDocument->pagePointSize(currentPage).toSize();
  QSize scaledSize = originalSize * zoomFactor;
  QImage image = pdfDocument->render(currentPage, scaledSize);
  pdfLabel->setPixmap(QPixmap::fromImage(image));
  emit pageRendered();
}

void PdfViewer::updateButtons() {
  prevButton->setEnabled(currentPage > 0);
  nextButton->setEnabled(currentPage < pdfDocument->pageCount() - 1);
}
