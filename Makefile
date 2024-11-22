Headers := include/bin_exp_tree.h include/dump_bet.h
Objects := obj/main.o obj/bin_exp_tree.o obj/dump_bet.o

main.out: ${Objects} $(Headers)
	g++ ${Objects} -o main.out

obj/main.o: src/main.cpp ${Headers}
	g++ -c src/main.cpp -o obj/main.o

obj/bin_exp_tree.o: src/bin_exp_tree.cpp ${Headers}
	g++ -c src/bin_exp_tree.cpp -o obj/bin_exp_tree.o

obj/dump_bet.o: src/dump_bet.cpp ${Headers}
	g++ -c src/dump_bet.cpp -o obj/dump_bet.o

run:
	./main.out

target:
	mkdir log
	touch log/dump.dot
	touch log/dump.html

clean:
	rm -r log
	rm main.out
