project "pax_core"
    kind "staticLib"

    language   "C++"
    cppdialect "C++14"

    includedirs { "./" }

    files {
        "pax_core/debug.hpp",
        "pax_core/types.hpp",
        "pax_core/types.cpp",
        "pax_core/report.hpp",
        "pax_core/report.cpp",
        "pax_core/array.hpp",
        "pax_core/write.hpp",
        "pax_core/write.cpp",
        "pax_core/buff.hpp",
        "pax_core/buff.cpp",
        "pax_core/arena.hpp",
        "pax_core/arena.cpp",
        "pax_core/slice.hpp",
        "pax_core/export.hpp",
    }

    -- filter { "system:windows" }
    --     defines { "_CTR_SECURE_NO_WARNINGS" }

    filter { "configurations:debug" }
        symbols "on"

    filter { "configurations:release" }
        optimize "on"

project "report"
    kind "consoleApp"

    language   "C++"
    cppdialect "C++14"

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

    includedirs { "./" }

    files {
        "pax_core/main_arena.cpp",
    }

    links { "pax_core" }

    filter { "configurations:debug" }
        symbols "on"

    filter { "configurations:release" }
        optimize "on"
