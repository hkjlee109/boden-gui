set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "Disable HarfBuzz in FreeType" FORCE)
add_subdirectory(deps/freetype EXCLUDE_FROM_ALL)
