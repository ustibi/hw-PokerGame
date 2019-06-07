#pragma once
#include "pokergame.h"
#include "biddingcard.h"
#include <QHBoxLayout>
#include <QTableWidget>
#include <QLabel>

class BridgeGame :
    public PokerGame
{
    Q_OBJECT
private:
    QGraphicsScene *scene2;
    QGraphicsView *view2;
    /*QGraphicsScene *scene3;
    QGraphicsView *view3;*/
    QTableWidget *table_widget;
    BiddingCard bidding_card[38];
    QHBoxLayout *layout;
    QVBoxLayout *layout2;

    int number_of_pass = 0;
    bool double_flag = 0;
    bool is_all_pass = 1;
    bool redouble_flag = 0;
    Player dealer;
    CardColor contract_color = Spade;
    CardNumber contract_number = Ace;
    Player declarer;
    QLabel *contract;
    int ns_trick_number;
    int ew_trick_number;

    static int table_row;
    static int table_column;

public:
    BridgeGame(QWidget *parent = Q_NULLPTR);
    void setCardsStatus(const Card &c, const Player hand);
    void setBiddingCardsInvisible(const CardColor color, const CardNumber number); //player—°‘Ò“ª’≈BiddingCard
    void setBiddingCardInTable(CardColor color, CardNumber number);
    void setContractInformation();
    QString setContractText();
    Player setTrick();
    ~BridgeGame();

signals:
    void biddingCardChoosed(CardColor color, CardNumber number);
    void contractChoosed();
};

