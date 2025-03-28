#ifndef PDF_VIEWER_HPP
#define PDF_VIEWER_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QPdfDocument>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

class PdfViewer : public QWidget {
  Q_OBJECT
public:
  PdfViewer(const QString &pdfPath, QWidget *parent = nullptr);

private slots:
  void startAutoScroll();
  void showPreviousPage();
  void showNextPage();
  void zoomIn();
  void zoomOut();

private:
  void updatePage();
  void updateButtons();

  QPdfDocument *pdfDocument;
  QLabel *pdfLabel;
  QPushButton *prevButton;
  QPushButton *nextButton;
  QPushButton *zoomInButton;
  QPushButton *zoomOutButton;
  QPushButton *autoScrollButton;
  int currentPage;
  double zoomFactor;

signals:
  void pageRendered();
};

#endif // PDF_VIEWER_HPP
