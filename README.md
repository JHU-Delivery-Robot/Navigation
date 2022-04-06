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
  - `catch2/` - Testing framework, installed via Meson wrap

## Installing

After cloning the repo, you will need C/C++ compilers, the Meson build system, and a backend supported by Meson.

Instructions on installing Meson can be found [here](https://mesonbuild.com/Quick-guide.html). The backends supported by Meson are Ninja, Visual Studio, and XCode. 

If you are using a backend other than Ninja, see [this](https://mesonbuild.com/Using-with-Visual-Studio.html).

Once you've got everything installed, you need to create build directories for Meson. For example, to create a build configuration called `debug` with the default settings, run `meson setup debug`. To create a release configuration, run `meson setup release -Dbuildtype=release -Db_lto=true -Doptimization=2`.

To compile the project using a specific configuration, simply run `meson compile` within that folder. To clean, run `meson compile --clean`.

## Simulation

The simulation is split into two parts: a backend and a frontend. The backend runs the actual simulation and outputs a file called `sim_output.json` wherever it was run from that contains everything that happened during the simulation. The frontend is a local replay tool that can be used to replay and visualize the simulation output file. To use the frontend, open `./sim/visualization/index.html`.

To use the simulation backend, run `/src/sim/sim` and pass it the path of a simulation config file. A simple example is provided in`sim_config.json`, which can be run with
```
> ./src/sim/sim ../sim_config.cjson
```
from within a build directory. Points are given as `[x, y]`, e.g. `[20.5, -34.23]`. Units are all in meters or seconds unless otherwise specified. Config is in JSON format, and units are implied, so don't include them with the value. All config parameters are optional, defaults are noted below.

Simulation config parameters:
- `end_distance` &mdash; If the robot gets within this distance of the goal position, the simulation will end. Default is 0.2 m.
- `time_step` &mdash; Increase in simulation time between iterations. Default is 0.02 s.
- `iteration_limit` &mdash; Maximum iterations simulation will run for, must be an integer. Default is 3000.
- `map_size` &mdash; Width/height of area that the replay frontend will display, centered on `[0,0]`. Must be an integer, default is 8 m.
- `start_position` &mdash; The position to start the robot at. Specified as a `point`, default is `[-3.0, -2.0]`.
- `start_angle` &mdash; Angle (measured counterclockwise from standard x-axis) to start robot at. Specified in degrees, default is 90.0&deg;.
- `goal_position` &mdash; The goal position for robot to try to drive to. Specified as a `point`, default is `[3.5, 3.0]`.
- `obstacles` &mdash; A polygonal obstacle, specified as a list of lists of points. Each obstacle must specify at least three points. Do not close the shape (that is, do not specify start point again at end). Default is no obstacles.