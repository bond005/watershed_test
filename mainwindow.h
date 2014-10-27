#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QVector>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionLoadImage_triggered();

private:
    Ui::MainWindow *ui;
    QImage m_sourceImage;
    QImage m_grayScaledImage;
    QImage m_smoothedImage;
    QImage m_segmentation;
    QVector<float> m_aPixelsOfSmoothedImage;
    QLabel* m_lblInfo;

    void calculateGrayScaledImage();
    void calculateSmoothedImage();
    void calculateSegmentation();
};

#endif // MAINWINDOW_H
