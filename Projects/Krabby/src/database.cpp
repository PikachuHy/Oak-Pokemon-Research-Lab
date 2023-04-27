#include "database.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
static const char* DEFAULT_CONNECTION = "qt_sql_default_connect";
Database::Database()
{
    init_database();
    if (!exist_table()) {
        create_table();
    }
}

Database::~Database()
{
    if (m_initSuccess) {
        database.close();
    }
}

void Database::init_database()
{
    if (QSqlDatabase::contains(DEFAULT_CONNECTION)) {
        database = QSqlDatabase::database(DEFAULT_CONNECTION);
    }
    else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        auto path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        QDir dir(path);
        if (!dir.exists()) {
            bool ok = dir.mkpath(path);
            if (!ok) {
                qDebug() << "ERROR: create path fail, " << path;
            }
        }
        auto db_path = path + "/krabby_score.db";
        qDebug() << "database:" << db_path;
        database.setDatabaseName(db_path);
    }
    if (!database.open()) {
        qDebug() << "ERROR: Failed to connect database.";
        m_initSuccess = false;
    }
    else {
        qDebug() << "connect to database success";
        m_initSuccess = true;
    }
}

bool Database::insert_score(Score score)
{
    QSqlQuery sqlQuery;
    sqlQuery.prepare("INSERT INTO score_tbl VALUES(:id, :name, :score, :create_time)");
    auto max_id = get_max_score_id();
    if (max_id < 0) {
        qDebug() << "ERROR: bad score id" << max_id;
        // return false;
        max_id = 100;
    }
    sqlQuery.bindValue(":id", max_id + 1);
    sqlQuery.bindValue(":name", QString::fromStdString(score.name));
    sqlQuery.bindValue(":score", score.score);
    auto create_time = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd HH:mm:ss");
    sqlQuery.bindValue(":create_time", create_time);
    auto ok = sqlQuery.exec();
    if (!ok) {
        qDebug() << "ERROR: insert record " << score.to_string() << sqlQuery.lastError();
        return false;
    }
    return true;
}

std::vector<Score> Database::fetch_scores()
{
    QSqlQuery sqlQuery;
    bool ok = sqlQuery.exec("SELECT * from score_tbl");
    if (!ok) {
        qDebug() << "ERROR: fetch scores" << sqlQuery.lastError();
        return {};
    }
    std::vector<Score> scores;
    while (sqlQuery.next()) {
        Score score;
        score.id = sqlQuery.value(0).toInt();
        score.name = sqlQuery.value(1).toString().toStdString();
        score.score = sqlQuery.value(2).toInt();
        auto create_time = sqlQuery.value(3).toDateTime();
        score.create_time = create_time.toSecsSinceEpoch();
        scores.push_back(std::move(score));
    }
    return scores;
}

int Database::get_max_score_id()
{
    QSqlQuery sqlQuery;
    bool ok = sqlQuery.exec("SELECT max(id) as max_id from score_tbl");
    if (!ok) {
        qDebug() << "ERROR: get max score id failed" << sqlQuery.lastError();
        return -1;
    }
    ok = sqlQuery.first();
    if (!ok) {
        qDebug() << "ERROR: call sqlQuery.first()" << sqlQuery.lastError();
    }
    int id = 0;
    id = sqlQuery.value(0).toInt();
    return id;
}

void Database::create_table()
{
    QSqlQuery sqlQuery;
    QString createSql = QString("\
CREATE TABLE score_tbl (\
    id INT PRIMARY KEY NOT NULL,\
    name TEXT NOT NULL,\
    score INT NOT NULL,\
    create_time timestamp NOT NULL\
);\
");
    sqlQuery.prepare(createSql);
    if (!sqlQuery.exec()) {
        qDebug() << "Error: Fail to create table" << sqlQuery.lastError();

    }
    else {
        qDebug() << "create table success";
    }
}

bool Database::exist_table()
{
    qDebug() << "database tables:" << database.tables();
    if (database.tables().contains("score_tbl")) {
        qDebug() << "table score_tbl exist";
        return true;
    }
    return false;
}
