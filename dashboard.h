#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QTimer>
namespace Ui {
class Dashboard;
}

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard() override;

    void drawIndicate(QPainter *painter);
    void drawBackground(QPainter *painter);
    void drawText(QPainter *painter);

   signals:
    void sigSendDataValue(int) ;
    public slots:
    void slotValueChanged(int nValue) ;
    void slotUpdate() ;
    protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::Dashboard *ui;
    int m_nRadius;
    int m_nAngle;
    int m_nSpeed;
    QTimer *m_pTimer ;
};

#endif // DASHBOARD_H
