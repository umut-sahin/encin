ifeq ($(release), 1)
	build_directory=release
	build_type=Release
else
	build_directory=debug
	build_type=Debug
endif

ifneq ($(verbose), 1)
	verbosity_redirect=> /dev/null
endif

ifndef (example)
	example=all
endif


.PHONY: build

ifneq ($(verbose), 1)
.SILENT:
endif


build:
	mkdir -p build/$(build_directory) && \
	cd build/$(build_directory) && \
	cmake \
		-DCMAKE_MAKE_PROGRAM=make  \
		-DCMAKE_C_COMPILER=gcc \
		-DCMAKE_BUILD_TYPE=$(build_type) \
		../.. $(verbosity_redirect) && \
	make --no-print-directory $(example) $(verbosity_redirect)

run: build
	./build/$(build_directory)/examples/$(example) $(arguments)

clean:
	rm -rf build/*
