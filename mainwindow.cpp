#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image_processing_lib.h"
#include "randlib.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_lblInfo = new QLabel(this);
    ui->statusBar->addWidget(m_lblInfo, 1);

    QPainter painter;
    QBrush whiteBrush = QBrush(QColor(Qt::white));

    m_sourceImage = QImage(QSize(200, 200),
                           QImage::Format_ARGB32_Premultiplied);
    painter.begin(&m_sourceImage);
    painter.fillRect(m_sourceImage.rect(), whiteBrush);
    painter.end();

    m_grayScaledImage = QImage(m_sourceImage.size(),
                               QImage::Format_ARGB32_Premultiplied);
    painter.begin(&m_grayScaledImage);
    painter.fillRect(m_grayScaledImage.rect(), whiteBrush);
    painter.end();

    m_smoothedImage = QImage(m_sourceImage.size(),
                             QImage::Format_ARGB32_Premultiplied);
    painter.begin(&m_smoothedImage);
    painter.fillRect(m_smoothedImage.rect(), whiteBrush);
    painter.end();

    m_segmentation = QImage(m_sourceImage.size(),
                            QImage::Format_ARGB32_Premultiplied);
    painter.begin(&m_segmentation);
    painter.fillRect(m_segmentation.rect(), whiteBrush);
    painter.end();

    ui->lblSourceImage->setPixmap(QPixmap::fromImage(m_sourceImage));
    ui->lblSourceImage->setFixedSize(m_sourceImage.size());
    ui->scrollAreaWidgetContents->adjustSize();

    ui->lblGrayScaledImage->setPixmap(QPixmap::fromImage(m_grayScaledImage));
    ui->lblGrayScaledImage->setFixedSize(m_grayScaledImage.size());
    ui->scrollAreaWidgetContents_3->adjustSize();

    ui->lblSmoothedImage->setPixmap(QPixmap::fromImage(m_smoothedImage));
    ui->lblSmoothedImage->setFixedSize(m_smoothedImage.size());
    ui->scrollAreaWidgetContents_4->adjustSize();

    ui->lblSegmentation->setPixmap(QPixmap::fromImage(m_segmentation));
    ui->lblSegmentation->setFixedSize(m_segmentation.size());
    ui->lblSegmentation->adjustSize();
    ui->scrollAreaWidgetContents_2->adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionLoadImage_triggered()
{
    QString sFileName = QFileDialog::getOpenFileName(
                this, trUtf8("Загрузить изображение"), ".",
                trUtf8("Файлы изображений (*.png *.bmp *.jpg)"));
    if (sFileName.isEmpty())
    {
        return;
    }
    if (!m_sourceImage.load(sFileName))
    {
        QMessageBox::critical(this, this->windowTitle(),
                              trUtf8("Не удалось загрузить картинку из "\
                                     "заданного файла."), QMessageBox::Ok);
        return;
    }

    calculateGrayScaledImage();
    calculateSmoothedImage();
    calculateSegmentation();

    ui->lblSourceImage->setPixmap(QPixmap::fromImage(m_sourceImage));
    ui->lblSourceImage->setFixedSize(m_sourceImage.size());
    ui->scrollAreaWidgetContents->adjustSize();

    ui->lblGrayScaledImage->setPixmap(QPixmap::fromImage(m_grayScaledImage));
    ui->lblGrayScaledImage->setFixedSize(m_grayScaledImage.size());
    ui->scrollAreaWidgetContents_3->adjustSize();

    ui->lblSmoothedImage->setPixmap(QPixmap::fromImage(m_smoothedImage));
    ui->lblSmoothedImage->setFixedSize(m_smoothedImage.size());
    ui->scrollAreaWidgetContents_4->adjustSize();

    ui->lblSegmentation->setPixmap(QPixmap::fromImage(m_segmentation));
    ui->lblSegmentation->setFixedSize(m_segmentation.size());
    ui->lblSegmentation->adjustSize();
    ui->scrollAreaWidgetContents_2->adjustSize();
}

void MainWindow::calculateGrayScaledImage()
{
    const int nImageWidth = m_sourceImage.width();
    const int nImageHeight = m_sourceImage.height();
    int x, y, color_val;
    m_grayScaledImage = QImage(m_sourceImage.size(),
                               QImage::Format_ARGB32_Premultiplied);
    for (y = 0; y < nImageHeight; y++)
    {
        for (x = 0; x < nImageWidth; x++)
        {
            color_val = qGray(m_sourceImage.pixel(x,y));
            m_grayScaledImage.setPixel(
                        x, y, qRgb(color_val, color_val, color_val));
        }
    }
}

void MainWindow::calculateSmoothedImage()
{
    const int nImageWidth = m_sourceImage.width();
    const int nImageHeight = m_sourceImage.height();
    int x, y, color_val, i = 0;
    m_aPixelsOfSmoothedImage.resize(nImageWidth * nImageHeight);
    m_smoothedImage = QImage(m_sourceImage.size(),
                             QImage::Format_ARGB32_Premultiplied);
    for (y = 0; y < nImageHeight; y++)
    {
        for (x = 0; x < nImageWidth; x++)
        {
            color_val = qGray(m_sourceImage.pixel(x,y));
            m_aPixelsOfSmoothedImage[i++] = color_val;
        }
    }
    smooth_image(m_aPixelsOfSmoothedImage.data(), nImageWidth, nImageHeight,
                 3, 3, false);
    i = 0;
    for (y = 0; y < nImageHeight; y++)
    {
        for (x = 0; x < nImageWidth; x++)
        {
            color_val = (int)m_aPixelsOfSmoothedImage[i++];
            m_smoothedImage.setPixel(
                        x, y, qRgb(color_val, color_val, color_val));
        }
    }
}

void MainWindow::calculateSegmentation()
{
    QTime startTime;
    const int nImageWidth = m_sourceImage.width();
    const int nImageHeight = m_sourceImage.height();
    startTime.start();
    QVector<TClusterOfImage> aClusters = segmentate_image_by_watershed_alg(
                m_aPixelsOfSmoothedImage.constData(),nImageWidth,nImageHeight);
    m_lblInfo->setText(trUtf8("Продолжительность процесса сегментации "\
                              "составила %1 мсек.").arg(startTime.elapsed()));

    QPainter painter;
    QBrush whiteBrush = QBrush(QColor(Qt::white));
    m_segmentation = QImage(m_sourceImage.size(),
                            QImage::Format_ARGB32_Premultiplied);
    painter.begin(&m_segmentation);
    painter.fillRect(m_segmentation.rect(), whiteBrush);
    if (aClusters.isEmpty())
    {
        painter.end();
        return;
    }

    const int nMaxHue = 359;
    const int nMaxValue = 255;
    QSet<int> aUsedColors;
    int i, j, iColor, nCurHue, nCurValue;
    QColor curColor;

    for (i = 0; i < aClusters.size(); i++)
    {
        nCurHue = (int)floor(generate_random_value() * nMaxHue + 0.5);
        nCurValue = (int)floor(generate_random_value() * nMaxValue + 0.5);
        iColor = nCurHue * nCurValue;
        while (aUsedColors.contains(iColor))
        {
            nCurHue = (int)floor(generate_random_value() * nMaxHue + 0.5);
            nCurValue = (int)floor(generate_random_value() * nMaxValue + 0.5);
            iColor = nCurHue * nCurValue;
        }
        aUsedColors.insert(iColor);
        curColor = QColor::fromHsv(nCurHue, 255, nCurValue);
        for (j = 0; j < aClusters[i].size(); j++)
        {
            m_segmentation.setPixel(aClusters[i][j].x, aClusters[i][j].y,
                                    curColor.rgb());
        }
    }
    painter.end();
}
