#Evolving Robocode Robots using grammaritcal evolution
This is a collection of files that can be used to evolve Robocode tanks using GE
Just now I am ressurecting this just to try and quickly evolve a nanobot.

Although its a multi target XCode project the only one I am currently working on is the "Delta" target
which uses a reduced grammar set more likely to generate a small robot.
The GE grammar evolves a java program to control the rocode tank using a "scaffold" program which the evolved code fills in.
Basically it takes a bit string and uses it to generate the java program. It uses the system outlined in 
Co-evolving robocode tanks Gecco 2011.
With the reduced grammar I have reduced the number of stacks available to the robot to one global stack, which can have 10 items at most,
it will be interesting to see if any of the robots take advantage of this as (maybe) a short term memory. I have ditched all the saving of 
previous event variables - if the robot wants to save anything it has to be in the global stack.
Otherwise there is the same framework, access to the local variables (being the event passed into the event holders) etc 
and some access to the maths. A
t the time of writing I think I have forgotten to get the radian robocode functions which
is silly as I think the Math trig classes the robot can use in its program are all in radians, CHECK!

So just now its up an running more or less as described in my earlier paper on evolving robocode. The Delta Main sets up 
a layered spatial evolution world and runs as long as you want. The best robots are likely found on the top layer. Ill update this later to show how to examine them better and maybe how to select the "best" ones.

You can run it just by running the RobocodeDelta target in xcode, it will set up the first layer and wait for you to add some client "battle managers"

Effectively what it does (this is the spatialCreatureDelta class) is create a layer of robots (critters and parasites), schecdules all the matches. Then it waits for helper clients to contact it,
it passes to them teh names of the battles it wants run and waits for the results. After all the battles are done these results are used to determine which critters and parasites are kept, which are bread from and
which are mutated. Again I have a paper on SCALP (Spatial Co-Evolution on Age Layered Planes), but I will probably add more code describing it later.

You can find the appropriate java program helpers in the BattleRunners directory.
Copy RobocodeRunner.java into your robocode directory, compile and start it up (after the c++ code is running). It will connect and start to run the battles.
Connect up as many clients as you have cores (you can also run them from different machines, but just make sure they have access to the robots - all they are passed is the name just now). There is readme in the BattleRunners directory that gives compile information, its not very tricky.

The RoboRunnerViewer allows you to view the battles but its a lot slower (but you can have it as one of your processes). These clients are brittle, quickly written and fall over. Keep an eye on them. There is no harm killing them I think the battles are re-run - although I have a vague memory of their being a bug and some silly scores being given if something goes wrong in a battle - anyway it sorts itself out in a generation or two.

If you want you can contact me at rharper2@mac.com. 

This is really still at the stage of being a personal project.

To do - going to alter the world so some human created robots appear on the top two layers. But no reason not to run this for a bit whilst I work on that.


I will get it working better and then change the instructions (soon! <- famous last...)

