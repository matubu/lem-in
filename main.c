#include "lem_in.h"

int main() {

    // (n + m) * n * m
    // m * n + n * taken[i].size * m 

    int n, m;
    vec graph[n];

    for (int i = 0; i < n; i++)
        init_vec(&graph[i], 0, 0);

    vec taken[n];
    for (int i = 0; i < n; i++)
        init_vec(&taken[i], 0, 0);

    for (int i = 0; i < 10; i++) {
        if (!push_back(&v, i))
            return 1;
        printf("v[%d] = %d\n", i, v.at(i));
    }

    while (v.size) {
        printf("back : %d\n", v.at(v.size - 1));
        pop_back(&v);
    }





    return 0;
    srand(time(NULL));
    priority_queue pq;
    init_priority_queue(&pq, greater);

    push(&pq, make_pair(1, 1));
    push(&pq, make_pair(1, 1));
    push(&pq, make_pair(1, 1));
    push(&pq, make_pair(1, 1));
    push(&pq, make_pair(1, 1));
    push(&pq, make_pair(1, 1));
    push(&pq, make_pair(1, 1));
    push(&pq, make_pair(1, 1));
    push(&pq, make_pair(1, 1));
    push(&pq, make_pair(1, 1));

    put_node(pq.root);
    printf("\n");

    while (pq.size) {
        pop(&pq); 
        printf("%d\n", pq.size);
    }
    put_node(pq.root);
    printf("\n");

    for (int i = 0 ; i < 10; i++) {
        if (rand() % 10) {
            printf("push\n");
            push(&pq, make_pair(rand() % 100, rand() % 100));
        }
        else {
            printf("pop\n");
            pop(&pq);
        }
        put_node(top(&pq)); 
        printf("\n");
        printf("pq.size = %d\n", pq.size);
    }

    clear_priority_queue(&pq);
    put_node(top(&pq)); 
    printf("\n");
    printf("pq.size = %d\n", pq.size);

    for (int i = 0 ; i < 10; i++) {
        if (rand() % 10) {
            printf("push\n");
            push(&pq, make_pair(rand() % 100, rand() % 100));
        }
        else {
            printf("pop\n");
            pop(&pq);
        }
        put_node(top(&pq)); 
        printf("\n");
        printf("pq.size = %d\n", pq.size);
    }
}