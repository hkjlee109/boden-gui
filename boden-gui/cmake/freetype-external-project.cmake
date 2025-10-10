set(FT_DISABLE_BROTLI ON CACHE BOOL "Disable brotli in FreeType" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "Disable bzip2 in FreeType" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "Disable harfbuzz in FreeType" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "Disable png in FreeType" FORCE)
set(FT_DISABLE_ZLIB ON CACHE BOOL "Disable zlib in FreeType" FORCE)

add_subdirectory(deps/freetype EXCLUDE_FROM_ALL)
