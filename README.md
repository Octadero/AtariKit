# Swift API for 'The Arcade Learning Environment'

Please, visit [Arcade Learning Environment](https://github.com/mgbellemare/Arcade-Learning-Environment) repository.

The Arcade Learning Environment (ALE) is a simple object-oriented framework that allows researchers and hobbyists to develop AI agents for Atari 2600 games. It is built on top of the Atari 2600 emulator [Stella](https://stella-emu.github.io/) and separates the details of emulation from agent design. This [video](https://www.youtube.com/watch?v=nzUiEkasXZI) depicts over 50 games currently supported in the ALE.

For an overview of our goals for the ALE read [The Arcade Learning Environment: An Evaluation Platform for General Agents](http://www.jair.org/papers/paper3912.html). If you use ALE in your research, we ask that you please cite this paper in reference to the environment (BibTeX entry at the end of this document). Also, if you have any questions or comments about the ALE, please contact us through our [mailing list](https://groups.google.com/forum/#!forum/arcade-learning-environment).


Feedback and suggestions are welcome and may be addressed to any active member of the ALE team.

### Features
- Object-oriented framework with support to add agents and games.
- Emulation core uncoupled from rendering and sound generation modules for fast emulation with minimal library dependencies.
- Automatic extraction of game score and end-of-game signal for more than 50 Atari 2600 games.
- Multi-platform code (compiled and tested under OS X and several Linux distributions, with Cygwin support).
- Communication between agents and emulation core can be accomplished through pipes, allowing for cross-language development (sample Java code included).
- Python development is supported through ctypes.
- Agents programmed in C++ have access to all features in the ALE.
- Visualization tools.

## Quick start


Install main dependences:
```
$ sudo apt-get install libsdl1.2-dev libsdl-gfx1.2-dev libsdl-image1.2-dev cmake
```
On macOS
```
$ brew install sdl
```

Clone ALE
```
$ git clone https://github.com/mgbellemare/Arcade-Learning-Environment.git
```

Compilation:

```
$ cd Arcade-Learning-Environment
$ cmake -DUSE_SDL=ON -DUSE_RLGLUE=OFF -DBUILD_EXAMPLES=OFF
$ make -j 4
$ sudo make install
```
## Swift package
Add to your package dependencies:

```
dependencies: [
    .package(url: "https://github.com/Octadero/AtariKit.git", from: "0.0.1"),
],
```

Create game using one of the roms and launch game:
```
let game = try Environment(romPath: "%pathToSources%/AtariKit/atari-roms/pong.bin")

// do action.
let reward = game.send(action: .playerAleft)

// get RGB dump of screen.
let screen = game.screenRGBBuffer()

// get screen size.
let size = game.screenSize()

// check if game is over.
if game.isOver { }
```
