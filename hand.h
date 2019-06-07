#pragma once

#include "card.h"

class Hand : public QObject
{
    Q_OBJECT
private:
    Player direction;
    QPointF first_position;
    QPointF target_position;
    Card* cards;
    int length;
    bool isVertical;

    const static QPointF points[4];
    const static QPointF target_points[4];

public:
    Hand();
    Hand(int length);
    //Hand()
    Player getDirection() const;
    void setDirection(const int direction);
    void setFirstPosition(const int x, const int y);
    void setFirstPosition(const QPointF point);
    void setTargetPosition(const int x, const int y);
    void setCards(const Card* cards);
    void sortByColor();
    Card* getCards() const;
    ~Hand();
    void receiveCard(const Card &c);

signals:
    void playCardOfHand(const Card &c, const Player direction);
};

