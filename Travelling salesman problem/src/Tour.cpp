/*
 * TDDD86 TSP
 * Douglas Liao (douli502)
 * This file contains the implementation of the Tour structure
 * which provied tool to create a tour and calculate relevant information.
 * See Tour.h for comments about each member. In tsp.cpp you will find which
 * algorithm is used to solve travlling salseman problem.
 */
#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

Tour::Tour(){   
    root = nullptr;
}

Tour::~Tour(){
    if(root == nullptr){
        return;
    }
    Node* current = root->next;
    while(current!=root){
        Node* temp = current;
        current = current->next;
        delete temp; // Deallocate memory
    }
    delete root;
}

void Tour::show(){
    // If no Tour exist
    if(root == nullptr){
        return;
    }
    Node* current = root->next;
    cout<<root<<endl;
    // Traverse the current tour
    while(current != root) {
        cout<<current->point<<endl;
        current = current->next;
    }
}

void Tour::draw(QGraphicsScene *scene){
    // If no Tour exist
    if(root == nullptr){
        return;
    }
    root->point.drawTo(root->next->point,scene);
    Node* current = root->next;
    // Traverse the current tour
    while(current != root) {
        current->point.drawTo(current->next->point,scene);
        current = current->next;
    }
}

int Tour::size(){
    int res = 0;
    // If no Tour exist
    if(root == nullptr){
        return res;
    }
    res++;
    Node* current = root->next;
    // Traverse the current tour
    while(current != root) {
       res++;
       current = current->next;
    }
    return res;
}

double Tour::distance(){
    double res = 0;
    // If no Tour exist
    if(root == nullptr){
        return res;
    }
    Node* current = root->next;
    res += root->point.distanceTo(root->next->point);
    // Traverse the current tour
    while(current != root) {
        res += current->point.distanceTo(current->next->point);
        current = current->next;
    }
    return res;

}

void Tour::insertNearest(Point p){
    // Initialize root by connecting itself
    if(root == nullptr){
        root = new Node(p);
        root->next = root;
        return;
    }
    Node* currentNode = root->next;
    Node* nearestNode = root;
    double nearestDistance = root->point.distanceTo(root->next->point);
    // Traverse the current tour
    while(currentNode!= root) {
        // Check if currentNode is the closest to p
        if(currentNode->point.distanceTo(p) < nearestDistance) {
            nearestDistance = currentNode->point.distanceTo(p);
            nearestNode = currentNode;
        }
        currentNode = currentNode->next;
    }
    // Add a new node and connect it to the closest node
    nearestNode->next = new Node(p,nearestNode->next);

}

void Tour::insertSmallest(Point p){
    // Initialize root by connecting itself
    if(root == nullptr){
        root = new Node(p);
        root->next = root;
        return;
    }
    Node* smallestNode = root;
    Node* currentNode = root->next;
    double shortestTour = smallestDistance(root,p);
    // Traverse the current tour
    while(currentNode != root){
        // Check if shorter distance is acheived
        if(smallestDistance(currentNode,p) < shortestTour){
            shortestTour = smallestDistance(currentNode,p);
            smallestNode = currentNode;
        }
        currentNode = currentNode->next;
    }
    // Add a new node where it gave shortest tour
    smallestNode->next = new Node(p,smallestNode->next);
}

double Tour::smallestDistance(Node* current, Point p){
    return (current->point.distanceTo(p)+current->next->point.distanceTo(p))
            - current->point.distanceTo(current->next->point);
}
