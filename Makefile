define compile_all
	g++ -std=c++14 -O4 -Isrc/core $(1) -o$(2) src/core/engine.cpp src/core/geom.cpp
	$(2)
endef

test:
	$(call compile_all, src/sketches/test.cpp, ./bin/test)

sphere:
	$(call compile_all, src/sketches/sphere.cpp, ./bin/sphere)

composition:
	$(call compile_all, src/sketches/scene-composition.cpp, ./bin/composition)

blackhole:
	$(call compile_all, src/sketches/blackhole.cpp, ./bin/blackhole)
