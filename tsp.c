#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <assert.h>
#include <bits/getopt_core.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_help(void) {
    fprintf(stdout, "Usage: tsp [-i inputfile] [-o outputfile] [-d] [-h]\n");
    fprintf(stdout, "Options:\n");
    fprintf(stdout, "  -i inputfile   Specifies the input file containing the graph\n");
    fprintf(stdout, "  -o outputfile  Specifies the output file for the results\n");
    fprintf(stdout, "  -d             Specifies if the graph is directed\n");
    fprintf(stdout, "  -h             Prints this help message\n");
}

void dfs(Graph *g, uint32_t root, Path *curr, Path *shortest, int *first) {
    graph_visit_vertex(g, root);
    path_add(curr, root, g);
    // base case
    // checks if there's a cycle
    if (path_vertices(curr) == graph_vertices(g)) {
        if (graph_get_weight(g, root, START_VERTEX) != 0) {
            path_add(curr, START_VERTEX, g);

            // If this is the first cycle found, or if the current cycle is shorter than the shortest found so far
            if (!(*first) || path_distance(curr) < path_distance(shortest)) {
                // Update the shortest path
                path_copy(shortest, curr);
                *first = 1;
            }
            // otherwise, do nothing

            // Remove the last vertex (start vertex) to continue searching
            path_remove(curr, g);
        }
    } else {
        for (uint32_t i = 0; i < graph_vertices(g); i++) {
            // uint32_t ww = graph_get_weight(g, root, i);
            // bool visited = graph_visited(g, i);
            if (graph_get_weight(g, root, i) != 0 && !graph_visited(g, i)) {
                dfs(g, i, curr, shortest, first);
            }
        }
    }

    graph_unvisit_vertex(g, root);
    path_remove(curr, g);
}

int main(int argc, char **argv) {
    int opt;
    bool dash_i = false;
    bool dash_o = false;
    bool dash_d = false;
    char *infilename;
    char *outfilename;
    while (1) {
        opt = getopt(argc, argv, "i:o:dh");
        if (opt == -1)
            break;
        if (opt == 'h') {
            print_help();
            return 0;
        } else if (opt == 'd') {
            dash_d = true;
        } else if (opt == 'o') {
            dash_o = true;
            outfilename = optarg;
        } else if (opt == 'i') {
            dash_i = true;
            infilename = optarg;
        } else if (opt == '?') {
            if (isprint(optopt))
                fprintf(stderr, "tsp:  unknown or poorly formatted option -%c\n", optopt);
            return 1;
        } else {
            fprintf(stderr, "tsp:  unknown opt");
            return 1;
        }
    }

    FILE *infile = stdin;
    FILE *outfile = stdout;
    if (dash_i) {
        infile = fopen(infilename, "r");
        if (!infile) {
            fprintf(stderr, "tsp:  error reading input file %s\n", infilename);
            print_help();
            return 1;
        }
    }
    if (dash_o) {
        outfile = fopen(outfilename, "w");
    }
    uint32_t num_vertices;
    if (fscanf(infile, "%u\n", &num_vertices) != 1) {
        fprintf(stderr, "tsp:  error reading number of vertices\n");
        exit(1);
    }
    // create a graph
    Graph *g = graph_create(num_vertices, dash_d);
    uint32_t cap = num_vertices * num_vertices;
    // get names of the vertices
    char *cities[num_vertices];
    char buffer[100] = { 0 };
    // char *line = fgets(buffer, sizeof(buffer), infile);
    for (uint32_t i = 0; i < num_vertices; i++) {
        char *line = fgets(buffer, sizeof(buffer), infile);
        if (line) {
            cities[i] = malloc(100 * sizeof(char));
            if (cities[i] == NULL) {
                exit(1);
            }
            // remove newline
            buffer[strlen(buffer) - 1] = 0;
            strcpy(cities[i], buffer);
            graph_add_vertex(g, cities[i], i);
            free(cities[i]);
        }
    }

    uint32_t num_edges;
    if (fscanf(infile, "%u\n", &num_edges) != 1) {
        fprintf(stderr, "tsp:  must provide number of edges\n");
        exit(1);
    }
    // add all the edges
    uint32_t start;
    uint32_t end;
    uint32_t w;
    for (uint32_t i = 0; i < num_edges; i++) {
        if (fscanf(infile, "%u %u %u\n", &start, &end, &w) != 3) {
            fprintf(stderr, "tsp:  error reading edge\n");
            exit(1);
        }
        graph_add_edge(g, start, end, w);
    }
    // graph
    // create path vars
    Path *current = path_create(cap);
    Path *shortest = path_create(cap);
    // call dfs
    int first = 0;
    dfs(g, START_VERTEX, current, shortest, &first);
    // path_add(current, START_VERTEX, g);
    // if cycle is found
    if (path_vertices(shortest) == 0) {
        fprintf(outfile, "No path found! Alissa is lost!\n");
    } else {
        // path_add(shortest, START_VERTEX, g);
        fprintf(outfile, "Alissa starts at:\n");
        path_print(shortest, outfile, g);
        fprintf(outfile, "Total Distance: %u\n", path_distance(shortest));
    }
    // graph_print(g);
    // free mem
    path_free(&current);
    path_free(&shortest);
    graph_free(&g);

    // fprintf(outfile,"end of program");
    if (infile != stdin)
        fclose(infile);
    if (outfile != stdout)
        fclose(outfile);
    return 0;
}
