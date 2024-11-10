#include <iostream>
#include <array>
#include <random>
#include <ctime>

const int maxScore = 21;
const int dealerLimit = 17;
bool win = false;

enum class CardRank
{
    rank_2,
    rank_3,
    rank_4,
    rank_5,
    rank_6,
    rank_7,
    rank_8,
    rank_9,
    rank_10,
    rank_jack,
    rank_queen,
    rank_king,
    rank_ace,

    max_rank
};

enum class CardSuit
{
    suit_club,
    suit_diamond,
    suit_heart,
    suit_spade,

    max_suits
};

struct Card 
{
    CardRank rank{};
    CardSuit suit{};
};

void printCard(const Card& card)
{
    switch (card.rank)
    {
    case CardRank::rank_2:
        std::cout << '2';
        break;
    case CardRank::rank_3:
        std::cout << '3';
        break;
    case CardRank::rank_4:
        std::cout << '4';
        break;
    case CardRank::rank_5:
        std::cout << '5';
        break;
    case CardRank::rank_6:
        std::cout << '6';
        break;
    case CardRank::rank_7:
        std::cout << '7';
        break;
    case CardRank::rank_8:
        std::cout << '8';
        break;
    case CardRank::rank_9:
        std::cout << '9';
        break;
    case CardRank::rank_10:
        std::cout << 'T';
        break;
    case CardRank::rank_jack:
        std::cout << 'J';
        break;
    case CardRank::rank_queen:
        std::cout << 'Q';
        break;
    case CardRank::rank_king:
        std::cout << 'K';
        break;
    case CardRank::rank_ace:
        std::cout << 'A';
        break;
    default:
        std::cout << '?';
        break;
    }


    switch (card.suit)
    {
    case CardSuit::suit_club:
        std::cout << 'C';
        break;
    case CardSuit::suit_diamond:
        std::cout << 'D';
        break;
    case CardSuit::suit_heart:
        std::cout << 'H';
        break;
    case CardSuit::suit_spade:
        std::cout << 'S';
        break;
    default:
        std::cout << '?';
        break;
    }
}

using deck_type = std::array<Card, 52>;

deck_type createDeck()
{
    deck_type deck{};
    deck_type::size_type card = 0;

    for (int i = 0; i < (int)CardSuit::max_suits; ++i)
    {
        for (int j = 0; j < (int)CardRank::max_rank; ++j)
        {
            deck[card].rank = static_cast<CardRank>(j);
            deck[card].suit = static_cast<CardSuit>(i);
            ++card;
        }
    }

    return deck;
}

void printDeck(const deck_type& deck)
{
    for (Card card : deck)
    {
        printCard(card);
        std::cout << ' ';
    }
    std::cout << '\n';
}

void shuffleDeck(deck_type& deck)
{
    static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

    std::shuffle(deck.begin(), deck.end(), mt);
}

int getCardValue(const Card& card)
{
    if (card.rank <= CardRank::rank_10)
    {
        return (static_cast<int>(card.rank) + 2);
    }


    switch (card.rank)
    {
    case CardRank::rank_jack:
    case CardRank::rank_queen:
    case CardRank::rank_king:
        return 10;
    case CardRank::rank_ace:
        return 11;
    default:
        return 0;
    }
}

bool isPlayerHitting()
{
    char c{};
    while (c != 'h' && c != 's')
    {
        std::cout << "Hit or Stand? (h/s): ";
        std::cin >> c;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    switch (c)
    {
    case 'h':
        return true;
    case 's':
        return false;
    default:
        return false;
    }
}

int playerHits(int playerScore, const deck_type& deck, deck_type::size_type cardIndex)
{
    int tempscore = 0;
    if (playerScore > maxScore)
    {
        std::cout << "You Busted!\n";
    }
    else
    {
        tempscore = getCardValue(deck[cardIndex++]);
        std::cout << "Your score is: " << playerScore + tempscore << '\n';
    }

    return tempscore;
}

int dealersTurn(int score, const deck_type& deck, deck_type::size_type cardIndex)
{
    int tempScore = 0;
    tempScore = getCardValue(deck[cardIndex]);
    std::cout << "Dealers score is: " << score + tempScore << '\n';
    if (tempScore + score > maxScore)
    {
        std::cout << "Dealer's busted!" << '\n';
    }
    return tempScore;
}

bool scoreOverflowed(int score)
{
    if (score > maxScore)
    {
        return true;
    }
    return false;
}

bool playBlackJack(const deck_type& deck)
{
    deck_type::size_type cardIndex = 0;

    int dealerScore = getCardValue(deck[cardIndex++]);
    std::cout << "Dealers score is: " << dealerScore << '\n';

    int playerScore = getCardValue(deck[cardIndex]) + getCardValue(deck[cardIndex + 1]);
    cardIndex += 2;
    std::cout << "Your score is: " << playerScore << '\n';

    while (true)
    {
        if (!isPlayerHitting())
        {
            while (true)
            {
                if (dealerScore < dealerLimit)
                {
                    dealerScore += dealersTurn(dealerScore, deck, cardIndex);
                    cardIndex++;

                    if (scoreOverflowed(dealerScore))
                    {
                        return true;
                    }
                }
                else
                {
                    return playerScore > dealerScore;
                }
            }
        }
        else
        {
            playerScore += playerHits(playerScore, deck, cardIndex);
            cardIndex++;
            if (scoreOverflowed(playerScore))
            {
                return false;
            }

            if (dealerScore < dealerLimit)
            {
                dealerScore += dealersTurn(dealerScore, deck, cardIndex);
                cardIndex++;

                if (scoreOverflowed(dealerScore))
                {
                    return true;
                }
            }
        }
    }
}

void printResult(bool winner)
{
    if (winner)
    {
        std::cout << "You won!" << '\n';
    }
    else
    {
        std::cout << "You lost!" << '\n';
    }
}

int main()
{
    auto deck = createDeck();
    shuffleDeck(deck);
    //printDeck(deck);
    //playBlackJack(deck);
    printResult(playBlackJack(deck));
}
