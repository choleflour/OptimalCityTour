#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct graph {
    uint32_t vertices;
    bool directed;
    bool *visited;
    char **names;
    uint32_t **weights;
} Graph;

Graph *graph_create(uint32_t vertices, bool directed) {
    Graph *g = calloc(1, sizeof(Graph));
    g->vertices = vertices;
    g->directed = directed;
    // use calloc to initialize everything with zeroes
    g->visited = calloc(vertices, sizeof(bool));
    g->names = calloc(vertices, sizeof(char *));
    // allocate g->weights with a pointer for each row
    g->weights = calloc(vertices, sizeof(g->weights[0]));
    // allocate each row in the adjacency matrix
    for (uint32_t i = 0; i < vertices; ++i) {
        g->weights[i] = calloc(vertices, sizeof(g->weights[0][0]));
    }
    return g;
}

void graph_free(Graph **gp) {
    for (uint32_t i = 0; i < (*gp)->vertices; ++i) {
        free((*gp)->weights[i]);
        (*gp)->weights[i] = NULL;
    }
    free((*gp)->weights);
    (*gp)->weights = NULL;

    for (uint32_t i = 0; i < (*gp)->vertices; i++) {
        free((*gp)->names[i]);
    }
    free((*gp)->names);
    (*gp)->names = NULL;

    free((*gp)->visited);
    (*gp)->visited = NULL;

    free(*gp);
    *gp = NULL;
}

uint32_t graph_vertices(const Graph *g) {
    return g->vertices;
}

void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weights) {
    g->weights[start][end] = weights;
    if (!(g->directed))
        g->weights[end][start] = weights;
}

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    if (g->directed) {
        return g->weights[start][end];
    } else {
        if (g->weights[start][end])
            return g->weights[start][end];
        else
            return g->weights[start][end];
    }
}

void graph_visit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = true;
}

void graph_unvisit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = false;
}

bool graph_visited(const Graph *g, uint32_t v) {
    return g->visited[v];
}

char **graph_get_names(const Graph *g) {
    return g->names;
}

void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    if (g->names[v])
        free(g->names[v]);
    g->names[v] = strdup(name);
}

const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    return g->names[v];
}

void graph_print(const Graph *g) {
    for (uint32_t i = 0; i < g->vertices; ++i) {
        // printf("%s", g->names[i]);
        printf("%5u", i);
    }
    printf("\n");
    for (uint32_t i = 0; i < g->vertices; ++i) {
        // printf("%s", g->names[i]);
        printf("%u", i);
        for (uint32_t k = 0; k < g->vertices; ++k) {
            printf("%5u", g->weights[i][k]);
        }
        printf("\n");
    }
}
