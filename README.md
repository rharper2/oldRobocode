#Evolving Robocode Robots using grammatcal evolution

> 8 August 2013 - some bug fixes to the new isParticipating stuff in spatial.h - it turns out the remove-copy-if algorithm didn't quite work how I thought
  as it didn't reduce the vector size so null pointer errors could occur.


This is a collection of files that can be used to evolve Robocode tanks using GE
Just now I am ressurecting this just to try and quickly evolve a nanobot.

Although its a multi target XCode project the only one I am currently working on is the "Delta" target
I have removed all the spurious targets, but kept the MvP one, which shows how SCALP can be used with human robot parasites
rather than a co-evolved team. It uses the older Beta grammar (it would be trivial to switch it to the smaller delta grammar - have a look
in the appropriate SpaitalRobocodeCreatureMvP file and make the correct changes there and in RobocodeMvPMain file).

The Delta grammar uses a reduced grammar set more likely to generate a small robot. The entry I submitted to GECCO2013 robocode actually didn't make any use of the one stack (other than to generate zero by calling stack.size()), so for the submission I removed all the references to the stack and this reduced it to nanorobot size.

The GE grammar evolves a java program to control the rocode tank using a "scaffold" program which the evolved code fills in.
Basically it takes a bit string and uses it to generate the java program. It uses the system outlined in 
Co-evolving robocode tanks Gecco 2011.

With the reduced (Delta) grammar I have reduced the number of stacks available to the robot to one global stack, which can have 10 items at most,
it will be interesting to see if any of the robots take advantage of this as (maybe) a short term memory. I have ditched all the saving of 
previous event variables - if the robot wants to save anything it has to be in the global stack.
Otherwise there is the same framework, access to the local variables (being the event passed into the event holders) etc 
and some access to the maths. 

I have now corrected the grammar to use the correct radian version of the functions.

So just now its up an running more or less as described in my earlier paper on evolving robocode. The Delta Main sets up 
a layered spatial evolution world and runs as long as you want. The best robots are likely found on the top layer. Ill update this later to show how to examine them better and maybe how to select the "best" ones.

#New experimental setup

I didn't have time to use this for the GECCO entry but its now possible to remove one of the occupants of a cell from the evolutionary system and do something else with it. Here, I was toying with the idea of one of the later layers having some human robots in the cells to try and make sure the evolved team had robust behavoiurs against human robots as well. As it happened the 160 generations I used for GECCO (it should have been more, but it was a rather spur of the moment entry decision) did evolve to beat all the samples (but not the super samples).

Anyway the idea behind it is as follows:

Normally: For each cell you select which critter is going to occupy it next generation by ranking them according to score and selecting the first one, 50%, second one, 25% etc. The algorithm used for this is to generate a random number. If the first bit is 1 we will select the "best" otherwise if the second bit is 1 we select the second... etc. We then take the "neighbours and previous" vector of the cell (which points to all the surrounding cells (and this one)) and sort for the nth best item) - where n is the one we are choosing. 50% of the time, its just getting the best critter.

So the change here is that we prune the neighbours and previous (each round) to remove any critter that returns false to the query (isParticipating()). This would indicate that its not participating in the genetic breeding. This allows us to put in that cell a different type of creature (here a human robot) that will be used to score the parasites competing against it, but won't get mixed up in the breeding.

Finally how do we decide which cells to fill. Well every time a layer is created, each creature and parasite gets a call to  virtual void inLayerWithLocation(int layer,int x);

That it can use to decide whether or not to set the isParticipating() flag to false etc. Examples of this is containted in spatialRobocodeCreatureDelta. Just now they are rather hard coded basic, I am sure I will play with this a bit more.

The system here has a number of the robots taken from the roborumble competition referenced in the spaticalRobocodeCreatureDelta class - they will, of course, have to be in the robots directory, so the evolving robots can fight against them. Ill try and remember to include a tar of the robots in files here, they will have to be untarred into the robots directory.

#Running the system

You can run it just by running the RobocodeDelta target in xcode, it will set up the first layer and wait for you to add some client "battle managers"

Effectively what it does (this is the spatialCreatureDelta class) is create a layer of robots (critters and parasites), schedules all the matches. Then it waits for helper clients to contact it,
it passes to them teh names of the battles it wants run and waits for the results. After all the battles are done these results are used to determine which critters and parasites are kept, which are bred from and
which are mutated. Again I have a paper on SCALP (Spatial Co-Evolution on Age Layered Planes), but I will probably add more code describing it later.

You can find the appropriate java program helpers in the BattleRunners directory.
Copy RobocodeRunner.java into your robocode directory, compile and start it up (after the c++ code is running). It will connect and start to run the battles.
Connect up as many clients as you have cores (you can also run them from different machines, but just make sure they have access to the robots - all they are passed is the name just now). There is readme in the BattleRunners directory that gives compile information, its not very tricky.

The RoboRunnerViewer allows you to view the battles but its a lot slower (but you can have it as one of your processes). These clients are brittle, quickly written and fall over. Keep an eye on them. There is no harm killing them I think the battles are re-run - although I have a vague memory of their being a bug and some silly scores being given if something goes wrong in a battle - anyway it sorts itself out in a generation or two.

If you want you can contact me at rharper2@mac.com. 

This is really still at the stage of being a personal project.

To do - going to alter the world so some human created robots appear on the top two layers. But no reason not to run this for a bit whilst I work on that.


I will get it working better and then change the instructions (soon! <- famous last...)

#Final update notes

I have updated the spatial.h file so that it now requires C++11 (although it is still compatible with the older stdlib). Moving to C++11 caused a few problems with the boost shared_ptrs for some unknown reason. This was solved by changing the vectors in location to standard pointers (which actually makes more sense) but it does mean that if you start to manually delete locations (and I can't think why you would) that you need to take care to make sure the vectors of pointers in adjacent locations are updated).
