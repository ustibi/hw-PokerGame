#include "card.h"
#include <QString>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <string>
#include <thread>
#include <QDebug>

int Card::number_of_status_2 = 0;

Card::Card(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    status = 0;
    setPosition(0, 0);
    setInformation(Spade, Ace);
}

Card::Card(const Card &card)
{
    this->status = card.status;
    this->position = card.position;
    this->target_position = card.target_position;
    this->color = card.color;
    this->number = card.number;
    this->pixmap = card.pixmap;
}

Card::Card(const CardColor color, const CardNumber number,
           const int x, const int y,
           QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    status = 0;
    setInformation(color, number);
    setPosition(x, y);
}

Card::Card(const int color, const int number,
    const int x, const int y,
    QGraphicsItem *parent)
{
    status = 0;
    setInformation(static_cast<CardColor>(color), static_cast<CardNumber>(number));
    setPosition(x, y);
}

void Card::setInformation(const CardColor color, const CardNumber number)
{
    this->color = color;
    this->number = number;
    
    string s = to_string(13 * color + number);
    const QString filename = ":/PokerGame/Resources/images/" + QString(s.data()) + ".png";
    
    QPixmap pic(filename);
    pixmap = pic.scaled(width, height);
}

void Card::setPosition(const int x, const int y)
{
    position.setX(x);
    position.setY(y);
    setPos(position);
}

void Card::setTargetPosition(const int x, const int y)
{
    target_position.setX(x);
    target_position.setY(y);
}

void Card::setTargetPosition(const QPointF point)
{
    target_position.setX(point.x());
    target_position.setY(point.y());
}

void Card::setStatus(const int status)
{
    this->status = status;
}

void Card::setNumberOfStatus2(const int number_of_status2)
{
    Card::number_of_status_2 = number_of_status2;
}

CardColor Card::getColor() const
{
    return color;
}

CardNumber Card::getNumber() const
{
    return number;
}

QPixmap Card::getPixmap() const
{
    return this->pixmap;
}

QPointF Card::getPosition() const
{
    return position;
}

int Card::getStatus() const
{
    return status;
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawPixmap(0, 0, width, height, pixmap);
    painter->restore();
}

QRectF Card::boundingRect() const
{
    QRectF bound(0, 0, width, height);
    return bound;
}

QPainterPath Card::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, width, height);
    return path;
}

const Card &Card::operator=(const Card &card)
{
    this->status = card.status;
    this->position = card.position;
    this->target_position = card.target_position;
    this->color = card.color;
    this->number = card.number;
    this->pixmap = card.pixmap;

    return *this;
}

const bool Card::operator==(const Card &card)
{
    return ((color == card.color) && (number == card.number));
}

bool Card::lessByColor(const Card &c) //返回1表示this小于c(在排序的意义下)
{
    switch (color)
    {
        case Spade: return 0;
        case Heart: return (c.color == Spade);
        case Club: return (c.color != Diamond);
        case Diamond: return 1;
        default: return 0; //异常处理！！！！！！！！！！！之后完成
    };
}

bool Card::lessByNumber(const Card &c)
{
    if (number == Ace)
        return 0;
    else if (c.number == Ace)
        return 1;
    else
        return (number < c.number);
}

Card::~Card()
{
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (status == 1) {
        setPos(target_position);
        status = 2;

        //qDebug() << color << number << status;

        /*char send_card[2] = {color + '0', number + '0'};
        QByteArray datagram(send_card);*/

        number_of_status_2++;
        emit playCard(*this);
    }

    if (number_of_status_2 == 4) {
        //QObject::connect()
        emit numberOfStatus2EqualTo4();
    }
}
