define compile_all
	g++ -std=c++14 $(1) -o$(2)
	$(2)
endef

sample:
	$(call compile_all, quick-sketch.cpp, ./bin/a0)

cmp:
	$(call compile_all, real-time.cpp, ./bin/a1)

hi:
	$(call compile_all, static-high-res.cpp, ./bin/a2)

bl:
	$(call compile_all, blackhole-high-res.cpp, ./bin/a3)