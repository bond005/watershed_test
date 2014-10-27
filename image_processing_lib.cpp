#include <cstring>

#include <QSet>
#include <QList>
#include <QVarLengthArray>
#include <QStack>
#include <QtAlgorithms>

#include "image_processing_lib.h"

/*****************************************************************************/
/* ВНУТРЕННИЕ ФУНКЦИИ МОДУЛЯ image_processing_lib */
/*****************************************************************************/

/* Найти связную область на растровом изображении, в которой все пиксели имеют
одинаковую яркость, равную яркости заранее определённого стартового пикселя.
   Само изображение задано развёрнутой по строкам матрицей яркостей пикселей
aPixelsOfImage[] шириной nImageWidth и высотой nImageHeight. Стартовый пиксель
задан своим номером iStartPixel в массиве aPixelsOfImage[].
   Функция возвращает список номеров пикселей в массиве aPixelsOfImage[],
образующих заданную область.
   Для упрощения алгоритма выделения связной области пикселей с одинаковой
яркостью используется вспомогательный булевский массив aFilledPixels[]. */
QList<int> get_region_with_same_brightness(
        const float aPixelsOfImage[], int nImageWidth, int nImageHeight,
        int iStartPixel, bool aFilledPixels[])
{
    QList<int> res;
    QStack<int> aPixelsForProcessing;
    int x, y, i;

    aPixelsForProcessing.append(iStartPixel);
    while (!aPixelsForProcessing.isEmpty())
    {
        i = aPixelsForProcessing.pop();
        x = i % nImageWidth; y = i / nImageWidth;
        aFilledPixels[i] = true;
        res.append(i);
        if (x > 0)
        {
            i = (x-1) + y * nImageWidth;
            if ((!aFilledPixels[i])
                    && (fabs(aPixelsOfImage[iStartPixel] - aPixelsOfImage[i])
                        <= FLT_EPSILON))
            {
                aPixelsForProcessing.push(i);
            }
        }
        if ((x > 0) && (y > 0))
        {
            i = (x-1) + (y-1) * nImageWidth;
            if ((!aFilledPixels[i])
                    && (fabs(aPixelsOfImage[iStartPixel] - aPixelsOfImage[i])
                        <= FLT_EPSILON))
            {
                aPixelsForProcessing.push(i);
            }
        }
        if (y > 0)
        {
            i = x + (y-1) * nImageWidth;
            if ((!aFilledPixels[i])
                    && (fabs(aPixelsOfImage[iStartPixel] - aPixelsOfImage[i])
                        <= FLT_EPSILON))
            {
                aPixelsForProcessing.push(i);
            }
        }
        if ((x < (nImageWidth-1)) && (y > 0))
        {
            i = (x+1) + (y-1) * nImageWidth;
            if ((!aFilledPixels[i])
                    && (fabs(aPixelsOfImage[iStartPixel] - aPixelsOfImage[i])
                        <= FLT_EPSILON))
            {
                aPixelsForProcessing.push(i);
            }
        }
        if (x < (nImageWidth-1))
        {
            i = (x+1) + y * nImageWidth;
            if ((!aFilledPixels[i])
                    && (fabs(aPixelsOfImage[iStartPixel] - aPixelsOfImage[i])
                        <= FLT_EPSILON))
            {
                aPixelsForProcessing.push(i);
            }
        }
        if ((x < (nImageWidth-1)) && (y < (nImageHeight-1)))
        {
            i = (x+1) + (y+1) * nImageWidth;
            if ((!aFilledPixels[i])
                    && (fabs(aPixelsOfImage[iStartPixel] - aPixelsOfImage[i])
                        <= FLT_EPSILON))
            {
                aPixelsForProcessing.push(i);
            }
        }
        if (y < (nImageHeight-1))
        {
            i = x + (y+1) * nImageWidth;
            if ((!aFilledPixels[i])
                    && (fabs(aPixelsOfImage[iStartPixel] - aPixelsOfImage[i])
                        <= FLT_EPSILON))
            {
                aPixelsForProcessing.push(i);
            }
        }
        if ((x > 0) && (y < (nImageHeight-1)))
        {
            i = (x-1) + (y+1) * nImageWidth;
            if ((!aFilledPixels[i])
                    && (fabs(aPixelsOfImage[iStartPixel] - aPixelsOfImage[i])
                        <= FLT_EPSILON))
            {
                aPixelsForProcessing.push(i);
            }
        }
    }
    return res;
}

/*QList<int> get_region_with_same_brightness(
        const float aPixelsOfImage[], int nImageWidth, int nImageHeight,
        int iStartPixel, bool aFilledPixels[])
{
    int i, x, y;
    bool isFilled;
    QList<int> res;

    aFilledPixels[iStartPixel] = true;
    res.append(iStartPixel);
    x = iStartPixel % nImageWidth - 1; i = iStartPixel - 1;
    while (x >= 0)
    {
        if (fabs(aPixelsOfImage[i]-aPixelsOfImage[iStartPixel]) > FLT_EPSILON)
        {
            break;
        }
        aFilledPixels[i] = true;
        res.append(i);
        i--; x--;
    }
    x = iStartPixel % nImageWidth + 1;
    i = iStartPixel + 1;
    while (x < nImageWidth)
    {
        if (fabs(aPixelsOfImage[i]-aPixelsOfImage[iStartPixel]) > FLT_EPSILON)
        {
            break;
        }
        aFilledPixels[i] = true;
        res.append(i);
        i++; x++;
    }

    y = iStartPixel / nImageWidth - 1;
    while (y >= 0)
    {
        isFilled = false;

        i = y * nImageWidth;
        if (aFilledPixels[i + nImageWidth]
                || aFilledPixels[i + nImageWidth + 1])
        {
            if (fabs(aPixelsOfImage[i] - aPixelsOfImage[iStartPixel])
                    <= FLT_EPSILON)
            {
                aFilledPixels[i] = true;
                res.append(i);
                isFilled = true;
            }
        }
        i++;

        for (x = 1; x < (nImageWidth - 1); x++)
        {
            if (aFilledPixels[i + nImageWidth]
                    || aFilledPixels[i + nImageWidth + 1]
                    || aFilledPixels[i + nImageWidth - 1])
            {
                if (fabs(aPixelsOfImage[i] - aPixelsOfImage[iStartPixel])
                        <= FLT_EPSILON)
                {
                    aFilledPixels[i] = true;
                    res.append(i);
                    isFilled = true;
                }
            }
        }

        if (aFilledPixels[i + nImageWidth]
                || aFilledPixels[i + nImageWidth - 1])
        {
            if (fabs(aPixelsOfImage[i] - aPixelsOfImage[iStartPixel])
                    <= FLT_EPSILON)
            {
                aFilledPixels[i] = true;
                res.append(i);
                isFilled = true;
            }
        }

        if (!isFilled)
        {
            break;
        }
        y--;
    }

    y = iStartPixel / nImageWidth + 1;
    while (y < nImageHeight)
    {
        isFilled = false;

        i = y * nImageWidth;
        if (aFilledPixels[i - nImageWidth]
                || aFilledPixels[i - nImageWidth + 1])
        {
            if (fabs(aPixelsOfImage[i] - aPixelsOfImage[iStartPixel])
                    <= FLT_EPSILON)
            {
                aFilledPixels[i] = true;
                res.append(i);
                isFilled = true;
            }
        }
        i++;

        for (x = 1; x < (nImageWidth - 1); x++)
        {
            if (aFilledPixels[i - nImageWidth]
                    || aFilledPixels[i - nImageWidth + 1]
                    || aFilledPixels[i - nImageWidth - 1])
            {
                if (fabs(aPixelsOfImage[i] - aPixelsOfImage[iStartPixel])
                        <= FLT_EPSILON)
                {
                    aFilledPixels[i] = true;
                    res.append(i);
                    isFilled = true;
                }
            }
        }

        if (aFilledPixels[i - nImageWidth]
                || aFilledPixels[i - nImageWidth - 1])
        {
            if (fabs(aPixelsOfImage[i] - aPixelsOfImage[iStartPixel])
                    <= FLT_EPSILON)
            {
                aFilledPixels[i] = true;
                res.append(i);
                isFilled = true;
            }
        }

        if (!isFilled)
        {
            break;
        }
        y++;
    }

    return res;
}*/

/* Проверить, находится ли в заданном пикселе локальный минимум яркости, т.е.
не превышает ли яркость этого пикселя яркости всех пикселей-соседей.
   Само изображение задано развёрнутой по строкам матрицей яркостей пикселей
aPixelsOfImage[] шириной nImageWidth и высотой nImageHeight. Проверяемый
пиксель задан своим номером iPixelIndex в массиве aPixelsOfImage[]. */
bool is_min_brightness(const float aPixelsOfImage[],
                       int nImageWidth, int nImageHeight, int iPixelIndex)
{
    int x, y, i;

    x = iPixelIndex % nImageWidth; y = iPixelIndex / nImageWidth;
    if (x > 0)
    {
        i = (x-1) + y * nImageWidth;
        if (aPixelsOfImage[i] < aPixelsOfImage[iPixelIndex])
        {
            return false;
        }
    }
    if (y > 0)
    {
        i = x + (y-1) * nImageWidth;
        if (aPixelsOfImage[i] < aPixelsOfImage[iPixelIndex])
        {
            return false;
        }
    }
    if ((x > 0) && (y > 0))
    {
        i = (x-1) + (y-1) * nImageWidth;
        if (aPixelsOfImage[i] < aPixelsOfImage[iPixelIndex])
        {
            return false;
        }
    }
    if (x < (nImageWidth - 1))
    {
        i = (x+1) + y * nImageWidth;
        if (aPixelsOfImage[i] < aPixelsOfImage[iPixelIndex])
        {
            return false;
        }
    }
    if (y < (nImageHeight - 1))
    {
        i = x + (y+1) * nImageWidth;
        if (aPixelsOfImage[i] < aPixelsOfImage[iPixelIndex])
        {
            return false;
        }
    }
    if ((x < (nImageWidth - 1)) && (y > 0))
    {
        i = (x+1) + (y-1) * nImageWidth;
        if (aPixelsOfImage[i] < aPixelsOfImage[iPixelIndex])
        {
            return false;
        }
    }
    if ((x > 0) && (y < (nImageHeight - 1)))
    {
        i = (x-1) + (y+1) * nImageWidth;
        if (aPixelsOfImage[i] < aPixelsOfImage[iPixelIndex])
        {
            return false;
        }
    }
    if ((x < (nImageWidth - 1)) && (y < (nImageHeight - 1)))
    {
        i = (x+1) + (y-1) * nImageWidth;
        if (aPixelsOfImage[i] < aPixelsOfImage[iPixelIndex])
        {
            return false;
        }
    }

    return true;
}

/* Найти новую яркостную "долину" (связную область пикселей, имеющих одинаковую
яркость, при этом меньшую, чем яркость всех соседних пикселей) на растровом
изображении, заданном развёрнутой по строкам матрицей яркостей пикселей
aPixelsOfImage[] шириной nImageWidth и высотой nImageHeight, с учётом того, что
некоторые пиксели этого изображения уже обработаны и потому исключаются из
рассмотрения. Вернуть массив координат пикселей, образующих эту "долину", либо
же пустой массив, если "долин" на изображении больше нет.
   Для определения того, обработан какой-либо пиксель или же нет, в функцию
передаётся целочисленный массив aUsedPixels[], представляющий собой признаки
обработки для каждого пикселя изображения (пиксель обработан, если значение его
признака равно 0, и не обработан, если значение его признака равно -1). При
обнаружении новой "долины" для образующих её пикселей соответствующие признаки
обработки устанавливаются в 0. */
TClusterOfImage find_valley_of_image(const float aPixelsOfImage[],
                                     int nImageWidth, int nImageHeight,
                                     int aUsedPixels[])
{
    const int n = nImageWidth * nImageHeight;
    TClusterOfImage res;
    TPoint2D newPoint;
    QList<int> aIndexesOfPixels;
    QList<int>::const_iterator it;
    QVarLengthArray<bool> aFilledPixels(n);
    bool isValley;
    int i, j;

    for (i = 0; i < n; i++)
    {
        if (aUsedPixels[i] == 0)
        {
            continue;
        }
        for (j = 0; j < n; j++)
        {
            aFilledPixels[j] = false;
        }
        aIndexesOfPixels = get_region_with_same_brightness(
                    aPixelsOfImage, nImageWidth, nImageHeight, i,
                    aFilledPixels.data());
        isValley = true;
        for (it = aIndexesOfPixels.begin(); it != aIndexesOfPixels.end(); it++)
        {
            aUsedPixels[*it] = 0;
            if (!is_min_brightness(aPixelsOfImage, nImageWidth, nImageHeight,
                                   *it))
            {
                isValley = false;
            }
        }
        if (isValley)
        {
            for (it = aIndexesOfPixels.begin(); it != aIndexesOfPixels.end(); it++)
            {
                newPoint.x = (*it) % nImageWidth;
                newPoint.y = (*it) / nImageWidth;
                res.append(newPoint);
            }
            break;
        }
    }

    return res;
}

/* Проверить, имеются ли у точки с координатами checkedPoint на растровом
изображении aFeaturesOfPixels[] такие точки-"соседи", которые относятся к
какому-либо из протокластеров, кроме протокластера под номером
iExcludedNeighbour.
   Исходное изображение, как уже упоминалось выше, является развёрнутой по
строкам растровой матрицей aFeaturesOfPixels[] шириной nImageWidth и высотой
nImageHeight, но в данном случае в качестве элементов этой матрицы выступают не
яркости пикселей, как обычно бывает для растровых изображений, а признаки
принадлежности пикселей тем или иным протокластерам. Если пиксель не относится
ни к одному протокластеру, то его признак принимает значение -1, в противном
случае же признак принимает значение номера соответствующего протокластера. */
bool pointHasNeighbours(const TPoint2D& checkedPoint, int aFeaturesOfPixels[],
                        int nImageWidth, int nImageHeight,
                        int iExcludedNeighbour)
{
    int i;

    if ((checkedPoint.x > 0) && (checkedPoint.y > 0))
    {
        i = (checkedPoint.x-1) + (checkedPoint.y-1) * nImageWidth;
        if ((aFeaturesOfPixels[i] >= 0)
                && (aFeaturesOfPixels[i] != iExcludedNeighbour))
        {
            return true;
        }
    }

    if (checkedPoint.y > 0)
    {
        i = checkedPoint.x + (checkedPoint.y-1) * nImageWidth;
        if ((aFeaturesOfPixels[i] >= 0)
                && (aFeaturesOfPixels[i] != iExcludedNeighbour))
        {
            return true;
        }
    }

    if ((checkedPoint.x < (nImageWidth-1)) && (checkedPoint.y > 0))
    {
        i = (checkedPoint.x+1) + (checkedPoint.y-1) * nImageWidth;
        if ((aFeaturesOfPixels[i] >= 0)
                && (aFeaturesOfPixels[i] != iExcludedNeighbour))
        {
            return true;
        }
    }

    if (checkedPoint.x < (nImageWidth-1))
    {
        i = (checkedPoint.x+1) + checkedPoint.y * nImageWidth;
        if ((aFeaturesOfPixels[i] >= 0)
                && (aFeaturesOfPixels[i] != iExcludedNeighbour))
        {
            return true;
        }
    }

    if ((checkedPoint.x < (nImageWidth-1))
            && (checkedPoint.y < (nImageHeight-1)))
    {
        i = (checkedPoint.x+1) + (checkedPoint.y+1) * nImageWidth;
        if ((aFeaturesOfPixels[i] >= 0)
                && (aFeaturesOfPixels[i] != iExcludedNeighbour))
        {
            return true;
        }
    }

    if (checkedPoint.y < (nImageHeight-1))
    {
        i = checkedPoint.x + (checkedPoint.y+1) * nImageWidth;
        if ((aFeaturesOfPixels[i] >= 0)
                && (aFeaturesOfPixels[i] != iExcludedNeighbour))
        {
            return true;
        }
    }

    if ((checkedPoint.x > 0) && (checkedPoint.y < (nImageHeight-1)))
    {
        i = (checkedPoint.x-1) + checkedPoint.y * nImageWidth;
        if ((aFeaturesOfPixels[i] >= 0)
                && (aFeaturesOfPixels[i] != iExcludedNeighbour))
        {
            return true;
        }
    }

    if (checkedPoint.x > 0)
    {
        i = (checkedPoint.x-1) + checkedPoint.y * nImageWidth;
        if ((aFeaturesOfPixels[i] >= 0)
                && (aFeaturesOfPixels[i] != iExcludedNeighbour))
        {
            return true;
        }
    }

    return false;
}

/* Для каждого пикселя из каждой связной области (протокластера) на растровом
изображении выполнить процедуру дилатации (расширения), но так, чтобы не
допустить слияния существующих связных областей.
   Исходное изображение является развёрнутой по строкам матрицей яркостей
пикселей aPixelsOfImage[] шириной nImageWidth и высотой nImageHeight. Кроме
того, в дополнение к матрице яркостей пикселей задаётся матрица признаков
пикселей aFeaturesOfPixels[], имеющая аналогичную структуру, но в качестве
элементов этой матрицы выступают не яркости пикселей,а признаки принадлежности
этих пикселей тем или иным протокластерам. Если пиксель не относится ни к
одному протокластеру, то его признак принимает значение -1, в противном случае
же признак принимает значение номера соответствующего протокластера.
   Сами же связные области (протокластеры) заданы двухуровневым массивом
aClusters, первый уровень которого соответствует выделенным протокластерам, а
второй уровень - координатам пикселей, входящих в каждый из протокластеров.
   В результате своей работы функция возвращает количество пикселей, попавших в
протокластеры после очередного шага дилатации, обновляет матрицу признаков
пикселей aFeaturesOfPixels[], а также соответствующим образом изменяет
структуру протокластеров aClusters. Если функция так и не смогла найти на
изображении новые пиксели для выполнения дилатации, то возвращается 0. */
int doDilatation(const float aPixelsOfImage[],int nImageWidth,int nImageHeight,
                 int aFeaturesOfPixels[], QVector<TClusterOfImage>&aClusters)
{
    int iCluster, iNode, i, iPixel, n, res = 0;
    QSet<int> aIndexesOfAddedPoints;
    QSet<int>::const_iterator it;
    TPoint2D newPoint;
    for (iCluster = 0; iCluster < aClusters.size(); iCluster++)
    {
        aIndexesOfAddedPoints.clear();
        for (iNode = 0; iNode < aClusters[iCluster].size(); iNode++)
        {
            iPixel = aClusters[iCluster][iNode].x
                    + aClusters[iCluster][iNode].y * nImageWidth;

            newPoint.x = aClusters[iCluster][iNode].x-1;
            newPoint.y = aClusters[iCluster][iNode].y-1;
            if ((newPoint.x >= 0) && (newPoint.y >= 0))
            {
                i = newPoint.x + newPoint.y * nImageWidth;
                if ((aFeaturesOfPixels[i] < 0)
                        && (aPixelsOfImage[i] >= aPixelsOfImage[iPixel]))
                {
                    if (!pointHasNeighbours(newPoint,aFeaturesOfPixels,
                                            nImageWidth,nImageHeight,iCluster))
                    {
                        aIndexesOfAddedPoints.insert(i);
                    }
                }
            }

            newPoint.x = aClusters[iCluster][iNode].x;
            newPoint.y = aClusters[iCluster][iNode].y-1;
            if (newPoint.y >= 0)
            {
                i = newPoint.x + newPoint.y * nImageWidth;
                if ((aFeaturesOfPixels[i] < 0)
                        && (aPixelsOfImage[i] >= aPixelsOfImage[iPixel]))
                {
                    if (!pointHasNeighbours(newPoint,aFeaturesOfPixels,
                                            nImageWidth,nImageHeight,iCluster))
                    {
                        aIndexesOfAddedPoints.insert(i);
                    }
                }
            }

            newPoint.x = aClusters[iCluster][iNode].x+1;
            newPoint.y = aClusters[iCluster][iNode].y-1;
            if ((newPoint.x < nImageWidth) && (newPoint.y >= 0))
            {
                i = newPoint.x + newPoint.y * nImageWidth;
                if ((aFeaturesOfPixels[i] < 0)
                        && (aPixelsOfImage[i] >= aPixelsOfImage[iPixel]))
                {
                    if (!pointHasNeighbours(newPoint,aFeaturesOfPixels,
                                            nImageWidth,nImageHeight,iCluster))
                    {
                        aIndexesOfAddedPoints.insert(i);
                    }
                }
            }

            newPoint.x = aClusters[iCluster][iNode].x+1;
            newPoint.y = aClusters[iCluster][iNode].y;
            if (newPoint.x < nImageWidth)
            {
                i = newPoint.x + newPoint.y * nImageWidth;
                if ((aFeaturesOfPixels[i] < 0)
                        && (aPixelsOfImage[i] >= aPixelsOfImage[iPixel]))
                {
                    if (!pointHasNeighbours(newPoint,aFeaturesOfPixels,
                                            nImageWidth,nImageHeight,iCluster))
                    {
                        aIndexesOfAddedPoints.insert(i);
                    }
                }
            }

            newPoint.x = aClusters[iCluster][iNode].x+1;
            newPoint.y = aClusters[iCluster][iNode].y+1;
            if ((newPoint.x < nImageWidth) && (newPoint.y < nImageHeight))
            {
                i = newPoint.x + newPoint.y * nImageWidth;
                if ((aFeaturesOfPixels[i] < 0)
                        && (aPixelsOfImage[i] >= aPixelsOfImage[iPixel]))
                {
                    if (!pointHasNeighbours(newPoint,aFeaturesOfPixels,
                                            nImageWidth,nImageHeight,iCluster))
                    {
                        aIndexesOfAddedPoints.insert(i);
                    }
                }
            }

            newPoint.x = aClusters[iCluster][iNode].x;
            newPoint.y = aClusters[iCluster][iNode].y+1;
            if (newPoint.y < nImageHeight)
            {
                i = newPoint.x + newPoint.y * nImageWidth;
                if ((aFeaturesOfPixels[i] < 0)
                        && (aPixelsOfImage[i] >= aPixelsOfImage[iPixel]))
                {
                    if (!pointHasNeighbours(newPoint,aFeaturesOfPixels,
                                            nImageWidth,nImageHeight,iCluster))
                    {
                        aIndexesOfAddedPoints.insert(i);
                    }
                }
            }

            newPoint.x = aClusters[iCluster][iNode].x-1;
            newPoint.y = aClusters[iCluster][iNode].y+1;
            if ((newPoint.x >= 0) && (newPoint.y < nImageHeight))
            {
                i = newPoint.x + newPoint.y * nImageWidth;
                if ((aFeaturesOfPixels[i] < 0)
                        && (aPixelsOfImage[i] >= aPixelsOfImage[iPixel]))
                {
                    if (!pointHasNeighbours(newPoint,aFeaturesOfPixels,
                                            nImageWidth,nImageHeight,iCluster))
                    {
                        aIndexesOfAddedPoints.insert(i);
                    }
                }
            }

            newPoint.x = aClusters[iCluster][iNode].x-1;
            newPoint.y = aClusters[iCluster][iNode].y;
            if (newPoint.x >= 0)
            {
                i = newPoint.x + newPoint.y * nImageWidth;
                if ((aFeaturesOfPixels[i] < 0)
                        && (aPixelsOfImage[i] >= aPixelsOfImage[iPixel]))
                {
                    if (!pointHasNeighbours(newPoint,aFeaturesOfPixels,
                                            nImageWidth,nImageHeight,iCluster))
                    {
                        aIndexesOfAddedPoints.insert(i);
                    }
                }
            }
        }
        if (!aIndexesOfAddedPoints.isEmpty())
        {
            n = aClusters[iCluster].size() + aIndexesOfAddedPoints.size();
            if (aClusters[iCluster].capacity() < n)
            {
                aClusters[iCluster].reserve(n);
            }
            for (it = aIndexesOfAddedPoints.cbegin();
                 it != aIndexesOfAddedPoints.cend();
                 it++)
            {
                newPoint.x = (*it) % nImageWidth;
                newPoint.y = (*it) / nImageWidth;
                aFeaturesOfPixels[*it] = iCluster;
                aClusters[iCluster].append(newPoint);
                res++;
            }
        }
    }
    return res;
}

/*int doDilatation(int aOldFeaturesOfPixels[], int aNewFeaturesOfPixels[],
                 int nImageWidth, int nImageHeight,
                 QVector<TClusterOfImage>&aSelectedClusters)
{
    const int nPixels = nImageWidth * nImageHeight;
    int iCluster, iPixel, i, x, y, res = 0;
    bool isOneNeighbour, isTwoNeighbours;
    memcpy(aNewFeaturesOfPixels, aOldFeaturesOfPixels, nPixels * sizeof(int));
    for (iPixel = 0; iPixel < nPixels; iPixel++)
    {
        if (aNewFeaturesOfPixels[iPixel] >= 0)
        {
            continue;
        }

        isOneNeighbour = false; isTwoNeighbours = false;
        x = iPixel % nImageWidth;
        y = iPixel / nImageWidth;
        iCluster = -1;

        if (x > 0)
        {
            i = x-1 + y * nImageWidth;
            if (aOldFeaturesOfPixels[i] >= 0)
            {
                isOneNeighbour = true;
                iCluster = aOldFeaturesOfPixels[i];
            }
        }

        if (y > 0)
        {
            i = x + (y-1) * nImageWidth;
            if (aOldFeaturesOfPixels[i] >= 0)
            {
                if (isOneNeighbour)
                {
                    if (aOldFeaturesOfPixels[i] != iCluster)
                    {
                        isTwoNeighbours = true;
                    }
                }
                else
                {
                    isOneNeighbour = true;
                    iCluster = aOldFeaturesOfPixels[i];
                }
            }
        }
        if (isTwoNeighbours)
        {
            continue;
        }

        if (x < (nImageWidth - 1))
        {
            i = x+1 + y * nImageWidth;
            if (aOldFeaturesOfPixels[i] >= 0)
            {
                if (isOneNeighbour)
                {
                    if (aOldFeaturesOfPixels[i] != iCluster)
                    {
                        isTwoNeighbours = true;
                    }
                }
                else
                {
                    isOneNeighbour = true;
                    iCluster = aOldFeaturesOfPixels[i];
                }
            }
        }
        if (isTwoNeighbours)
        {
            continue;
        }

        if (y < (nImageHeight - 1))
        {
            i = x + (y+1) * nImageWidth;
            if (aOldFeaturesOfPixels[i] >= 0)
            {
                if (isOneNeighbour)
                {
                    if (aOldFeaturesOfPixels[i] != iCluster)
                    {
                        isTwoNeighbours = true;
                    }
                }
                else
                {
                    isOneNeighbour = true;
                    iCluster = aOldFeaturesOfPixels[i];
                }
            }
        }
        if (isTwoNeighbours)
        {
            continue;
        }

        if ((x > 0) && (y > 0))
        {
            i = x-1 + (y-1) * nImageWidth;
            if (aOldFeaturesOfPixels[i] >= 0)
            {
                if (isOneNeighbour)
                {
                    if (aOldFeaturesOfPixels[i] != iCluster)
                    {
                        isTwoNeighbours = true;
                    }
                }
                else
                {
                    isOneNeighbour = true;
                    iCluster = aOldFeaturesOfPixels[i];
                }
            }
        }
        if (isTwoNeighbours)
        {
            continue;
        }

        if ((x < (nImageWidth - 1)) && (y < (nImageHeight - 1)))
        {
            i = x+1 + (y+1) * nImageWidth;
            if (aOldFeaturesOfPixels[i] >= 0)
            {
                if (isOneNeighbour)
                {
                    if (aOldFeaturesOfPixels[i] != iCluster)
                    {
                        isTwoNeighbours = true;
                    }
                }
                else
                {
                    isOneNeighbour = true;
                    iCluster = aOldFeaturesOfPixels[i];
                }
            }
        }
        if (isTwoNeighbours)
        {
            continue;
        }

        if ((x > 0) && (y < (nImageHeight - 1)))
        {
            i = x-1 + (y+1) * nImageWidth;
            if (aOldFeaturesOfPixels[i] >= 0)
            {
                if (isOneNeighbour)
                {
                    if (aOldFeaturesOfPixels[i] != iCluster)
                    {
                        isTwoNeighbours = true;
                    }
                }
                else
                {
                    isOneNeighbour = true;
                    iCluster = aOldFeaturesOfPixels[i];
                }
            }
        }
        if (isTwoNeighbours)
        {
            continue;
        }

        if ((x < (nImageWidth - 1)) && (y > 0))
        {
            i = x+1 + (y-1) * nImageWidth;
            if (aOldFeaturesOfPixels[i] >= 0)
            {
                if (isOneNeighbour)
                {
                    if (aOldFeaturesOfPixels[i] != iCluster)
                    {
                        isTwoNeighbours = true;
                    }
                }
                else
                {
                    isOneNeighbour = true;
                    iCluster = aOldFeaturesOfPixels[i];
                }
            }
        }
        if (isTwoNeighbours)
        {
            continue;
        }

        if (!isOneNeighbour)
        {
            continue;
        }
        aNewFeaturesOfPixels[iPixel] = iCluster;
        aSelectedClusters[iCluster].append(TPoint2D(x,y));
        res++;
    }

    return res;
}*/

/*****************************************************************************/
/* ВНЕШНИЕ ФУНКЦИИ МОДУЛЯ image_processing_lib */
/*****************************************************************************/

void filter_image(float aPixelsOfImage[], int nImageWidth, int nImageHeight,
                  const float aFilterMask[], int nMaskWidth, int nMaskHeight)
{
    const int n = nImageWidth * nImageHeight;
    const int xMaskCenter = nMaskWidth / 2, yMaskCenter = nMaskHeight / 2;
    int xImage, yImage, xImage2, yImage2, xMask, yMask, iImage, iImage2, iMask;
    QVarLengthArray<float> aPixelsAfterFiltering(n);
    float tmp_value;

    memset(aPixelsAfterFiltering.data(), 0, n * sizeof(float));
    iImage = 0;
    for (yImage = 0; yImage < nImageHeight; yImage++)
    {
        for (xImage = 0; xImage < nImageWidth; xImage++)
        {
            iMask = 0;
            for (yMask = 0; yMask < nMaskHeight; yMask++)
            {
                for (xMask = 0; xMask < nMaskWidth; xMask++)
                {
                    xImage2 = xImage + (xMask - xMaskCenter);
                    yImage2 = yImage + (yMask - yMaskCenter);
                    /*if (((xImage2 < 0) && (yImage2 < 0))
                            || ((xImage2 >= nImageWidth)
                                && (yImage2 >= nImageHeight))
                            || ((xImage2 < 0) && (yImage2 >= nImageHeight))
                            || ((xImage2 >= nImageWidth) && (yImage2 < 0)))
                    {
                        iMask++;
                        continue;
                    }*/
                    if ((xImage2 < 0) && (yImage2 < 0))
                    {
                        iImage2 = 0;
                    }
                    else if ((xImage2 >= nImageWidth)
                             && (yImage2 >= nImageHeight))
                    {
                        iImage2 = nImageWidth-1 + (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 < 0) && (yImage2 >= nImageHeight))
                    {
                        iImage2 = (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 >= nImageWidth) && (yImage2 < 0))
                    {
                        iImage2 = nImageWidth-1;
                    }
                    else if (xImage2 < 0)
                    {
                        iImage2 = yImage2 * nImageWidth;
                    }
                    else if (xImage2 >= nImageWidth)
                    {
                        iImage2 = nImageWidth-1 + yImage2 * nImageWidth;
                    }
                    else if (yImage2 < 0)
                    {
                        iImage2 = xImage2;
                    }
                    else if (yImage2 >= nImageHeight)
                    {
                        iImage2 = xImage2 + (nImageHeight-1) * nImageWidth;
                    }
                    else
                    {
                        iImage2 = xImage2 + yImage2 * nImageWidth;
                    }
                    /*if ((xImage2 < 0) || (xImage2 >= nImageWidth)
                            || (yImage2 < 0) || (yImage2 >= nImageHeight))
                    {
                        iMask++;
                        continue;
                    }
                    iImage2 = xImage2 + yImage2 * nImageWidth;*/
                    tmp_value = aPixelsOfImage[iImage2] * aFilterMask[iMask];
                    aPixelsAfterFiltering[iImage] += tmp_value;
                    iMask++;
                }
            }
            iImage++;
        }
    }
    memcpy(aPixelsOfImage, aPixelsAfterFiltering.data(), n * sizeof(float));
}

void filter_image(float aPixelsOfImage[], int nImageWidth, int nImageHeight,
                  const float aFilterMask[], int nMaskWidth, int nMaskHeight,
                  float aTmpPixelsOfImage[])
{
    const int n = nImageWidth * nImageHeight;
    const int xMaskCenter = nMaskWidth / 2, yMaskCenter = nMaskHeight / 2;
    int xImage, yImage, xImage2, yImage2, xMask, yMask, iImage, iImage2, iMask;
    float tmp_value;

    memset(aTmpPixelsOfImage, 0, sizeof(float) * n);
    iImage = 0;
    for (yImage = 0; yImage < nImageHeight; yImage++)
    {
        for (xImage = 0; xImage < nImageWidth; xImage++)
        {
            iMask = 0;
            for (yMask = 0; yMask < nMaskHeight; yMask++)
            {
                for (xMask = 0; xMask < nMaskWidth; xMask++)
                {
                    xImage2 = xImage + (xMask - xMaskCenter);
                    yImage2 = yImage + (yMask - yMaskCenter);
                    /*if (((xImage2 < 0) && (yImage2 < 0))
                            || ((xImage2 >= nImageWidth)
                                && (yImage2 >= nImageHeight))
                            || ((xImage2 < 0) && (yImage2 >= nImageHeight))
                            || ((xImage2 >= nImageWidth) && (yImage2 < 0)))
                    {
                        iMask++;
                        continue;
                    }*/
                    if ((xImage2 < 0) && (yImage2 < 0))
                    {
                        iImage2 = 0;
                    }
                    else if ((xImage2 >= nImageWidth)
                             && (yImage2 >= nImageHeight))
                    {
                        iImage2 = nImageWidth-1 + (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 < 0) && (yImage2 >= nImageHeight))
                    {
                        iImage2 = (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 >= nImageWidth) && (yImage2 < 0))
                    {
                        iImage2 = nImageWidth-1;
                    }
                    else if (xImage2 < 0)
                    {
                        iImage2 = yImage2 * nImageWidth;
                    }
                    else if (xImage2 >= nImageWidth)
                    {
                        iImage2 = nImageWidth-1 + yImage2 * nImageWidth;
                    }
                    else if (yImage2 < 0)
                    {
                        iImage2 = xImage2;
                    }
                    else if (yImage2 >= nImageHeight)
                    {
                        iImage2 = xImage2 + (nImageHeight-1) * nImageWidth;
                    }
                    else
                    {
                        iImage2 = xImage2 + yImage2 * nImageWidth;
                    }
                    /*if ((xImage2 < 0) || (xImage2 >= nImageWidth)
                            || (yImage2 < 0) || (yImage2 >= nImageHeight))
                    {
                        iMask++;
                        continue;
                    }
                    iImage2 = xImage2 + yImage2 * nImageWidth;*/
                    tmp_value = aPixelsOfImage[iImage2] * aFilterMask[iMask];
                    aTmpPixelsOfImage[iImage] += tmp_value;
                    iMask++;
                }
            }
            iImage++;
        }
    }
    memcpy(aPixelsOfImage, aTmpPixelsOfImage, n * sizeof(float));
}

void smooth_image(float aPixelsOfImage[], int nImageWidth, int nImageHeight,
                  int nMaskWidth, int nMaskHeight, bool bIsMedian)
{
    const int nMaskSize = nMaskWidth * nMaskHeight;
    QVarLengthArray<float> aMask(nMaskSize);

    if (!bIsMedian)
    {
        for (int i = 0; i < nMaskSize; i++)
        {
            aMask[i] = 1.0 / (float)nMaskSize;
        }
        filter_image(aPixelsOfImage, nImageWidth, nImageHeight,
                     aMask.constData(), nMaskWidth, nMaskHeight);
        return;
    }

    const int n = nImageWidth * nImageHeight;
    const int xMaskCenter = nMaskWidth / 2, yMaskCenter = nMaskHeight / 2;
    int xImage, yImage, xImage2, yImage2, xMask, yMask, iImage, iImage2, iMask;
    QVarLengthArray<float> aPixelsAfterFiltering(n);

    memset(aPixelsAfterFiltering.data(), 0, n * sizeof(float));
    iImage = 0;
    for (yImage = 0; yImage < nImageHeight; yImage++)
    {
        for (xImage = 0; xImage < nImageWidth; xImage++)
        {
            iMask = 0;
            for (yMask = 0; yMask < nMaskHeight; yMask++)
            {
                for (xMask = 0; xMask < nMaskWidth; xMask++)
                {
                    xImage2 = xImage + (xMask - xMaskCenter);
                    yImage2 = yImage + (yMask - yMaskCenter);
                    /*if (((xImage2 < 0) && (yImage2 < 0))
                            || ((xImage2 >= nImageWidth)
                                && (yImage2 >= nImageHeight))
                            || ((xImage2 < 0) && (yImage2 >= nImageHeight))
                            || ((xImage2 >= nImageWidth) && (yImage2 < 0)))
                    {
                        aMask[iMask] = 0.0;
                        iMask++;
                        continue;
                    }*/
                    if ((xImage2 < 0) && (yImage2 < 0))
                    {
                        iImage2 = 0;
                    }
                    else if ((xImage2 >= nImageWidth)
                             && (yImage2 >= nImageHeight))
                    {
                        iImage2 = nImageWidth-1 + (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 < 0) && (yImage2 >= nImageHeight))
                    {
                        iImage2 = (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 >= nImageWidth) && (yImage2 < 0))
                    {
                        iImage2 = nImageWidth-1;
                    }
                    else if (xImage2 < 0)
                    {
                        iImage2 = yImage2 * nImageWidth;
                    }
                    else if (xImage2 >= nImageWidth)
                    {
                        iImage2 = nImageWidth-1 + yImage2 * nImageWidth;
                    }
                    else if (yImage2 < 0)
                    {
                        iImage2 = xImage2;
                    }
                    else if (yImage2 >= nImageHeight)
                    {
                        iImage2 = xImage2 + (nImageHeight-1) * nImageWidth;
                    }
                    else
                    {
                        iImage2 = xImage2 + yImage2 * nImageWidth;
                    }
                    /*if ((xImage2 < 0) || (xImage2 >= nImageWidth)
                            || (yImage2 < 0) || (yImage2 >= nImageHeight))
                    {
                        aMask[iMask] = 0.0;
                        iMask++;
                        continue;
                    }
                    iImage2 = xImage2 + yImage2 * nImageWidth;*/
                    aMask[iMask] = aPixelsOfImage[iImage2];
                    iMask++;
                }
            }
            qSort(aMask);
            aPixelsAfterFiltering[iImage] = aMask[nMaskSize / 2];
            iImage++;
        }
    }
    memcpy(aPixelsOfImage, aPixelsAfterFiltering.data(), n * sizeof(float));
}

void smooth_image(float aPixelsOfImage[], int nImageWidth, int nImageHeight,
                  int nMaskWidth, int nMaskHeight, bool bIsMedian,
                  float aTmpPixelsOfImage[])
{
    const int nMaskSize = nMaskWidth * nMaskHeight;
    QVarLengthArray<float> aMask(nMaskSize);

    if (!bIsMedian)
    {
        for (int i = 0; i < nMaskSize; i++)
        {
            aMask[i] = 1.0 / (float)nMaskSize;
        }
        filter_image(aPixelsOfImage, nImageWidth, nImageHeight,
                     aMask.constData(), nMaskWidth, nMaskHeight,
                     aTmpPixelsOfImage);
        return;
    }

    const int n = nImageWidth * nImageHeight;
    const int xMaskCenter = nMaskWidth / 2, yMaskCenter = nMaskHeight / 2;
    int xImage, yImage, xImage2, yImage2, xMask, yMask, iImage, iImage2, iMask;

    memset(aTmpPixelsOfImage, 0, sizeof(float) * n);
    iImage = 0;
    for (yImage = 0; yImage < nImageHeight; yImage++)
    {
        for (xImage = 0; xImage < nImageWidth; xImage++)
        {
            iMask = 0;
            for (yMask = 0; yMask < nMaskHeight; yMask++)
            {
                for (xMask = 0; xMask < nMaskWidth; xMask++)
                {
                    xImage2 = xImage + (xMask - xMaskCenter);
                    yImage2 = yImage + (yMask - yMaskCenter);
                    /*if (((xImage2 < 0) && (yImage2 < 0))
                            || ((xImage2 >= nImageWidth)
                                && (yImage2 >= nImageHeight))
                            || ((xImage2 < 0) && (yImage2 >= nImageHeight))
                            || ((xImage2 >= nImageWidth) && (yImage2 < 0)))
                    {
                        aMask[iMask] = 0.0;
                        iMask++;
                        continue;
                    }*/
                    if ((xImage2 < 0) && (yImage2 < 0))
                    {
                        iImage2 = 0;
                    }
                    else if ((xImage2 >= nImageWidth)
                             && (yImage2 >= nImageHeight))
                    {
                        iImage2 = nImageWidth-1 + (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 < 0) && (yImage2 >= nImageHeight))
                    {
                        iImage2 = (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 >= nImageWidth) && (yImage2 < 0))
                    {
                        iImage2 = nImageWidth-1;
                    }
                    else if (xImage2 < 0)
                    {
                        iImage2 = yImage2 * nImageWidth;
                    }
                    else if (xImage2 >= nImageWidth)
                    {
                        iImage2 = nImageWidth-1 + yImage2 * nImageWidth;
                    }
                    else if (yImage2 < 0)
                    {
                        iImage2 = xImage2;
                    }
                    else if (yImage2 >= nImageHeight)
                    {
                        iImage2 = xImage2 + (nImageHeight-1) * nImageWidth;
                    }
                    else
                    {
                        iImage2 = xImage2 + yImage2 * nImageWidth;
                    }
                    /*if ((xImage2 < 0) || (xImage2 >= nImageWidth)
                            || (yImage2 < 0) || (yImage2 >= nImageHeight))
                    {
                        aMask[iMask] = 0.0;
                        iMask++;
                        continue;
                    }
                    iImage2 = xImage2 + yImage2 * nImageWidth;*/
                    aMask[iMask] = aPixelsOfImage[iImage2];
                    iMask++;
                }
            }
            qSort(aMask);
            aTmpPixelsOfImage[iImage] = aMask[nMaskSize / 2];
            iImage++;
        }
    }
    memcpy(aPixelsOfImage, aTmpPixelsOfImage, n * sizeof(float));
}

void calculate_image_gradient(float aPixelsOfImage[],
                              int nImageWidth, int nImageHeight)
{
    const float aFilterMask1[] = {
        -1.0, -2.0, -1.0,
         0.0,  0.0,  0.0,
         1.0,  2.0,  1.0
    };
    const float aFilterMask2[] = {
        -1.0,  0.0,  1.0,
        -2.0,  0.0,  2.0,
        -1.0,  0.0,  1.0
    };
    const int nMaskWidth = 3, nMaskHeight = 3;
    const int n = nImageWidth * nImageHeight;
    const int xMaskCenter = nMaskWidth / 2, yMaskCenter = nMaskHeight / 2;
    int xImage, yImage, xImage2, yImage2, xMask, yMask, iImage, iImage2, iMask;
    QVarLengthArray<float> aPixelsAfterFiltering(n);
    float tmp_value;

    iImage = 0;
    for (yImage = 0; yImage < nImageHeight; yImage++)
    {
        for (xImage = 0; xImage < nImageWidth; xImage++)
        {
            iMask = 0;
            tmp_value = 0.0;
            for (yMask = 0; yMask < nMaskHeight; yMask++)
            {
                for (xMask = 0; xMask < nMaskWidth; xMask++)
                {
                    xImage2 = xImage + (xMask - xMaskCenter);
                    yImage2 = yImage + (yMask - yMaskCenter);
                    if ((xImage2 < 0) && (yImage2 < 0))
                    {
                        iImage2 = 0;
                    }
                    else if ((xImage2 >= nImageWidth)
                             && (yImage2 >= nImageHeight))
                    {
                        iImage2 = nImageWidth-1 + (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 < 0) && (yImage2 >= nImageHeight))
                    {
                        iImage2 = (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 >= nImageWidth) && (yImage2 < 0))
                    {
                        iImage2 = nImageWidth-1;
                    }
                    else if (xImage2 < 0)
                    {
                        iImage2 = yImage2 * nImageWidth;
                    }
                    else if (xImage2 >= nImageWidth)
                    {
                        iImage2 = nImageWidth-1 + yImage2 * nImageWidth;
                    }
                    else if (yImage2 < 0)
                    {
                        iImage2 = xImage2;
                    }
                    else if (yImage2 >= nImageHeight)
                    {
                        iImage2 = xImage2 + (nImageHeight-1) * nImageWidth;
                    }
                    else
                    {
                        iImage2 = xImage2 + yImage2 * nImageWidth;
                    }
                    /*if ((xImage2 < 0) || (xImage2 >= nImageWidth)
                            || (yImage2 < 0) || (yImage2 >= nImageHeight))
                    {
                        iMask++;
                        continue;
                    }
                    iImage2 = iImage + (xMask - xMaskCenter)
                            + (yMask - yMaskCenter) * nMaskWidth;*/
                    tmp_value += aPixelsOfImage[iImage2] * aFilterMask1[iMask];
                    iMask++;
                }
            }
            aPixelsAfterFiltering[iImage] = tmp_value;

            iMask = 0;
            tmp_value = 0.0;
            for (yMask = 0; yMask < nMaskHeight; yMask++)
            {
                for (xMask = 0; xMask < nMaskWidth; xMask++)
                {
                    xImage2 = xImage + (xMask - xMaskCenter);
                    yImage2 = yImage + (yMask - yMaskCenter);
                    if ((xImage2 < 0) && (yImage2 < 0))
                    {
                        iImage2 = 0;
                    }
                    else if ((xImage2 >= nImageWidth)
                             && (yImage2 >= nImageHeight))
                    {
                        iImage2 = nImageWidth-1 + (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 < 0) && (yImage2 >= nImageHeight))
                    {
                        iImage2 = (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 >= nImageWidth) && (yImage2 < 0))
                    {
                        iImage2 = nImageWidth-1;
                    }
                    else if (xImage2 < 0)
                    {
                        iImage2 = yImage2 * nImageWidth;
                    }
                    else if (xImage2 >= nImageWidth)
                    {
                        iImage2 = nImageWidth-1 + yImage2 * nImageWidth;
                    }
                    else if (yImage2 < 0)
                    {
                        iImage2 = xImage2;
                    }
                    else if (yImage2 >= nImageHeight)
                    {
                        iImage2 = xImage2 + (nImageHeight-1) * nImageWidth;
                    }
                    else
                    {
                        iImage2 = xImage2 + yImage2 * nImageWidth;
                    }
                    /*if ((xImage2 < 0) || (xImage2 >= nImageWidth)
                            || (yImage2 < 0) || (yImage2 >= nImageHeight))
                    {
                        iMask++;
                        continue;
                    }
                    iImage2 = iImage + (xMask - xMaskCenter)
                            + (yMask - yMaskCenter) * nMaskWidth;*/
                    tmp_value += aPixelsOfImage[iImage2] * aFilterMask2[iMask];
                    iMask++;
                }
            }
            aPixelsAfterFiltering[iImage] += fabs(tmp_value);

            iImage++;
        }
    }
    memcpy(aPixelsOfImage, aPixelsAfterFiltering.data(), n * sizeof(float));
}

void calculate_image_gradient(float aPixelsOfImage[],
                              int nImageWidth, int nImageHeight,
                              float aTmpPixelsOfImage[])
{
    const float aFilterMask1[] = {
        -1.0, -2.0, -1.0,
         0.0,  0.0,  0.0,
         1.0,  2.0,  1.0
    };
    const float aFilterMask2[] = {
        -1.0,  0.0,  1.0,
        -2.0,  0.0,  2.0,
        -1.0,  0.0,  1.0
    };
    const int nMaskWidth = 3, nMaskHeight = 3;
    const int n = nImageWidth * nImageHeight;
    const int xMaskCenter = nMaskWidth / 2, yMaskCenter = nMaskHeight / 2;
    int xImage, yImage, xImage2, yImage2, xMask, yMask, iImage, iImage2, iMask;
    float tmp_value;

    iImage = 0;
    for (yImage = 0; yImage < nImageHeight; yImage++)
    {
        for (xImage = 0; xImage < nImageWidth; xImage++)
        {
            iMask = 0;
            tmp_value = 0.0;
            for (yMask = 0; yMask < nMaskHeight; yMask++)
            {
                for (xMask = 0; xMask < nMaskWidth; xMask++)
                {
                    xImage2 = xImage + (xMask - xMaskCenter);
                    yImage2 = yImage + (yMask - yMaskCenter);
                    if ((xImage2 < 0) && (yImage2 < 0))
                    {
                        iImage2 = 0;
                    }
                    else if ((xImage2 >= nImageWidth)
                             && (yImage2 >= nImageHeight))
                    {
                        iImage2 = nImageWidth-1 + (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 < 0) && (yImage2 >= nImageHeight))
                    {
                        iImage2 = (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 >= nImageWidth) && (yImage2 < 0))
                    {
                        iImage2 = nImageWidth-1;
                    }
                    else if (xImage2 < 0)
                    {
                        iImage2 = yImage2 * nImageWidth;
                    }
                    else if (xImage2 >= nImageWidth)
                    {
                        iImage2 = nImageWidth-1 + yImage2 * nImageWidth;
                    }
                    else if (yImage2 < 0)
                    {
                        iImage2 = xImage2;
                    }
                    else if (yImage2 >= nImageHeight)
                    {
                        iImage2 = xImage2 + (nImageHeight-1) * nImageWidth;
                    }
                    else
                    {
                        iImage2 = xImage2 + yImage2 * nImageWidth;
                    }
                    /*if ((xImage2 < 0) || (xImage2 >= nImageWidth)
                            || (yImage2 < 0) || (yImage2 >= nImageHeight))
                    {
                        iMask++;
                        continue;
                    }
                    iImage2 = iImage + (xMask - xMaskCenter)
                            + (yMask - yMaskCenter) * nMaskWidth;*/
                    tmp_value += aPixelsOfImage[iImage2] * aFilterMask1[iMask];
                    iMask++;
                }
            }
            aTmpPixelsOfImage[iImage] = tmp_value;

            iMask = 0;
            tmp_value = 0.0;
            for (yMask = 0; yMask < nMaskHeight; yMask++)
            {
                for (xMask = 0; xMask < nMaskWidth; xMask++)
                {
                    xImage2 = xImage + (xMask - xMaskCenter);
                    yImage2 = yImage + (yMask - yMaskCenter);
                    if ((xImage2 < 0) && (yImage2 < 0))
                    {
                        iImage2 = 0;
                    }
                    else if ((xImage2 >= nImageWidth)
                             && (yImage2 >= nImageHeight))
                    {
                        iImage2 = nImageWidth-1 + (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 < 0) && (yImage2 >= nImageHeight))
                    {
                        iImage2 = (nImageHeight-1)*nImageWidth;
                    }
                    else if ((xImage2 >= nImageWidth) && (yImage2 < 0))
                    {
                        iImage2 = nImageWidth-1;
                    }
                    else if (xImage2 < 0)
                    {
                        iImage2 = yImage2 * nImageWidth;
                    }
                    else if (xImage2 >= nImageWidth)
                    {
                        iImage2 = nImageWidth-1 + yImage2 * nImageWidth;
                    }
                    else if (yImage2 < 0)
                    {
                        iImage2 = xImage2;
                    }
                    else if (yImage2 >= nImageHeight)
                    {
                        iImage2 = xImage2 + (nImageHeight-1) * nImageWidth;
                    }
                    else
                    {
                        iImage2 = xImage2 + yImage2 * nImageWidth;
                    }
                    /*if ((xImage2 < 0) || (xImage2 >= nImageWidth)
                            || (yImage2 < 0) || (yImage2 >= nImageHeight))
                    {
                        iMask++;
                        continue;
                    }
                    iImage2 = iImage + (xMask - xMaskCenter)
                            + (yMask - yMaskCenter) * nMaskWidth;*/
                    tmp_value += aPixelsOfImage[iImage2] * aFilterMask2[iMask];
                    iMask++;
                }
            }
            aTmpPixelsOfImage[iImage] += fabs(tmp_value);

            iImage++;
        }
    }
    memcpy(aPixelsOfImage, aTmpPixelsOfImage, n * sizeof(float));
}

QVector<TClusterOfImage> segmentate_image_by_watershed_alg(
        const float aPixelsOfImage[], int nImageWidth, int nImageHeight)
{
    const int n = nImageWidth * nImageHeight;
    QVarLengthArray<int> aUsedPixels(n);
    QVector<TClusterOfImage> res;
    TClusterOfImage newCluster;
    int i, j;

    for (i = 0; i < n; i++)
    {
        aUsedPixels[i] = -1;
    }
    newCluster = find_valley_of_image(aPixelsOfImage, nImageWidth,nImageHeight,
                                      aUsedPixels.data());
    if (newCluster.isEmpty())
    {
        return res;
    }
    res.append(newCluster);
    newCluster = find_valley_of_image(aPixelsOfImage, nImageWidth,nImageHeight,
                                      aUsedPixels.data());
    while (!newCluster.isEmpty())
    {
        res.append(newCluster);
        newCluster = find_valley_of_image(aPixelsOfImage,
                                          nImageWidth,nImageHeight,
                                          aUsedPixels.data());
    }

    for (i = 0; i < n; i++)
    {
        aUsedPixels[i] = -1;
    }
    for (i = 0; i < res.size(); i++)
    {
        for (j = 0; j < res[i].size(); j++)
        {
            aUsedPixels[res[i][j].x + res[i][j].y * nImageWidth] = i;
        }
    }

    while (doDilatation(aPixelsOfImage, nImageWidth, nImageHeight,
                        aUsedPixels.data(), res) > 0);

    return res;
}
