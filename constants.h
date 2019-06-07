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
    Ace, //color == Other时，代表Double
    Deuce, //color == Other时，代表redouble
    Three, //color == Other时，代表pass
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

