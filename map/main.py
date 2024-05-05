import random

def generate_maze(size):
    # Initialize the maze with walls (1's)
    maze = [[1] * (2 * size + 1) for _ in range(2 * size + 1)]

    # Start recursive backtracking from the corner (1,1)
    def recursive_backtrack(x, y):
        # Directions: up, down, right, left
        directions = [(0, 2), (0, -2), (2, 0), (-2, 0)]
        random.shuffle(directions)  # Shuffle to ensure randomness in the path generation
        
        for (dx, dy) in directions:
            nx, ny = x + dx, y + dy
            if 1 <= nx < 2 * size and 1 <= ny < 2 * size and maze[nx][ny] == 1:
                # If the next cell is within bounds and is a wall, visit it
                maze[nx][ny] = 0  # Make the new cell part of the path
                maze[x + dx // 2][y + dy // 2] = 0  # Remove the wall between the current cell and the new cell
                recursive_backtrack(nx, ny)

    # Start from the top left inner corner and ensure it's part of the maze
    maze[1][1] = 0
    recursive_backtrack(1, 1)

    return maze

def print_maze(maze):
    for row in maze:
        print(' '.join(str(cell) for cell in row))

size = 16  # This generates a 33x33 grid, but we'll use only 32x32
maze = generate_maze(size)
print_maze(maze)

