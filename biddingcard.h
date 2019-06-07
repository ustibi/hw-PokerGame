#pragma once

#include <QGraphicsItem>
#include <QtNetwork/QUdpSocket>
#include "constants.h"
using namespace std;

class BiddingCard : public QObject, public QGraphicsItem
{
    Q_OBJECT

private:
    int status;// 0代表不能出牌，1代表可以出牌，2代表已经打出这张牌
    QPointF position;
    Player player;
    CardColor color;
    CardNumber number;
    QPixmap pixmap;
    QUdpSocket *bidding_card_sender;
    QUdpSocket *bidding_card_receiver;

    static const int width = 40;
    static const int height = 40;

public:
    BiddingCard(QGraphicsItem *parent = Q_NULLPTR);
    BiddingCard(const CardColor color, const CardNumber number,
        QGraphicsItem *parent = Q_NULLPTR);
    void setInformation(const CardColor color, const CardNumber number);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    CardColor getColor() const;
    CardNumber getNumber() const;
    ~BiddingCard();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void processPendingDatagram();

signals:
    void playBiddingCard(const CardColor color, const CardNumber number);
};
