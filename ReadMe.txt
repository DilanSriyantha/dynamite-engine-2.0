You have to configure the Visual Studio 2022 project in the right order to link all the dependencies and libraries 
I have used in my project. Please follow the steps below in order to configure you Visual Studio 2022 project.

1. Right click on the project in the solution explorer and click properties.

2. Select "All Configurations" from the configurations dropdown.

3. Select "All Platforms" from the plaforms dropdown.

2. In the properties window, expand C/C++ menu and click on General. (C/C++ -> General)

3. Paste the following directories in the "Additional Include Directories" list.

$(ProjectDir)DynamiteEngine\Deps/nlohmann_json_single_include
$(ProjectDir)DynamiteEngine\Deps/libcurl-vc14-x86-release-dll-ipv6-sspi-schannel/include
$(ProjectDir)DynamiteEngine\src/engine/UIElements
$(ProjectDir)DynamiteEngine\src/engine/Systems
$(ProjectDir)DynamiteEngine\src/engine/Utils
$(ProjectDir)DynamiteEngine\src/engine/Types
$(ProjectDir)DynamiteEngine\src/engine/Interfaces
$(ProjectDir)DynamiteEngine\src/engine/Components
$(ProjectDir)DynamiteEngine\src/engine/ECS
$(ProjectDir)DynamiteEngine\src/engine
$(ProjectDir)DynamiteEngine\src
$(ProjectDir)DynamiteEngine\Deps/SDL2_mixer/include
$(ProjectDir)DynamiteEngine\Deps/SDL2_ttf/include
$(ProjectDir)DynamiteEngine\Deps/SDL2_image/include
$(ProjectDir)DynamiteEngine\Deps/SDL2/include

4. Select "Linker" in the properties window and click on "General" (Linker -> General)

5. Paste the following directories in the "Additional Libraries Directories" list.

$(ProjectDir)DynamiteEngine\Deps/libcurl-vc14-x86-release-dll-ipv6-sspi-schannel/lib
$(ProjectDir)DynamiteEngine\Deps/SDL2_mixer/lib/x86
$(ProjectDir)DynamiteEngine\Deps/SDL2_ttf/lib/x86
$(ProjectDir)DynamiteEngine\Deps/SDL2_image/lib/x86
$(ProjectDir)DynamiteEngine\Deps/SDL2/lib/x86

6. Click on "Input" under the "Linker" options listed on the properties window.

7. Paste the following directories in the "Additional Dependencies" list.

SDL2.lib
SDL2main.lib
SDL2_image.lib
SDL2_ttf.lib
SDL2_mixer.lib
libcurl.lib