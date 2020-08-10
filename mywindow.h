#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QColorDialog>
#include <QColor>
#include "punkt.h"
#include <vector>
#include <algorithm>

namespace Ui {
    class MyWindow;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = 0);

    ~MyWindow();

private:
    Ui::MyWindow *ui;

    QImage *img;
    QImage *T;

    QColor colorRysowania;
    QColor colorWypelniania;
    QColor colorCzysc;
    QColor pomocnik;

    int szer;
    int wys;

    punkt pocz;
    punkt pkt;

    int ilePunktow = 0;
    int ilePrzeciec = 0;

    std::vector<punkt> wektorPunktow;
    std::vector<punkt> wektorDoScanLine;
    std::vector<int> wektorPrzeciec;

    void czysc();
    void rysujOdcinek(punkt &a, punkt &b);
    void zamalujPiksel(punkt &piksel, QColor &color);
    double kwadratOdleglosciPunktow(punkt &P, punkt &Q);
    bool czyLiniaPrzecinaWielokat(int &y, punkt &startOdcinka, punkt &stopOdcinka);
    float wspolczynnikDoXPrzeciecia(punkt &startOdcinka, punkt &stopOdcinka);
    int xPrzeciecia(int &y, punkt &startOdcinka, punkt &stopOdcinka);
    void wielokatScanLine();
    void rysujPunktyKwadraty(punkt &P);

private slots:
    void on_cleanButton_clicked();
    void on_exitButton_clicked();
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent*);
    void on_przyciskKoloruRysowania_clicked();
    void on_przyciskKoloruWypelniania_clicked();
};

#endif
