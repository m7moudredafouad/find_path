# Find Path Game

> Remember: I just this for fun and I may not complete the project

Simple game to find the path from one point to another, it is easy to implement other path find algorithm due to the `Strategy Design Pattern` used to apply the algorithm.

The game also is multithreaded:-
* Render thread
* Event handling thread

There is an event manager to handle different types of event, it used `Observer Design Pattern`. May be it doesn't handle all events now but it can easily extended.


### Run
```bash
sudo apt-get install libsfml-dev
. build/setup.sh
build && run
```

## Demo
[![Watch the demo](https://img.youtube.com/vi/5VWmaXAzjQ8/maxresdefault.jpg)](https://youtu.be/5VWmaXAzjQ8)
