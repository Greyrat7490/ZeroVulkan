lib_name = ZeroVulkan
out_name = Test

lib_folder = $(lib_name)/src
out_folder = $(out_name)/src

LIB_LIBS = -lpthread -lX11 -lxcb -lvulkan
OUT_LIBS = -lpthread -lX11 -lxcb -lvulkan # lib will be added later so it will automatically choose release/debug

CPPFLAGS = -Wall -Wextra -pedantic -std=c++17 -MD
CPPFLAGS += -I/usr/local/include -I/usr/include -L/usr/local/lib

LIB_CPPFLAGS = $(CPPFLAGS) -I$(lib_folder) $(LIB_LIBS)
OUT_CPPFLAGS = $(CPPFLAGS) -I$(lib_folder) -I$(out_folder) $(OUT_LIBS)


out_release = build/release/$(out_name)
lib_release = build/release/$(lib_name).a

out_debug = build/debug/$(out_name)
lib_debug = build/debug/$(lib_name).a

lib_src_files = $(shell find $(lib_folder) -name "*.cpp")
out_src_files = $(shell find $(out_folder) -name "*.cpp")

lib_debug_objs = $(patsubst $(lib_folder)/%.cpp, build/debug/obj/$(lib_name)/%.o, $(lib_src_files))
lib_release_objs = $(patsubst $(lib_folder)/%.cpp, build/release/obj/$(lib_name)/%.o, $(lib_src_files))

out_debug_objs = $(patsubst $(out_folder)/%.cpp, build/debug/obj/$(out_name)/%.o, $(out_src_files))
out_release_objs = $(patsubst $(out_folder)/%.cpp, build/release/obj/$(out_name)/%.o, $(out_src_files))

.PHONY: all clean release debug run test

all: release

clean:
	rm -rf build

release: LIB_CPPFLAGS += -O2
release: OUT_CPPFLAGS += -O2
release: $(out_release)

debug: LIB_CPPFLAGS += -g -O0 -D Z_DEBUG
debug: OUT_CPPFLAGS += -g -O0 -D Z_DEBUG
debug: $(out_debug)

run: release
	@echo ""
	@echo "running release version..."
	$(out_release)

test: debug
	@echo ""
	@echo "running debug version..."
	$(out_debug)


# create output executable
$(out_release): $(lib_release) $(out_release_objs)
	g++ $(out_release_objs) -o $(out_release) $(OUT_CPPFLAGS) $(lib_release)

$(out_debug): $(lib_debug) $(out_debug_objs)
	g++ $(out_debug_objs) -o $(out_debug) $(OUT_CPPFLAGS) $(lib_debug)

# create static library
$(lib_release): $(lib_release_objs)
	ar rcs $(lib_release) $(lib_release_objs)
	@echo ""; echo ""; echo ""

$(lib_debug): $(lib_debug_objs)
	ar rcs $(lib_debug) $(lib_debug_objs)
	@echo ""; echo ""; echo ""

# compile object files
build/release/obj/ZeroVulkan/%.o: ZeroVulkan/src/%.cpp
	@echo ''
	@mkdir -p $(shell dirname $@)
	g++ -c $< -o $@ $(LIB_CPPFLAGS)
build/debug/obj/ZeroVulkan/%.o: ZeroVulkan/src/%.cpp
	@echo ''
	@mkdir -p $(shell dirname $@)
	g++ -c $< -o $@ $(LIB_CPPFLAGS)

build/release/obj/Test/%.o: Test/src/%.cpp
	@echo ''
	@mkdir -p $(shell dirname $@)
	g++ -c $< -o $@ $(OUT_CPPFLAGS)
build/debug/obj/Test/%.o: Test/src/%.cpp
	@echo ''
	@mkdir -p $(shell dirname $@)
	g++ -c $< -o $@ $(OUT_CPPFLAGS)

-include $(lib_debug_objs:.o=.d)
-include $(out_debug_objs:.o=.d)
-include $(lib_release_objs:.o=.d)
-include $(out_release_objs:.o=.d)
