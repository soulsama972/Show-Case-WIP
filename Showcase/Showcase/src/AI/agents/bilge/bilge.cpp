#include<iostream>
#include<cstring>
#include<algorithm>
#include<unordered_map>
#include<random>
#include<vector>
#include<thread>
#include<chrono>
#include<ctime>

#define WIDTH 6
#define HEIGHT 12
#define NUM_OF_PIECES 10

bool firstTime = true;


enum PiecesType
{
    INVALID = -1,
    FISH,
    CRAB,
    JELIYFISH,
    BLUESQURE,
    CYANSQURE,
    BLUECYANSQURE,
    DARKBLUECIRCLE,
    CYANCIRCLE,
    GREENCIRCLE,
    BLUETRIANGLE,
};

struct Range
{
    int xStart, xEnd;
    int yStart, yEnd;
};

struct CheckMove
{
    int x;
    int y;
};


struct ScoreInfo
{
    int score = 0;
    int combo = 0;
};



using Strike = CheckMove;
using M = CheckMove;

class Bilge
{
public:
    int arr[HEIGHT][WIDTH];
    int waterLevel;

    Bilge() = default;
    Bilge(int array[HEIGHT][WIDTH])
    {
        init(array);
    }

    void init(int array[HEIGHT][WIDTH])
    {
        memcpy(arr, array, HEIGHT * WIDTH * sizeof(int));
        waterLevel = 3;
    }

    ScoreInfo switchPieces(int x, int y, bool ignoreNewPices = false)
    {
        ScoreInfo sInfo;
        std::vector<CheckMove> cm;
        std::vector<Strike> strikes;
        std::unordered_map<int, int>visted;
        int strikeCounter = 0;
        int tempX;

        if (x == WIDTH - 1) x--;
        if (!isValidPos(x, y)) return sInfo;

        int pieceA = getValue(x, y);
        int pieceB = getValue(x + 1, y);
        if (pieceA == PiecesType::CRAB || pieceB == PiecesType::CRAB ||
            (pieceA == PiecesType::FISH && pieceB == PiecesType::FISH) ||
            (pieceA == PiecesType::JELIYFISH && pieceB == PiecesType::JELIYFISH))
            return { 0, -1 };


        if (pieceA == PiecesType::JELIYFISH || pieceB == PiecesType::JELIYFISH)
        {
            tempX = pieceA == PiecesType::JELIYFISH ? x + 1 : x;
            AppendStrikeByValue(strikes, getValue(tempX, y));
            sInfo.score += static_cast<int>(strikes.size());
            sInfo.combo++;
        }

        else if (pieceA == PiecesType::FISH || pieceB == PiecesType::FISH)
        {
            tempX = pieceB == PiecesType::FISH ? x : x - 1;
            appendStrike(strikes, visted, { tempX, tempX + 3, y - 1, y + 2 });
            sInfo.score += static_cast<int>(strikes.size());
            sInfo.combo++;
        }
        else
        {
            setValue(x, y, pieceB);
            setValue(x + 1, y, pieceA);
            cm.push_back({ x, y });
        }

        if (!ignoreNewPices) addNewPieces();

        while (cm.size() || strikes.size())
        {
            strikeCounter = 0;
            for (auto& move : cm)
            {
                if (appendStrike(strikes, visted, getRangeStrike(move.x, move.y, true)))
                    strikeCounter++;
                if (appendStrike(strikes, visted, getRangeStrike(move.x + 1, move.y, true)))
                    strikeCounter++;
                if (appendStrike(strikes, visted, getRangeStrike(move.x, move.y, false)))
                    strikeCounter++;
                if (appendStrike(strikes, visted, getRangeStrike(move.x + 1, move.y, false)))
                    strikeCounter++;
            }
            //AddScore(static_cast<int>(strikes.size()), sInfo);
            AddScore(strikeCounter, sInfo);
            shiftBoardArray(strikes, cm);
            strikes.clear();
            visted.clear();
        }

        if (!ignoreNewPices) addNewPieces();

        return sInfo;
    }

private:

    inline void AddScore(int counter, ScoreInfo& sInfo) const
    {
        auto p = [](int x, int count)
        {
            int total = x;
            for (int i = 1; i < count; i++) total *= x;
            return total;
        };
        if (counter > 0)
        {
            sInfo.score += sInfo.combo ? 1 : p(3, counter);
            sInfo.combo++;
        }
    }

    inline bool isValidPos(int x, int y) const
    {
        if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
            return false;
        return true;
    }

    inline int getValue(int x, int y) const
    {
        if (!isValidPos(x, y))
            return PiecesType::INVALID;
        return this->arr[y][x];
    }

    inline void setValue(int x, int y, int value)
    {
        if (!isValidPos(x, y))
            return;
        arr[y][x] = value;
    }

    void AppendStrikeByValue(std::vector<Strike>& strikes, int value) const
    {
        for (int y = 0; y < HEIGHT; y++)
            for (int x = 0; x < WIDTH; x++)
                if (getValue(x, y) == value)
                    strikes.push_back({ x,y });
    }

    bool appendStrike(std::vector<Strike>& strikes, std::unordered_map<int, int>& visted, const Range& range) const
    {
        bool result = false;
        if (range.yEnd - range.yStart < 3 && range.xEnd - range.xStart < 3)
            return result;

        result = true;
        for (int y = range.yStart; y < range.yEnd; y++)
        {
            for (int x = range.xStart; x < range.xEnd; x++)
            {
                int index = y * WIDTH + x;
                if (isValidPos(x, y) && visted.find(index) == visted.end())
                {
                    strikes.push_back({ x, y });
                    visted[index] = 1;

                }
            }
        }
        return result;
    }

    void addNewPieces()
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                float chance = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                if (getValue(x, y) == PiecesType::INVALID)
                {
                    int newValue = chance < 0.03f ? rand() % 3 : (rand() % NUM_OF_PIECES - 3) + 3;
                    setValue(x, y, newValue);
                }
            }
        }
    }

    void shiftBoardArray(std::vector<Strike>& strikes, std::vector<CheckMove>& cm)
    {
        std::unordered_map<int, int> map;
        std::sort(strikes.begin(), strikes.end(), [](Strike a, Strike b) {return a.y > b.y; });
        cm.clear();

        for (auto& pos : strikes)
        {
            int carbCount = 0;
            for (int iY = pos.y; iY < HEIGHT - 1; iY++)
            {
                int piece = getValue(pos.x, iY + 1 + carbCount);

                if (piece == PiecesType::CRAB && iY < HEIGHT - this->waterLevel)
                {
                    carbCount += 1;
                    continue;
                }
                setValue(pos.x, iY - carbCount, piece);
                int index = (iY - carbCount) * WIDTH + pos.x;
                if (map.find(index) == map.end())
                {
                    map[index] = 0;
                    cm.push_back({ pos.x, iY - carbCount });
                }
            }

            for (int i = 1; i < carbCount + 2; i++)
            {
                setValue(pos.x, HEIGHT - i, PiecesType::INVALID);
                int index = (HEIGHT - i) * WIDTH + pos.x;
                if (map.find(index) == map.end())
                {
                    map[index] = 0;
                    cm.push_back({ pos.x, HEIGHT - i });
                }
            }
        }
    }

    Range getRangeStrike(int x, int y, bool isColumn) const
    {
        Range range = { x, x + 1, y , y + 1 };
        bool reversed = true;
        int step = 1;
        int value = getValue(x, y);
        int i;

        if (value <= PiecesType::JELIYFISH)
            return range;

        if (isColumn)
        {
            for (i = x + 1; i < WIDTH; i++)
            {
                if (getValue(i, y) != value)
                {
                    range.xEnd = i;
                    break;
                }
            }

            for (i = x - 1; i > 0; i--)
            {
                if (getValue(i, y) != value)
                {
                    range.xStart = i + 1;
                    break;
                }
            }
        }

        else
        {
            for (i = y + 1; i < HEIGHT; i++)
            {
                if (getValue(x, i) != value)
                {
                    range.yEnd = i;
                    break;
                }
            }
            for (i = y - 1; i > 0; i--)
            {
                if (getValue(x, i) != value)
                {
                    range.yStart = i + 1;
                    break;
                }
            }
        }
        return range;
    }
};

void GetBestMove(std::vector<M>& currentMove, std::vector<M>& bestMove, int& bestScore, int start, int end, int arr[HEIGHT][WIDTH], int depth)
{
    Bilge b;
    ScoreInfo sInfo;
    if (!depth) return;

    for (int i = start; i < end; i++)
    {
        int x = i % (WIDTH - 1);
        int y = i / (WIDTH - 1);

        b.init(arr);
        sInfo = b.switchPieces(x, y, true);
        currentMove.push_back({ x, y });
        if (sInfo.score == 0)
            GetBestMove(currentMove, bestMove, bestScore, 0, HEIGHT * (WIDTH - 1), b.arr, depth - 1);
        
        else if (sInfo.score > bestScore || (currentMove.size() < bestMove.size() && sInfo.score == bestScore))
        {
            bestMove.assign(currentMove.begin(), currentMove.end());
            bestScore = sInfo.score;
        }
        currentMove.pop_back();
    }
}

void calclateBestMove(int& score, std::vector<M>& bestMove, int arr[HEIGHT][WIDTH], int depth, int numOfThread)
{
    std::vector<M> cm;
    std::vector<std::thread> threads;

    auto* currentMoves = new std::vector<M>[numOfThread];
    auto* bestMoves = new std::vector<M>[numOfThread];
    int* bestScores = new int[numOfThread];
    memset(bestScores, 0, sizeof(int) * numOfThread);
    
    int spiltSize = HEIGHT * (WIDTH - 1) / numOfThread;

    for (int i = 0; i < numOfThread; i++)
        threads.push_back(std::thread(GetBestMove, std::ref(currentMoves[i]), std::ref(bestMoves[i]), std::ref(bestScores[i]), i * spiltSize, (i + 1) * spiltSize, arr, depth));

    for (auto& t : threads)
        t.join();

    for (int i = 0; i < numOfThread; i++)
    {
        if (bestScores[i] > score || (bestScores[i] == score && bestMoves[i].size() < bestMove.size()))
        {
            bestMove.assign(bestMoves[i].begin(), bestMoves[i].end());
            score = bestScores[i];
        }
    }
 

    delete[] currentMoves;
    delete[] bestMoves;
    delete[] bestScores;

}

extern "C" __declspec(dllexport) void getBestMove(int* score, int* moves, int arr[HEIGHT][WIDTH], int depth, int numofThreads)
{
   std::vector<M> m;
   calclateBestMove(*score, m, arr, depth, numofThreads);
   for (int i = 0; i < m.size(); i++)
   {
       moves[2 * i] = m[i].x;
       moves[2 * i + 1] = m[i].y;
   }

}


