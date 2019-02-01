
all:
	gcc -o client3 client3.c chat.c -lpthread
	gcc -o server3 server3.c chat.c -lpthread

clean:
	rm -rf	server3 client3

help:
	@echo "멀티스레드를 이용한 1:1 채팅. 'end'로 종료까지 구현"
