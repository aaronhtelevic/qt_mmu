import sys
from PyQt6.QtWidgets import QPushButton, QHBoxLayout
from PyQt6.QtWidgets import QApplication, QMainWindow, QTabWidget, QWidget, QVBoxLayout
from PyQt6.QtWebEngineWidgets import QWebEngineView
from PyQt6.QtPdfWidgets import QPdfView
from PyQt6.QtPdf import QPdfDocument
from PyQt6.QtMultimedia import QMediaPlayer
from PyQt6.QtMultimediaWidgets import QVideoWidget
from PyQt6.QtCore import QUrl, QPointF

import gi

gi.require_version("Gst", "1.0")
gi.require_version("Gtk", "4.0")
from gi.repository import Gst, Gtk


Gst.init(None)  # Initialize GStreamer


class VideoPlayer(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.layout = QVBoxLayout(self)
        self.setLayout(self.layout)

        self.video_widget = QVideoWidget()
        self.layout.addWidget(self.video_widget)

        self.player = QMediaPlayer(self)
        self.player.setVideoOutput(self.video_widget)

        # Set the GStreamer pipeline
        # pipeline = "gst-pipeline: videotestsrc ! autovideosink"
        pipeline = "/home/aaron/video1.mp4"
        self.player.setSource(QUrl(pipeline))
        self.player.play()


class WebView(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        layout = QVBoxLayout(self)
        self.webview = QWebEngineView()
        self.webview.setUrl(QUrl("https://www.qt.io"))  # Default URL
        layout.addWidget(self.webview)
        self.setLayout(layout)


class PdfViewer(QWidget):
    def __init__(self, pdf_path, parent=None):
        super().__init__(parent)
        self.current_page = 0

        layout = QVBoxLayout(self)
        self.pdf_viewer = QPdfView(self)
        self.pdf_document = QPdfDocument(self)
        self.pdf_document.load(pdf_path)
        self.pdf_viewer.setDocument(self.pdf_document)

        # Navigation buttons
        nav_layout = QHBoxLayout()
        self.prev_button = QPushButton("Previous", self)
        self.next_button = QPushButton("Next", self)
        self.prev_button.clicked.connect(self.show_previous_page)
        self.next_button.clicked.connect(self.show_next_page)
        nav_layout.addWidget(self.prev_button)
        nav_layout.addWidget(self.next_button)

        layout.addWidget(self.pdf_viewer)
        layout.addLayout(nav_layout)
        self.setLayout(layout)

        self.update_buttons()

    def show_previous_page(self):
        if self.current_page > 0:
            self.current_page -= 1
            self.pdf_viewer.pageNavigator().jump(self.current_page, QPointF(0, 0))
            self.update_buttons()

    def show_next_page(self):
        if self.current_page < self.pdf_document.pageCount() - 1:
            self.current_page += 1
            self.pdf_viewer.pageNavigator().jump(self.current_page, QPointF(0, 0))
            self.update_buttons()

    def update_buttons(self):
        self.prev_button.setEnabled(self.current_page > 0)
        self.next_button.setEnabled(
            self.current_page < self.pdf_document.pageCount() - 1
        )


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Qt GStreamer, Web, and PDF App")
        self.setGeometry(100, 100, 800, 600)

        tabs = QTabWidget()
        tabs.addTab(VideoPlayer(), "Video Player")
        tabs.addTab(WebView(), "Web Browser")
        tabs.addTab(PdfViewer("sample.pdf"), "PDF Viewer")

        self.setCentralWidget(tabs)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    mainWin = MainWindow()
    mainWin.show()
    sys.exit(app.exec())
