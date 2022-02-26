# Navigation

Contains the primary robot navigation and control software, as well as a simulator

## Contents

- [Cheat sheet](#cheat-sheet)
- [Installing](#installing)
  - [Windows](#windows)
  - [Linux](#linux)
  - [Mac](#mac)
- [Building](#building)
- [Simulator usage](#simulator)
- [Project structure](#project-structure)
- [Add components to project](#add-components-to-project)

## Cheat Sheet

| Action | Command |
| --- | --- |
| Set up meson build target named `build` | `meson setup build` |
| Run build for meson build target named `build` | `meson compile -C build` |
| Clean build target named `build` | `meson compile -C build --clean` |
| Run unit tests in `build` target | `./build/test/tests` |
| Run simulator using config `sim_config.csv` | `./build/src/sim/sim sim_config.csv` |

## Installing

After completing the relevant installation instructions for your platform, see the [Building](#building) instructions next to make sure your installation works.

### Windows

These instructions will use the [scoop package manager](https://scoop.sh). However, you are entirely free to use the new Windows Package Manager, Chocolately, or just install the necessary software manually. If you aren't sure, just use scoop.

Install `scoop` by following the instructions at https://scoop.sh. To posterity, open `Powershell` (or better yet Powershell Core aka `pwsh` if it's installed), and run the command
```
iwr -useb get.scoop.sh | iex
```
If you get an error, run `Set-ExecutionPolicy RemoteSigned -scope CurrentUser` and then re-run `iwr -useb get.scoop.sh | iex`.

Next, install Git, Python 3, the Ninja build system, and the GCC compiler suite. Using scoop, simply run:
```
scoop install git python ninja
```

Next, clone this repository. Open your terminal (Powershell/pwsh if you have no other preference), use the change directory command `cd` to navigate to the folder you want this project to live under, and run `git clone https://github.com/JHU-Delivery-Robot/Navigation.git`.

Finally, we install meson. This should just be `pip install meson`. Run `meson --version` to make sure it is installed properly, if meson cannot be found you need to place Python package scripts onto your PATH. If you don't know how to do this, just uninstall Python and re-install using scoop which will do it for you.

### Linux

Use your system package manager to acquire Git, the GCC compiler suite, a recent version of Python 3, and the Ninja build system. If you are using Ubuntu/Debian, this is simply
```
sudo apt install git gcc python3 ninja-build
```
Next, clone this repository into the folder of your choice using `git clone https://github.com/JHU-Delivery-Robot/Navigation.git`.

Finally, we install meson. This should just be `pip install meson`. Run `meson --version` to make sure it is installed properly, if meson cannot be found you need to place Python package scripts onto your PATH.

### Mac

#### Intel

First, install brew from [here](https://brew.sh/)

Then, ensure that you have python 3 installed by running `python --version` (or `python3 --version`) in your terminal. If you do not, install the latest python from [here](https://www.python.org/downloads/)

Install the following by running the associated commands:
- GCC: `brew install gcc`
- Meson: `pip install meson`
- Ninja: `brew install gcc`

Next, enable python certs by doing the following:
1. Open your finder and go to **Applications** in the left panel
2. Find and open the Python 3.xx folder
3. Double-click the *Install Certificates.command* file. Let its terminal run to completion, and input whatever you need to in order to let it run

Clone this repository and open the Navigation directory in a terminal. Now, use the following commands to build the executables:
1. First, you create your *build* folder by running: `CC=/usr/local/opt/gcc/bin/gcc-11 CXX=/usr/local/opt/gcc/bin/g++-11 meson setup build`
2. Then, you compile the code by running: `meson compile -C build`. Re-compile as needed
3. Finally, when you want to clean out your binaries, use this command: `meson compile -C build --clean`. You may re-run step 2 to re-compile everything
4. If you want to do a full cleaning of all your binaries and libraries, delete your build folder. You may re-run steps 1 and 2 to bring back libraries and binaries

#### M1

(WORK IN PROGRESS)

## Building

Once you've got everything installed, you need to create build directories for Meson. For example, to create a build configuration called `debug` with the default settings, run `meson setup debug`. To create a release configuration, run `meson setup release -Dbuildtype=release -Db_lto=true -Doptimization=2`.

To compile the project using a specific configuration, simply run `meson compile` within that folder. To clean, run `meson compile --clean`.

## Simulator

The simulation is split into two parts: a backend and a frontend. The backend runs the actual simulation and outputs a file called `sim_output.json` wherever it was run from that contains everything that happened during the simulation. The frontend is a local replay tool that can be used to replay and visualize the simulation output file. To use the frontend, open `./sim/visualization/index.html`.

To use the simulation backend, run `/src/sim/sim` and pass it the path of a simulation config file. A simple example is provided in`sim_config.csv`, which can be run with
```
> ./src/sim/sim ../sim_config.csv
```
from within a build directory. Points are given as `(x, y)`, e.g. `(20.5, -34.23)`. Units are all in meters or seconds unless otherwise specified. Config is in CSV format, so each line is just `key, value`. Units are implied, don't include them with the value. All config parameters are optional, defaults are noted below.

Simulation config parameters:
- `end_distance` &mdash; If the robot gets within this distance of the goal position, the simulation will end. Default is 0.2 m.
- `time_step` &mdash; Increase in simulation time between iterations. Default is 0.02 s.
- `iteration_limit` &mdash; Maximum iterations simulation will run for, must be an integer. Default is 3000.
- `map_size` &mdash; Width/height of area that the replay frontend will display, centered on `(0,0)`. Must be an integer, default is 8 m.
- `start_position` &mdash; The position to start the robot at. Specified as a `point`, default is `(-3.0, -2.0)`.
- `start_angle` &mdash; Angle (measured counterclockwise from standard x-axis) to start robot at. Specified in degrees, default is 90.0&deg;.
- `goal_position` &mdash; The goal position for robot to try to drive to. Specified as a `point`, default is `(3.5, 3.0)`.
- `obstacles` &mdash; A polygonal obstacle, specified as a comma separated list of points all on the same line. This key can be specified multiple times to add multiple obstacles. Must specify at least three points, do not close (specify start point again at end). Default is no obstacles.

## Project Structure

- `src/` - All primary source
  - `common/` - Shared utilities and types
  - `events/` - Event queue component used for error reporting and server communications handling.
  - `hal/` - Hardware Abstraction Layer
    - `impl/` - HAL implementation for our primary hardware platform
  - `robot/` - Top-level control and navigation software
  - `sim/` - Simulation of sensors and physics for testing `robot`
    - `sim_hal_impl/` - HAL implementation for virtual simulation environment
    - `visualization/` - Simulation replay/visualization tools
- `test/` - Unit & integration tests
  - `common/`
  - `sim/`
- `subprojects/` - Third-party dependencies
  - `asio` - Platform-independent asychronous IO library
  - `catch2` - Testing framework, installed via Meson wrap

## Add components to project

TODO
