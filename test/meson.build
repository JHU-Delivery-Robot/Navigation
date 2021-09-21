all_test_src = [ 'catch_main.cpp' ]

subdir('common')
subdir('sim')

tests = executable(
    'tests', all_test_src,
    include_directories: [ common_includes, sim_includes ],
    link_with: [ common_lib, sim_lib ],
    dependencies: catch2_dep
)

test('tests', tests)
