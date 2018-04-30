# SteadyHand

An auto-aim/recoil assist program for [Counter-Strike: Global Offensive](https://en.wikipedia.org/wiki/Counter-Strike:_Global_Offensive) (CS:GO) written in C++ using the [Windows API](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632585(v=vs.85).aspx), [openFrameworks](http://openframeworks.cc/), Valve's [Game-State Integration](https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration), and [Crow](https://github.com/ipkn/crow).

By Keon (Isaac) Park, CS @ Illinois.

**Disclaimer:** This program is a proof-of-concept, meaning that it is not meant to be used in live CS:GO games to cheat. If you want to test SteadyHand in-game, run CS:GO with the launch option **-insecure** and play in a offline map/match. I developed this project in order to learn and practice C++, Windows programming, and GUI design.

### Why and How

In CS:GO, every gun recoils in a [set pattern](http://csgoskills.com/academy/spray-patterns/). For example, every time a player fires an M4 rifle in-game, the gun will recoil in a pattern that looks like a flipped-seven. To compensate for recoil, players move their mouse in such a way that the mouse movement mirrors the weapon's recoil across the y=x axis. Because the recoil pattern for each gun is constant, it is feasible to manually record a set mouse movement path (i.e. a macro) for a spray pattern once and have a program (i.e. SteadyHand) exactly replicate it every time the user fires the particular gun in the game.

By simulating mouse movements in place of the user (through the [Windows C++ API](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632585(v=vs.85).aspx)), SteadyHand automatically controls in-game weapon recoil and maintains accurate aim without human error whenever the player fires a gun. SteadyHand also automatically detects real-time which weapon the user has equipped and loads the corresponding pattern to control the recoil. It accomplishes this using [Valve's Game-State Integration](https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration), which sends live information about in-game events in JSON format to a HTTP endpoint.

### Usage (Non-Development)

0. I will upload the complete SteadyHand executable file as a release when I am finished. Alongside the .exe file, you will need to set up your CS:GO client for game-state integration: [detailed instructions on Valve dev website](http://bit.ly/2D32JSu).

1. Launch the program executable. A GUI window will display, showing a dropdown menu with options.
    - Select "Aim Assist Mode" to start using SteadyHand's automatic recoil compensation.
    - Select "Record Patterns" to record new weapon spray patterns.
    
2. The "Currently Equipped" part of the GUI auto-updates to show which weapon the program currently detects from the player in-game. This should correctly correspond to the actively equipped gun in-game if game-state integration was correctly set up.

3. **Aim Assist Mode**
    - If SteadyHand was able to locate the patterns/ folder in the same directory of your executable, then it will have loaded all of the spray patterns and correctly compensate recoil for the gun equipped by the player in-game.
    - Test this feature by firing a full spray, half spray, short bursts, etc with various weapons. The cross-hair will also reset to the center automatically.
    
4. **Recording Patterns Mode**
    - Once this mode is toggled on, SteadyHand will start recording all mouse movements you make **while the mouse left button is pressed down**. All other mouse movements are tracked but never saved to file. 
    - Test this feature by manually compensating a full spray of a gun of your choice (i.e. SteadyHand will record this because the mouse left button is clicked down while firing in-game). Afterwards, release the left mouse button. Then press "u" on the keyboard to save (to file) the mouse path movement that was just recorded.
    - SteadyHand will detect which gun that was just recorded and appropriately save the movement to the gun's pattern file upon the "u" key press.
    
5. Press ENTER to quit the program.

***More detailed instructions to come***

### Build (for Development) and Dependencies

[Visual Studio (2015 and above)](http://openframeworks.cc/setup/vs/) is needed for VC++ and Windows.h. The solution and project files must be generated using the [openFrameworks project-generator](http://openframeworks.cc/learning/01_basics/create_a_new_project/) application. The [ofxDatGui addon](https://braitsch.github.io/ofxDatGui/index.html#installation) needs to be included using the project-generator; follow the detailed instructions on the ofxDatGui website.

After generating the openFrameworks project/solution, external dependencies need to be included. 
- [Boost](https://www.boost.org/users/history/version_1_67_0.html) needs to be downloaded, compiled, and included into the project; follow instructions in this [SO answer](https://stackoverflow.com/questions/2629421/how-to-use-boost-in-visual-studio-2010/2655683#2655683). 
- Afterwards, [Crow](https://github.com/ipkn/crow) will need to be downloaded and included into the project to set up the HTTP endpoint server to use game-state integration. Download [this](https://github.com/ipkn/crow/tree/master/include) specific directory in Crow's github repository and include the header and folder. 
- This [JSON library](https://github.com/nlohmann/json) is also required to parse payloads from the CS:GO client.

***More detailed instructions to come***

    


