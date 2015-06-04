


#include "methodBase.h"


operation::operation(
                  Wt::Dbo::SqlConnectionPool&     connectionPool,
                  Wt::Http::Response&                   response,
                  const Wt::Http::Request&              request 
                 )
    : _connectionPool(connectionPool),
      _response(response),
      _request(request)
    {
    }


void    operation::operate() {}
