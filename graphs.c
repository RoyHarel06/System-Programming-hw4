#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

struct Intersection_t{
  int name;
  int edge_count;
  int* edge_destination;
  int* edge_weight;
};
typedef struct Intersection_t* Intersection;

typedef struct {
  Intersection* intersections;
  int intersection_count;
} Graph;

Intersection getIntersection(Graph* graph, int intersection_name) {
    for (int i = 0; i < graph->intersection_count; i++) {
        Intersection current = *(graph->intersections + i);
        if (current->name == intersection_name)
            return current;
    }
    
    return NULL;
}

void addIntersection(Graph* graph) {
    Intersection new = malloc(sizeof(Intersection));

    int input;
    scanf("%d", &input);
    new->name = input;

    new->edge_count = 0;
    new->edge_destination = malloc(0);
    new->edge_weight  = malloc(0);

    int i = 0;
    while (scanf("%d", &input) != 0) {
        new->edge_count++;

        new->edge_destination = realloc(new->edge_destination, sizeof(int)*new->edge_count);
        new->edge_weight = realloc(new->edge_weight, sizeof(int)*new->edge_count);

        *(new->edge_destination + i) = input;

        scanf("%d", &input);
        *(new->edge_weight + i) = input;
    }

    graph->intersections = realloc(graph->intersections, (graph->intersection_count+1)*sizeof(Intersection));
    *(graph->intersections + graph->intersection_count) = new;
    graph->intersection_count++;
}
void addEmptyIntersection(Graph* graph, int name) {
    Intersection new = malloc(sizeof(Intersection));

    new->name = name;
    new->edge_count = 0;
    new->edge_destination = malloc(0);
    new->edge_weight  = malloc(0);

    graph->intersections = realloc(graph->intersections, (graph->intersection_count+1)*sizeof(Intersection));
    *(graph->intersections + graph->intersection_count) = new;
    graph->intersection_count++;
}

Graph* initGraph(int intersection_count) {
    Graph* graph = malloc(sizeof(Graph));

    char input;
    while (scanf("%c", &input) != 0 && input == 'n') {
        addIntersection(graph);
    }

    for (int i = 0; i < intersection_count; i++) {
        if (getIntersection(graph, i) == NULL)
            addEmptyIntersection(graph, i);
    }
    
    return graph;
}

void deleteEdgesToIntersection(Intersection intersection, int intersection_name) {
    bool deleted = false;
    for (int i = 0; i < intersection->edge_count; i++) {
        if (*(intersection->edge_destination + i) == intersection_name) {
            deleted = true;
        }
        else if (deleted) {
            *(intersection->edge_destination + i - 1) = *(intersection->edge_destination + i);
            *(intersection->edge_weight + i - 1) = *(intersection->edge_weight + i);
        }
    }

    intersection->edge_count--;
    intersection->edge_destination = realloc(intersection->edge_destination, intersection->edge_count*sizeof(int));
    intersection->edge_weight = realloc(intersection->edge_weight, intersection->edge_count*sizeof(int));
}

void deleteIntersection(Graph* graph, int intersection_name) {
    bool deleted = false;
    for (int i = 0; i < graph->intersection_count; i++) {
        Intersection current = *(graph->intersections + i);
        if (current->name == intersection_name) {
            free(current);
            deleted = true;
        }
        else if (deleted) {
            deleteEdgesToIntersection(current, intersection_name);
            *(graph->intersections + i - 1) = current;
        }
        else {
            deleteEdgesToIntersection(current, intersection_name);
        }
    }

    graph->intersection_count--;
    graph->intersections = realloc(graph->intersections, graph->intersection_count*sizeof(Intersection));
}

int shortestRoute(Graph* graph, int start, int end) {
    // ...
}

inline void swap(int* path, int a, int b)
{
  int temp = *(path+a);
  *(path+a) = *(path+b);
  *(path+b) = temp;
}
int checkAllPermutations(Graph* graph, int path_start, int* path, int start_i, int end_i)
{
    if(start_i == end_i) /* checking a possible permutation */
    {
        if (shortestRoute(graph, path_start, *(path+0)) == -1)
            return -1;
        int length = shortestRoute(graph, path_start, *(path+0));

        for (int i = 0; i < end_i; i++)
        {
            if (shortestRoute(graph, *(path+i), *(path+i+1)) == -1)
                return -1;
            length += shortestRoute(graph, *(path+i), *(path+i+1));
        }
        return length;
    }

    int min = permute(graph, path_start, path, start_i + 1, end_i); /* start at next element */

    /* permute remaining elements recursively */
    for(int i = start_i + 1; i < end_i; i++) 
    {
        if( *(path+start_i) == *(path+i) )
            continue;
 
	    swap(path, start_i, i);
	
	    int temp = permute(graph, path_start, path, start_i + 1, end_i);
        if (temp < min)
            min = temp;

	    swap(path, start_i, i); /* restore element order */ 
    }

    return min;
}
int shortestPathMidpoints(Graph* graph) {
    int number_of_stops;
    scanf("%d", &number_of_stops);

    int start;
    scanf("%d", &start);

    int* path = malloc(number_of_stops-1);
    for (int i = 0; i < number_of_stops-1; i++)
    {
        scanf("%d", path+i);
    }
    
    return checkAllPermutations(graph, start, path, 0, number_of_stops-2);
}

void main() {
    
}