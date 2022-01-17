project(
    'navigation',
    'cpp', 'c',
    version : '0.0.1',
    default_options: [
        'cpp_std=c++17',
        'werror=true',
        'warning_level=3',
    ]
)

asio = dependency('asio')
catch2_dep = dependency('catch2')

subdir('src')
subdir('test')
