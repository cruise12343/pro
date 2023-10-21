#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <cstdlib>
#include <limits>
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <conio.h>
#define KEY_UP    72
#define KEY_LEFT  75
#define KEY_RIGHT 77
#define KEY_DOWN  80
//#include "Random.h"

// Increase amount of new lines if your board isn't
// at the very bottom of the console
constexpr int g_consoleLines{ 25 };

class Direction
{
public:
    enum Type
    {
        up,
        down,
        left,
        right,
        max_directions,
    };

    Direction(Type type)
        :m_type(type)
    {
    }
    Type getType() const
    {
        return m_type;
    }

    Direction operator-() const
    {
        switch (m_type)
        {
        case up:    return Direction{ down };
        case down:  return Direction{ up };
        case left:  return Direction{ right };
        case right: return Direction{ left };
        }

        assert(0 && "Unsupported direction was passed!");
        return Direction{ up };
    }

    friend std::ostream& operator<<(std::ostream& stream, Direction dir)
    {
        switch (dir.getType())
        {
        case Direction::up:     return (stream << "up");
        case Direction::down:   return (stream << "down");
        case Direction::left:   return (stream << "left");
        case Direction::right:  return (stream << "right");
        default:                return (stream << "unknown direction");
        }
    }

    static Direction getRandomDirection()
    {
        //Type random{ static_cast<Type>(Random::get(0, Type::max_directions - 1)) };
        Type random{ static_cast<Type>((rand() % (Type::max_directions - 1 -0 + 1)) + 0) };
        return Direction{ random };
    }

private:
    Type m_type{};

};

struct Point
{
    int x{};
    int y{};

    friend bool operator==(Point p1, Point p2)
    {
        return p1.x == p2.x && p1.y == p2.y;
    }

    friend bool operator!=(Point p1, Point p2)
    {
        return !(p1 == p2);
    }

    Point getAdjacentPoint(Direction dir) const
    {
        switch (dir.getType())
        {
        case Direction::up:     return Point{ x,     y - 1 };
        case Direction::down:   return Point{ x,     y + 1 };
        case Direction::left:   return Point{ x - 1, y };
        case Direction::right:  return Point{ x + 1, y };
        }

        return *this;
    }
};

namespace UserInput
{
    bool isValidCommand(char ch)
    {
        return ch == KEY_UP
            || ch == KEY_DOWN
            || ch == KEY_LEFT
            || ch == KEY_RIGHT
            || ch == 'q';
    }

    void ignoreLine()
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    char getCharacter()
    {
        char operation{};
        std::cin >> operation;
        ignoreLine(); // remove any extraneous input
        return operation;
    }

    char getCommandFromUser()
    {
        char ch{};
        while (!isValidCommand(ch))
            ch = getch();

        return ch;
    }

    Direction charToDirection(char ch)
    {
        switch (ch)
        {
        case KEY_UP: return Direction{ Direction::up };
        case KEY_DOWN: return Direction{ Direction::down };
        case KEY_LEFT: return Direction{ Direction::left };
        case KEY_RIGHT: return Direction{ Direction::right };
        }

        assert(0 && "Unsupported direction was passed!");
        return Direction{ Direction::up };
    }
};

class Tile
{
public:
    Tile() = default;
    explicit Tile(int number)
        :m_num(number)
    {
    }

    friend std::ostream& operator<<(std::ostream& stream, Tile tile)
    {
        if (tile.m_num > 9) // if two digit number
            stream << " " << tile.m_num << " ";
        else if (tile.m_num > 0) // if one digit number
            stream << "  " << tile.m_num << " ";
        else if (tile.m_num == 0) // if empty spot
            stream << "    ";
        return stream;
    }
    bool isEmpty() const
    {
        return m_num == 0;
    }

    int getNum() const { return m_num; }

private:
    int m_num{};
};

class Board
{
public:

    Board() = default;

    static void printEmptyLines(int count)
    {
        for (int i = 0; i < count; ++i)
            std::cout << '\n';
    }

    friend std::ostream& operator<<(std::ostream& stream, const Board& board)
    {
        // Before drawing always print some empty lines
        // so that only one board appears at a time
        // and it's always shown at the bottom of the window
        // because console window scrolls automatically when there is no
        // enough space.
        printEmptyLines(g_consoleLines);

        for (int y = 0; y < SIZE; ++y)
        {
            for (int x = 0; x < SIZE; ++x)
                stream << board.m_tiles[y][x];
            stream << '\n';
        }

        return stream;
    }

    Point getEmptyTilePos() const
    {
        for (int y = 0; y < SIZE; ++y)
            for (int x = 0; x < SIZE; ++x)
                if (m_tiles[y][x].isEmpty())
                    return { x,y };

        assert(0 && "There is no empty tile in the board!!!");
        return { -1,-1 };
    }

    static bool isValidTilePos(Point pt)
    {
        return (pt.x >= 0 && pt.x < SIZE)
            && (pt.y >= 0 && pt.y < SIZE);
    }

    void swapTiles(Point pt1, Point pt2)
    {
        std::swap(m_tiles[pt1.y][pt1.x], m_tiles[pt2.y][pt2.x]);
    }

    // Compare two boards to see if they are equal
    friend bool operator==(const Board& f1, const Board& f2)
    {
        for (int y = 0; y < SIZE; ++y)
            for (int x = 0; x < SIZE; ++x)
                if (f1.m_tiles[y][x].getNum() != f2.m_tiles[y][x].getNum())
                    return false;

        return true;
    }

    // returns true if user moved successfully
    bool moveTile(Direction dir)
    {
        Point emptyTile{ getEmptyTilePos() };
        Point adj{ emptyTile.getAdjacentPoint(-dir) };

        if (!isValidTilePos(adj))
            return false;

        swapTiles(adj, emptyTile);
        return true;
    }

    bool playerWon() const
    {
        static Board s_solved{};  // generate a solved board
        return s_solved == *this; // player wins if current board == solved board
    }

    void randomize()
    {
        // just move empty tile randomly 1000 times
        // (just like you would do in real life)
        for (int i = 0; i < 1000; ++i)
        {
            bool success = moveTile(Direction::getRandomDirection());
            // If we tried to move in invalid direction, don't count this iteration
            if (!success)
                --i;
        }
    }

private:
    static const int SIZE = 4;
    Tile m_tiles[SIZE][SIZE]{
        Tile{ 1 }, Tile { 2 }, Tile { 3 } , Tile { 4 },
        Tile { 5 } , Tile { 6 }, Tile { 7 }, Tile { 8 },
        Tile { 9 }, Tile { 10 }, Tile { 11 }, Tile { 12 },
        Tile { 13 }, Tile { 14 }, Tile { 15 }, Tile { 0 } };
};

int main()
{
    Board board{};
    board.randomize();
    std::cout << board;

    while (!board.playerWon())
    {
        char ch{ UserInput::getCommandFromUser() };

        // Handle non-direction commands
        if (ch == 'q')
        {
            std::cout << "\n\nBye!\n\n";
            return 0;
        }

        // Handle direction commands
        Direction dir{ UserInput::charToDirection(ch) };

        bool userMoved{ board.moveTile(dir) };
        if (userMoved)
            std::cout << board;
    }

    std::cout << "\n\nYou won!\n\n";
    return 0;
}