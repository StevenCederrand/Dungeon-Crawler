# Dungeon-Crawler
Repo för lilla spel projektet DV1572.

Rules of code: 
- You may use any bracketing. 
- Member variables are to start with "m_".
- Function names are to start with small character. 

Dates: 
[x] Pitch: 2/4
- High Concept: 5/4
[x] Codebase: 5/4

DOCUMENTATION

Setup: 
> To setup the project to use fmod you have to do the following. If 
you're running a x64 windows then get the fmod dll's from the x32 lib 
folder. Paste them in C:\Windows\System32. Then take the same dll's and 
paste them in C:\Windows\SysWOW64. 
This should set everything up.

Console Log:
>Include "Log.h" in the cpp file. Call one of these with the information you want to log.
- LOG_INFO(" log this string for an example ");
- LOG_TRACE" log this string for an example ");
- LOG_WARNING(" log this string for an example ");
- LOG_FATAL(" log this string for an example ");

How to change state within a state:
>include "StateManager.h" in the cpp file
Call either: 
m_stateManager->pushTemporaryState(State*);
"This will push a new state in the state array ( Use popState to get back to the old state )

 m_stateManager->setState(State*);
 "This will delete all states in the state array and add the specified state."
