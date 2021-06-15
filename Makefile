SRCDIR=src
BUILDDIR=build
GENERATOR=Unix Makefiles
BUILD_TYPE=Release

.PHONY: all cmake_build cmake debug clean run

all: clean cmake_build

cmake_build: cmake
	@$(MAKE) -C $(BUILDDIR)
	@echo "Binaries can be found in $(BUILDDIR)"

cmake:
	@cmake --version > /dev/null || (echo "You need cmake to build this program: https://cmake.org/download/" && exit 1)
	mkdir -p $(BUILDDIR); \
	cd $(BUILDDIR); \
	cmake .. \
		-G "$(GENERATOR)" \
		-DCMAKE_C_COMPILER=$(CC) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

debug:
	mkdir -p $(BUILDDIR); \
	cd $(BUILDDIR); \
	cmake ..

clean:
	rm -rf $(BUILDDIR)

run:
	./$(BUILDDIR)/pomodoro