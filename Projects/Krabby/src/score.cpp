#include "score.h"
#include <sstream>
Score::Score(std::string name, int score): name(std::move(name)), score(score)
{

}

std::string Score::to_string() const
{
    std::stringstream ss;
    ss << "{";
    ss << " ";
    ss << "id: " << id;
    ss << ",";
    ss << " ";
    ss << "name: " << name;
    ss << ", ";
    ss << " ";
    ss << "score: " << score;
    ss << ", ";
    ss << "create_time: " << create_time;
    ss << " ";
    ss << "}";
    return ss.str();
}
