

compile with :

g++ methods/methodBase.cpp methods/post.cpp methods/get.cpp dbo/pollSession.cpp -std=c++11 -lwt -lwthttp -lwtdbo -lwtdbosqlite3 -lboost_system pollMain.cpp pollResource.cpp -o bin/app


run with :

sudo bin/app --docroot . --approot . --http-address 0.0.0.0 --http-port 80


api blueprint :

http://docs.ahmadabdolkhanie.apiary.io/#reference/default/polls-api-root


base url :

localhost/api

usage example :

GET localhost/api/questions/1

note : you may use a software of your choice to send request but I used the chrome extension DHC
