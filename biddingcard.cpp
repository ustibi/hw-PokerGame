#include "biddingcard.h"
#include <string>
#include <QString>
#include <QPainter>

BiddingCard::BiddingCard(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    this->status = 1;

    bidding_card_sender = new QUdpSocket(this);
    bidding_card_receiver = new QUdpSocket(this);
    bidding_card_receiver->bind(31701, QUdpSocket::ShareAddress);
    QObject::connect(bidding_card_receiver, &QUdpSocket::readyRead, this, &BiddingCard::processPendingDatagram);
}

BiddingCard::BiddingCard(const CardColor color, const CardNumber number,
    QGraphicsItem *parent) : QGraphicsItem(parent)
{
    this->status = 1;
    setInformation(color, number);
}

void BiddingCard::setInformation(const CardColor color, const CardNumber number)
{
    this->status = 1;
    this->color = color;
    this->number = number;

    string s;
    if (color != 5)
        s = to_string(color + 5 * number);
    else
        s = to_string(35 + number);

    const QString filename = ":/PokerGame/Resources/biddingcard/" + QString(s.data()) + ".png";

    QPixmap pic(filename);
    pixmap = pic;//.scaled(width, height);
}

void BiddingCard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawPixmap(0, 0, width, height, pixmap);
    painter->restore();
}

QRectF BiddingCard::boundingRect() const
{
    QRectF bound(0, 0, width, height);
    return bound;
}

QPainterPath BiddingCard::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, width, height);
    return path;
}

CardColor BiddingCard::getColor() const
{
    return color;
}

CardNumber BiddingCard::getNumber() const
{
    return number;
}

void BiddingCard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    char card_data[2] = { color + '0', number + '0' };
    QByteArray datagram(card_data);
    bidding_card_sender->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 31701);
    emit playBiddingCard(color, number);
}

void BiddingCard::processPendingDatagram()
{
    while (bidding_card_receiver->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(bidding_card_receiver->pendingDatagramSize());
        bidding_card_receiver->readDatagram(datagram.data(), datagram.size());

        qDebug() << datagram;
    }
}

BiddingCard::~BiddingCard()
{
}
