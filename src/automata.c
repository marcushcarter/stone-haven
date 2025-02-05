
/* Explanation Time: 

    -this is a project that uses cellular automoma to generate cave like structures in 2d lists 
    -will be used for a mining game where caves need to be generated that i am making right now

    -in the list, a cell is either a rock (1/.) or empty space (0/#)
    -fill_prob -> how likely a cell is to be rock on initialization (ex 0.45 = 45%)
    -iterations -> the number of times the autonoma process is repeated
    
    my cellular autonoma logic works like this:
    -It first initalizes the grid with random rocks based on fill_prob
    -It then repeats this loop for iterations:
        -If a cell is a rock (1) and it has fewer than 4 neighboring rocks, it turns into empty space (0).
        -If a cell is empty (0) and it has 5 or more neighboring rocks, it turns into a rock (1).
    -it then loops through the list and prints it out based on if it is a rock (.) or empty (#)

    P.S. It is also fairly fast which is not something my previous attempts were not

    for example, my previous nois egerator took on average around 3 minutes (the time before it starts to print it out) to generate a 50x40000 grid
    this new one takes around 2.3 seconds 

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

// Function to count neighbors around a given cell
int count_neighbors(int **grid, int x, int y, int width, int height) {
    int count = 0;

    // Check the 8 neighboring cells
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; // Skips the cell that is checking the other 8
            int nx = x + dx, ny = y + dy;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                count += grid[ny][nx];
            }
        }
    }
    return count;
}

// Function to generate cave using cellular automata
void generate_cave(int **grid, int width, int height, float fill_prob, int iterations) {
    // Initialize grid based on fill_prob
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = (rand() / (float)RAND_MAX) < fill_prob ? 1 : 0;
        }
    }

    // applies the rules of the cellular autanoma for iterations
    for (int i = 0; i < iterations; i++) {
        int **new_grid = (int **)malloc(height * sizeof(int *));
        for (int j = 0; j < height; j++) {
            new_grid[j] = (int *)malloc(width * sizeof(int));
        }

        // apply the rules to each cell
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int neighbors = count_neighbors(grid, x, y, width, height);
                // If a cell is a rock (1) and it has fewer than 4 neighboring rocks, it turns into empty space (0).
                // If a cell is empty (0) and it has 5 or more neighboring rocks, it turns into a rock (1).
                if (grid[y][x] == 1) {
                    new_grid[y][x] = (neighbors >= 4) ? 1 : 0;
                } else {
                    new_grid[y][x] = (neighbors >= 5) ? 1 : 0;
                }
            }
        }

        // Copy new grid back to the original grid
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                grid[y][x] = new_grid[y][x];
            }
        }

        for (int j = 0; j < height; j++) {
            free(new_grid[j]);
        }
        free(new_grid);
    }
}

void print_grid(int **grid, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c", grid[y][x] == 1 ? '#' : '.');
        }
        printf("\n");
    }
}

// int main() {
//     srand(time(NULL));

//     int width, height, iterations;
//     float fill_prob;

//     printf("Enter the width of the cave (usual 50): ");
//     scanf("%d", &width);
//     if (width == 0) { width = 50; }

//     printf("Enter the height of the cave (usual 30): ");
//     scanf("%d", &height);
//     if (height == 0) { height = 30; }

//     printf("Enter the fill probability (0 to 1) (usual 0.45): ");
//     scanf("%f", &fill_prob);
//     if (fill_prob < 0) { fill_prob = 0; }
//     if (fill_prob > 1) { fill_prob = 1; }

//     printf("Enter the number of iterations (usual 5): ");
//     scanf("%d", &iterations);
//     if (iterations > 1) { iterations = 1; }

//     int **grid = (int **)malloc(height * sizeof(int *));
//     for (int i = 0; i < height; i++) {
//         grid[i] = (int *)malloc(width * sizeof(int));
//     }

//     generate_cave(grid, width, height, fill_prob, iterations);
//     print_grid(grid, width, height);

//     printf("Press any key to continue...");
//     while (!_kbhit()) {
//     }
//     _getch();

//     for (int i = 0; i < height; i++) {
//         free(grid[i]);
//     }
//     free(grid);

//     return 0;
// }
