#include "TrevorCalc3.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <memory>
#include <cmath>
#include "GreatCircle.h"
#include "OriginPlane.h"
#include "Point.h"


// TODO : makefile...
//  g++ -std=c++11 *.h *.cc -o main


/**
 * Get a random point in 3d space (assumed that it's 1 unit from origin)
 */
Point randomPoint() {
	float theta = fmod(rand(), 2 * PI);
	float phi = fmod(rand(), PI);
	return Point(theta, phi);
}

/**
 * Use two random points to generate a random circle
 */
GreatCircle *randomCircle() {
	return new GreatCircle(randomPoint(), randomPoint());
}

/**
 * Print a list of circles in nice JSON format to a string stream.
 */
void printCircles(std::stringstream *ss, std::vector<GreatCircle*> circles) {
	*ss << "var circles = [" << std::endl;
	int i = 0;
	for (GreatCircle *c : circles) {
		*ss << "{points:[";
		c->printFivePoints(ss);
		*ss << "],";
		*ss << "intersects:[";
		int j = 0;
		for (std::shared_ptr<Point> p : c->intersections) {
			*ss << "{p:";
			p->printJSON(ss);
			*ss << ",";
			*ss << "id:";
			*ss << p->id;
			*ss << "}";
			if (j++ < c->intersections.size())
				*ss << ",";
			*ss << std::endl;
		}

		*ss <<"]}";
		if (i++ < circles.size())
			*ss << ",";
		*ss << std::endl;
	}
	*ss << "]";
	*ss << std::endl;
}

/**
 * Print a list of points in nice JSON format to a string stream.
 */
void printPoints(std::stringstream *ss, std::vector<std::shared_ptr<Point> > points) {
	*ss << "var points = [" << std::endl;
	int i = 0;
	for (std::shared_ptr<Point> p : points) {
		*ss << "{p:";
		p->printJSON(ss);
		*ss << ",";
		*ss << "id:";
		*ss << p->id;
		*ss << "}";
		if (i++ < points.size())
			*ss << ",";
		*ss << std::endl;
	}
	*ss << "]";
}

/**
 * Generate random great circles and print them
 * to a string stream.
 * @return
 *   A JSON string describing our great circles and their points of 
 *   intersection
 */
std::string generateCircles(int numcircles)
{
	std::stringstream ss;
	
	float circleSpace = PI / (6 * numcircles);
	std::vector<GreatCircle*> circles;
	std::vector<std::shared_ptr<Point> > intersections;

	for (int i = 0; i < numcircles; i++) {
		GreatCircle *c = randomCircle();
		// make sure c gets destroyed as necessary
		int bumps = 0;
		
		// we wrap this in a loop to make sure we don't get circles that break our rules...
		bool OK = false;
		
		while (!OK) {
			std::vector<std::shared_ptr<Point> > itemp;
			// compare new random circle with existing circles
			for (GreatCircle *other : circles) {
				std::vector<std::shared_ptr<Point> > ints = c->twoCircleIntersections(other);
				// NOTE -- std::vector ints must clean up new points returned
				itemp.push_back(ints[0]);
				itemp.push_back(ints[1]);
			}
			
			// make sure new circle isn't too close to other intersections
			OK = true;
			for (std::shared_ptr<Point> p : itemp) {
				for (std::shared_ptr<Point> ep : intersections) {
					if (fabs(p->theta - ep->theta) < circleSpace && fabs(p->phi - ep->phi) < circleSpace) {
						
						// std::cout << (fabs(p->theta - ep->theta)) << ", " << (fabs(p->phi - ep->phi)) << std::endl;
						OK = false;
						
						while (itemp.size() > 0) {
							itemp.pop_back();
						}
						if (bumps < 20) {
							GreatCircle *newC = new GreatCircle(Point(c->p1.theta + fmod(rand(), PI), c->p1.phi + fmod(rand(), PI)), c->p2);
							delete c;
							// NOTE: memory leak unless Circle has a destructor to clean up origin plane and its points
							c = newC;
							bumps++;
						}
						else {
							bumps = 0;
						}
						break;
					}
				}
				if (!OK)
					break;
			}
			// make sure temp points are gone
			while (itemp.size() > 0) {
				itemp.pop_back();
			}
		}
		// now we have a new circle that isn't too close to others
		for (GreatCircle *other : circles) {
			int idx = intersections.size();
			std::vector<std::shared_ptr<Point> > ints = c->twoCircleIntersections(other);
			// NOTE: std::vector ints is responsible for 2 new points

			ints[0]->id = idx;
			ints[1]->id = idx+1;

			// track intersections globally and locally to 2 circles involved:
			intersections.push_back(ints[0]);
			intersections.push_back(ints[1]);
			c->intersections.push_back(ints[0]);
			c->intersections.push_back(ints[1]);
			other->intersections.push_back(ints[0]);
			// TODO: use shared pointers... management of ints 0 and 1 just got ugly
			other->intersections.push_back(ints[1]);
		}
		circles.push_back(c);
	}

	for (int i = 0; i < circles.size(); i++) {
		GreatCircle *c = circles[i];

		// for all points in this circle's list of intersection points
		for (int j = 0; j < c->intersections.size(); j++) {

			// assume closest is 0 for comparison (or 1 if this is 0)
			std::shared_ptr<Point> closest = intersections[0];
			if (j == 0) {
				closest = c->intersections[1];
			}

			// for all other points in the list
			for (int k = 0; k < c->intersections.size(); k++) {
				if (j != k) {

					// compare distance, updated closest as that with smallest distance
					if (c->intersections[k]->distanceTo(c->intersections[j]) < closest->distanceTo(c->intersections[j])) {
						closest = c->intersections[k];
					}
				}
			}

			// the closest point in the circle's intersections must be a neighbor
			c->intersections[j]->neighbors.push_back(closest);

			// todo: what about other neighbor? there are two...
		}

	}

	printCircles(&ss, circles);
	printPoints(&ss, intersections);

	// clean up circles memory
	for (int i = 0; i < circles.size(); i++) {
		delete circles[i];
	}

	return ss.str();
}

int main(int argc, char **argv) {
	// seed random generations with current time
	srand(time(0));	
	
	if (argc == 2) {
		std::string circleJSON = generateCircles(atoi(argv[1]));
		std::cout << circleJSON << std::endl;
	}	else {
		std::string circleJSON = generateCircles(5);
		std::cout << circleJSON << std::endl;
	}
	
	/*
	ofstream myfile;
	myfile.open("C:\\Users\\Trevor\\Documents\\school\\algs\\p2\\circles.js");
	myfile << circleJSON;
	myfile.close();
	*/
}
