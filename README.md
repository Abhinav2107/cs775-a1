#BVH Player
The report for this bot can be found at http://www.cse.iitb.ac.in/~ranveer/bvhplayer/

##Usage
* Compile using "make"
* Remove executable and object files using "make clean"
* Remove all generated files using "make distclean"
* Run using "./BVHPlayer -i <input-bvh-file>"

###Here are the key mappings:
* `Space`: Play/Pause
* `N`: Step Backward one frame
* `M`: Step Forward one frame
* `R`: Pause and reset to canonical pose (Frame: -1)
* `1`: Select Fixed Camera
* `2`: Select Following Camera
* `T`: Toggle Render Modes between skeleton and transformer

##External libraries used
* SOIL (Simple OpenGL Image Library): We used this to load images into our program.
* GLU: We didn't make much use of it, except for changing the lookat (gluLookat).
* GLUT: Used for drawing 3D objects.
* GLFW: Used for windowing and timers.

##References
* The Assignment from the course CS475 (Fall 2014) was used. http://www.cse.iitb.ac.in/~ranveer/graphics/
