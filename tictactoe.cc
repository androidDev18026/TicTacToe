#include <iostream>
#include <string>
#include <sstream>

enum Move
{
    EMPTY = ' ',
    CROSS = 'X',
    CIRCLE = 'O'
};

class Board
{
private:
    Board() = default;
    char slots[9] = {Move::EMPTY, Move::EMPTY, Move::EMPTY, Move::EMPTY, Move::EMPTY, Move::EMPTY,
                     Move::EMPTY, Move::EMPTY, Move::EMPTY};

public:
    Board(const Board &) = delete;
    static Board &GetBoard()
    {
        static Board board;
        return board;
    }
    ~Board() { std::cout << "Deleted board instance" << std::endl; }

    constexpr void Draw() const
    {
        const char *pattern = "\
+---+---+---+\n\
+ %c | %c | %c +\n\
+---+---+---+\n\
+ %c | %c | %c +\n\
+---+---+---+\n\
+ %c | %c | %c +\n\
+---+---+---+\n";

        printf(pattern, slots[0], slots[1], slots[2], slots[3], slots[4], slots[5],
               slots[6], slots[7], slots[8]);
    }

    bool SetSlot(const unsigned index, char move)
    {
        if (index < 9 && slots[index] == Move::EMPTY && (move == Move::CROSS || move == Move::CIRCLE))
        {
            slots[index] = move;
            return true;
        }
        return false;
    }

    const char& GetSlot(unsigned idx) const { return slots[idx]; }
};

class Player
{
private:
    std::string name;
    char color;

public:
    Player(const Player &) = delete;
    Player() = delete;
    Player(std::string n, char c)
        : name(n), color(c) { printf("New Player [%s]\n", n.c_str()); }

    ~Player() { printf("Deleted Player [%s]\n", name.c_str()); }

    const char &GetColor() const { return color; }
    void SetColor(const char &c) { color = c; }

    bool operator==(const Player &other) { return other.color == this->color; }

    void MakeMove() const
    {
        unsigned row, col;
        printf("\n[%s (%c)] Enter move: ", name.c_str(), color);
        std::string line;
        std::getline(std::cin, line);
        std::stringstream ss(line);
        
        if (ss.good())
        {
            ss >> row >> col;

            bool move = Board::GetBoard().SetSlot((row - 1) * 3 + (col - 1), color);
            if (move)
            {
                // printf("[%s] Added %c to Board\n",name.c_str(),color);
                Board::GetBoard().Draw();
            }
            else
            {
                std::cout << "Invalid Move!" << std::endl;
                MakeMove();
            }
        }
    }
};

struct TicTacToe
{
    Player* p1, *p2;
    static short moves;

    TicTacToe(Player *player1, Player *player2)
        : p1(player1), p2(player2)
    {
        printf("Initializing new Tic Tac Toe Game!\n\n");
        printf("Player 1 is (%c)\nPlayer 2 is (%c)\n\n", player1->GetColor(), player2->GetColor());
    }
    ~TicTacToe() = default;

    bool GameOver() const
    {
        Board &b = Board::GetBoard();
        const char &P1C = p1->GetColor();

        if (((b.GetSlot(0) == Move::CROSS) && (b.GetSlot(1) == Move::CROSS) && (b.GetSlot(2) == Move::CROSS)) ||
            ((b.GetSlot(3) == Move::CROSS) && (b.GetSlot(4) == Move::CROSS) && (b.GetSlot(5) == Move::CROSS)) ||
            ((b.GetSlot(6) == Move::CROSS) && (b.GetSlot(7) == Move::CROSS) && (b.GetSlot(8) == Move::CROSS)) ||
            ((b.GetSlot(0) == Move::CROSS) && (b.GetSlot(3) == Move::CROSS) && (b.GetSlot(6) == Move::CROSS)) ||
            ((b.GetSlot(1) == Move::CROSS) && (b.GetSlot(4) == Move::CROSS) && (b.GetSlot(7) == Move::CROSS)) ||
            ((b.GetSlot(2) == Move::CROSS) && (b.GetSlot(5) == Move::CROSS) && (b.GetSlot(8) == Move::CROSS)) ||
            ((b.GetSlot(0) == Move::CROSS) && (b.GetSlot(4) == Move::CROSS) && (b.GetSlot(8) == Move::CROSS)) ||
            ((b.GetSlot(2) == Move::CROSS) && (b.GetSlot(4) == Move::CROSS) && (b.GetSlot(6) == Move::CROSS)))

        {
            (P1C == Move::CROSS) ? puts("\nResult: Player 1 (X) Wins") : puts("\nResult: Player 2 (X) Wins");
            return true;
        }
        if (((b.GetSlot(0) == Move::CIRCLE) && (b.GetSlot(1) == Move::CIRCLE) && (b.GetSlot(2) == Move::CIRCLE)) ||
            ((b.GetSlot(3) == Move::CIRCLE) && (b.GetSlot(4) == Move::CIRCLE) && (b.GetSlot(5) == Move::CIRCLE)) ||
            ((b.GetSlot(6) == Move::CIRCLE) && (b.GetSlot(7) == Move::CIRCLE) && (b.GetSlot(8) == Move::CIRCLE)) ||
            ((b.GetSlot(0) == Move::CIRCLE) && (b.GetSlot(3) == Move::CIRCLE) && (b.GetSlot(6) == Move::CIRCLE)) ||
            ((b.GetSlot(1) == Move::CIRCLE) && (b.GetSlot(4) == Move::CIRCLE) && (b.GetSlot(7) == Move::CIRCLE)) ||
            ((b.GetSlot(2) == Move::CIRCLE) && (b.GetSlot(5) == Move::CIRCLE) && (b.GetSlot(8) == Move::CIRCLE)) ||
            ((b.GetSlot(0) == Move::CIRCLE) && (b.GetSlot(4) == Move::CIRCLE) && (b.GetSlot(8) == Move::CIRCLE)) ||
            ((b.GetSlot(2) == Move::CIRCLE) && (b.GetSlot(4) == Move::CIRCLE) && (b.GetSlot(6) == Move::CIRCLE)))

        {
            (P1C == Move::CIRCLE) ? puts("\nResult: Player 1 (O) Wins") : puts("\nResult: Player 2 (O) Wins");
            return true;
        }

        for (unsigned i = 0; i < 9; ++i)
        {
            if (b.GetSlot(i) == static_cast<char>(Move::EMPTY))
                return false;
        }

        puts("\nResult: Draw");
        return true;
    }

    void GameLoop() const
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);

        if (dis(gen) > 0.5f)
        {
            puts("Player 1 is first!");
            while (!GameOver())
            {
                p1->MakeMove();
                ++TicTacToe::moves;
                if (GameOver())
                    break;
                p2->MakeMove();
                ++TicTacToe::moves;
            }
        }
        else
        {
            puts("Player 2 is first!");
            while (!GameOver())
            {
                p2->MakeMove();
                ++TicTacToe::moves;
                if (GameOver())
                    break;
                p1->MakeMove();
                ++TicTacToe::moves;
            }
        }
    }
};

static std::pair<std::string,char> PreparePlayer()
{
    std::string name;
    char c;
    
    printf("Enter Name: ");
    std::cin >> name;
    do 
    {
        std::cout << "Enter symbol: ";
        std::cin >> c;
    } while (c != Move::CIRCLE && c != Move::CROSS);
        
    return std::make_pair(name,c);
}
 
short TicTacToe::moves = 0;

int main()
{  
    #ifdef _DEBUG
    Player p1("Mark", 'X');
    Player p2("Nick", 'O');
    #else
    std::cout << "=========Player 1=========" << std::endl;
    std::pair<std::string,char> p1Info = PreparePlayer();
    std::cout << "=========Player 2=========" << std::endl;
    std::pair<std::string,char> p2Info = PreparePlayer();
    
    auto[name1, c1] = p1Info;
    auto[name2, c2] = p2Info;
    
    Player p1(name1, c1);
    Player p2(name2, c2);
    #endif
    
    if (p1 == p2)
    {
        if (p1.GetColor() == Move::CIRCLE)
            p2.SetColor(Move::CROSS);
        else
            p2.SetColor(Move::CIRCLE);
    }
    
    TicTacToe game(&p1, &p2);
    
    game.GameLoop();
    
    printf("\nNumber of moves: %u\n\n", TicTacToe::moves);

    return 0;
}
