# SceneGraph
Create a multithread safe implementation of scene graph data structure.  
<!> Dev environment of this project is based on https://github.com/omarekik/StartCli

## Build:
By using the following command, docker image will be built from github repository and by running the container, it will : 
 * Build SceneGraph demo (Conan + CMake + C++20 compiler)
 * Run unit tests
 * Run examples and keep bash opened to try more examples of execution
Build solution localy by using create_package script or conan commands as mentioned in the script: 
```
git clone https://github.com/omarekik/SceneGraph.git && cd SceneGraph/start
chmod +x scripts/create_package.sh && ./scripts/create_package.sh Release
```
## Example of execution:
Running the CLI with the program option -h will show the help message:
```console
# cd build/main && Start -h
Program options:
  -h [ --help ]             display help menu
  -c [ --create ]           option boolean
  -w [ --width ] arg (=10)  option integer with default value

```

## Notes:
 * For every new project, in the CMakeLists.txt, it is possible to specify the target_compile_options of warning by using the variable compiler_warnings initialized in start/scripts/compiler_warnings.cmake. And also it is possible to set_target_properties of CXX_CLANG_TIDY to specify clang-tidy as static anlyzer for checking conformity with cppcoreguidelines, and in case of warning it will block the build till fixed as configured in start/scripts/static_checking.cmake

## VSCode:
 * In .vscode/tasks.json, a task named "code_format" is created. To format the code following clang format style defined in .clang-format file, type `ctrl+shift+b` and select the format task.
 * In .vscode/tasks.json, two tasks named "build_debug"/"build_release" are created. To launch the build, type `ctrl+shift+b` and select from the build tasks. 
 * Debugging configuration is defined in .vscode/launch.json. For mac/linux environment, vscode extension of CodeLLDB and lldb debugger are required. For windows environment, cppvsdbg is needed. To start debugging type `F5`, and to run without debugging type `ctrl+F5`.  
 * For debugging CMake scripts, use CMake Tools extension from microsoft as mentioned in these links [\[1](https://devblogs.microsoft.com/cppblog/introducing-cmake-debugger-in-vs-code-debug-your-cmake-scripts-using-open-source-cmake-debugger/), [2](https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/debug.md), [3\]
](https://devblogs.microsoft.com/cppblog/debug-vcpkg-portfiles-in-cmake-script-mode-with-visual-studio-code/). In ubuntu environment, in case of vscode extension is not detecting the cmake binary installed through Snap, try to install from source the cmake version 3.27.9.  

## Git pre-commit hook:
As .git folder is versioned, the pre-commit hook can not be cloned from repo and it should be added manually. Under .git/hooks add a file named pre-commit without extension, and put this content in it:  
```
#!/bin/sh

# Relative path to your custom script within the project folder
custom_script_pre_commit="./start/scripts/format_code.sh"

# Run the custom script
if [ -f "$custom_script_pre_commit" ]; then
  echo "pre-commit hook: Running ${custom_script_pre_commit}"
  sh "$custom_script_pre_commit"
else
  echo "${custom_script_pre_commit} not found. Commit aborted."
  exit 1
fi
```  

## Known issues:
 * If there is unstaged source/header files, the script start/scripts/format_code.sh will show "Formatting is wrong" even if clang format did not modify any file. Actually, the script should be used after staging all source files and before commiting, so its result gonna be accurate.

