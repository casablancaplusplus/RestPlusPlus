


#include "pollResource.h"



pollResource::pollResource(Wt::Dbo::SqlConnectionPool&  connectionPool)
    : _connectionPool(connectionPool)
    {
    }

void    pollResource::handleRequest (
                        const Wt::Http::Request&    request,
                        Wt::Http::Response&         response
        )
{

    if(request.method() == "GET" ) 
        initiateGet(response, request);

    else if ( request.method() == "POST" )  
            initiatePost(response, request);
    else {

        std::cout << "_____NOT IMPLEMENTED_____" << std::endl;
        response.setStatus(501);
    }

    
}

void pollResource::initiateGet(Wt::Http::Response  response,
                               const Wt::Http::Request& request 
        )                             
{

    operation_ = new get(_connectionPool, response, request);
    
    operation_-> operate();
}

void pollResource::initiatePost(Wt::Http::Response& response,
                                const Wt::Http::Request& request 
        )
{
                                
    operation_ = new post(_connectionPool, response, request);
   
    operation_-> operate();

}
