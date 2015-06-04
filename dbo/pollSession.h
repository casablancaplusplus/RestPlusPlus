#ifndef     _POLL_SESSION_H_
#define     _POLL_SESSION_H_

#include <Wt/Dbo/Session>
#include <Wt/Dbo/backend/Sqlite3>

namespace dbo = Wt::Dbo;

class PollSession : public dbo::Session
{
    public:
        PollSession(dbo::SqlConnectionPool& connectionPool);

        static dbo::SqlConnectionPool *createConnectionPool(
                const std::string& sqlite3);

    private:
        dbo::SqlConnectionPool& connectionPool_;
};

#endif // _POLLS_SESSION_H_
