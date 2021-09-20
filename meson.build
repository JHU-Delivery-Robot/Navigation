project(
    'navigation', 'cpp', 'c',
    default_options: [
        'cpp_std=c++17',
        'werror=true'
    ]
)

stb_proj = subproject('stb', default_options : ['werror=false', 'warning_level=0'])
stb_dep = stb_proj.get_variable('stb_dep')

catch2_proj = subproject('catch2', default_options : ['werror=false', 'warning_level=0'])
catch2_dep = catch2_proj.get_variable('catch2_dep')

subdir('src')
subdir('test')
