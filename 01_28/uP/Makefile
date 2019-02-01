# 이렇게 sim.h 링크 안해주면 sim.h가 수정될경우 안바껴짐.
# 두개의 파일이 수정되면 컴파일하고 안바꼇으면 컴파일 안함

all: sim.h		
	g++ -o iss main.cpp 

run:
	./iss myhex.bin 10

clean:
	rm -rf iss
