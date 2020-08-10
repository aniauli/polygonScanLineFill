#include "mywindow.h"
#include "ui_mywindow.h"

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);

    szer = ui->rysujFrame->width();
    wys = ui->rysujFrame->height();
    pocz.x = ui->rysujFrame->x();
    pocz.y = ui->rysujFrame->y();

    img = new QImage(szer,wys,QImage::Format_RGB32);
    T = new QImage(szer,wys,QImage::Format_RGB32);

    colorRysowania.setRgb(169, 169, 169, 255);
    colorWypelniania.setRgb(170,255, 255, 255);
    QString qss = QString("background-color: %1").arg(colorRysowania.name());
    ui->przyciskKoloruRysowania->setStyleSheet(qss);
    QString qssi = QString("background-color: %1").arg(colorWypelniania.name());
    ui->przyciskKoloruWypelniania->setStyleSheet(qssi);
}

MyWindow::~MyWindow()
{
    delete ui;
}

void MyWindow::on_exitButton_clicked()
{
    qApp->quit();
}

void MyWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    p.drawImage(pocz.x, pocz.y, *img);
}

void MyWindow::on_cleanButton_clicked()
{
    czysc();
    update();
}

void MyWindow::on_przyciskKoloruRysowania_clicked()
{
    colorRysowania = QColorDialog::getColor();
    QString qss = QString("background-color: %1").arg(colorRysowania.name());
    ui->przyciskKoloruRysowania->setStyleSheet(qss);
}

void MyWindow::on_przyciskKoloruWypelniania_clicked()
{
    colorWypelniania = QColorDialog::getColor();
    QString qss = QString("background-color: %1").arg(colorWypelniania.name());
    ui->przyciskKoloruWypelniania->setStyleSheet(qss);
}

void MyWindow::zamalujPiksel(punkt &piksel, QColor &color)
{
    unsigned char *ptr;
    ptr = img->bits();
    int szer = img->width();
    int skladowaRed = 4*piksel.x + szer*4*piksel.y + 2;
    int skladowaGreen = 4*piksel.x + szer*4*piksel.y + 1;
    int skladowaBlue = 4*piksel.x + szer*4*piksel.y;

    if ((piksel.x>=0)&&(piksel.y>=0)&&(piksel.x<szer)&&(piksel.y<wys))
    {
            ptr[skladowaRed]= color.red();
            ptr[skladowaGreen] = color.green();
            ptr[skladowaBlue] = color.blue();
    }
}

void MyWindow::czysc()
{
    punkt pkt;

    colorCzysc.setRgb(255, 255, 255, 255);

    wektorPunktow.clear();
    ilePunktow = 0;

    for(pkt.y = 0; pkt.y < wys; pkt.y ++)
    {
        for(pkt.x = 0; pkt.x < szer; pkt.x ++)
        {
            zamalujPiksel(pkt, colorCzysc);
        }
    }
}

void MyWindow::rysujOdcinek(punkt &a, punkt &b)
{
    int x, y, yp, xp;
    punkt nowy;
    double m;

    if(a.x == b.x)
    {
        if(a.y > b.y)
        {
            for(y = b.y; y <= a.y; ++ y)
            {
                nowy.x = a.x;
                nowy.y = y;
                zamalujPiksel(nowy, colorRysowania);
            }
        }
        else
        {
            for(y = a.y; y <= b.y; ++ y)
            {
                nowy.x = a.x;
                nowy.y = y;
                zamalujPiksel(nowy, colorRysowania);
            }
        }
    }
    else
    {
        m = (double)(b.y - a.y)/(b.x - a.x);
        if(m < -1 || m > 1)
        {
            if(a.y > b.y)
            {
                for(y = b.y; y <= a.y; y++)
                {
                    x = (y - a.y)/m + a.x;
                    xp = (int)floor(x + 0.5);
                    nowy.x = xp;
                    nowy.y = y;
                    zamalujPiksel(nowy, colorRysowania);
                }
            }
            else
            {
                for(y = a.y; y <= b.y; y++)
                {
                    x = (y - a.y)/m + a.x;
                    xp = (int)floor(x + 0.5);
                    nowy.x = xp;
                    nowy.y = y;
                    zamalujPiksel(nowy, colorRysowania);
                }
            }
        }
        else
        {
            if(a.x > b.x)
            {
                for(x = b.x; x <= a.x; x++)
                {
                    y = m * (x - a.x) + a.y;
                    yp = (int)floor(y + 0.5);
                    nowy.x = x;
                    nowy.y = yp;
                    zamalujPiksel(nowy, colorRysowania);
                }
            }
            else
            {
                for(x = a.x; x <= b.x; x++)
                {
                    y = m * (x - a.x) + a.y;
                    yp = (int)floor(y + 0.5);
                    nowy.x = x;
                    nowy.y = yp;
                    zamalujPiksel(nowy, colorRysowania);
                }
            }
        }
    }
}

void MyWindow::rysujPunktyKwadraty(punkt &P)
{
    punkt pkt;

    for(pkt.x = P.x - 3; pkt.x < P.x + 3; ++ pkt.x)
    {
        for(pkt.y = P.y - 3; pkt.y <P.y + 3; ++ pkt.y)
        {
            zamalujPiksel(pkt, colorRysowania);
        }
    }
}


double MyWindow::kwadratOdleglosciPunktow(punkt &P, punkt &Q)
{
    double kwadratOdleglosci;
    kwadratOdleglosci = (P.x - Q.x)*(P.x - Q.x) + (P.y - Q.y)*(P.y - Q.y);
    return kwadratOdleglosci;
}

bool MyWindow::czyLiniaPrzecinaWielokat(int &y, punkt &startOdcinka, punkt &stopOdcinka){
    punkt wierzcholekGorny;
    punkt wierzcholekDolny;
    if(startOdcinka.y > stopOdcinka.y){
        wierzcholekGorny = startOdcinka;
        wierzcholekDolny = stopOdcinka;
    }
    else{
        wierzcholekGorny = stopOdcinka;
        wierzcholekDolny = startOdcinka;
    }
    if(y <= wierzcholekGorny.y && y > wierzcholekDolny.y)
        return true;
    else
        return false;
}

float MyWindow::wspolczynnikDoXPrzeciecia(punkt &startOdcinka, punkt &stopOdcinka){
    float wynik;
    wynik = 1.0*(stopOdcinka.x - startOdcinka.x)/(stopOdcinka.y - startOdcinka.y);
    return wynik;
}

int MyWindow::xPrzeciecia(int &y, punkt &startOdcinka, punkt &stopOdcinka){
    if(stopOdcinka.x - startOdcinka.x == 0 || stopOdcinka.y - startOdcinka.y == 0){
        if(startOdcinka.x < stopOdcinka.x)
            return startOdcinka.x;
        else
            return stopOdcinka.x;
    }
    else{
        return (startOdcinka.x + (y - startOdcinka.y) * wspolczynnikDoXPrzeciecia(startOdcinka, stopOdcinka));
    }
}

void MyWindow::wielokatScanLine(){
    int yMin, yMax;
    pomocnik = colorRysowania;
    colorRysowania = colorWypelniania;

    for(int i =0; i < ilePunktow - 1; ++ i){
        wektorDoScanLine.push_back(wektorPunktow[i]);
    }
    sort(wektorDoScanLine.begin(), wektorDoScanLine.end());
    yMax = wektorDoScanLine.back().y;
    yMin = wektorDoScanLine.front().y;
    for(int y = yMin; y <= yMax; ++ y){
        for(int i = 0; i < ilePunktow-2; ++ i){
            if(czyLiniaPrzecinaWielokat(y, wektorPunktow[i], wektorPunktow[i+1])){
                wektorPrzeciec.push_back(xPrzeciecia(y, wektorPunktow[i], wektorPunktow[i+1]));
                ilePrzeciec++;
            }
        }
        if(czyLiniaPrzecinaWielokat(y, wektorPunktow[ilePunktow-2], wektorPunktow[0])){
            wektorPrzeciec.push_back(xPrzeciecia(y, wektorPunktow[ilePunktow-2], wektorPunktow[0]));
            ilePrzeciec++;
        }
        sort(wektorPrzeciec.begin(), wektorPrzeciec.end());
        int i = 0;
        if(ilePrzeciec > 1)
        {
            while(i < ilePrzeciec - 1){
                punkt a;
                punkt b;
                a.x = wektorPrzeciec[i];
                a.y = y;
                b.x = wektorPrzeciec[i+1];
                b.y = y;
                rysujOdcinek(a,b);
                i = i + 2;
            }
        }
        wektorPrzeciec.clear();
        ilePrzeciec = 0;
    }
    colorRysowania = pomocnik;
    for(int i =0; i < ilePunktow - 2; ++ i){
        rysujOdcinek(wektorPunktow[i], wektorPunktow[i+1]);
    }
    rysujOdcinek(wektorPunktow[ilePunktow - 2], wektorPunktow[0]);
    wektorDoScanLine.clear();
}

void MyWindow::mousePressEvent(QMouseEvent *event)
{    
    pkt.x = event->x();
    pkt.y = event->y();

    pkt.x -= pocz.x;
    pkt.y -= pocz.y;

    zamalujPiksel(pkt, colorRysowania);
    ilePunktow ++;

    if(wektorPunktow.empty()){
        wektorPunktow.push_back(pkt);
    }
    else{
        if(ilePunktow >=2 && kwadratOdleglosciPunktow(wektorPunktow.front(), pkt) < 750){
            rysujOdcinek(wektorPunktow.back(), wektorPunktow.front());
            wielokatScanLine();
            wektorPunktow.clear();
            ilePunktow = 0;
        }
        else{
            rysujOdcinek(wektorPunktow.back(), pkt);
            wektorPunktow.push_back(pkt);
        }
    }

    update();
}




