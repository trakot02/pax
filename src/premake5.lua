project "pax_core"
    kind "staticLib"

    language   "C++"
    cppdialect "C++14"

    architecture "x86_64"

    includedirs { "./" }

    files {
        "pax_core/export.hpp",
        "pax_core/system.hpp",
        "pax_core/debug.hpp",
        "pax_core/types.hpp",
        "pax_core/types.cpp",
        "pax_core/array.hpp",
        "pax_core/mem/utils.hpp",
        "pax_core/mem/utils.cpp",
        "pax_core/mem/alloc.hpp",
        "pax_core/mem/alloc.cpp",
        "pax_core/mem/base.hpp",
        "pax_core/mem/base.cpp",
        "pax_core/mem/bump.hpp",
        "pax_core/mem/bump.cpp",
        "pax_core/mem/arena.hpp",
        "pax_core/mem/arena.cpp",
        "pax_core/slice.hpp",
        "pax_core/write.hpp",
        "pax_core/write.cpp",
        "pax_core/read.hpp",
        "pax_core/read.cpp",
        "pax_core/buff.hpp",
        "pax_core/buff.cpp",
        "pax_core/file.hpp",
        "pax_core/file.cpp",
        "pax_core/match.hpp",
        "pax_core/parse.hpp",
        "pax_core/parse.cpp",
        "pax_core/parse_csv.hpp",
        "pax_core/parse_csv.cpp",
        "pax_core/match_csv.hpp",
        "pax_core/match_csv.cpp",
        "pax_core/read_csv.hpp",
        "pax_core/read_csv.cpp",
        "pax_core/report.hpp",
        "pax_core/report.cpp",
    }

    filter { "system:windows" }
        files {
            "pax_core/file_windows.hpp",
            "pax_core/file_windows.cpp",
        }

    filter { "system:linux" }
        files {
            "pax_core/file_posix.hpp",
            "pax_core/file_posix.cpp",
        }

    filter { "configurations:debug" }
        symbols "on"

    filter { "configurations:release" }
        optimize "on"

project "report"
    kind "consoleApp"

    language   "C++"
    cppdialect "C++14"

    architecture "x86_64"

    includedirs { "./" }

    files {
        "pax_core/main_report.cpp",
    }

    links { "pax_core" }

    filter { "configurations:debug" }
        symbols "on"

    filter { "configurations:release" }
        optimize "on"

project "buffer"
    kind "consoleApp"

    language   "C++"
    cppdialect "C++14"

    architecture "x86_64"

    includedirs { "./" }

    files {
        "pax_core/main_buffer.cpp",
    }

    links { "pax_core" }

    filter { "configurations:debug" }
        symbols "on"

    filter { "configurations:release" }
        optimize "on"

project "arena"
    kind "consoleApp"

    language   "C++"
    cppdialect "C++14"

    architecture "x86_64"

    includedirs { "./" }

    files {
        "pax_core/main_arena.cpp",
    }

    links { "pax_core" }

    filter { "configurations:debug" }
        symbols "on"

    filter { "configurations:release" }
        optimize "on"

project "file"
    kind "consoleApp"

    language   "C++"
    cppdialect "C++14"

    architecture "x86_64"

    includedirs { "./" }

    files {
        "pax_core/main_file.cpp",
    }

    links { "pax_core" }

    filter { "configurations:debug" }
        symbols "on"

    filter { "configurations:release" }
        optimize "on"
