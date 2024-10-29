require "ecc/ecc"

workspace "pax"
    configurations { "debug", "release" }

    targetdir "%{wks.location}/out/bin_%{prj.name}"
    objdir    "%{wks.location}/out/obj_%{prj.name}"

    include "src/"
