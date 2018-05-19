BIN_DIR := bin
PROGRAM := $(BIN_DIR)/ElonaFoobar

FORMAT := clang-format
FIND := find
XARGS := xargs
MKDIR := mkdir


# On Windows, FindXXX.cmake do not work except for FindBoost.cmake So you need
# to specify each path manually.
# Note: You need to link SDL2.lib and SDL2main.lib
# Note: leave "/SUBSYSTEM:WINDOWS" flag as it is.
# CMAKE_ARGS := \
# -DLUA_INCLUDE_DIR="C:\\path-to-lua\\src" \
# -DLUA_LIBRARIES="C:\\path-to-lua\\lua.lib" \
# -DSDL2_INCLUDE_DIR="C:\\path-to-SDL2\\include" \
# -DSDL2_LIBRARIES="C:\\path-to-SDL2\\SDL2*.lib" \
# -DSDL2_IMAGE_INCLUDE_DIR="C:\\path-to-SDL2_image\\include" \
# -DSDL2_IMAGE_LIBRARIES="C:\\path-to-SDL2_image\\SDL2_image.lib" \
# -DSDL2_TTF_INCLUDE_DIR="C:\\path-to-SDL2_ttf\\include" \
# -DSDL2_TTF_LIBRARIES="C:\\path-to-SDL2_ttf\\SDL2_ttf.lib" \
# -DSDL2_MIXER_INCLUDE_DIR="C:\\path-to-SDL2_mixer\\include" \
# -DSDL2_MIXER_LIBRARIES="C:\\path-to-SDL2_mixer\\SDL2_mixer.lib" \
# -DCMAKE_EXE_LINKER_FLAGS="/SUBSYSTEM:WINDOWS /LIBPATH:\"C:\\\\path-to-boost\\\\lib" \
# -G "Visual Studio 15 2017 Win64"


.PHONY: FORCE


all: build


build: $(BIN_DIR) $(PROGRAM)


$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)


$(PROGRAM): FORCE
	cd $(BIN_DIR); cmake .. $(CMAKE_ARGS); make


clean: FORCE
	-@$(RM) -rf $(BIN_DIR)


format: FORCE
	$(FIND) . \( -name "*.cpp" -or -name "*.hpp" \) -print0 | $(XARGS) -0 $(FORMAT) -i

ldoc:
	mkdir -p $(BIN_DIR)/doc
	cp doc/ldoc.css $(BIN_DIR)/doc/ldoc.css
	cp doc/uikit.min.css $(BIN_DIR)/doc/uikit.min.css
	cd $(BIN_DIR) && ldoc -c ../doc/config.ld -l ../doc -s ../doc ../doc/api/

luacheck:
	luacheck --version
	luacheck mods/

rebuild: clean build
