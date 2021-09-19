project('stb', 'c')

stb_includes = include_directories('.')
stb = static_library('stb', 'stb_image.c', 'stb_image_write.c')

stb_dep = declare_dependency(include_directories : stb_includes, link_with : stb)
