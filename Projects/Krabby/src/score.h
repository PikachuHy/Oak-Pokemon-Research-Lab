#ifndef SCORE_H
#define SCORE_H

#include <string>
class Database;
class ScoreChartWidget;
class Score
{
public:
    Score(std::string name, int score);
    std::string to_string() const;
private:
    Score() {};
    int id;
    std::string name;
    int score;
    int create_time;
    friend class Database;
    friend class ScoreChartWidget;
};

#endif // SCORE_H
