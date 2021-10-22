/*
 * TDDD86 Trailblazer
 * Douglas Liao (douli502)
 *
 * This file contains implementation of different known searching algorithms that operates on
 * a 2-d array where start and destination is selected.
 */

#include "costs.h"
#include "trailblazer.h"
#include <queue>
#include <algorithm>
#include <pqueue.h>
#include <stack>
using namespace std;

/*
 * Depth first search algorithm.
 */
vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;

    // Call on recursive help function
    if(recursiveDFS(graph,start,end)) {
        createPath(path,start,end);
        return path;
    }
    return {};
}

/*
 * Breadth first search.
 */
vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    queue<Vertex*> queueOfVertex;
    queueOfVertex.push(start);
    while(!queueOfVertex.empty()) {
        Vertex* curVertex = queueOfVertex.front();
        curVertex->setColor(GREEN);

        // Reached the destination
        if(curVertex == end) {
            createPath(path,start,end);
            return path;
        }
        for(Node* successor : graph.getNeighbors(curVertex)) {
            // Track the path
            if(!successor->visited) {
                successor->setColor(YELLOW);
                successor->visited = true;
                successor->previous = curVertex;
                queueOfVertex.push(successor);
            }
        }
        queueOfVertex.pop();
    }
    return path;
}

/*
 * Dijkstras algorithm.
 */
vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Node*> path;
    PriorityQueue<Node*> pqueue;
    start->cost = 0;
    pqueue.enqueue(start, start->cost);

    for(Node* node : graph.getNodeSet()){
        if(node != start){
            node->cost = INFINITY;
        }
        pqueue.enqueue(node,node->cost);
    }

    while(!pqueue.isEmpty()) {
        Vertex* curVertex = pqueue.dequeue();
        curVertex->setColor(GREEN);
        curVertex->visited = true;

        // Reached destination
        if(curVertex == end) {
            createPath(path,start,end);
            return path;
        }
        for(Node* successor : graph.getNeighbors(curVertex)) {
            if(!successor->visited){
                double cost = curVertex->cost + graph.getEdge(curVertex,successor)->cost;
                if(cost<successor->cost){
                    // Better value
                    successor->cost = cost;
                    successor->previous = curVertex;
                    successor->setColor(YELLOW);
                    pqueue.changePriority(successor,cost);
                }
            }
        }
    }
    return path;
}

/*
 * A* algorithm.
 */
vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Node*> path;
    PriorityQueue<Node*> pqueue;   
    start->cost = start->heuristic(end);
    pqueue.enqueue(start,start->cost);
    for(Node* node : graph.getNodeSet()){
        if(node != start){
            node->cost = INFINITY;
        }
        pqueue.enqueue(node,node->cost);
    }
    while(!pqueue.isEmpty()) {
        Vertex* curVertex = pqueue.dequeue();
        curVertex->setColor(GREEN);
        curVertex->visited = true;

        // Reached destination
        if(curVertex == end) {
            createPath(path,start,end);
            return path;
        }
        // Check curVertex neighbors
        for(Node* successor : graph.getNeighbors(curVertex)) {
            if(!successor->visited){
                double cost = curVertex->cost + graph.getEdge(curVertex,successor)->cost;
                if(cost<successor->cost) {
                    successor->cost = cost;
                    successor->previous = curVertex;
                    successor->setColor(YELLOW);
                    pqueue.changePriority(successor,cost+successor->heuristic(end));
                }
            }
        }
    }
    return path;
}

/*
 * Create path through back tracking.
 */
void createPath(vector<Node*>& path, Vertex* start, Vertex* end) {
    do {
        path.push_back(end);
        end = end->previous;
    }
    while(end != start);

    // Add start vertex
    path.push_back(end);
    reverse(path.begin(), path.end());
}

/*
 * Recursive function for Depth First Search algorithm. Recursion occurs
 * after each point has been found.
 */
bool recursiveDFS(BasicGraph& graph, Vertex* start, Vertex* end) {
    start->visited = true;
    start->setColor(GREEN);

    // Reached destination
    if(start == end) {
        return true;
    }
    for(Node* successor : graph.getNeighbors(start)) {
        if(!successor->visited) {
            // Track our path
            successor->setColor(YELLOW);
            successor->previous = start;
            // Move to next node
            if(recursiveDFS(graph,successor,end)) {
                return true;
            }
        }
    }
    // If we didn't find a way (dead end) paint GRAY
    start->setColor(GRAY);
    return false;
}
