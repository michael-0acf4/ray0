sample:
	g++ -std=c++14 quick-sketch.cpp -o ./bin/a0
	./bin/a0

cmp:
	g++ -std=c++14 real-time.cpp -o ./bin/a1
	./bin/a1

hi:
	g++ -std=c++14 static-high-res.cpp -o ./bin/a2
	./bin/a2

bl:
	g++ -std=c++14 blackhole.cpp -o ./bin/a3
	./bin/a3