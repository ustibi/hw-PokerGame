#pragma once

enum CardColor
{
    Spade,
    Heart,
    Diamond,
    Club,
    NoTrump,
    Other
};

enum CardNumber
{
    Ace, //color == Otherʱ������Double
    Deuce, //color == Otherʱ������redouble
    Three, //color == Otherʱ������pass
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Joker
};

enum Player
{
    North,
    East,
    South,
    West
};

