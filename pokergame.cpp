#include "pokergame.h"
#include "card.h"
#include <thread>
#include <QList>
#include <QDebug>

int PokerGame::number_of_trick = 0;

PokerGame::PokerGame(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    showMaximized();

    /*c = new Card[10];
    for (int i = 0; i < 10; i++) {
        c[i].setInformation(static_cast<enum CardColor>(i % 4), static_cast<enum CardNumber>(i));
        c[i].setPosition(30 * i, 0);
        //c[i].setTargetPosition(200 + 30 * i, 200);
    }*/



    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 830, 650);

    view = new QGraphicsView(this);
    view->setFixedSize(835, 650);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //for (int i = 0; i < 10; i++)
    //    scene->addItem(c + i);
    view->setScene(scene);

    //setCentralWidget(view);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
            deck.addCard(Card(i, j));
    deck.shuffle();

    for (int i = 0; i < 4; i++) {
        player[i].setCards(deck.getCards() + 13 * i);
        player[i].setDirection(i);
        player[i].sortByColor();
        for (int j = 0; j < 13; j++) {
            scene->addItem(player[i].getCards() + j);
            QObject::connect(player[i].getCards() + j, &Card::playCard,
                player + i, &Hand::receiveCard, Qt::DirectConnection);
            QObject::connect(player[i].getCards() + j, &Card::numberOfStatus2EqualTo4,
                this, &PokerGame::status2Invisible, Qt::QueuedConnection);
        }
    }





    //QObject::connect(player[0].getCards(), &Card::mousePressEvent, &central, &CardTemp::receiveCard);
    //QObject::connect(player[0].getCards(), SIGNAL(Card::numberOfStatus2EqualTo4()), this, SLOT(status2Invisible()));

    /*QPixmap pixmap;
    const QString path = ":/PokerGame/Resources/images/";
    string s = to_string(20);
    const QString file = QString(s.data()) + ".png";
    QString filename = path + file;

    if (pixmap.load(filename))
        scene->addText(tr("true"));
    else
        scene->addText(filename);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem();
    item->setPixmap(pixmap);*/
    //scene->addItem(item);

    //scene->addPixmap(*(c.getPixmap()));
    //scene->addText(tr("what"));
    //view->setScene(scene);
    for (int i = 0; i < 13; i++) {
        data[i].leader = '\0';
        for (int j = 0; j < 4; j++)
            data[i].card[j] = -1;
    }
}

QGraphicsView* PokerGame::getView() const
{
    return view;
}

void PokerGame::status2Invisible()
{
    this_thread::sleep_for(1s);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++) {
            if ((player[i].getCards() + j)->getStatus() == 2)
                (player[i].getCards() + j)->setVisible(false);
        }
    Card::setNumberOfStatus2(0);
}

struct trick* PokerGame::getData()
{
    return data;
}

Player PokerGame::char_to_player(char c)
{
    switch (c) {
    case 'N': return North;
    case 'S': return South;
    case 'E': return East;
    case 'W': return West;
    }
}

PokerGame::~PokerGame()
{
    //delete[] c;
}
