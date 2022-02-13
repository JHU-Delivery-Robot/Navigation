hal_impl_includes = include_directories('.')
hal_impl_src = files([
    'hal_provider_impl.cpp',
    'TG30_lidar.cpp',
])

hal_impl_lib = static_library(
    'hal_impl_lib',
    hal_impl_src + common_src + events_src,
    include_directories : [common_includes, hal_includes, events_includes],
    dependencies: [ asio_dep ],
)

hal_impl_dep = declare_dependency(
    link_with: hal_impl_lib,
    include_directories: [ hal_impl_includes ],
    dependencies: [ asio_dep ],
)
