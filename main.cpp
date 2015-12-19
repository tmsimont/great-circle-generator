#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>

#define PI 3.1415926535897932384626433832795
#define FAC1 (180.0/PI)
#define FAC2 (PI/180.0)
#define r 1

using namespace std;

// g++ -std=c++11 main.cpp -o main

/**
 * Return the cross product of 2 3x1 vectors
 */
vector<float> crossProduct(vector<float> v1, vector<float> v2) {
	vector<float> xproduct;
	xproduct.push_back((v1[1] * v2[2]) - (v2[1] * v1[2]));
	xproduct.push_back(-1 * ((v1[0] * v2[2]) - (v2[0] * v1[2])));
	xproduct.push_back((v1[0] * v2[1]) - (v2[0] * v1[1]));
	return xproduct;
}

/**
 * Get the unit vector of the given vector
 */
vector<float> unitVector(vector<float> v) {
	float l = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
	v[0] = v[0] / l;
	v[1] = v[1] / l;
	v[2] = v[2] / l;
	return v;
}

/**
 * Point represents a set of coordinates in Cartesian and Polar
 * coordinates that describes a point on the surface of a sphere.
 * This point supports conversion from Cartesian to Polar, and
 * can be output as d3.js globe friendly JSON format.
 */
class Point {
public:
	int id;
	float theta;
	float phi;
	float x;
	float y;
	float z;
	vector<Point*> neighbors;

	/**
	* Point is at origin by default
	*/
	Point() {
		theta = phi = x = y = z = 0;
	}

	/**
	* Given spherical coordinates, build point and populate Cartesian
	*/
	Point(float theta, float phi) {
		this->theta = fmod(theta, 2 * PI);
		this->phi = fmod(phi, 2 * PI);
		x = r * sin(this->phi) * cos(this->theta);
		y = r * sin(this->phi) * sin(this->theta);
		z = r * cos(this->phi);
	}

	/**
	* Given Cartesian coordinates, build point and populate spherical
	*/
	Point(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		phi = acos(z);
		theta = atan2(y, x);
	}

	/**
	 * Get the haversine distance to another point (distance between 
	 * two points on a sphere)
	 */
	float distanceTo(Point *other) {
		float dlon = other->theta - theta;
		float dlat = other->phi - phi;
		float a = pow(sin(dlat / 2), 2) + cos(phi) * cos(other->phi) * pow(sin(dlon / 2), 2);
		float c = 2 * atan2(sqrt(a), sqrt(1 - a));
		return c;
	}

	void print(std::stringstream *ss) {
		*ss << printf("x: %f, y: %f, z: %f", x, y, z) << endl;
		*ss << printf("theta: %f, phi: %f", theta, phi) << endl;
	}

	/**
	 * The JSON output here is designed to work with the coordinate 
	 * system used by d3.js
	 */
	void printJSON(std::stringstream *ss) {
		*ss << "[" << theta*FAC1 - 90 << ", " << 90 - phi * FAC1 << "]";
	}
};


/**
 * This represents a plane in 3d space passing through 
 * the (0,0,0) origin
 */
class OriginPlane {
private:
	/**
	 * calculate and set the normal vector of this plane.
	 */
	void calculateNormal() {
		n = crossProduct(vector<float> {p2.x, p2.y, p2.z}, vector<float> {p3.x, p3.y, p3.z});
	}
public:
	// any 3 points make a plane
	Point p1;
	Point p2;
	Point p3;
	
	// n is the normal vector
	vector<float> n;
	
	/**
	 * Only 2 points are needed for construction. The 3rd point
	 * is the (0,0,0) origin.
	 */
	OriginPlane(Point p2, Point p3) {
		p1 = Point();
		this->p2 = p2;
		this->p3 = p3;
		calculateNormal();
	}
	
	/**
	 * Return the vector of intersetion between this plane
	 * and another plane.
	 * @param p
	 *  OriginPlane object with normal vector to compare for intersection vector
	 */
	vector<float> intersectionVector(OriginPlane *p) {
		return crossProduct(p->n, n);
	}
	
	/**
	 * Print to the given buffer the normal vector of this plane.
	 */
	void printN(std::stringstream ss) {
		ss << printf("%f, %f, %f", n[0], n[1], n[2]) << endl;
	}
};

/**
* Data and functions for representing a single great circle
*/
class GreatCircle {
public:
	// list of intersections with other great circles
	vector<Point*> intersections;
	// each great circle corresponds to an origin plane
	OriginPlane *plane;
	
	// we only need 2 points to define our circle
	Point p1;
	Point p2;
	
	/**
	 * We need 2 points on the circle for construction.
	 */
	GreatCircle(Point p1, Point p2) {
		plane = new OriginPlane(p1, p2);
		this->p1 = p1;
		this->p2 = p2;
	}
	
	/**
	 * Each pair of great circles has exactly 2 intersections.
	 * Use this function to fine the 2 intersections with this circle
	 * and another.
	 * @param c2
	 *   The other circle to use to find 2 intersections
	 */
	vector<Point*> twoCircleIntersections(GreatCircle *c2) {
		// use the line intersection of the circle's planes
		vector<float> v = plane->intersectionVector(c2->plane);
		v = unitVector(v);
		// use the unit vector that is the intersection vector to generate 1 point
		Point *i1 = new Point(v[0], v[1], v[2]);
		// rotate to opposite side of the circle for other point
		Point *i2 = new Point(i1->theta + PI, 2 * PI - (i1->phi + PI));
		return vector<Point*> {i1, i2};
	}
	
	/**
	 * To draw this circle on a sphere, we need 5 nicely spaced points.
	 * The first and fifth points are the same point.
	 * This helps us draw 4 segments around a circle, which is what we need
	 * when drawing the circle in d3.js
	 */
	void printFivePoints(std::stringstream *ss) {

		// a vector that is cross product of p1's position vector 
		// and the plane's normal is 90 degrees rotated along great circle from p1
		// (it is orthogonal to p1 and to the circle's plane's normal...)
		vector<float> doublyOrthoVector = crossProduct(vector<float>{p1.x, p1.y, p1.z}, plane->n);
		doublyOrthoVector = unitVector(doublyOrthoVector);
		
		// build a point from that doubly orthognal cross product vector
		Point pd = Point(doublyOrthoVector[0], doublyOrthoVector[1], doublyOrthoVector[2]);

		// build points opposite to our magic cross product and the given p1 point
		Point oppositeP1 = Point(p1.theta + PI, 2 * PI - (p1.phi + PI));
		Point oppositeP2 = Point(pd.theta + PI, 2 * PI - (pd.phi + PI));

		// we now print out 4 nicely spaced out points for d3.js
		p1.printJSON(ss);
		*ss << ",";
		pd.printJSON(ss);
		*ss << ",";
		oppositeP1.printJSON(ss);
		*ss << ",";
		oppositeP2.printJSON(ss);
		*ss << ",";
		p1.printJSON(ss);
	}
	
};

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
void printCircles(std::stringstream *ss, vector<GreatCircle*> circles) {
	*ss << "var circles = [" << endl;
	int i = 0;
	for (GreatCircle *c : circles) {
		*ss << "{points:[";
		c->printFivePoints(ss);
		*ss << "],";
		*ss << "intersects:[";
		int j = 0;
		for (Point *p : c->intersections) {
			*ss << "{p:";
			p->printJSON(ss);
			*ss << ",";
			*ss << "id:";
			*ss << p->id;
			*ss << "}";
			if (j++ < c->intersections.size())
				*ss << ",";
			*ss << endl;
		}

		*ss <<"]}";
		if (i++ < circles.size())
			*ss << ",";
		*ss << endl;
	}
	*ss << "]";
	*ss << endl;
}

/**
 * Print a list of points in nice JSON format to a string stream.
 */
void printPoints(std::stringstream *ss, vector<Point*> points) {
	*ss << "var points = [" << endl;
	int i = 0;
	for (Point *p : points) {
		*ss << "{p:";
		p->printJSON(ss);
		*ss << ",";
		*ss << "id:";
		*ss << p->id;
		*ss << "}";
		if (i++ < points.size())
			*ss << ",";
		*ss << endl;
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
static std::string generateCircles(int numcircles)
{
	std::stringstream ss;
	
	float circleSpace = PI / (6 * numcircles);
	vector<GreatCircle*> circles;
	vector<Point*> intersections;

	// seed random generations with current time
	srand(time(NULL));

	for (int i = 0; i < numcircles; i++) {
		GreatCircle *c = randomCircle();
		int bumps = 0;
		
		// we wrap this in a loop to make sure we don't get circles that break our rules...
		bool OK = false;
		
		while (!OK) {
			vector<Point*> itemp;
			// compare new random circle with existing circles
			for (GreatCircle *other : circles) {
				vector<Point*> ints = c->twoCircleIntersections(other);
				itemp.push_back(ints[0]);
				itemp.push_back(ints[1]);
			}
			
			// make sure new circle isn't too close to other intersections
			OK = true;
			for (Point *p : itemp) {
				for (Point *ep : intersections) {
					if (abs(p->theta - ep->theta) < circleSpace && abs(p->phi - ep->phi) < circleSpace) {
		
						// @todo: figure out why this becomes an infinite loop with too many circles...
						//cout << (abs(p->theta - ep->theta)) << ", " << (abs(p->phi - ep->phi)) << endl;
						OK = false;
						
						while (itemp.size() > 0) {
							delete itemp[itemp.size()-1];
							itemp.pop_back();
						}
						if (bumps < 20) {
							GreatCircle *newC = new GreatCircle(Point(c->p1.theta + fmod(rand(), PI), c->p1.phi + fmod(rand(), PI)), c->p2);
							delete c;
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
				delete itemp[itemp.size() - 1];
				itemp.pop_back();
			}
		}
		// now we have a new circle that isn't too close to others
		for (GreatCircle *other : circles) {
			int idx = intersections.size();
			vector<Point*> ints = c->twoCircleIntersections(other);

			ints[0]->id = idx;
			ints[1]->id = idx+1;

			// track intersections globally and locally to 2 circles involved:
			intersections.push_back(ints[0]);
			intersections.push_back(ints[1]);
			c->intersections.push_back(ints[0]);
			c->intersections.push_back(ints[1]);
			other->intersections.push_back(ints[0]);
			other->intersections.push_back(ints[1]);
		}
		circles.push_back(c);
	}

	for (int i = 0; i < circles.size(); i++) {
		GreatCircle *c = circles[i];

		// for all points in this circle's list of intersection points
		for (int j = 0; j < c->intersections.size(); j++) {

			// assume closest is 0 for comparison (or 1 if this is 0)
			Point* closest = c->intersections[0];
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


	return ss.str();
}

int main() {
	std::string circleJSON = generateCircles(5);
	cout << circleJSON << endl;
	/*
	ofstream myfile;
	myfile.open("C:\\Users\\Trevor\\Documents\\school\\algs\\p2\\circles.js");
	myfile << circleJSON;
	myfile.close();
	*/
}
