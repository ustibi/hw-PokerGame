#pragma once

#include "card.h"

class CardTemp// : public QGraphicsItem
{
private:
    int capacity;
    int card_number;
    Card* first_card;

public:
    CardTemp();
    CardTemp(int n);
    CardTemp(const CardTemp &card_temp);
    void addCard(const Card &c);
    void shuffle(const int seed = 0);
    Card* getCards() const;
    //QRectF boundingRect() const;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    ~CardTemp();

/*public slots:
    void receiveCard(const Card &c);
    void clear();*/
};

