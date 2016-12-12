# Learn OpenGL
This is my repository for following along with the [Learn OpenGL tutorials](https://www.learnopengl.com/).

## Development
I'm developing this code on MacOS 10.11, with Apple's Clang version 8.00. The build process is managed by [BFG9000](https://jimporter.github.io/bfg9000/), and uses [Ninja](https://ninja-build.org/) to actually build the executable file.

### Quick Start
You can install all of the dependencies with the following commands:
```bash
brew install ninja lxml glfw
pip install bfg9000
```

Use the following commands to get started building the tutorial project:

```bash
git clone https://github.com/Hamled/learn-opengl.git
cd learn-opengl
9k build/
cd build/
ninja all
./learn-opengl
```
