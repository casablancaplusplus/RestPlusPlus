#ifndef     _POLL_RESOURCE_H_
#define     _POLL_RESOURCE_H_


#include <Wt/WResource>
#include <Wt/Http/Response>
#include <Wt/Dbo/SqlConnectionPool>

#include "methods/methodBase.h"
#include "methods/get.h"
#include "methods/post.h"

class pollResource  :   public  Wt::WResource {

    public:
        pollResource(
                        Wt::Dbo::SqlConnectionPool& connectionPool
                    );

        virtual     ~pollResource(){}

        void initiateGet(Wt::Http::Response    response, 
                         const Wt::Http::Request&   request
                        );

        void initiatePost(Wt::Http::Response&    response,
                          const Wt::Http::Request&  request           
                         );

    protected:
        virtual void    handleRequest
                (
                const Wt::Http::Request&    request,
                Wt::Http::Response&         response
                );
    private:
        Wt::Dbo::SqlConnectionPool&     _connectionPool;
        operation                       *operation_;
};

#endif  
