#pragma once

#include <QtWidgets/QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "ui_pokergame.h"
#include "hand.h"
#include "cardtemp.h"

struct trick {
    char leader; // 'N', 'E', 'S', 'W'
    int card[4];
};

class PokerGame : public QMainWindow
{
    Q_OBJECT

public:
    PokerGame(QWidget *parent = Q_NULLPTR);
    ~PokerGame();
    QGraphicsView* getView() const;
    void status2Invisible();
    struct trick* getData();
    static int number_of_trick;
    Player char_to_player(char c);
    QUdpSocket *poker_game_sender;
    QUdpSocket *poker_game_receiver;

protected:
    Ui::PokerGameClass ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    Hand player[4];
    //Card* c;
    CardTemp deck = CardTemp(52);
    CardTemp central = CardTemp(4);
    struct trick data[13];
};
