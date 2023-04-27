#ifndef DATABASE_H
#define DATABASE_H
#include "score.h"
#include <QSqlDatabase>
class Database
{
public:
    Database();
    ~Database();
    void init_database();
    bool insert_score(Score score);
    std::vector<Score> fetch_scores();
    int get_max_score_id();
private:
    void create_table();
    bool exist_table();
private:
    bool m_initSuccess;
    QSqlDatabase database;
};

#endif // DATABASE_H
