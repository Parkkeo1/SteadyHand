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


#### April 21 - 22
- The following changes are mostly on the new WIP branch, of-testing.
- Successfully implemented the Crow web framework to create a HTTP endpoint server that receives and parses JSON payloads from the CSGO client to detect which weapon the player has equipped.
- Integrated the Crow stuff into openFrameworks's ofxThread to have the server run in a separate thread while the program runs.
- Program now automatically detects which weapon the player has equipped and modifies its "curr_pattern" pattern pointer to the appropriate (already) loaded pattern.
- All of the available patterns are loaded into the program on startup from the patterns/ folder in the bin/ directory.
- So far set it to recognize only three weapons: AK-47, M4-A4, M4-A1S.
- Implemented some basic ofxDatGui GUI stuff, most notably the dropdown menu that lets the user switch between using and recording.
- TODO: Keep integrating more of my previous code into the oF project. Flesh out GUI options. Figure out a way to switch efficiently between recording and using.

#### April 26 - 27
- Main focus is on integrating the mouse mover code into oF. Currently implemented as a thread, but may instead just opt to use objects with start/stop functions for the message loops.
- Fixed (after hours of SO, WinAPI docs, and debugging) issues with Get/SetWindowLongPtr to recognize & register input devices appropriately.
- WndProc functions are now object-oriented and class members in the thread class. May abandon threading for mouse moving/recording in favor of objects.
- Current Issues to fix: GUI is somehow inaccessible while running the mouse mover thread. Make sure MoveWithPattern() does not segfault while mover thread is tracking mouse clicks.

#### April 27 (Cont.)
- Changed implementation of mouse mover to OOP instead of threads. Still trying to figure out how to exit safely/correctly from the message loop without exiting the entire application.
- Also having issue where the message loop (where the program tracks my mouse movements/clicks) has full control of the window and thus doesnt let me interact with the GUI window.
- Successfully using oF GUI window (instead of a hidden one) to hook to the mouse movements (fewer windows needed).
- Integrated MoveWithPattern to new WndProc functions for mouse mover. Program now executes the pattern upon m_left press.
- Uploaded demo video for code review checkpoint: https://youtu.be/6U3dU2RN2sM
- TODOs: Fix Window control issue and switching between states. Thread-ify MoveWithPattern stuff to make the recoil control respond to mid-spray m_left releases (when I release m1 during a spray it should ideally stop controlling).
- TODOs: Manually record more patterns using MouseMeat and test recoil control with different weapons on the fly.

#### April 28 - 29
- Successfully tested automatic weapon detection and dynamic pattern control.
- Threadified MoveWithPattern using std::thread, so recoil control can now stop midway through a spray.
- Adjusted some minor values in MoveWithPattern. Some properties in the PatternObject are now deprecated, so I need to according delete them.
- TODOs: Fix Window control issue and switching between states. Refactor code to remove deprecated variables and values.

#### April 29 (Cont.)
- Refactored mouse_mover code as subclass of mouse_handler (same for mouse_recorder). Successfully integrated mouse_recorder into oF app.
- Removed deprecated code, but still more to do; may change patternobject from struct to just a typedef for a vector of tuples.
- Updated GUI components to reflect current weapons for both recording and using states.
- TODOs: More refactoring (especially PatternObject). Work on README. Continue to work on window control bug. Start adding more weapons?
