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

Next, install Git, Python 3, the Ninja build system, CMAke, and the GCC compiler suite. Using scoop, this is simply:
```
scoop install git python ninja gcc cmake
```

You will also need to either have a recent version of Visual Studio already installed (*not* VS Code). If you do not, please install the Visual Studio Build Tools. Go to [https://visualstudio.microsoft.com/downloads/](https://visualstudio.microsoft.com/downloads/), scroll down to "All Downloads", then > Tools for Visual Studio, and download and install "Build Tools for Visual Studio \<year\>".

Next, [install vcpkg](https://vcpkg.io/en/getting-started.html).

Next, clone this repository by navigating to the folder you want to keep Deliverbot software in and then running `git clone --recurse-submodules https://github.com/JHU-Delivery-Robot/Navigation.git`.

Next, we install the Meson build system. This should just be `pip install meson`. Run `meson --version` to make sure it is installed properly &mdash; if meson cannot be found your Python scripts are likely not in your PATH. If you are using an existing installation of Python and run into this issue, just uninstall Python, re-install it using scoop, and then install Meson again. 

### Linux

Use your system package manager to acquire Git, the GCC compiler suite, CMake, a recent version of Python 3, and the Ninja build system. If you are using Ubuntu/Debian, this is simply
```
sudo apt install git gcc python3 ninja-build cmake
```

Next, [install vcpkg](https://vcpkg.io/en/getting-started.html).

Next, clone this repository into the folder of your choice using `git clone --recurse-submodules https://github.com/JHU-Delivery-Robot/Navigation.git`.

Next, we install meson. This should just be `pip install meson`. Run `meson --version` to make sure it is installed properly, if meson cannot be found you need to place Python package scripts onto your PATH.

### Mac

Please check whether your Mac has an Intel CPU, or an ARM M1 (aka "Apple Silicon").

First, install brew from [here](https://brew.sh/)

Then, ensure that you have python 3 installed by running `python --version` (or `python3 --version`) in your terminal. If you do not, install the latest python from [here](https://www.python.org/downloads/)

Install the following by running the associated commands:
- GCC: `brew install gcc`
- Meson: `pip install meson`
- Ninja: `brew install ninja`
- pkg-config: `brew install pkg-config`
- CMake: `brew install cmake`

Next, enable python certs by doing the following:
1. Open your finder and go to **Applications** in the left panel
2. Find and open the Python 3.xx folder
3. Double-click the *Install Certificates.command* file. Let its terminal run to completion, and input whatever you need to in order to let it run

Next, [install vcpkg](https://vcpkg.io/en/getting-started.html).

Clone this repository using `git clone --recurse-submodules https://github.com/JHU-Delivery-Robot/Navigation.git` and open the Navigation directory in a terminal.

## Building

Once you've got everything installed, you need to build and install some third-party dependencies. From *inside this project's folder* (aka the "Navigation" folder), run `<path to vcpkg install>/vcpkg install --triplet <vcpkg-triplet>` where `<vcpkg-triplet>` is the vcpkg triplet identifier for your platform. For Linux, you should use the `x64-linux` triplet, for Windows with MinGW gcc use `x64-mingw-static`, and for MacOS use `x64-osx` for Intel Macs and `arm64-osx` for ARM M1 Macs. *Note* This make take a while to complete, but this only needs to be done once during the first install, or when we update our vcpkg dependencies.

*Note* If you are using Mac, before doing the next step first run
```
export CC="/usr/local/opt/gcc/bin/gcc-11"
export CXX="/usr/local/opt/gcc/bin/g++-11"
```

Next, you will need to create build directories for Meson. First, you must create an environment variable `CMAKE_PREFIX_PATH` and set it to `<navigation project>/vcpkg_installed/<triplet>` &mdash; make sure to use the *full* absolute path. Then, to create a build configuration called `build` with the default settings, run `meson setup build`. If you want to create a release configuration, run `meson setup release -Dbuildtype=release -Db_lto=true -Doptimization=2`. For development purposes, you should use a default debug configuration.

You will also need to generate gRPC stubs for both `src/comms/development.proto` and `src/comms/routing.proto`. To generate gRPC stubs from a `.proto` file, run the following commands (in the project root directory):
```
./vcpkg_installed/<triplet>/tools/protobuf/protoc --proto_path="src/comms" --cpp_out="src/comms" <path to .proto>
./vcpkg_installed/<triplet>/tools/protobuf/protoc --proto_path="src/comms" --grpc_out="src/comms" --plugin=protoc-gen-grpc="./vcpkg_installed/<triplet>/tools/grpc/grpc_cpp_plugin" <path to .proto>
```
*Note* If you are using Windows, replace "grpc_cpp_plugin" with "grpc_cpp_plugin.exe" when running the second command.

*Note* the triplet might be different here - for example, if you built vcpkg packages with the `x64-mingw-static` triplet, then `grpc_cpp_plugin` might instead be under the `x64-windows` triplet.

To compile the project using a specific configuration, simply run `meson compile` within that folder. To clean, run `meson compile --clean`.

## Robot Config

To run, the robot software must be provided with a JSON config file. See [Server/Certificates](https://github.com/JHU-Delivery-Robot/Server#certificates) for help setting up certificates for local testing.

Parameters:
- `control_server_url` &mdash; Control server URL address, for local testing use `localhost:443`.
- `root_ca_cert` &mdash; File path to root CA certificate, for local testing use "./certs/local_test_ca.crt" or similar.
- `robot_cert` &mdash; File path to robot certificate, for local testing use "./certs/local_test_robot.crt" or similar.
- `robot_key` &mdash; File path to robot certificate key, for local testing use `./certs/local_test_robot.key` or similar.
- `lidar_port` &mdash; LiDAR device port, e.g. `COM3`.

## Simulator

The simulation is split into two parts: a backend and a frontend. The backend runs the actual simulation and outputs a file called `sim_output.json` wherever it was run from that contains everything that happened during the simulation. The frontend is a local replay tool that can be used to replay and visualize the simulation output file. To use the frontend, open `./sim/visualization/index.html`, and go to the "Replay tool" section.

To use the simulation backend, run `<build folder>/src/navigation_sim` and pass it the path of a simulation config file. A simple example is provided in `sim_config.json`, which can be run with
```
./<build folder>/src/navigation_sim robot_config.json sim_config.json sim_output/output.json
```
from within a build directory.

To create or edit the simulation config, use the "Config editor" section of the simulation frontend, and either load an existing config file, or create one from scratch. To run your config, select "Save config" to download it, and run it in the simulation, and then watch the replay in the "Replay tool".

### Simulation config format details

Points are given as `[x, y]`, e.g. `[20.5, -34.23]`. Units are all in meters or seconds unless otherwise specified. Config is in JSON format, and units are implied, so don't include them with the value. All config parameters are optional, defaults are noted below.

Simulation config parameters:
- `end_distance` &mdash; If the robot gets within this distance of the goal position, the simulation will end. Default is 0.2 m.
- `time_step` &mdash; Increase in simulation time between iterations. Default is 0.02 s.
- `iteration_limit` &mdash; Maximum iterations simulation will run for, must be an integer. Default is 3000.
- `map_size` &mdash; Width/height of area that the replay frontend will display, centered on `[0,0]`. Must be an integer, default is 8 m.
- `origin` &mdash; Location to center coordinate system at. Specified as `[lat, lon]`, the default is `[39.3289, -76.6215]` which corresponds to Gilman Hall on the Johns Hopkins Campus.
- `robot` &mash; initial pose of robot
  - `position` &mdash; The position to start the robot at. Specified as a `point`, default is `[-3.0, -2.0]`.
  - `angle` &mdash; Angle (measured counterclockwise from standard x-axis) to start robot at. Specified in degrees, default is 90.0&deg;.
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
