## SceneGraph
Create a thread safe implementation of scene graph data structure.  
<!> To have basic knowledge about scene graph data structure, you may have a look in this [tutorial](https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf) which is part of this [list](https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/)  
<!> Dev environment of this project is based on [StartCli](https://github.com/omarekik/StartCli) boilerplate 

## Build:
By using the following command, github repository will be cloned, create_package script will be invoked and it will : 
 * Build SceneGraph demo (Conan + CMake + C++20 compiler)
 * Run unit tests
 * Run the demo application

```
git clone https://github.com/omarekik/SceneGraph.git && cd SceneGraph/start
chmod +x scripts/create_package.sh && ./scripts/create_package.sh Release
```
<!> The github actions [pipeline](https://github.com/omarekik/SceneGraph/actions/workflows/gcc_msvc_clang.yml) is based on the same steps of the create_package script and the demo application can be downloaded from artifacts for linux and windows.
## Example of execution:
Running the demo will show the following message:
```console
# cd build/main && Start 
Client Client1 is updating due to notification from SceneNode: Root1
Rendering Node: Root1
Rendering Node: Child1_1
Client Client2 is updating due to notification from SceneNode: Root1
Rendering Node: Root1
Rendering Node: Child1_1
Client Client1 is updating due to notification from SceneNode: Root1
Rendering Node: Root1
Rendering Node: Child1_2
Rendering Node: Child1_1
Client Client2 is updating due to notification from SceneNode: Root1
Rendering Node: Root1
Rendering Node: Child1_2
Rendering Node: Child1_1
Client Client1 is updating due to notification from SceneNode: Child1_1
Rendering Node: Child1_1
Rendering Node: Child2_1
Client Client2 is updating due to notification from SceneNode: Child1_1
Rendering Node: Child1_1
Rendering Node: Child2_1
Client Client1 is updating due to notification from SceneNode: Root1
Rendering Node: Root1
Rendering Node: Child2_2
Rendering Node: Child1_2
Rendering Node: Child1_1
Rendering Node: Child2_1
Client Client2 is updating due to notification from SceneNode: Root1
Rendering Node: Root1
Rendering Node: Child2_2
Rendering Node: Child1_2
Rendering Node: Child1_1
Rendering Node: Child2_1
```


