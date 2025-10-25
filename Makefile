BUILD_TYPE ?= "Debug"

.PHONY: build
build:
	if [ ! "-d" "build/$(BUILD_TYPE)" ]; then mkdir -p "build/$(BUILD_TYPE)"; fi
	cd "build/$(BUILD_TYPE)" && \
		cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DBUILD_SHARED_LIBS=OFF ../.. && \
		cmake --build . -j
	cp -r src/shaders "build/$(BUILD_TYPE)/shaders"

.PHONY: build-debug
build-debug:
	BUILD_TYPE=Debug make build

.PHONY: build-release
build-release:
	BUILD_TYPE=Release make build

.PHONY: clean
clean:
	rm -rf ./build/*
