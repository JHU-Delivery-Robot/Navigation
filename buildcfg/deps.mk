# Source files needed only for the simulation program
# Build system include file
# Only include once, and do not use directly!

# root paths
ROBOT_ROOT       := $(PROJECT_ROOT)/robot
SIM_ROOT         := $(PROJECT_ROOT)/sim
TEST_COMMON_ROOT := $(PROJECT_ROOT)/tests/common

# library deps
LIBSTB_OBJS      := stb_image stb_image_write
LIBSTB_DEPS      := $(call objpath,$(LIBSTB_OBJS),$(PROJECT_ROOT)/lib/stb/$(BUILDIR))
LIBCATCH_OBJS    := catch_main
LIBCATCH_DEPS    := $(call objpath,$(LIBCATCH_OBJS),$(PROJECT_ROOT)/lib/catch/$(BUILDIR))

# hal deps
HAL_OBJS := sensor_hal motor_hal

# sub-project deps
ROBOCTRL_OBJS := main fastmath potential_map speed_controller
# dependency paths
SIM_COMMON   := motion obstacle_map sensor-gen
# Source files needed only for the simulation program
SIM_OBJS     := main $(SIM_COMMON)
TEST_COMMON  :=
