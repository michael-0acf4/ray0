define compile_all
	g++ -std=c++14 $(1) -o$(2) engine.cpp geom.cpp
	$(2)
endef

test:
	$(call compile_all, test.cpp, ./bin/a0)

sphere:
	$(call compile_all, sphere.cpp, ./bin/a0)

real:
	$(call compile_all, scene-composition.cpp, ./bin/a1)

bl:
	$(call compile_all, blackhole.cpp, ./bin/a3)
