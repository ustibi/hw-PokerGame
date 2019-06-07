#pragma once

#include <QGraphicsItem>
#include <QtNetwork/QUdpSocket>
#include "constants.h"
using namespace std;

class Card : public QObject, public QGraphicsItem
{
    Q_OBJECT

private:
    int status;// 0代表不能出牌，1代表可以出牌，2代表已经打出这张牌
    QPointF position;
    QPointF target_position;
    CardColor color;
    CardNumber number;
    QPixmap pixmap;
    QUdpSocket* card_sender;
    QUdpSocket* card_receiver;

    static const int width = 100;
    static const int height = 154;
    static int number_of_status_2;

public:
    Card(QGraphicsItem *parent = NULL);
    Card(const Card &card);
    Card(const CardColor color, const CardNumber number,
         const int x, const int y,
         QGraphicsItem *parent = NULL);
    Card(const int color, const int number,
        const int x = 0, const int y = 0,
        QGraphicsItem *parent = NULL);
    void setInformation(const CardColor color, const CardNumber number);
    void setPosition(const int x, const int y);
    void setTargetPosition(const int x, const int y);
    void setTargetPosition(const QPointF point);
    void setStatus(const int status);
    static void setNumberOfStatus2(const int number_of_status2);
    CardColor getColor() const;
    CardNumber getNumber() const;
    QPixmap getPixmap() const;
    QPointF getPosition() const;
    int getStatus() const;
    static int getNumberOfStatus2();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    const Card &operator=(const Card &card);
    const bool operator==(const Card &card);
    bool lessByColor(const Card &c);
    bool lessByNumber(const Card &c);
    ~Card();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void numberOfStatus2EqualTo4();
    void playCard(const Card &c);
};
