.PHONY: debug release test clean

FDEBUG=-std=c23 -Wall -Og -fcolor-diagnostics

debug:
	@mkdir -p build/debug
	@printf "→ Building @ %s\n" build/debug
	$(CC) ios_json.c $(FDEBUG)
	@mv a.out build/debug/ios_json

release:
	@mkdir -p build/release
	@printf "→ Building @ %s\n" build/release
	$(CC) ios_json.c -std=c23 -O3 -march=native
	@mv a.out build/release/ios_json

unit:
	@mkdir -p build/test
	$(CC) ios_json.c unit.c $(FDEBUG) 
	@printf "→ Running tests\n"
	@mv a.out build/test/unit
	build/test/unit

clean:
	@rm -rf build
