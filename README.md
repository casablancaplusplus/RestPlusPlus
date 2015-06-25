

compile with :

g++ methods/methodBase.cpp methods/post.cpp methods/get.cpp dbo/pollSession.cpp -std=c++11 -lwt -lwthttp -lwtdbo -lwtdbosqlite3 -lboost_system pollMain.cpp pollResource.cpp -o bin/app

api blueprint :

http://docs.ahmadabdolkhanie.apiary.io/#reference/default/polls-api-root
