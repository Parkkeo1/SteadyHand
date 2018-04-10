# My Project Proposal

An auto-aim/recoil assist program for [Counter-Strike: Global Offensive](https://en.wikipedia.org/wiki/Counter-Strike:_Global_Offensive) (CS:GO) written in C++ using the [Windows API](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632585(v=vs.85).aspx), [openFrameworks](http://openframeworks.cc/), Valve's [Game-State Integration](https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration), and [Simple-Web-Server](https://github.com/eidheim/Simple-Web-Server)<sup>[[*]](#crow)</sup>.

### Premise & Introduction

Counter-Strike is a first-person multiplayer shooter game. For my project, I will focus on manipulating, analyzing, and interacting with the recoil mechanics of weapons in the game.

In CS:GO, every gun recoils in a [set pattern](http://csgoskills.com/academy/spray-patterns/). For example, every time a player fires an M4 rifle in-game, the gun will recoil in a pattern that looks like a flipped-seven. To compensate for recoil, players move their mouse in such a way that the mouse movement mirrors the weapon's spray pattern across the y=x axis.

Because the spray pattern for a given weapon is constant, it is theoretically feasible to record a set mouse movement path for a pattern once (ideally with perfect recoil control) and have a program exactly replicate it (i.e. simulating mouse movements using coordinates/paths) every time the particular gun is fired by the user in the game.

### Details & Goals

By simulating mouse movements in place of the user (through the [Windows C++ API](https://msdn.microsoft.com/en-us/library/windows/desktop/ms632585(v=vs.85).aspx)), the program will automatically control in-game weapon recoil and maintain accurate aim without human error whenever the player fires a gun while playing Counter-Strike. More specifically, the user would be able to trigger the program's aim-assist by pressing a certain key (say Mouse1) while playing the game.

Also, the program would (ideally) automatically load the appropriate weapon spray pattern by detecting which the gun the player has equipped in real-time. Thus, whenever the user fires any gun in-game, the program would automatically compensate for the recoil using the correct pattern. 
To accomplish this, the program will use [Simple-Web-Server](https://github.com/eidheim/Simple-Web-Server)<sup>[[*]](#crow)</sup>, a HTTP requests library, and Valve's [game-state integration](https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration) to receive JSON payloads from the CS:GO client that contain real-time information about in-game events, including the current gun equipped by the player.

openFrameworks, more specifically its addons ofxGui and [ofxDatGui](https://braitsch.github.io/ofxDatGui/), will be used for the front-end interface of the program. The GUI will allow users to modify/view individual spray patterns, configure hotkeys for usage, and adjust other settings.<sup>[[*]](#gui)</sup> This GUI will be the "window" requirement of the project.

### My Prior Experience

In the past, I have used CS:GO's game-state integration service to create an [in-game performance/statistics tracker](https://github.com/Parkkeo1/CS-Py) for players using Python, Flask, and SQL. Regarding my experience in Counter-Strike, I have 1300+ hours played in-game.

#### Notes

<a name="crow">**I may instead opt to use [Crow](https://github.com/ipkn/crow), a Flask-inspired micro web framework for C++, as the project goes along.* </a>

<a name="gui">**The exact contents and functionality of the GUI of the program is still tentative, and I'm still brainstorming ideas.* </a>




