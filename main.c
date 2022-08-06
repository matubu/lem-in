#include <time.h>
#include <stdio.h>
#include "lem_in.h"
#include "parse.h"
#include "string.h"

#define time(code) ({ \
	clock_t	before = clock(); \
	typeof(code) tmp = code; \
	printf("\033[1;94m" #code "\033[0m: took %lfs\n", (double)(clock() - before) / CLOCKS_PER_SEC); \
	tmp; \
})

void	lem_in(char *filename) {
	t_farm	*farm = time(parse_farm(filename));

	// Algo goes gere

	free_farm(farm);
}


// int main() {
// 	room_map mp;
// 	init_room_map(&mp, less_str);
// 	//room_node a = {.id = 0, .x = 1, .y = 1};
// 	insert_room(&mp, make_room_pair("pouet", (t_room){.id = 0, .x = 1, .y = 1}));
// 	char str[] = "abclksadj;lsdkjf;ldsk";
// 	for (int i = 0; i < 10000; i++) {
// 		insert_room(&mp, make_room_pair(str, (t_room){.id = 0, .x = 1, .y = 1}));
// 		str[0] = rand()%127 + 1;
// 		str[1] = rand()%127 + 1;
// 		str[2] = rand()%127 + 1;
// 		str[3] = rand()%127 + 1;
// 		str[4] = rand()%127 + 1;
// 		str[5] = rand()%127 + 1;
// 	}
// 	insert_room(&mp, make_room_pair("plop", (t_room){.id = 42, .x = 1, .y = 1}));

// 	room_node *room = get_room(&mp, "pouet");
// 	printf("[%zu] x=%zu y=%zu\n", room->value.second.id, room->value.second.x, room->value.second.y);

// 	room = get_room(&mp, "plop");
// 	printf("[%zu] x=%zu y=%zu\n", room->value.second.id, room->value.second.x, room->value.second.y);
// }

int	main(int argc, char **argv) {
	if (argc < 2) {
		lem_in("map/simple");
		return (0);
	}
	for (int i = 1; i < argc; ++i) {
		lem_in(argv[i]);
	}
}

// int main() {

//     // (n + m) * n * m
//     // m * n + n * taken[i].size * m 

//     int n, m;
//     vec graph[n];

//     for (int i = 0; i < n; i++)
//         init_vec(&graph[i], 0, 0);

//     vec taken[n];
//     for (int i = 0; i < n; i++)
//         init_vec(&taken[i], 0, 0);

//     for (int i = 0; i < 10; i++) {
//         if (!push_back(&v, i))
//             return 1;
//         printf("v[%d] = %d\n", i, v.at(i));
//     }

//     while (v.size) {
//         printf("back : %d\n", v.at(v.size - 1));
//         pop_back(&v);
//     }





//     return 0;
//     srand(time(NULL));
//     priority_queue pq;
//     init_priority_queue(&pq, greater);

//     push(&pq, make_pair(1, 1));
//     push(&pq, make_pair(1, 1));
//     push(&pq, make_pair(1, 1));
//     push(&pq, make_pair(1, 1));
//     push(&pq, make_pair(1, 1));
//     push(&pq, make_pair(1, 1));
//     push(&pq, make_pair(1, 1));
//     push(&pq, make_pair(1, 1));
//     push(&pq, make_pair(1, 1));
//     push(&pq, make_pair(1, 1));

//     put_node(pq.root);
//     printf("\n");

//     while (pq.size) {
//         pop(&pq); 
//         printf("%d\n", pq.size);
//     }
//     put_node(pq.root);
//     printf("\n");

//     for (int i = 0 ; i < 10; i++) {
//         if (rand() % 10) {
//             printf("push\n");
//             push(&pq, make_pair(rand() % 100, rand() % 100));
//         }
//         else {
//             printf("pop\n");
//             pop(&pq);
//         }
//         put_node(top(&pq)); 
//         printf("\n");
//         printf("pq.size = %d\n", pq.size);
//     }

//     clear_priority_queue(&pq);
//     put_node(top(&pq)); 
//     printf("\n");
//     printf("pq.size = %d\n", pq.size);

//     for (int i = 0 ; i < 10; i++) {
//         if (rand() % 10) {
//             printf("push\n");
//             push(&pq, make_pair(rand() % 100, rand() % 100));
//         }
//         else {
//             printf("pop\n");
//             pop(&pq);
//         }
//         put_node(top(&pq)); 
//         printf("\n");
//         printf("pq.size = %d\n", pq.size);
//     }
// }