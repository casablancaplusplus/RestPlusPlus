#ifndef     _METHOD_BASE_H_
#define     _METHOD_BASE_H_ 

#include <Wt/Dbo/SqlConnectionPool>
#include <Wt/Http/Response>
#include <Wt/Http/Request>

#include <string>

#include "../dbo/pollSession.h"
#include "../dbo/dbo.h"

/* This is a base class intended to be inherited by the C++ classes that correspond to a certain 
 * Http methods such as GET, PUT, POST, etc. 
 */


class operation {
    
    public:
        operation(
                  Wt::Dbo::SqlConnectionPool&     connectionPool,
                  Wt::Http::Response&                   response,
                  const Wt::Http::Request&              request 
                  );
        
        virtual     ~operation(){} // inline
        
        // virtual function that operates on the request and basically the url 
        // with a behaviour thats overriden in very http method class that inherits 
        // this base class
        virtual void    operate();

    protected:
        
        Wt::Dbo::SqlConnectionPool& _connectionPool;  // connection pool used to interact with the db
        Wt::Http::Response&     _response;
        const Wt::Http::Request&    _request;
};

#endif //    _METHOD_BASE_H_



