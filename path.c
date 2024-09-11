#include "graph.h"
#include "stack.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
typedef struct path {
    uint32_t total_weight;
    Stack *vertices;
} Path;

Path *path_create(uint32_t capacity) {
    Path *p = calloc(1, sizeof(Path));
    p->vertices = stack_create(capacity);
    p->total_weight = 0;
    return p;
}

void path_free(Path **pp) {
    stack_free(&(*pp)->vertices);
    free(*pp);
    *pp = NULL;
}

uint32_t path_vertices(const Path *p) {
    return stack_size(p->vertices);
}

uint32_t path_distance(const Path *p) {
    return p->total_weight;
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    if (path_vertices(p) == 0) {
        stack_push(p->vertices, val);
    } else {
        uint32_t prev;
        stack_peek(p->vertices, &prev);
        uint32_t weight = graph_get_weight(g, prev, val);
        // get the vertex from val
        stack_push(p->vertices, val);
        p->total_weight += weight;
    }
}

uint32_t path_remove(Path *p, const Graph *g) {
    uint32_t rm;
    uint32_t prev;
    assert(path_vertices(p) != 0);
    if (path_vertices(p) == 1) {
        stack_pop(p->vertices, &rm);
        return rm;
    } else {
        stack_pop(p->vertices, &rm);
        stack_peek(p->vertices, &prev);
        // get weight for the edge between prev and rm
        uint32_t weight = graph_get_weight(g, prev, rm);
        p->total_weight -= weight;
        return rm;
    }
}

void path_copy(Path *dst, const Path *src) {
    // copy stack and total weight
    dst->total_weight = src->total_weight;
    stack_copy(dst->vertices, src->vertices);
}

void path_print(const Path *p, FILE *f, const Graph *g) {
    const char *cities[stack_size(p->vertices)];
    char *cpy[stack_size(p->vertices)];

    Stack *copy = stack_create(stack_size(p->vertices));
    stack_copy(copy, p->vertices);

    for (uint32_t i = 0; i < stack_size(p->vertices); i++) {
        uint32_t v;
        stack_pop(copy, &v);
        uint32_t idx = stack_size(p->vertices) - 1 - i;
        cities[idx] = graph_get_vertex_name(g, v);
        cpy[idx] = (char *) malloc(strlen(cities[idx]) + 1);
        strcpy(cpy[idx], cities[idx]);
    }

    // Print the path
    for (uint32_t i = 0; i < stack_size(p->vertices); i++) {
        fprintf(f, "%s\n", cpy[i]);
    }
    // free mem allocated
    for (uint32_t i = 0; i < stack_size(p->vertices); i++) {
        free(cpy[i]);
    }
    stack_free(&copy);
}
