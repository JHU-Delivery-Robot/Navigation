src = [ 'main.cpp', 'test_vector2.cpp' ]

commontest = executable(
    'commontest', src,
    include_directories: common_includes, link_with: common_lib,
    dependencies: catch2_dep
)

test('commontest', commontest)
