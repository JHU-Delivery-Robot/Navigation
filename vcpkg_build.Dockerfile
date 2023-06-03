FROM ubuntu:22.04 as build

RUN apt-get update --fix-missing -y && apt-get install -y \
# For cloning vcpkg
    git \
# vcpkg's dependencies
    curl zip unzip tar \
# For building packages with vcpkg
    gcc g++ pkg-config cmake ninja-build python3

WORKDIR /opt/

RUN git clone https://github.com/Microsoft/vcpkg.git && ./vcpkg/bootstrap-vcpkg.sh

WORKDIR /src/robot_navigation
COPY ./vcpkg.json .

RUN /opt/vcpkg/vcpkg install --triplet x64-linux

FROM ubuntu:22.04 as package
COPY --from=build /src/robot_navigation/vcpkg_installed/x64-linux/ /src/robot_navigation/vcpkg_installed/x64-linux/
