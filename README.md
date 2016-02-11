## Great Circle Generator

![Screenshot](http://www.trevorsimonton.com/assets/posts/greatcircles/screencapture.PNG)

This C++ program generates "great circles" on a globe, calculates their intersections, and outputs all of the data to a JavaScript that can be embedded directly into a page for visualization.

The HTML, CSS and JavaScript files in this repo use the generated JavaScript to visualize the great circles with a d3.js globe.

`circles.js` is a pre-calculated output of the main program.

Use `./main > circles.js` after compiling main to overwrite this file with fresh circles.

More about how this project works and why I built this on [my blog](http://www.trevorsimonton.com/cs/projects/2016/01/07/great-circle-generator.html).

I don't currently have a make file but you can compile with g++:
`g++ src/base/* -std=c++11 -I src -o main`
