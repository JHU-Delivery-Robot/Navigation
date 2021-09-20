# Navigation

Contains the primary robot navigation and control software, as well as a simulator

## Contents

- `src/` - All primary source
  - `/hal` - Hardware Abstraction Layer
  - `/robot` - Control and navigation software
  - `/sim` - Simulation of sensors and physics for testing `robot`
- `test/` - Unit & integration tests
  - `sim/`
- `subprojects/` - Third-party dependencies
  - `catch2/` - Testing framework
  - `stb/` - Image library

## Installing

After cloning the repo, you will need C/C++ compilers, the Meson build system, and a backend supported by Meson.

Instructions on installing Meson can be found [here](https://mesonbuild.com/Quick-guide.html). The backends supported by Meson are Ninja, Visual Studio, and XCode. 

If you are using a backend other than Ninja, see [this](https://mesonbuild.com/Using-with-Visual-Studio.html).

Once you've got everything installed, you need to create build directories for Meson. For example, to create a build configuration called `debug` with the default settings, run `meson setup debug`. To create a release configuration, run `meson setup release -Dbuildtype=release -Db_lto=true -Doptimization=2`.

To compile the project using a specific configuration, simply run `meson compile` within that folder. To clean, run `meson compile --clean`.
