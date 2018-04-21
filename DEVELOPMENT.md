# Final Project - SteadyHand Development Log

**All dates are in 2018.**

#### April 10 - 15
- Setting up project structure, especially the recording and mouse movement functionalities.
- Initially tried to multithread recording mouse movement data, but decided against it, as it seemed to have no performance improvements.
- Using a hidden window to capture raw input from the Windows API, I can record the mouse even when my program is out of focus.
- Test-recorded the AK-47 and M4-A4 spray patterns, loaded the pattern to an object, and successfully simulated the recorded movements, including the delays between each coordinate.
- WIP implementing saving the data to a file to be loaded later, keyboard hotkeys to start/stop recording and save to file.
- TODO: Finish the above and implement multithreading using openFrameworks's ofxThread and start integrating program into an oF project.

#### April 19 - 20
- Implemented saving the recorded mouse data to a file. 
- Fixed some hotkey bugs (now only recognizing presses, not both presses and releases).
- Integrated existing code into a new, oF project/solution. 
- Got the recording functionality working, but having trouble with the WndProc function (does not like OOP), so for now I have the WinMain and WndProc functions in main.cpp. Fortunately, tested recording seemed to be successful.
- Created two new branches- before-oop and wip-oop to mark out points of progress and to make sure I'm not pushing buggy code onto master as much.
- TODO: re-implement the pattern parsing and mouse moving into OOP as well. Afterwards, start working on the GUI/oF stuff. Not sure if I'm still planning to multithread this program.

#### April 20 - 21
- Continued to work in the wip-oop branch to clean up code and fix linker errors. Re-implemented pattern parsing and mouse moving. Tested to make sure both recording and moving works separately with the new structure.
- Solved "already defined in main.obj" errors by using 'extern' keywords for const and global variables.
- Going to merge wip-oop into master branch.
- TODO: Implement detecting mouse left-click press while in 'move' mode so that the recoil is compensated only when user is firing the weapon in-game.
- TODO: start working on the GUI/oF stuff, try without multithreading.