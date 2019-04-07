# Welcome to the Escape-the-Clocktower wiki!

<img src="https://github.com/cosc345etc/Escape-the-Clocktower/blob/master/Escape%20The%20Clocktower.png" alt="Your image title" width="1500"/>

# Table of contents
1. [What Is It?](##Introduction)
2. [How Are We Going To Make It?](##HowAreWeGoingToMakeIt?)
3. [Obstacles](##Obstacles)
4. [Who's Making It?](##Who'sMakingIt?)
5. [How Long Will It Take To Make?](##HowLongWillItTakeToMake?)
6. [What Else Already Does What Our Program Does?](##WhatElseAlreadyDoesWhatOurProgramDoes?)
7. [What Does Ours Do Differently?](##WhatDoesOursDoDifferently?)

<a name="What Is It?"></a>
## What Is It? 
Escape the Clocktower (EtC) is a first-person grid-based dungeon-crawler being developed exclusively for Andrew Trotman’s laptop. EtC will feature a step-based movement system, 120 by 180 black and white image display, compelling bureaucracy-based puzzle-solving, and a host of colourful (though not literally) characters to interact with.

The game takes place in the fictional institution of Ogato University in a parallel universe not entirely unlike our own. You play as the protagonist: an unnamed student who has woken up in the top floor of the university’s clocktower. Armed with nothing but a hangover and a bed-sheet toga, can you avoid humiliation, dodge expulsion, and, ultimately, escape the clocktower?

## How Are We Going To Make It? <a name="How Are We Going To Make It?"></a>
Escape the Clocktower will be written in C, with the aid of the Allegro library for handling graphics, user inputs and GUI windows. Our code will be written using the Allman layout.

In order to ensure that the group is all in understanding with the current progress of the project, and that there are no misunderstandings in the implementation of features, we have a private Discord channel. This allows us to communicate outside of lab times. We also have a weekly meeting scheduled on Wednesday mornings at 10am.

Any documentation and art is hosted on Google Drive documents, and shared via our Discord channel. Our Github repository will also host our code base, and any other necessary files.

In terms of game specifics, we will need to create a data structure to represent a map, and a player object that will traverse the map. The game is also going to need a system for knowing where the player is in relation to the map structure, and displaying graphics according to the direction the player is facing.

The narrative of the game be will designed in drafts. The first draft will provide guidance on what game mechanics would be useful to include. Later iterations will refine the narrative so that it doesn't rely on any elements that prove impractical to implement.

<a name="Obstacles"></a>
## Obstacles 
The core function of our game engine is to take a “map” containing a player object, and display the appropriate graphical representation of what the player can see. This is a non-trivial task in C, especially when we are working with a code limit. Part of our solution is to take advantage of the Allegro software library which includes image display tools, but given how crucial this function is to the project, we are still devoting the majority of our alpha development time to this task.

The code limit will inevitably constrain what gameplay mechanics we can implement. This will inform both our narrative and level design; we are conscious of trying to keep the game fun but feature-minimal.

This is the first software publication for every member of the team. As such, we need to drastically upskill in Make, learn how to effectively test our code, and how to effectively version control and bug-track—all of which detracts from our fundamental development time.

<a name="Who’s Making It?"></a>
## Who’s Making It? 
Every member of the group is effectively a programmer, but in addition we have assigned specific areas of responsibility according to our strengths and weaknesses.

**Lead Programmer -** _Anaru Hudson_  
While not necessarily having the most experience with C,  Anaru is very keen to build on and extend his C programming knowledge. Also having never developed a game before, Anaru is interested in learning more about handling problems such as rendering graphics/images, and the internal data structures of the game—specifically the storing and navigation of mazes.

**Narrative Design -** _Moira Thomson_   
Has the most experience writing and analysing stories, so is taking the lead on developing and refining the plot, puzzles and dialogue. Moira aims to approach coding, whether direct contributions or advice, with the goal of stepping back and looking for simple, robust solutions rather than the first kludge that comes to mind. 

**Testing, Research and Programming -** _Stefan Walker_  
Stefan enjoys C and is keen to extend his C programming knowledge.

**Art and Design -** _Mickey Treadwell_  
Mickey is the least experienced C programming in the group, but the most experienced pixel artist. He will be using the assignment as a means to upskill in C, while contributing the lion’s share of art assets and helping with administration.

<a name="How Long Will It Take To Make?"></a>
## How Long Will It Take To Make?
We have a hard deadline of the 
May 27th 2019 for the alpha release, 
Navigating a single level 
Interactivity with an item
a tentative deadline of July 29th for a beta release, 
NPC's
Multiple levels
and a tentative deadline of 30th September for a full release. 
Accordingly we have planned out development schedule as follows:
**GANTT CHART GOES HERE**

<a name="What Else Already Does What Our Program Does?"></a>
## What Else Already Does What Our Program Does?
Escape the clocktower draws inspiration from previous first-person grid-based dungeon-crawlers like Wizardry, Scarab of Ra, and Dungeon Master. 
It shares the step-wise movement system common to other games of the genre, a simple inventory system similar to Scarab of Ra, a dialogue system like that of the later Dungeon Master games, and a graphical style reminiscent of Wizardry: Proving Grounds of the Mad Overlord.

The mechanical similarity between Escape The Clocktower and its predecessors is out of necessity: our client specifically requires that the game be written in 1000 lines of C or fewer—an artificial limitation that parallels the mechanical limitations games developers worked with in the era that grid-based rpgs were originally developed.

The stylistic similarities are entirely voluntary and deliberate however. The project brief doesn’t specify any display resolution, and it would cost no more code to call higher quality images. The black and white retro aesthetic is intended as an homage to Wizardry and similar pioneers of the genre. It also gives us an opportunity to poke fun at the game itself, which appears ridiculously anachronistic at the time of development.

<a name="What Does Ours Do Differently?"></a>
## What Does Ours Do Differently?
Even games that are mechanically similar are inherently a unique experience to play, hence the existence of genre games (or, more broadly, genre fiction). This is especially true of narrative driven video games: Escape the Clocktower tells a unique story, and it does so in a unique style.

The development team are currently attending a university not unlike the University of Ogato, and the intended audience is other inhabitants of that same institution- the program we are producing is extremely bespoke. We are also assuming the audience’s familiarity with the first-person dungeon crawler genre, and intend to subvert their expectations accordingly. Where Etrian Odyssey puts the player in an enchanted dungeon, EtC places the player in a university admin building. Where Eye of the Beholder pits the protagonist against the undead, EtC present fearsome receptionists.
