#include "bridgegame.h"
#include <stdlib.h>
#include <QDebug>
#include <algorithm>

int BridgeGame::table_row = 1;
int BridgeGame::table_column = 1;

BridgeGame::BridgeGame(QWidget *parent)
    : PokerGame(parent)
{
    dealer = North;
    table_column = dealer;
    ns_trick_number = 0;
    ew_trick_number = 0;

    for (int i = 0; i < 4; i++)
        connect(player + i, &Hand::playCardOfHand, this, &BridgeGame::setCardsStatus, Qt::DirectConnection);

    scene2 = new QGraphicsScene(this);
    scene2->setSceneRect(0, 0, 260, 410);
    contract = new QLabel;
    contract->setVisible(false);
    contract->setFixedSize(263, 20);
    scene2->addWidget(contract);

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 7; j++) {
            bidding_card[i + 5 * j].setInformation(static_cast<CardColor>(i), static_cast<CardNumber>(j));
            bidding_card[i + 5 * j].setPos(210 - 50 * i, 50 * j + 10);
            scene2->addItem(bidding_card + (i + 5 * j));
            connect(bidding_card + i + 5 * j, &BiddingCard::playBiddingCard, this, &BridgeGame::setBiddingCardsInvisible);
        }
    for (int j = 0; j < 3; j++) {
        bidding_card[35 + j].setInformation(static_cast<CardColor>(5), static_cast<CardNumber>(j));
        bidding_card[35 + j].setPos(210 - 50 * j, 360);
        scene2->addItem(bidding_card + (35 + j));
        connect(bidding_card + 35 + j, &BiddingCard::playBiddingCard, this, &BridgeGame::setBiddingCardsInvisible);
    }
    bidding_card[35].setFocusProxy(false);
    bidding_card[36].setFocusProxy(false);
    bidding_card[35].setVisible(false);
    bidding_card[36].setVisible(false);

    view2 = new QGraphicsView(this);
    view2->setFixedSize(265, 413);
    view2->setScene(scene2);
    view2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    table_widget = new QTableWidget;
    table_widget->setColumnCount(4);
    table_widget->setRowCount(6);

    QList<QString> column_headers = { "N", "E", "S", "W" };
    QStringList horizontal_headers(column_headers);
    table_widget->setHorizontalHeaderLabels(horizontal_headers);
    table_widget->verticalHeader()->hide();

    table_widget->setFocusPolicy(Qt::NoFocus);
    table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_widget->setIconSize(QSize(60, 60));

    /*scene3 = new QGraphicsScene(this);
    scene3->setSceneRect(0, 0, 260, 240);
    scene3->addWidget(table_widget);

    view3 = new QGraphicsView(this);
    view3->setFixedSize(260, 240);
    view3->setScene(scene3);*/

    layout2 = new QVBoxLayout();
    layout2->addWidget(table_widget);
    layout2->addWidget(view2);

    layout = new QHBoxLayout();
    //layout->addWidget(this->getView());
    layout->addWidget(view);
    layout->addLayout(layout2);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);

    connect(this, &BridgeGame::biddingCardChoosed, this, &BridgeGame::setBiddingCardInTable);
    connect(this, &BridgeGame::contractChoosed, this, &BridgeGame::setContractInformation);

}

void BridgeGame::setCardsStatus(const Card &c, const Player direction)
{
    bool flag1 = true;
    bool flag2 = true;
    for (int i = 0; i < 4; i++)
        if (data[number_of_trick].card[i] == -1) {
            flag1 = false;
            data[number_of_trick].card[i] = c.getColor() * 13 + c.getNumber();
            if (i == 3)
                flag2 = false;
            break;
        }
    if (flag1) {
        char int_to_player[4] =
        {
            'N',
            'E',
            'S',
            'W'
        };
        number_of_trick++;
        data[number_of_trick].leader = int_to_player[direction];
        data[number_of_trick].card[0] = c.getColor() * 13 + c.getNumber();
    }

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
            if ((player[i].getCards() + j)->getStatus() != 2)
                (player[i].getCards() + j)->setStatus(0);

    Player next_direction;
    if (flag2) {
        if (direction == West)
            next_direction = North;
        else
            next_direction = static_cast<Player>(direction + 1);
        //qDebug() << "Next direction" << next_direction;

        for (int j = 0; j < 13; j++) {
            //qDebug() << j << (player[next_direction].getCards() + j)->getStatus();
            if ((player[next_direction].getCards() + j)->getStatus() != 2)
                (player[next_direction].getCards() + j)->setStatus(1);
        }

        for (int j = 0; j < 13; j++)
            if (((player[next_direction].getCards() + j)->getColor() == (static_cast<CardColor>(data[number_of_trick].card[0] / 13))) &&
                ((player[next_direction].getCards() + j)->getStatus() != 2)) {
                //qDebug() << j << "run" << (player[next_direction].getCards() + j)->getStatus();
                for (int k = 0; k < 13; k++)
                    if (((player[next_direction].getCards() + k)->getColor() != (static_cast<CardColor>(data[number_of_trick].card[0] / 13))) &&
                        ((player[next_direction].getCards() + k)->getStatus() != 2))
                        (player[next_direction].getCards() + k)->setStatus(0);
                break;
            }
    }
    else {
        //判断下个方向
        next_direction = setTrick();
        //next_direction = static_cast<Player>(max_element(data[number_of_trick].card, (data[number_of_trick].card + 4)) - data[number_of_trick].card);
        //写函数实现
        for (int j = 0; j < 13; j++) {
            //qDebug() << j << (player[next_direction].getCards() + j)->getStatus();
            if ((player[next_direction].getCards() + j)->getStatus() != 2)
                (player[next_direction].getCards() + j)->setStatus(1);
        }
    }
}

void BridgeGame::setBiddingCardsInvisible(const CardColor color, const CardNumber number)
{/*
    CardColor color = card.getColor();
    CardNumber number = card.getNumber();*/
    if (color < 5) { // 收到信号，将较小的叫牌卡设为不可见
        number_of_pass = 0;
        double_flag = 0;
        is_all_pass = 0;
        bidding_card[35].setFocusProxy(bidding_card + 35);
        bidding_card[35].setVisible(true);
        bidding_card[36].setFocusProxy(false);
        bidding_card[36].setVisible(false);
        for (int j = 0; j < number; j++)
            for (int i = 0; i < 5; i++) {
                bidding_card[i + 5 * j].setVisible(false);
                bidding_card[i + 5 * j].setFocusProxy(false);
            }
        for (int i = 0; i <= color; i++) {
            bidding_card[i + 5 * number].setVisible(false);
            bidding_card[i + 5 * number].setFocusProxy(false);
        }
    }
    else {
        if (number == 1) { // redouble
            redouble_flag = 1;
            bidding_card[35].setFocusProxy(false);
            bidding_card[35].setVisible(false);
            bidding_card[36].setFocusProxy(false);
            bidding_card[36].setVisible(false);
        }
        else if (number == 2) { // pass
            number_of_pass++;
            if (number_of_pass == 1) {
                bidding_card[35].setFocusProxy(false);
                bidding_card[35].setVisible(false);
                if (double_flag == 1) {
                    bidding_card[36].setFocusProxy(false);
                    bidding_card[36].setVisible(false);
                }
            }
            else if (number_of_pass == 2) {
                if (double_flag == 1) {
                    double_flag = 0;
                    bidding_card[36].setFocusProxy(bidding_card + 36);
                    bidding_card[36].setVisible(true);
                }
                else if (is_all_pass == 0) {
                    bidding_card[35].setFocusProxy(bidding_card + 35);
                    bidding_card[35].setVisible(true);
                }
                if (redouble_flag == 1) {
                    bidding_card[35].setFocusProxy(false);
                    bidding_card[35].setVisible(false);
                    bidding_card[36].setFocusProxy(false);
                    bidding_card[36].setVisible(false);
                }
            }
            else if (number_of_pass == 3) {
                if (is_all_pass == 0) {
                    for (int i = 0; i < 5; i++)
                        for (int j = 0; j < 7; j++) {
                            bidding_card[i + 5 * j].setFocusProxy(false);
                            bidding_card[i + 5 * j].setVisible(false);
                        }
                    for (int j = 0; j < 3; j++) {
                        bidding_card[35 + j].setFocusProxy(false);
                        bidding_card[35 + j].setVisible(false);
                    }
                    emit contractChoosed();
                }
            }
            else {
                for (int i = 0; i < 5; i++)
                    for (int j = 0; j < 7; j++) {
                        bidding_card[i + 5 * j].setFocusProxy(false);
                        bidding_card[i + 5 * j].setVisible(false);
                    }
                for (int j = 0; j < 3; j++) {
                    bidding_card[35 + j].setFocusProxy(false);
                    bidding_card[35 + j].setVisible(false);
                }
                emit contractChoosed();
            }
        }
        else {
            number_of_pass = 0;
            bidding_card[35].setFocusProxy(false);
            bidding_card[36].setFocusProxy(false);
            bidding_card[35].setVisible(false);
            bidding_card[36].setVisible(false);
            if (number == 0) { // double
                double_flag = 1;
                redouble_flag = 0;
                bidding_card[36].setFocusProxy(bidding_card + 36);
                bidding_card[36].setVisible(true);
            }
        }
    }
    emit biddingCardChoosed(color, number); // 发出信号，设置叫牌记录表
}

void BridgeGame::setBiddingCardInTable(CardColor color, CardNumber number) // 设置叫牌记录表
{
    QTableWidgetItem *item = new QTableWidgetItem;

    string s;
    if (color != 5)
        s = to_string(color + 5 * number);
    else
        s = to_string(35 + number);

    QString name = QString(s.data());
    item->setWhatsThis(name);
    const QString filename = ":/PokerGame/Resources/biddingcard/" + name + "-.png";
    QPixmap pic(filename);
    QIcon icon(pic);
    item->setIcon(icon);

    if (table_row == table_widget->rowCount() - 1) // 检查列表行数是否能够容纳叫牌流程
        table_widget->setRowCount(table_widget->rowCount() + 1);

    table_widget->setItem(table_row, table_column, item);
    if (table_column < 4)
        table_column++;
    else {
        table_column = 1;
        table_row++;
    }

}

void BridgeGame::setContractInformation()
{
    //QString contract_text = table_widget->item(table_row - 1, table_column)->whatsThis();
    //QString contract_text_in_number;
    QString contract_text;
    //QString declarer_text;
    /*if (table_row == 1)
        contract_text_in_number = table_widget->item(table_row, table_column - 3)->whatsThis();
    else
        contract_text_in_number = table_widget->item(table_row - 1, table_column)->whatsThis();*/

        //if (contract_text_in_number == QString("37"))
    string int_to_color[5] =
    {
        "C",
        "D",
        "H",
        "S",
        "NT"
    };

    char int_to_player[4] =
    {
        'N',
        'E',
        'S',
        'W'
    };

    if (is_all_pass == 1)
        contract_text = QString(tr("All Pass"));
    else {
        double_flag = 0;
        redouble_flag = 0;
        QString it;
        int column = table_column;
        int row = table_row;
        //it = table_widget->item(table_row, table_column - 1)->whatsThis();
        while (1) {
            if (column == 0) {
                column = 3;
                row--;
            }
            else
                column--;

            it = table_widget->item(row, column)->whatsThis();
            if (atoi(it.toStdString().data()) == 35)
                double_flag = 1;
            if (atoi(it.toStdString().data()) == 36)
                redouble_flag = 1;

            if (atoi(it.toStdString().data()) < 35)
                break;
        };

        /*qDebug() << double_flag;
        qDebug() << redouble_flag;*/
        string str = it.toStdString();
        int n = atoi(str.data());
        // qDebug() << n;
        contract_number = static_cast<CardNumber>(n / 5);
        contract_color = static_cast<CardColor>(n % 5);
        // qDebug() << n << contract_number << contract_color;
        //contract_text = QString(contract_number + '1') + QString(int_to_color[contract_color].data());

        if (redouble_flag == true)
            contract_text == contract_text.append(QString("XX"));
        else if (double_flag == true)
            contract_text == contract_text.append(QString('X'));

        column = -1;
        row = 1;
        while (1) {
            if (column == 3) {
                column = 0;
                row++;
            }
            else
                column++;

            it = table_widget->item(row, column)->whatsThis();
            if ((double_flag == 1) && (redouble_flag == 0)) {
                if ((atoi(it.toStdString().data()) % 5 == contract_color) &&
                    (atoi(it.toStdString().data()) <= 35) &&
                    ((column == table_column % 4) || (column == ((table_column + 2) % 4)))) {
                    declarer = static_cast<Player>(column);
                    /*contract_text = "Declarer: " + QString(int_to_player[declarer])
                        + "  Contract:" + contract_text
                        + " | NS:" + "0" + " | EW:" + "0";*/
                    contract_text = setContractText();
                    break;
                }
            }
            else {
                if ((atoi(it.toStdString().data()) % 5 == contract_color) &&
                    (atoi(it.toStdString().data()) <= 35) &&
                    ((column == (table_column + 1) % 4) || (column == ((table_column + 3) % 4)))) {
                    declarer = static_cast<Player>(column);
                    /*contract_text = "Declarer: " + QString(int_to_player[declarer])
                        + "  Contract:" + contract_text
                        + " | NS:" + "0" + " | EW:" + "0";*/
                    contract_text = setContractText();
                    break;
                }
            }
        };
        data[0].leader = int_to_player[declarer + 1];
        for (int i = 0; i < 13; i++)
            (player[(declarer + 1) % 4].getCards() + i)->setStatus(1);
    }
    contract->setText(contract_text);
    contract->setVisible(true);
}

Player BridgeGame::setTrick()
{
    Player winner = static_cast<Player>(char_to_player(data[number_of_trick].leader));
    if (contract_color == 4) {
        CardColor main_color = static_cast<CardColor>(data[number_of_trick].card[0] / 13);
        CardNumber max_number = static_cast<CardNumber>(data[number_of_trick].card[0] % 13);
        if (max_number == 0)
            max_number = static_cast <CardNumber>(100); // 很大的数
        for (int i = 1; i < 4; i++) {
            if (((data[number_of_trick].card[i] / 13) == main_color) &&
                (((data[number_of_trick].card[i] % 13) > max_number) || ((data[number_of_trick].card[i] % 13) == 0))) {
                if ((data[number_of_trick].card[i] % 13) != 0)
                    max_number = static_cast<CardNumber>(data[number_of_trick].card[i] % 13);
                else
                    max_number = static_cast <CardNumber>(100); // 很大的数
                winner = static_cast<Player>((char_to_player(data[number_of_trick].leader) + i) % 4);
            }
        }
    }
    else {
        CardColor trump_color = static_cast<CardColor>(3 - contract_color);
        CardColor main_color = static_cast<CardColor>(data[number_of_trick].card[0] / 13);
        CardNumber max_number = static_cast<CardNumber>(data[number_of_trick].card[0] % 13);
        bool flag3 = true;
        if (max_number == 0)
            max_number = static_cast<CardNumber>(100); // 很大的数
        for (int i = 1; i < 4; i++) {
            if (((data[number_of_trick].card[i] / 13) == main_color) &&
                (((data[number_of_trick].card[i] % 13) > max_number) || ((data[number_of_trick].card[i] % 13) == 0))) {
                if ((data[number_of_trick].card[i] % 13) != 0)
                    max_number = static_cast<CardNumber>(data[number_of_trick].card[i] % 13);
                else
                    max_number = static_cast<CardNumber>(100); // 很大的数
                winner = static_cast<Player>((char_to_player(data[number_of_trick].leader) + i) % 4);
            }
            else if (((data[number_of_trick].card[i] / 13) == trump_color) && (flag3 == 1)) {
                flag3 = 0;
                main_color = trump_color;
                max_number = static_cast<CardNumber>(data[number_of_trick].card[i] % 13);
                if (max_number == 0)
                    max_number = static_cast <CardNumber>(100); // 很大的数
                winner = static_cast<Player>((char_to_player(data[number_of_trick].leader) + i) % 4);
            }
        }
    }

    if (winner % 2)
        ew_trick_number++;
    else
        ns_trick_number++;
    contract->setText(setContractText());
    return winner;
}

QString BridgeGame::setContractText()
{
    string int_to_color[5] =
    {
        "C",
        "D",
        "H",
        "S",
        "NT"
    };
    char int_to_player[4] =
    {
        'N',
        'E',
        'S',
        'W'
    };

    QString contract_text;
    contract_text = "Declarer: " + QString(int_to_player[declarer])
        + "  Contract:" + QString(contract_number + '1') + QString(int_to_color[contract_color].data())
        + " | NS:" + QString(ns_trick_number + '0') + " | EW:" + QString(ew_trick_number + '0');

    return contract_text;
}

BridgeGame::~BridgeGame()
{
}
