.PHONY: default setup-emsdk activate-emsdk build serve

default:
	echo "Hello, World!"

setup-emsdk:
	git clone https://github.com/emscripten-core/emsdk.git
	cd emsdk && ./emsdk install latest && cd ..

activate-emsdk:
	cd emsdk && ./emsdk activate latest && source ./emsdk_env.sh && cd ..

build:
	em++ -std=c++17 src/renderer.cpp src/game.cpp src/app.cpp src/main.cpp -O2 -s TOTAL_MEMORY=67108864 -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 --preload-file src/assets -o ./build/index.js

build-g++:
	g++ -std=c++17 src/renderer.cpp src/game.cpp src/app.cpp src/main.cpp -O2 -lSDL2_ttf -lSDL2_mixer -lSDL2

serve:
	@echo "Serving files from ./build"
	cd build && python3 -m http.server --directory .

run:
	@echo "Running the built executable..."
	chmod +x ./a.out
	./a.out
	@echo "Execution completed."

BAZEL=USE_BAZEL_VERSION=8.4.2 npx @bazel/bazelisk

bazel-build:
	@echo "Building with Bazel..."
	$(BAZEL) build //:tetris
	@echo "Build completed. Executable is located at bazel-bin/tetris"

bazel-lint:
	@echo "Linting Bazel files..."
	go run github.com/bazelbuild/buildtools/buildifier@latest -r .
	@echo "Bazel files linted successfully."

bazel-run:
	@echo "Running the Bazel-built executable..."
	$(BAZEL) run //:tetris
	@echo "Execution completed."
