#include "dashboard.h"
#include "ui_dashboard.h"
#include <QPainter>
#include <QPixmap>
#include "math.h"
Dashboard::Dashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dashboard)
{
    ui->setupUi(this);
    m_nRadius = 100;
    m_nAngle = 90;
    m_nSpeed = 0;
    m_pTimer = new QTimer(this);

    connect(this,&Dashboard::sigSendDataValue, this, &Dashboard::slotValueChanged);
    // 从这里开始，每隔2s产生一个0-120 范围内的随机数
    connect(m_pTimer,&QTimer::timeout,this, &Dashboard::slotUpdate);
    m_pTimer->start(2000);
}

void Dashboard::paintEvent(QPaintEvent *)
{
    int width = this->width();
    int height = this->height();
    QPainter painter(this) ;
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.translate(width / 2 + 25, height / 2 + 4);
    int side = qMin(width, height);
    painter.scale(side / 200.0, side / 200.0);
    drawBackground(&painter);
    drawIndicate(&painter);
    drawText(&painter);
}

// 绘制指示器(仪表盘指针)
void Dashboard::drawIndicate(QPainter *painter)
{
    static const QPoint indicateHand[4] = {
        QPoint(-2, 0),
        QPoint(0, 50),
        QPoint(2, 0),
        QPoint(0, -5)
    };  
    double degRotate = m_nAngle + (330 - m_nAngle) / 120 * m_nSpeed;
    QColor indicateColor(54, 123, 183, 255);
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(indicateColor);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(indicateHand, 4);
    painter->restore();
}

void Dashboard::drawBackground(QPainter *painter)
{
    painter = new QPainter(this);
    QPixmap pic = QPixmap(":/bg.jpg");
    //pic = pic.scaled(1204,1191);
    painter->drawPixmap(0,0,this->width(),this->height(),pic);
}

void Dashboard::drawText (QPainter *painter){
    painter->save() ;
    painter->setFont(QFont("Arial",11,QFont::Bold,true));
    painter->setPen(QColor(220,220, 220));
    QFontMetricsF fontMet = QFontMetricsF(painter->font());
    QString strSpeed = QString::number(m_nSpeed) + "MPH";
    int w = static_cast<int>(fontMet.width(strSpeed));
    painter->drawText(QPoint(-w/2 - 20, static_cast<int>(0.5 * m_nRadius - 5)),strSpeed);

    painter->setFont(QFont("Arial",8,QFont::Bold,true));
    painter->setPen(QColor(54, 123, 183));
    QString strSpeed_km = QString::number(m_nSpeed * 1.6) + "km/h";
    painter->drawText(QPoint(-w/2 - 8, static_cast<int>(0.5 * m_nRadius - 20)),strSpeed_km);
    painter->restore();
}

void Dashboard::slotValueChanged(int nValue)
{
    m_nSpeed = nValue;
    update();
}

void Dashboard::slotUpdate()
{
    emit sigSendDataValue(qrand() % 121);
}

Dashboard::~Dashboard()
{
    delete ui;
}
