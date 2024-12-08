define compile_all
	g++ -std=c++14 -O4 -Isrc/core $(1) -o$(2) src/core/engine.cpp src/core/geom.cpp
	$(2)
endef

ifeq ($(OS),Windows_NT)
	MKDIR_CMD = mkdir bin
else
	MKDIR_CMD = mkdir -p bin
endif

test: bin
	$(call compile_all, src/sketches/test.cpp, ./bin/test)

sphere: bin
	$(call compile_all, src/sketches/sphere.cpp, ./bin/sphere)

composition: bin
	$(call compile_all, src/sketches/scene-composition.cpp, ./bin/composition)

donut: bin
	$(call compile_all, src/sketches/donut.cpp, ./bin/donut)

gradient: bin
	$(call compile_all, src/sketches/gradient.cpp, ./bin/gradient)

blackhole: bin
	$(call compile_all, src/sketches/blackhole.cpp, ./bin/blackhole)

bin:
	$(MKDIR_CMD)
