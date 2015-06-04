#ifndef     _POST_H_
#define     _POST_H_


#include <regex>
#include <vector>

#include <Wt/Json/Object>
#include <Wt/Json/Parser>
#include <Wt/Json/Array>

#include "methodBase.h"

// this class takes care of an Http POST request
// member functions named begin with "post"

class post  : public    operation {
    
    public:
        post(
                Wt::Dbo::SqlConnectionPool&   connectionPool,
                Wt::Http::Response&             response,
                const Wt::Http::Request&        request 
                );

       virtual  ~post(){}

       // this method is specialized to carry other request properties
       // like a body for post, etc
        virtual void operate();
       
        // member function for voting on a choice
        // url pattern : /questions/{question_id}/choices/{choice_id}
        void    postVote();

        // member function for creating a new question
        // url pattern : /questions
        void    postNewQuestion();



    private:
        // regex object that help us determine what member function
        // to call for a certain url
        static  std::regex  _postVote;
        static  std::regex  _postNewQuestion;

};






#endif
