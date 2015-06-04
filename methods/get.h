#ifndef     _GET_H_
#define     _GET_H_


#include <regex>

#include "methodBase.h"


/* this class corresponds to an Http GET request
 * and it takes care of generating the content for 
 * a GET request
 * member functions of this class begin with "get"
 */

class get   :   public  operation {

    public:
        get(
                Wt::Dbo::SqlConnectionPool&   connectionPool,
                Wt::Http::Response&                 response,
                const Wt::Http::Request&            request 
                );

        virtual  ~get(){}

        virtual void operate();

        // member function for viewing a questions details
        // url pattern : /questions/{question_id}
        void    getQuestionDetails();

        // member function for retrieving question collections
        // based on number of page
        // url pattern : /questions{?page}
        void    getQuestionCollection();


    private:
        // regex objects the help us determine what member function
        // to call for a certain url
        static  std::regex  _getQuestionDetails;
        static  std::regex  _getQuestionCollection;
};

#endif
