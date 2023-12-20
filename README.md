# Goal-Oriented Action Planning
The project is a follow up to a bachelor thesis as part of the study program Animation&Game at the UAS Darmstadt. Visual assets such as sprites are developed by third party. 

It provides a solution to a Goal-Oriented Action Planning AI which uses simple threading, plan validation, recursive goal search and more ( [My GOAP implementation](https://yassineboutaouas.github.io/Portfolio/goap.html)).
It is based on the implementation from Jeff Orkin and Peter Higley ([See](https://www.gdcvault.com/play/1022019/Goal-Oriented-Action-Planning-Ten)).

<img width="1920" alt="Screenshot of working C++ program" src="https://github.com/YassineBoutaouas/GOAP_Project/assets/127434490/d06732b4-c663-4012-8f60-17ecd7e25b0b">
Screenshot of the working C++ application. Current plan highlighted yellow.

## Project Structure
The project was developed with C++ 17 for Windows. It uses the ([sfml](https://www.sfml-dev.org/)) to allow for further development using game assets, graphics, sounds etc.

1. AI_Core (.lib): Contains the GOAP-AI itself and all of the classes it needs to operate.
   - References:
    - Profiler(.lib)
2. Profiler (.lib): A simple project that generates profiling data that can be visually shown in Google Chrome based on this implementation - ([See](https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e))
3. Game_Core (.lib): Contains general game related classes and a game manager which provides an event system to create a game loop. Additionally, it manages object instances.
   - References:
    - Profiler(.lib)
    - Sfml
4. Sample_Project_Worker (.lib): Implements the GOAP AI and provides an example of the classes and components needed to execute it. It also implements placholder visuals using the sfml library.
   - References:
    - AI_Core(.lib)
    - Game_Core(.lib)
    - Sfml
5. GOAP_Project (.exe): Provides an entry point to the program and calls game manager events to create a game loop.
   - References:
    - Sfml
    - Sample_Project_Worker(.lib)
  
  ## Important directories:
  The GOAP AI together with the profiler is designed to work independently to the rest of the projects.

  - .../AI_Core
  - .../Profiler

## In order to create and extend an agent:

1. Create a class instance of Agent
2. Create a min. of two WorldState instances to represent a global world state and a goal.
3. Populate them with values using the WorldState.Add(...) function
4. Use the Agent.ActionPlanner->AddWorldState(worldState) and the Agent.ActionPlanner->AddGoal(goal) function to add the world state and the goal respectively.
5. Create new actions, preovide them with preconditions and effects through the use of Action.Preconditions.Add(...) or Action.Effects.Add(...)
6. Add the actions to the planner using Agent.ActionPlanner->AddAction(...)
7. Last, a plan can be requested using Agent.ActionPlanner->RequestPlan()

**A full example implementation is provided in the Sample_Project_Worker(.lib)**

### Full list of third party assets

- Graphics library: [Sfml](https://www.sfml-dev.org/)
- Texture assets (character & background): [Oak Woods — Environment Asset](https://brullov.itch.io/oak-woods)
- Font assets: [VCR OSD Mono](https://www.dafont.com/de/vcr-osd-mono.font)
