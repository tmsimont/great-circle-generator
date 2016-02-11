// copyright 2016 Trevor Simonton
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <memory>
#include <cmath>
#include "base/TrevorCalc3.h"
#include "base/GreatCircle.h"
#include "base/OriginPlane.h"
#include "base/Point.h"


using std::vector;
using std::shared_ptr;
using std::string;
using std::cout;
using std::stringstream;
using std::endl;

unsigned int globalSeed;


/**
 * Get a random point in 3d space (assumed that it's 1 unit from origin)
 */
Point randomPoint() {
  float theta = fmod(rand_r(&globalSeed), 2 * PI);
  float phi = fmod(rand_r(&globalSeed), PI);
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
void printCircles(stringstream *ss, vector<GreatCircle*> circles) {
  *ss << "var circles = [" << endl;
  int i = 0;
  for (GreatCircle *c : circles) {
    *ss << "{points:[";
    c->printFivePoints(ss);
    *ss << "],";
    *ss << "intersects:[";
    int j = 0;
    for (shared_ptr<Point> p : c->intersections) {
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
void printPoints(stringstream *ss, vector<shared_ptr<Point> > points) {
  *ss << "var points = [" << endl;
  int i = 0;
  for (shared_ptr<Point> p : points) {
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
string generateCircles(int numcircles) {
  stringstream ss;

  float circleSpace = PI / (6 * numcircles);
  vector<GreatCircle*> circles;
  vector<shared_ptr<Point> > intersections;

  for (int i = 0; i < numcircles; i++) {
    GreatCircle *c = randomCircle();
    // make sure c gets destroyed as necessary
    int bumps = 0;

    // we wrap this in a loop to make sure we don't get
    // circles that break our rules...
    bool OK = false;

    while (!OK) {
      vector<shared_ptr<Point> > itemp;
      // compare new random circle with existing circles
      for (GreatCircle *other : circles) {
        vector<shared_ptr<Point> > ints = c->twoCircleIntersections(other);
        // NOTE -- vector ints must clean up new points returned
        itemp.push_back(ints[0]);
        itemp.push_back(ints[1]);
      }

      // make sure new circle isn't too close to other intersections
      OK = true;
      for (shared_ptr<Point> p : itemp) {
        for (shared_ptr<Point> ep : intersections) {
          if (fabs(p->theta - ep->theta) < circleSpace
                && fabs(p->phi - ep->phi) < circleSpace) {
            OK = false;

            while (itemp.size() > 0) {
              itemp.pop_back();
            }
            if (bumps < 20) {
              GreatCircle *newC = new GreatCircle(
                  Point(c->p1.theta + fmod(rand_r(&globalSeed), PI),
                        c->p1.phi + fmod(rand_r(&globalSeed), PI)), c->p2);
              delete c;
              c = newC;
              bumps++;
            } else {
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
      vector<shared_ptr<Point> > ints = c->twoCircleIntersections(other);
      // NOTE: vector ints is responsible for 2 new points

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
      shared_ptr<Point> closest = intersections[0];
      if (j == 0) {
        closest = c->intersections[1];
      }

      // for all other points in the list
      for (int k = 0; k < c->intersections.size(); k++) {
        if (j != k) {
          // compare distance, updated closest as that with smallest distance
          if (c->intersections[k]
                  ->distanceTo(c->intersections[j])
              < closest
                  ->distanceTo(c->intersections[j])) {
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
  globalSeed = time(0);

  if (argc == 2) {
    string circleJSON = generateCircles(atoi(argv[1]));
    cout << circleJSON << endl;
  } else {
    string circleJSON = generateCircles(5);
    cout << circleJSON << endl;
  }
}
