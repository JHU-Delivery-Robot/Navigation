all_test_src = [ 'catch_main.cpp' ]

subdir('common')
subdir('sim')

tests = executable(
    'tests', all_test_src + common_src + events_src + sim_src,
    include_directories: navigation_includes,
    dependencies: [ thread_dep, catch2_dep ]
)

test('tests', tests)
