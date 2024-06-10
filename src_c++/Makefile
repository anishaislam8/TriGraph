SRC=4_probability_estimator_db.cpp \
    4_probability_estimator_local.cpp \
    graph.cpp \
    graph.h \
    utils.cpp \
    utils.h

ALL: estimator.exe estimator

estimator: $(SRC)
	g++ -pg -O3 -o estimator 4_probability_estimator_local.cpp graph.cpp utils.cpp -lcrypto -lsqlite3
estimator.exe: $(SRC)
	g++ -O3 -o estimator.exe 4_probability_estimator_local.cpp graph.cpp utils.cpp -lcrypto -lsqlite3
gmon.out: estimator
	timeout -sSIGUSR1 60 ./estimator 
output.png: gmon.out estimator
	gprof estimator gmon.out | gprof2dot -s -w | dot -Gdpi=100 -Tpng -o output.png
	
