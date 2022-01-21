all_test_src = [ 'catch_main.cpp' ]

subdir('common')
subdir('sim')

tests = executable(
    'tests', all_test_src + common_src,
    include_directories: [ common_includes, sim_includes ],
    link_with: [ sim_lib ],
    dependencies: catch2_dep
)

test('tests', tests)
