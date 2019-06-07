#include "hand.h"
#include <algorithm>
#include <ctime>

const QPointF Hand::points[4] =
{
    QPointF(180, 10),
    QPointF(800, 100),
    QPointF(180, 485),
    QPointF(170, 100)
};

const QPointF Hand::target_points[4] =
{
    QPointF(365, 170),
    QPointF(630, 250),
    QPointF(365, 330),
    QPointF(340, 250)
};

Hand::Hand()
{
    length = 13;
    cards = new Card[length];
    isVertical = 0;
}

Hand::Hand(int length)
{
    this->length = length;
    cards = new Card[length];
    isVertical = 0;
}

Player Hand::getDirection() const
{
    return direction;
}

void Hand::setDirection(const int direction)
{
    this->direction = static_cast<Player>(direction);
    isVertical = direction % 2;
    setFirstPosition(points[direction]);

    if (isVertical) {
        /*QPointF p[4];
        for (int j = 0; j < 4; j++) {
            p[j].setX(first_position.x());
            p[j].setY(first_position.y() + 60 * j);
        }*/
        for (int i = 0; i < 13; i++) {
            cards[i].setRotation(90);
            cards[i].setPosition(first_position.x(), first_position.y() + 30 * i);
            /*switch (cards[i].getColor())
            {
            case Spade:
                cards[i].setPos(p[0]);
                p[0].setX(p[0].x() + 30);
                break;
            case Heart:
                cards[i].setPos(p[1]);
                p[1].setX(p[1].x() + 30);
                break;
            case Club:
                cards[i].setPos(p[2]);
                p[2].setX(p[2].x() + 30);
                break;
            case Diamond:
                cards[i].setPos(p[3]);
                p[3].setX(p[3].x() + 30);
                break;
            }*/
            cards[i].setTargetPosition(target_points[direction]);
        }
    }
    else
        for (int i = 0; i < 13; i++) {
            cards[i].setPosition(first_position.x() + 30 * i, first_position.y());
            cards[i].setTargetPosition(target_points[direction]);
        }
}

void Hand::setFirstPosition(const int x, const int y)
{
    first_position.setX(x);
    first_position.setY(y);
}

void Hand::setFirstPosition(const QPointF point)
{
    first_position.setX(point.x());
    first_position.setY(point.y());
}

void Hand::setTargetPosition(const int x, const int y)
{
    target_position.setX(x);
    target_position.setY(y);
}

void Hand::setCards(const Card* cards)
{
    for (int i = 0; i < length; i++)
        this->cards[i] = cards[i];
}

void Hand::sortByColor()
{
    for (int i = 0; i < length - 1; i++)
        for (int j = 0; j < length - 1 - i; j++)
            if (cards[j].lessByColor(cards[j + 1]))
                swap(cards[j], cards[j + 1]);
    for (int i = 0; i < length - 1; i++)
        for (int j = 0; j < length - 1 - i; j++)
            if ((cards[j].getColor() == cards[j + 1].getColor()) &&
                (cards[j].lessByNumber(cards[j + 1])))
                swap(cards[j], cards[j + 1]);
}

Card* Hand::getCards() const
{
    return cards;
}

Hand::~Hand()
{
    delete[] cards;
}

void Hand::receiveCard(const Card &c)
{
    /*for (int i = 0; i < length; i++)
        if (cards[i] == c)
            emit playCardOfHand(c, this->direction);*/
    emit playCardOfHand(c, this->direction);
}
