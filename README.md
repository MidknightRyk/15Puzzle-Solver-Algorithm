# 15Puzzle-Solver-Algorithm

A 15-puzzle solver implementation using the Manhattan heuristic approach and recursive IDA* (Iterative Deepening A*) algorithm.

## Description

This program solves 15-puzzle instances efficiently using:
- **Manhattan distance heuristic**: Calculates the sum of distances each tile is from its goal position
- **IDA* algorithm**: Iterative deepening search with admissible heuristic for optimal solutions

## How to Use

1. **Compilation**: Make sure all files are in the same directory and run the Makefile:
   ```bash
   make
   ```

2. **Execution**: Run the program with a puzzle file as an argument:
   ```bash
   ./15puzzle <puzzle_file>
   ```
   
   Example:
   ```bash
   ./15puzzle Test-Puzzles/test3.puzzle
   ```

## File Structure

- `puzzle-solver.c` - Main solver implementation
- `Makefile` - Build configuration
- `Test-Puzzles/` - Directory containing sample puzzle files

## Puzzle File Format

Puzzle files should contain a 4x4 grid representation of the 15-puzzle, where 0 represents the empty space.

## Cleaning Up

To remove compiled files:
```bash
make clean
```
