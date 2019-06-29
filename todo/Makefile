BUILD_TOOL=ninja
BUILD_TOOL_CMAKE="Ninja"
BUILD_DIR=build

CMAKE_VARS=

.PHONY: all _build clean statsbuild

all: _build

_build: $(BUILD_DIR)
	@cd $^ && ${BUILD_TOOL}

$(BUILD_DIR):
	@mkdir -p $@
	@cd $(BUILD_DIR) && cmake -G ${BUILD_TOOL_CMAKE} ..

clean:
	$(RM) -rf ${BUILD_DIR}

help:
	@echo "Available targets:"
	@echo "  - all: Builds binaries in the '${BUILD_DIR}' directory"
	@echo "  - clean: Removes the '${BUILD_DIR}' directory"
