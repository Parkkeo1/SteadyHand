# SteadyHand

An auto-aim/recoil assist program for [Counter-Strike: Global Offensive](https://en.wikipedia.org/wiki/Counter-Strike:_Global_Offensive) (CS:GO) created by Keon (Isaac) Park, CS @ Illinois, and written in C++ using the [Windows API](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632585(v=vs.85).aspx), [openFrameworks](http://openframeworks.cc/), Valve's [Game-State Integration](https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration), and [Crow](https://github.com/ipkn/crow).

**[Demo Video](https://youtu.be/eF7Y9nflMeQ)** from 2018. *There are differences in the UI between the current master branch and video demo (based off the legacy2018 branch), but the key aspects and functionality of the program remain the same. The main differences involve better coding style and documentation/comments in the master branch.*

**Disclaimer:** This program is a proof-of-concept, meaning that it is not meant to be used in live CS:GO games to cheat. If you want to test SteadyHand in-game, run CS:GO with the launch option **-insecure** and play in a offline map/match. I developed this project in order to learn and practice C++, Windows programming, and GUI design.

### Why and How

In CS:GO, every gun recoils in a [set pattern](http://csgoskills.com/academy/spray-patterns/). For example, every time a player fires an M4 rifle in-game, the gun will recoil in a pattern that looks like a flipped-seven. To compensate for recoil, players move their mouse in such a way that the mouse movement mirrors the weapon's recoil across the y=x axis. Because the recoil pattern for each gun is constant, it is feasible to manually record a set mouse movement path (i.e. a macro) for a spray pattern once and have a program (i.e. SteadyHand) exactly replicate it every time the user fires the particular gun in the game.

By simulating mouse movements in place of the user (through the [Windows C++ API](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632585(v=vs.85).aspx)), SteadyHand automatically controls in-game weapon recoil and maintains accurate aim without human error whenever the player fires a gun. SteadyHand also automatically detects real-time which weapon the user has equipped and loads the corresponding pattern to control the recoil. It accomplishes this using [Valve's Game-State Integration](https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration), which sends live information about in-game events in JSON format to a HTTP endpoint.

**Since CS:GO is mainly available on Windows and SteadyHand uses the Windows API, SteadyHand can only be used on the Windows operating system, preferably 7/10.**

### Architecture

SteadyHand is structured as an openFrameworks application with a real-time GUI, background webserver, and a Win32 API backend. **Detailed and in-depth documentation and explanation are present within the files themselves (for the master branch)**, but here's a brief overview:


- GUI: Allows user to switch between "Active" and "Recording" modes, as described below. Displays which weapon SteadyHand detects the user to be using in real-time so that user can validate that the program is recording/aiming for the correct weapon in-game.
    * Relevant files: `ofApp.h, ofApp.cpp`

- Webserver: Detects in real-time the currently equipped weapon of the user in-game so that the rest of the program can record a pattern for or automatically control the recoil for the correct weapon, depending on the current mode ("Active" vs. "Recording").
    * Relevant files: `threads.h, threads.cpp`
    
- Win32 API backend: Handles the "recording weapon recoil patterns" and "recoil aimbot" functionality of SteadyHand. Reads inputs from user's mouse and keyboard via Windows. Simulates user mouse input via Windows.
    * Relevant files: `mouse_handler.h, mouse_handler.cpp, mouse_mover.h, mouse_mover.cpp, mouse_recorder.h, mouse_recorder.cpp`


**As for the changes added in August 2019 to the master branch, they largely involved refactoring the code to have better style (fewer global variables/constants, more OOP, etc) and more documentation.**

### Building and Setup

[Visual Studio (2015 and above)](http://openframeworks.cc/setup/vs/) is recommended/needed for VC++ and the Windows.h file. [openFrameworks](http://openframeworks.cc/) is required. The solution and project files must be generated using the [openFrameworks project-generator](http://openframeworks.cc/learning/01_basics/create_a_new_project/) application, and while doing so, the [ofxDatGui addon](https://braitsch.github.io/ofxDatGui/index.html#installation) needs to be included; follow the detailed instructions on the ofxDatGui and openFrameworks website.

After generating the openFrameworks project/solution, external dependencies need to be included.


- [Boost](https://www.boost.org/users/history/version_1_67_0.html) needs to be downloaded, compiled, and included into the project; follow instructions in this [SO answer](https://stackoverflow.com/questions/2629421/how-to-use-boost-in-visual-studio-2010/2655683#2655683). 
- Afterwards, [Crow](https://github.com/ipkn/crow) will need to be included into the project to set up the HTTP endpoint server to use game-state integration. **This library is already located in this github repository**, so just add the crow/ folder to the additional include directories in your project like you did for ofxDatGui and Boost. 
- This [JSON library](https://github.com/nlohmann/json) is also required to parse JSON payloads. Like Crow, **this library is already in the repository**. Add the nlohmann/ folder to additional include directories as well.

### Usage Instructions

0. Set up your CS:GO client for game-state integration: [detailed instructions on Valve dev website](http://bit.ly/2D32JSu). The required cfg file is in this repository as "gamestate_integration_main.cfg".

1. After building the executable through Visual Studio, launch the program executable. A GUI window will display, showing a dropdown menu with options.
    - Select "Active" to start using SteadyHand's automatic recoil compensation.
    - Select "Recording" Patterns" to record new weapon spray patterns.
    
2. The "Currently Equipped" part of the GUI auto-updates to show which weapon the program currently detects from the player in-game. This will correctly correspond to the actively equipped gun in-game if game-state integration was correctly set up and the state of SteadyHand is set to one of the two states described above.

3. **Active Mode**
    - If SteadyHand was able to locate the patterns/ folder in the same directory of your executable, then it will have loaded all of the spray patterns and correctly compensate recoil for the gun equipped by the player in-game.
    - The bin/patterns/ directory in this repository already contain some example pattern files, but users may need to record their own weapon patterns to better fit their game-specific settings (in-game sensitivity, resolution, more weapons).
    - Test this feature by firing a full spray, half spray, short bursts, etc with various weapons. The cross-hair will also reset to the center automatically.
    
4. **Recording Mode**
    - Once this mode is toggled on, SteadyHand will start recording all mouse movements you make **while the mouse left button is pressed down**. All other mouse movements are tracked but never saved to file. 
    - Test this feature by manually compensating a full spray of a gun of your choice (i.e. SteadyHand will record this because the mouse left button is clicked down while firing in-game). Afterwards, release the left mouse button. Then press "u" on the keyboard to save (to file) the mouse path movement that was just recorded.
    - SteadyHand will detect which gun that was just recorded and appropriately save the movement to the gun's pattern file upon the "u" key press.
    - If you are unsatisfied with the spray that was just recorded, press "i" on the keyboard to clear the temporary buffer and redo the recording. You can also re-record patterns even if you already saved a prior one to file.
    
5. Press ENTER in any state of SteadyHand to quit.
