run:
	@mkdir -p build
	@cmake -G Ninja -B build/
	@cmake --build build/
	@./build/bellhop

clean:
	@rm -rf build/
