#ifndef TOUR_H
#define TOUR_H

#include "Node.h"
#include "Point.h"

class Tour {
public:
    /*
     * Constructor.
     */
    Tour();

    /*
     * Realeses the occupied memory.
     */
    ~Tour();

    /*
     * Show the Tour.
     */
    void show();

    /*
     * Draws the Tour.
     */
    void draw(QGraphicsScene* scene);

    /*
     * Show size of Tour
     */
    int size();

    /*
     * Returns distance of the Tour
     */
    double distance();

    /*
     * Insert p to the closest node in Tour
     */
    void insertNearest(Point p);

    /*
     * Insert p between nodes that gives shortest Tour
     */
    void insertSmallest(Point p);

    /*
     * Help function for calculating difference if node is added between current and current->next
     */
    double smallestDistance(Node* current, Point p);

private:
Node* root;


};

#endif // TOUR_H
