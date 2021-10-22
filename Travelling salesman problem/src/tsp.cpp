/*
 * TDDD86 TSP
 * This client program uses your Tour class and contains the 'main'
 * function to open the input file and set up the program's primitive GUI.
 */

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "Point.h"
#include "Tour.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    string filename = "tsp0.txt";
    ifstream input;
    input.open(filename);

    // Get dimensions
    int width;
    int height;
    input >> width;
    input >> height;

    // Setup graphical window
    QGraphicsView *view = new QGraphicsView();
    QGraphicsScene *scene = new QGraphicsScene();
    view->setScene(scene);
    view->scale(1, -1); //screen y-axis is inverted
    view->setSceneRect(0, 0, width, height);
    view->show();

    // Run insertion heuristic
    Tour tour;
    double x;
    double y;
    while (input >> x >> y) {
        Point p(x, y);
        //uncomment the heurisitc you want to use
        //tour.insertNearest(p);
        //tour.insertSmallest(p);
        //uncomment the 4 lines below to animate
        //tour.draw(scene);
        //std::chrono::milliseconds dura(50);
        //std::this_thread::sleep_for(dura);
        //a.processEvents();
    }
    input.close();


    tour.show();
    // Print tour to standard output
    cout << "Tour distance: " << std::fixed << std::setprecision(4)
         << std::showpoint << tour.distance() << endl;
    cout << "Number of points: " << tour.size() << endl;

    tour.draw(scene);


    return a.exec(); // Start Qt event loop

}
