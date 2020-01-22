CXX = g++
CXXFLAGS = -O2 -Wall -Iglui-2.36/src/include

LIBS = -Lglui-2.36/src/lib -lglui -L/usr/lib64/nvidia -lglut -lGL -lGLU \
       -Wl,-rpath=/usr/lib64/nvidia

all : traceview chop_by_rank chop_by_time


traceview : traceview.cpp 
	$(CXX) $(CXXFLAGS) traceview.cpp $(LIBS) -o traceview

chop_by_rank : chop_by_rank.c
	gcc -g -O2 chop_by_rank.c -o chop_by_rank

chop_by_time : chop_by_time.c
	gcc -g -O2 chop_by_time.c -o chop_by_time

clean :
	rm -f traceview chop_by_rank chop_by_time
