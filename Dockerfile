FROM vcpkg_build as build

RUN apt-get update --fix-missing -y && apt-get install -y \
        gcc g++ pkg-config cmake ninja-build python3 python3-pip
RUN pip install --upgrade pip && pip install meson

ARG CMAKE_PREFIX_PATH=/src/robot_navigation/vcpkg_installed/x64-linux/
WORKDIR /src/robot_navigation/

COPY ./meson.build .
COPY ./subprojects ./subprojects
COPY ./src ./src
COPY ./test ./test

RUN meson setup build
RUN ./vcpkg_installed/x64-linux/tools/protobuf/protoc --proto_path="src/comms" --cpp_out="src/comms" ./src/comms/routing.proto
RUN ./vcpkg_installed/x64-linux/tools/protobuf/protoc --proto_path="src/comms" --grpc_out="src/comms" \
        --plugin=protoc-gen-grpc="./vcpkg_installed/x64-linux/tools/grpc/grpc_cpp_plugin" ./src/comms/routing.proto

RUN ./vcpkg_installed/x64-linux/tools/protobuf/protoc --proto_path="src/comms" --cpp_out="src/comms" ./src/comms/development.proto
RUN ./vcpkg_installed/x64-linux/tools/protobuf/protoc --proto_path="src/comms" --grpc_out="src/comms" \
        --plugin=protoc-gen-grpc="./vcpkg_installed/x64-linux/tools/grpc/grpc_cpp_plugin" ./src/comms/development.proto

RUN meson compile -C build

FROM ubuntu:22.04 as robot

COPY --from=build /src/robot_navigation/build/src/navigation_sim /usr/local/bin/robot_navigation/

ENTRYPOINT [ "/usr/local/bin/robot_navigation/navigation_sim" ]
