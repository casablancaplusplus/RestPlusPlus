#include <Wt/Dbo/FixedSqlConnectionPool>

#include "pollSession.h"
#include "dbo.h"

PollSession::PollSession(dbo::SqlConnectionPool& connectionPool)
    :connectionPool_(connectionPool)
{
    setConnectionPool(connectionPool_);
    
    mapClass<questions>("questions");
    mapClass<choices>("choices");

    try {
        dbo::Transaction t(*this);
        this->createTables();
        t.commit();
    }catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Using existing database";
    }
}

dbo::SqlConnectionPool  *PollSession::createConnectionPool(
        const std::string& sqliteDb)
{
    dbo::backend::Sqlite3   *connection = new dbo::backend::Sqlite3(sqliteDb);

    connection->setProperty("show-queries", "true");
    connection->setDateTimeStorage(Wt::Dbo::SqlDateTime,
            Wt::Dbo::backend::Sqlite3::PseudoISO8601AsText);

    return new dbo::FixedSqlConnectionPool(connection, 10);
}

