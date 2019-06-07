#include "cardtemp.h"

#include <cstdlib>
#include <ctime>
#include <algorithm>

CardTemp::CardTemp()
{
    capacity = 1;
    card_number = 0;
    first_card = new Card[capacity];
}

CardTemp::CardTemp(int n)
{
    capacity = n;
    card_number = 0;
    first_card = new Card[capacity];
}

CardTemp::CardTemp(const CardTemp &card_temp)
{
    capacity = card_temp.capacity;
    card_number = card_temp.capacity;
    first_card = new Card[capacity];
    for (int i = 0; i < capacity; i++)
        first_card[i] = card_temp.first_card[i];
}

void CardTemp::addCard(const Card &c)
{
    first_card[card_number++] = c;
}

void CardTemp::shuffle(const int seed)
{
    int randomNumber = 0;
    if (seed)
        srand(seed);
    else
        srand(time(NULL));
    for (int i = capacity - 1; i > 0; i--) {
        randomNumber = rand() % i;
        swap(first_card[i], first_card[randomNumber]);
    }
}

Card* CardTemp::getCards() const
{
    return first_card;
}

CardTemp::~CardTemp()
{
    delete[] first_card;
}

/*void CardTemp::receiveCard(const Card &c)
{
    first_card[card_number++] = c;
}

void CardTemp::clear()
{
    delete[] first_card;
    first_card = new Card[capacity];
    card_number = 0;
}
*/
