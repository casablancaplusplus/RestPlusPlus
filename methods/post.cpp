



#include "post.h"
//utility
std::vector<std::string>    analyzePostUrl(const std::string&   url)
{
    std::vector<std::string>    result;
    std::string::size_type      pos = 0;
    std::string::size_type      count;

    while(1) 
    {
        pos = url.find('/',pos)+1;
        count = url.find('/', pos);

        if( count == std::basic_string<char>::npos) 
        {
            result.push_back(url.substr(pos, url.size()));
            break;
        }
        else   
            result.push_back(url.substr(pos, count-pos));
    }

    return result;
}

static int  count = 0;
// debug function
void    dbug() {
    std::cout << "I'm here " << count << std::endl;
    count++;
}

// TODO : improve the patterns
std::regex  post::_postVote =               std::regex(
        "/questions/[0-9]+/choices/[0-9]+", std::regex_constants::icase);

std::regex  post::_postNewQuestion =        std::regex(
        "/questions",                       std::regex_constants::icase);


post::post(
        Wt::Dbo::SqlConnectionPool&     connectionPool,
        Wt::Http::Response&                     response,
        const Wt::Http::Request&                request 
        )
    : operation( connectionPool, response, request )
    {
    }
void    post::operate() {
   
    std::string     _url = _request.pathInfo();

    if(std::regex_match(_url.begin(), _url.end(), _postVote)) {
        
        postVote();
    }
    else if(std::regex_match(_url.begin(), _url.end(), _postNewQuestion)) {
        
        postNewQuestion();
    }
    else { 
        std::cout << " _________ NOT A GOOD POST ________ " << std::endl;
        std::cout << "_______" << _url << "__________" << std::endl;
    
    }
}

void    post::postVote() {
    
    std::vector<std::string>    analyzedUrl = analyzePostUrl(_request.pathInfo());

    PollSession     session(_connectionPool);
    Wt::Dbo::Transaction    t(session);

    try {
            int     question_id = std::stoi(analyzedUrl[1]);
            int     target_choice_id   = std::stoi(analyzedUrl[3]);

            Wt::Dbo::ptr<questions>     question_ptr =
                session.find<questions>().where("id = ? ").bind(question_id);
                
            std::cout << "question id " << question_id << std::endl;
            std::cout << "_______________________" << std::endl;
            std::cout << "choice id " << target_choice_id << std::endl;
            
            typedef Wt::Dbo::collection<Wt::Dbo::ptr<choices> >     choiceColl;
            //choiceColl     choice_collection = question_ptr-> Choices;
            bool        voted = false;
            for(choiceColl::const_iterator      it = question_ptr->Choices.begin();
                                                it != question_ptr->Choices.end();
                                                it++)
            {
                if((*it)->choice_id == target_choice_id){

                    (*it).modify()->votes += 1;
                    voted = true;
                }
            }
            
            if( voted ) {
                _response.addHeader("Location","/questions/"+
                        std::to_string(question_id));
            }
            else {
                _response.setMimeType("application/json");
                _response.out() << "{\n"
                                << "    \"erro\":\"No such choice\"\n"
                                << "}\n";
            }
    }
    catch(Wt::Dbo::Exception&   e) {
        _response.setMimeType("application/json");
        _response.out () << e.what() << std::endl;
    }
    catch(std::exception&       e) {
        _response.setMimeType("application/json");
        _response.out() << e.what() << std::endl;
    }
    t.commit();
    std::cout <<" _____________________ " << std::endl;
    std::cout << " You just voted on a choice " << std::endl;
    std::cout << "_____________________ " << std::endl;

}






void    post::postNewQuestion() {
    
    std::ostream&   out = _response.out();
    

    int contentLength = _request.contentLength();

    
    if(_request.contentLength() != 0) {
        



        char    *buffer = new char [contentLength + 1];
        _request.in().read(buffer, contentLength);
        buffer[contentLength] = 0;
        
        Wt::Json::Object   body;
        Wt::Json::ParseError   error;

        bool    result = Wt::Json::parse(buffer, body, error, true);

        if(result == false) {
            _response.setMimeType("application/json");
            //out << "{   \"what\":\"" << error.what() << "\"     }" << std::endl;
            out << error.what() << std::endl;

            std::cout << "________________________" << std::endl;
            std::cout << "Your question was not created due to json parsing error." << std::endl;
            std::cout << "________________________" << std::endl;
        }
        else
        {
            Wt::Json::Value     question = body.get("question");
            Wt::Json::Value     choiceArray = body.get("choices");




            if(question.isNull() || choiceArray.isNull())
            {
                out << "{ \"what\":\"Something is missing\"}" << std::endl;
            }
            else if(question.type() != Wt::Json::Type::StringType ||
                    choiceArray.type() != Wt::Json::Type::ArrayType
                   )
            {
                out << "{ \"what\":\"Something is wrong\"}" << std::endl;
            }
            else
            {
                typedef Wt::Json::Array    Array;
                std::string questionString = question;
                Array    choiceArrayValues = choiceArray;
            
    
                // a database session         
                PollSession     session(_connectionPool);
                Wt::Dbo::Transaction    t(session);
                // create a question object and presist it 
                questions    *question_= new questions();
                question_-> question = questionString;
                question_-> published_at = Wt::WDate::fromString(
                        "2015/03/08","yyyy/MM/dd");
                question_-> url = ""; // temporary url NOTE : should be updated
                 
                Wt::Dbo::ptr<questions> question_ptr =
                    session.add(question_);
               
                int thisQuestionsId;
                // get the question id that was assigned automatically
                try {

                    thisQuestionsId = session.query<int>("select id from questions")
                        .where("question = ?").bind(questionString);
                }catch(Wt::Dbo::NoUniqueResultException&    e) {

                    std::cout << "__________NO UNIQUE RESULT EXCEPTION_______" << std::endl;

                    thisQuestionsId = session.query<int>("select count(*) from questions");
                    
                }
                
                // this line of code was moved 
                question_ptr.modify()->url = "/questions/"+
                    std::to_string(thisQuestionsId);

                typedef std::vector<choices*>     choiceVec;
                choiceVec    Choices;
                

                int     choice_id = 1;
                for(Array::const_iterator   it = choiceArrayValues.begin();
                                            it != choiceArrayValues.end();
                                            ++it)
                {
                    choices     *choice_ = new choices();

                    std::string c = *it;
                    choice_-> choice = c;
                    
                    std::string cId = std::to_string(choice_id);
                    std::string    id = std::to_string(thisQuestionsId); 
                    
                    choice_-> url = "/questions/"+id+"/choices/"+cId;
                    choice_-> votes = 0;
                    choice_-> choice_id = choice_id; choice_id++;
                    choice_-> question = question_ptr;

                    Choices.push_back(choice_);

                }

                typedef     std::vector<Wt::Dbo::ptr<choices> > choicePtrVec;
                choicePtrVec        choicePtrVec_;

                for(choiceVec::const_iterator   it = Choices.begin();
                                                it != Choices.end();
                                                ++it)
                {
                    choicePtrVec_.push_back(session.add(*it));
                }
                
                
                t.commit();

                // previous programm termination was caused by this line of code
                _response.setMimeType("application/json");
                _response.setStatus(201);   // HTTP status code 200 Created
                
                std::string publishDate = question_ptr->published_at.toString().toUTF8();
                
                // pass dbug();
                _response.addHeader("Location",question_ptr->url); 
                out << "{" << std::endl;
                // pass dbug();
                out << "    \"question\":\""<< question_ptr->question << "\"," << std::endl;
                out << "    \"published_at\":\""<< publishDate << "\"," 
                    << std::endl;
                out << "    \"url\":\""<< question_ptr->url << "\"," << std::endl;
                out << "    \"choices\": [" << std::endl;
                for(choicePtrVec::const_iterator   it = choicePtrVec_.begin();
                                                   it != choicePtrVec_.end();
                                                   ++it)
                {
                   
                    out << "    {" << std::endl;
                    out << "        \"choice\":\"" << (*it)->choice <<"\"," << std::endl;
                    out << "        \"url\":\"" << (*it)->url << "\"," << std::endl;
                    out << "        \"votes\":\"" << (*it)->votes << "\"" << std::endl;
                    out << "    }";
                    if(it == choicePtrVec_.end()) out << std::endl;
                    else out << ",";
                }
                
                out << "    ]" << std::endl;
                out << "}" << std::endl;
            
        }
        
                std::cout << "____________________ " << std::endl;
                std::cout << " You just created a new question " << std::endl;
                std::cout << "____________________ " << std::endl;
        }

    }
    else
    {
        out << "{ you didn't provide a body for the new question}"<< std::endl;
    }
        
 

    

    
}

