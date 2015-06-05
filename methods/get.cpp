


#include "get.h"
// utility
std::vector<std::string>    analyzeGetUrl(const std::string&   url)
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

get::get(
        Wt::Dbo::SqlConnectionPool&     connectionPool,
        Wt::Http::Response&                     response,
        const Wt::Http::Request&                request 
        )
    : operation( connectionPool, response, request )
    {
    }

void    get::operate() {
    
    

    std::string     _url = _request.pathInfo();

    if(std::regex_match(_url.begin(), _url.end(), _getQuestionDetails)) {
        
        getQuestionDetails();
    }
    else if(std::regex_match(_url.begin(), _url.end(), _getQuestionCollection)) {

        getQuestionCollection();
    }
    else if(std::regex_match(_url.begin(), _url.end(), _getEntryPoint)) {

        getEntryPoint();
    }
    else {
        std::cout << "_____ NOT A GOOD GET ______ " << std::endl;
    }
}

void    get::getQuestionDetails() {

    std::vector<std::string>    analyzedUrl = analyzeGetUrl(_request.pathInfo());

    PollSession     session(_connectionPool);
    Wt::Dbo::Transaction    t(session);
    try {
            std::cout << "______ id _________" << std::endl;
            std::cout << analyzedUrl[1] << std::endl;
            std::cout << "_______ id ________" << std::endl;
            int     question_id = std::stoi(analyzedUrl[1]);
            Wt::Dbo::ptr<questions>     question_ptr =
                session.find<questions>().where("id = ?").bind(question_id);

            std::ostream&       out = _response.out();
            _response.setMimeType("application/json");

            out << "{" << std::endl;
            out << "    \"question\":\"" << question_ptr->question << "\",\n";
            std::string publishDate = question_ptr->published_at.toString()
                .toUTF8();
            out << "    \"published_at\":\"" << publishDate << "\"," << std::endl;
            out << "    \"url\":\"" << question_ptr->url << "\"," << std::endl;
            out << "    \"choices\": [" << std::endl;

            typedef Wt::Dbo::collection<Wt::Dbo::ptr<choices> >     Choices;
            for(Choices::const_iterator it = question_ptr->Choices.begin();
                                        it != question_ptr->Choices.end();
                                        ++it)
            {
                out << "{";
                out << "    \"choice\":\"" << (*it)-> choice << "\"," << std::endl;
                out << "    \"url\":\""    << (*it)-> url    << "\"," << std::endl;
                out << "    \"votes\":\""  << (*it)-> votes  << "\" " << std::endl;
                out << "}";
                if( it == question_ptr->Choices.end() ) out << std::endl;
                else    out << "," << std::endl;
            }

            out << "    ]" << std::endl;
            out << "}" << std::endl;

            std::cout << "__________________________ " << std::endl;
            std::cout << " You requested a questions details " << std::endl;
            std::cout << "__________________________ " << std::endl;


    }
    catch(Wt::Dbo::Exception&   e) {
        _response.setMimeType("application/json");
        _response.out() << e.what() << std::endl;

        std::cout <<    "___________________________ " << std::endl;
        std::cout <<    " Your details couldn't be fetched due to some error \n";
        std::cout <<    "___________________________ " << std::endl;
    }

    
}

void    get::getQuestionCollection() {

    _response.setMimeType("application/json");
    std::ostream&       out = _response.out();
   
    PollSession     session(_connectionPool);
    Wt::Dbo::Transaction    t(session);

    try {

        const   std::string     page = *(_request.getParameter("page"));
        std::string     most  = std::to_string((std::stoi(page)*10)+1);
        std::string     least = std::to_string(1);
        if( std::stoi(page) == 0 )  throw "You didn't provide the page number";
        else { least = std::to_string(std::stoi(most)-11);
        }
        
        typedef Wt::Dbo::collection<Wt::Dbo::ptr<questions> >   queCollection;
        queCollection       qCollection =
            session.find<questions>().where(" id < "+most+" and id >"+ least);
        out << '[' << std::endl; 
        for(queCollection::const_iterator   it = qCollection.begin();
                                            it != qCollection.end();
                                            ++it)
        {
            Wt::Dbo::ptr<questions>     question_ptr = *it;
            
            out << "{" << std::endl;
            out << "    \"question\":\"" << question_ptr->question << "\",\n";
            std::string publishDate = question_ptr->published_at.toString()
                .toUTF8();
            out << "    \"published_at\":\"" << publishDate << "\"," << std::endl;
            out << "    \"url\":\"" << question_ptr->url << "\"," << std::endl;
            out << "    \"choices\": [" << std::endl;

            typedef Wt::Dbo::collection<Wt::Dbo::ptr<choices> >     Choices;
            for(Choices::const_iterator it = question_ptr->Choices.begin();
                                        it != question_ptr->Choices.end();
                                        ++it)
            {
                out << "{";
                out << "    \"choice\":\"" << (*it)-> choice << "\"," << std::endl;
                out << "    \"url\":\""    << (*it)-> url    << "\"," << std::endl;
                out << "    \"votes\":\""  << (*it)-> votes  << "\" " << std::endl;
                out << "}";
                if( it == question_ptr->Choices.end() ) out << std::endl;
                else    out << "," << std::endl;
            }

            out << "    ]" << std::endl;
            out << "}";
            if( it == qCollection.end() ) out << std::endl;
            else    out << ",";
        }
        out << "]" << std::endl;



    }
    catch(Wt::Dbo::Exception&  e) {
        out << e.what() << std::endl;
    }
    catch(std::exception&    e) {
        out << e.what() << std::endl;
    }
    catch(...) { out << "This is an exception of unkown type. deal with it later.";}
        
    std::cout << "__________________________ " << std::endl;
    std::cout << " You requested a question collection " << std::endl;
    std::cout << "__________________________ " << std::endl;
}

void    get::getEntryPoint() {
    _response.setMimeType("application/json");
    _response.out() << "{\n"
                    << "    \"questions_url\":\"/api/questions\"\n"
                    << "}\n";
}
// TODO : improve the patterns . so far they don't detect zeros
std::regex  get::_getQuestionDetails =        std::regex(
        "/questions/[1-9]+",                  std::regex_constants::icase);

std::regex  get::_getQuestionCollection =     std::regex(
        "/questions",           std::regex_constants::icase);

std::regex  get::_getEntryPoint =   std::regex(
        "/",    std::regex_constants::icase);
