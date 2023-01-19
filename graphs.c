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
int getIntersectionIndex(Graph* graph, int name) {
    for (size_t i = 0; i < graph->intersection_count; i++) {
        Intersection current = *(graph->intersections + i);

        if (current->name == name)
            return i;
    }
    
    return -1;
}
Intersection getIntersectionByIndex(Graph* graph, int index) {
    return *(graph->intersections + index);
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

Graph* initGraph() {
    Graph* graph = malloc(sizeof(Graph));

    int intersection_count;
    scanf("%d", &intersection_count);

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

void deleteIntersection(Graph* graph) {
    int intersection_name;
    scanf("%d", &intersection_name);

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

int minDistance(int* shortest_distances, bool* handled, int length)
{
    unsigned int min = -1;
    unsigned int min_index = -1;

    for (int i = 0; i < length; i++) {
        if (*(handled+i) == false && *(shortest_distances+i) <= min) {
            min = *(shortest_distances+i);
            min_index = i;
        }
    }

    return (int) min_index;
}

/**
 * Implements Dijkstra's Algorithm.
*/
int shortestRoute(Graph* graph, int start, int end)
{
    unsigned int* shortest_distances = malloc(graph->intersection_count);
    bool* handled = malloc(graph->intersection_count);

    for (int i = 0; i < graph->intersection_count; i++) {
        *(shortest_distances+i) = -1;
        *(handled+i) = false;
    }

    *(shortest_distances+getIntersectionIndex(graph, start)) = 0;

    for (int i = 0; i < graph->intersection_count - 1; i++) {
        int current_index = minDistance(shortest_distances, handled, graph->intersection_count);
        handled[current_index] = true;

        for (int j = 0; j < graph->intersection_count; j++) {
            bool is_handled = !*(handled+j);
            bool is_possible = *(shortest_distances+current_index) != -1;

            bool is_there_edge = false;
            int edge_weight = 0;
            for (int k = 0; k < getIntersectionByIndex(graph, current_index)->edge_count; k++)
            {
                if (*(getIntersectionByIndex(graph, current_index)->edge_destination+k) == getIntersectionByIndex(graph, j)->name) {
                    is_there_edge = true;
                    edge_weight = *(getIntersectionByIndex(graph, current_index)->edge_weight+k);
                }
            }

            bool is_shorter = *(shortest_distances+current_index) + edge_weight < *(shortest_distances+j);
            
            // there is an edge from u to v, and total
            // weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (is_handled && is_possible && is_there_edge && is_shorter) {
                *(shortest_distances+j) = *(shortest_distances+current_index) + edge_weight;
            }
        }
    }

    return *(shortest_distances+getIntersectionIndex(graph, end));
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
    Graph* graph;
    char input;

    while (scanf("%c", &input) != 0) {
        if (input == 'A') {
            graph = initGraph();
        }
        else if (input == 'B') {
            addIntersection(graph);
        }
        else if (input == 'D') {
            deleteIntersection(graph);
        }
        else if (input == 'S') {
            int start, end;
            scanf("%d", &start);
            scanf("%d", &end);
            int ret = shortestRoute(graph, start, end);
            printf("%d\n", ret);
        }
        else if (input == 'T') {
            int ret = shortestPathMidpoints(graph);
            printf("%d\n", ret);
        }
    }
}