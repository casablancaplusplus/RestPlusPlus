#include <string>
#include <regex>


#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/SqlConnectionPool>

#include "utilities.h"
#include "../dbo.h"

//TODO : add a parameter map argument so that you get to recive get parameters
std::string    validGet(const std::string&    url, 
        Wt::Dbo::SqlConnectionPool& connectionPool) {
    
    PollsSession    session(connectionPool);

    // base reg /
    std::string     baseReg = "/";

    // questions details reg /questions/{question_id}
    std::string     qdReg = "/questions/[1-9]+";

    // question collection /questions{?page}
    std::string     qcReg = "/questions\\?page=[1-9]+";


    std::regex     BaseReg(baseReg,    std::regex_constants::icase);
    std::regex     QdReg(qdReg,    std::regex_constants::icase);
    std::regex     QcReg(qcReg,    std::regex_constants::icase);


    if(std::regex_match(url.begin(), url.end(), BaseReg) ){
            return "This the base path";

    }
    else if (std::regex_match(url.begin(), url.end(), QdReg)) {
            //return " you requested a questions details \n";
           
            std::string questionId = "";
            {
            std::vector<std::string> urlParts = analyzeUrl(url);
            questionId = urlParts[1];
            }                
    //        return questionId;  // works
            Wt::Dbo::Transaction    t(session);
            Wt::Dbo::ptr<questions> question = session.find<questions>()
                .where("id = ? ").bind(questionId);
            std::string result = question->question + " | " 
                                 + question->url + " | ";

            typedef Wt::Dbo::collection<Wt::Dbo::ptr<choices>>::const_iterator
                ccit;

            for ( ccit  it = question->Choices.begin();
                        it != question->Choices.end();
                        it++)
            {
                result += (*it)->choice + " - ";
            }

            return result;
    }
    else if (std::regex_match(url.begin(), url.end(), QcReg)) {
            return " you requested a question collection \n";
    }
    else return  "GET is not allowed or implemented for this\
                        URI\n" ;

}


std::string     validPost(const std::string&    url) {
   
    // voteOnAChoice pattern
    std::string     voac = "/questions/[0-9]+/choices/[0-9]+";

    // createNewQuestion pattern
    std::string     cnq = "/questions";


    std::regex      voteOnAChoice(voac, std::regex_constants::icase);
    std::regex      createNewQuestion(cnq,  std::regex_constants::icase);

    if(std::regex_match(url.begin(), url.end(), voteOnAChoice)) {
            return "You voted on a choice";
    }
    else if(std::regex_match(url.begin(), url.end(), createNewQuestion) ) {
        return "You created a new question";
    }
    else return "POST is not allowed or implemented for this URI";
}
        
