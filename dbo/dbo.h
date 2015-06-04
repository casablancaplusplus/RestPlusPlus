#include <Wt/Dbo/Dbo>
#include <string>
#include <Wt/WDateTime>
#include <Wt/WDate>
#include <Wt/Dbo/WtSqlTraits>


namespace dbo = Wt::Dbo;

class questions;

class choices {

    
    public: 
        std::string choice;
        std::string url;
        int votes;
        int choice_id;
       
        // the question this choice belongs to 
        dbo::ptr<questions> question;

        template<class Action>
            void persist(Action& a)
            {
                dbo::field(a, choice_id, "choice_id");
                dbo::field(a, choice, "choice");
                dbo::field(a, url,    "url");
                dbo::field(a, votes,  "votes");

                dbo::belongsTo(a, question, "tquestion");
            }
            
};


class questions {

    public:
        std::string question;
        Wt::WDate published_at;
        std::string url;
// not necessary        int question_id;

        dbo::collection<dbo::ptr<choices> > Choices;
        
        template<class Action>
            void persist(Action& a)
            {
               // not necessary dbo::field(a, question_id, "question_id");
                dbo::field(a, url,         "url");
                dbo::field(a, question, "question");
                dbo::field(a, published_at, "published_at");
                // the column that links the questions to their choices
                dbo::hasMany(a, Choices, dbo::ManyToOne, "tquestion");
            }
};

        
    
