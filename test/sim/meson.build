src = [ 'main.cpp', 'test_obstacle_map.cpp', 'test_sensor_gen.cpp' ]

simtest = executable(
    'simtest', src,
    include_directories: sim_includes, link_with: sim_lib,
    dependencies: [ catch2_dep, stb_dep ]
)

test('simtest', simtest)
