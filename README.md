# Navigation

Contains the primary robot navigation software for the JHU Deliverbot project, as well as a simulator.

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
| Install & build vcpkg dependencies | `<vcpkg install path>/vcpkg install --triplet <vcpkg-triplet>` |
| Run build for meson build target named `build` | `meson compile -C build` |
| Clean build target named `build` | `meson compile -C build --clean` |
| Run unit tests in `build` target | `./build/test/tests` |
| Run simulator using config `sim_config.csv` | `./build/src/navigation_sim ./robot_config.json ./sim_config.json ./sim_output/output.json` |

## Installing

After completing the relevant installation instructions for your platform, see the [Building](#building) instructions next to make sure your installation works. The following instructions use GCC compilers, however MSVC and (recent versions of) Clang should also work.

### Windows

These instructions will use the [scoop package manager](https://scoop.sh). However, you should be able to install the software with another package manager or manually if you wish.

Install `scoop` by following the instructions at https://scoop.sh &mdash; pay attention to the instructions about what to do if you see an error.

Next, install Git, Python 3, the Ninja build system, and the GCC compiler suite. Using scoop, this is simply:
```
scoop install git python ninja gcc
```

Next, [install vcpkg](https://vcpkg.io/en/getting-started.html).

Next, clone this repository by navigating to the folder you want to keep Deliverbot software in and then running `git clone --recurse-submodules https://github.com/JHU-Delivery-Robot/Navigation.git`.

Next, we install the Meson build system. This should just be `pip install meson`. Run `meson --version` to make sure it is installed properly &mdash; if meson cannot be found your Python scripts are likely not in your PATH. If you are using an existing installation of Python and run into this issue, just uninstall Python, re-install it using scoop, and then install Meson again. 

Next, we install the vcpkg package manager by running `.\vcpkg\bootstrap-vcpkg.bat` from the root directory of this project.

### Linux

Use your system package manager to acquire Git, the GCC compiler suite, a recent version of Python 3, and the Ninja build system. If you are using Ubuntu/Debian, this is simply
```
sudo apt install git gcc python3 ninja-build cmake
```

Next, [install vcpkg](https://vcpkg.io/en/getting-started.html).

Next, clone this repository into the folder of your choice using `git clone --recurse-submodules https://github.com/JHU-Delivery-Robot/Navigation.git`.

Next, we install meson. This should just be `pip install meson`. Run `meson --version` to make sure it is installed properly, if meson cannot be found you need to place Python package scripts onto your PATH.

Next, we install the vcpkg package manager by running `./vcpkg/bootstrap-vcpkg.sh` from the root directory of this project.

### Mac

#### Intel Mac (x86-64)

First, install brew from [here](https://brew.sh/)

Then, ensure that you have python 3 installed by running `python --version` (or `python3 --version`) in your terminal. If you do not, install the latest python from [here](https://www.python.org/downloads/)

Install the following by running the associated commands:
- GCC: `brew install gcc`
- Meson: `pip install meson`
- Ninja: `brew install ninja`

Next, enable python certs by doing the following:
1. Open your finder and go to **Applications** in the left panel
2. Find and open the Python 3.xx folder
3. Double-click the *Install Certificates.command* file. Let its terminal run to completion, and input whatever you need to in order to let it run

Next, [install vcpkg](https://vcpkg.io/en/getting-started.html).

Clone this repository using `git clone --recurse-submodules https://github.com/JHU-Delivery-Robot/Navigation.git` and open the Navigation directory in a terminal. Next, we install the vcpkg package manager by running `./vcpkg/bootstrap-vcpkg.sh` from the root directory of this project.

Now, use the following commands to build the executables:
1. First, you create your *build* folder by running: `CC=/usr/local/opt/gcc/bin/gcc-11 CXX=/usr/local/opt/gcc/bin/g++-11 meson setup build`
2. Then, you compile the code by running: `meson compile -C build`. Re-compile as needed
3. Finally, when you want to clean out your binaries, use this command: `meson compile -C build --clean`. You may re-run step 2 to re-compile everything
4. If you want to do a full cleaning of all your binaries and libraries, delete your build folder. You may re-run steps 1 and 2 to bring back libraries and binaries

#### M1 Mac (ARM)

(WORK IN PROGRESS)

## Building

Once you've got everything installed, you need to build and install some third-party dependencies. From inside this project's folder, run `./vcpkg/vcpkg install --triplet <vcpkg-triplet>` where `<vcpkg-triplet>` is the vcpkg triplet identifier for your platform. For Linux, you should use the `x64-linux` triplet, for Windows with MinGW gcc use `x64-mingw-static`, and for MacOS use `x64-osx`. This make take some time to complete, but only needs to be done during the first install, or when we update our vcpkg dependencies.

Next, you need to create build directories for Meson. For example, to create a build configuration called `debug` with the default settings, run `meson setup debug`. To create a release configuration, run `meson setup release -Dbuildtype=release -Db_lto=true -Doptimization=2`. **NOTE**: In order to follow Meson to find our dependencies from vcpkg, _before running `meson setup`_ you must create an environment variable `CMAKE_PREFIX_PATH` and set it to `<navigation project>/vcpkg_installed/<triplet>` &mdash; make sure to use the full absolute path.

To generate gRPC code from a `.proto` file, run the following commands (in the project root directory):
```
./vcpkg_installed/<triplet>/tools/protobuf/protoc.exe --proto_path="src/comms" --cpp_out="src/comms" <path to .proto>
./vcpkg_installed/<triplet>/tools/protobuf/protoc.exe --proto_path="src/comms" --grpc_out="src/comms" --plugin=protoc-gen-grpc="./vcpkg_installed/<triplet>/tools/grpc/grpc_cpp_plugin.exe" <path to .proto>
```
Note that the triplet might be different here - for example, if you built vcpkg packages with the `x64-mingw-static` triplet, then `grpc_cpp_plugin` might instead be under the `x64-windows` triplet.

To compile the project using a specific configuration, simply run `meson compile` within that folder. To clean, run `meson compile --clean`.

## Simulator

The simulation is split into two parts: a backend and a frontend. The backend runs the actual simulation and outputs a file called `sim_output.json` wherever it was run from that contains everything that happened during the simulation. The frontend is a local replay tool that can be used to replay and visualize the simulation output file. To use the frontend, open `./sim/visualization/index.html`.

To use the simulation backend, run `/src/navigation_` and pass it the path of a simulation config file. A simple example is provided in `sim_config.json`, which can be run with
```
> .<build folder>/src/navigation_sim robot_config.json sim_config.json sim_output/output.json
```
from within a build directory. Points are given as `[x, y]`, e.g. `[20.5, -34.23]`. Units are all in meters or seconds unless otherwise specified. Config is in JSON format, and units are implied, so don't include them with the value. All config parameters are optional, defaults are noted below.

Simulation config parameters:
- `control_server_url` &mdash; Control server URL address, default is `127.0.0.1:9000`.
- `end_distance` &mdash; If the robot gets within this distance of the goal position, the simulation will end. Default is 0.2 m.
- `time_step` &mdash; Increase in simulation time between iterations. Default is 0.02 s.
- `iteration_limit` &mdash; Maximum iterations simulation will run for, must be an integer. Default is 3000.
- `map_size` &mdash; Width/height of area that the replay frontend will display, centered on `[0,0]`. Must be an integer, default is 8 m.
- `origin` &mdash; Location to center coordinate system at. Specified as `[lat, lon]`, the default is `[39.3289, -76.6215]` which corresponds to Gilman Hall on the Johns Hopkins Campus.
- `start_position` &mdash; The position to start the robot at. Specified as a `point`, default is `[-3.0, -2.0]`.
- `start_angle` &mdash; Angle (measured counterclockwise from standard x-axis) to start robot at. Specified in degrees, default is 90.0&deg;.
- `waypoints` &mdash; List of goal positions for robot to try to drive to, should form a rough path from `start_position` to desired location. Specified as an array of `point`, default is `[[3.5, 3.0]]` - that is, by default the robot will try to drive directly from `[-3.0, -2.0]` to `[3.5, 3.0]`.
- `obstacles` &mdash; A polygonal obstacle, specified as a list of lists of points. Each obstacle must specify at least three points. Do not close the shape (that is, do not specify start point again at end). Default is no obstacles.

## Project Structure

- `src/` - All primary source
  - `common/` - Shared utilities and types
  - `comms/` - Server-robot communications
  - `events/` - Component for sending events/messages between software components
  - `hal/` - Hardware Abstraction Layer
    - `robot_impl/` - HAL implementation for our primary hardware platform
    - `sim_impl/` - HAL implementation for virtual simulation environment
  - `robot/` - Top-level control and navigation software
  - `sim/` - Simulation of sensors and physics for testing `robot`
    - `visualization/` - Simulation replay/visualization tools
- `test/` - Unit & integration tests
- `subprojects/` - Third-party dependencies
