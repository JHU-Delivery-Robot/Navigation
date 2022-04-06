all_test_src = [ 'catch_main.cpp' ]

subdir('common')
subdir('sim')

tests = executable(
    'tests', all_test_src + common_src + events_src + sim_src,
    include_directories: [ common_includes, events_includes, hal_includes, robot_includes, sim_includes ],
    dependencies: [ catch2_dep, json_dep ]
)

test('tests', tests)
