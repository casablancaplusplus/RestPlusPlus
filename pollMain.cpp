
#include <Wt/WServer>
#include <Wt/Dbo/SqlConnectionPool>

#include "dbo/pollSession.h"
#include "pollResource.h"



int main( int argc, char ** argv) {

    try {
        Wt::WServer server(argv[0], "");

        try {
            server.setServerConfiguration(argc, argv);
            Wt::Dbo::SqlConnectionPool *pollDb
                = PollSession::createConnectionPool(server.appRoot() + "newPoll.db");

            pollResource res(*pollDb);
            server.addResource(&res, "/api");
           
            try {
                server.start();
                Wt::WServer::waitForShutdown();
                server.stop();
            } catch(Wt::WServer::Exception&     e) {
                std::cout << e.what() << std::endl;
                return 1;
            }

            return 0;
        } catch( Wt::WServer::Exception& e) {
            std::cout << e.what() << std::endl;
            return 1;
        } catch( std::exception& e) {
            std::cout << e.what() << std::endl;
            return 1;
        }
    }catch(Wt::WServer::Exception&  e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}

