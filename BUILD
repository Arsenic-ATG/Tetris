load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "tetris_lib",
    srcs = [
        "src/app.cpp",
        "src/game.cpp",
        "src/renderer.cpp",
    ],
    hdrs = [
        "src/app.hpp",
        "src/game.hpp",
        "src/renderer.hpp",
        "src/utils.hpp",
    ],
    copts = [
        "-std=c++17",
        "-O2",
    ],
    data = [
        "src/assets/clacon.ttf",
        "src/assets/tetris.wav",
    ],
    linkopts = [
        "-lSDL2_ttf",
        "-lSDL2_mixer",
        "-lSDL2",
    ],
    deps = [],
)

# g++ -std=c++17 src/renderer.cpp src/game.cpp src/app.cpp src/main.cpp -O2 -lSDL2_ttf -lSDL2_mixer -lSDL2
cc_binary(
    name = "tetris",
    srcs = ["src/main.cpp"],
    deps = [":tetris_lib"],
)
