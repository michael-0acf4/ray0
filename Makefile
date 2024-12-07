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

hi:
	$(call compile_all, static-high-res.cpp, ./bin/a2)

bl:
	$(call compile_all, blackhole-high-res.cpp, ./bin/a3)

sandb:
	$(call compile_all, ./sandbox/main.cpp, ./bin/a4)