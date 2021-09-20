project(
    'navigation', 'cpp', 'c',
    default_options: [
        'cpp_std=c++17',
        'werror=true'
    ]
)

stb_proj = subproject('stb', default_options : ['werror=false', 'warning_level=0'])
stb_dep = stb_proj.get_variable('stb_dep')

catch2_dep = dependency('catch2')

subdir('src')
subdir('test')
