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

# Meson will find correct threading library (i.e. pthreads or W32)
thread_dep = dependency('threads')

# External dependencies
asio_dep = dependency('asio')
catch2_dep = dependency('catch2')

subdir('src')
subdir('test')
