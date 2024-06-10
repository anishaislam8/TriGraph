SRC=4_probability_estimator_db.cpp \
    4_probability_estimator_local.cpp \
    graph.cpp \
    graph.h \
    utils.cpp \
    utils.h

ALL: estimator.exe estimator

estimator: $(SRC)
	g++ -pg -O3 -std=c++11 -o estimator 4_probability_estimator_local.cpp graph.cpp utils.cpp -lcrypto -lsqlite3
estimator.exe: $(SRC)
	g++ -O3 -o estimator.exe 4_probability_estimator_local.cpp graph.cpp utils.cpp -lcrypto -lsqlite3
