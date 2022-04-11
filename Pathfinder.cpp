	#include "Pathfinder.h"
	string Pathfinder::toString() const
	{
	    stringstream ss;
		for (int layer = 0; layer < NUM_LAYERS; layer++) {
			for(int row = 0; row < ROW_SIZE; row++) {
				for(int col = 0; col < COL_SIZE; col++) {
					ss << maze_grid[layer][row][col];
                    if (col != COL_SIZE - 1) {
                        ss << " ";
                    }
				}
                ss << endl;
			}
            if (layer != NUM_LAYERS - 1) {
                ss << endl;
            }
		}
	    return ss.str();
	}

	/*
	* createRandomMaze
	*
	* Generates a random maze and stores it as the current maze.
	*
	* The generated maze must contain a roughly equal number of 1s and 0s and must have a 1
	* in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).  The generated maze may be
	* solvable or unsolvable, and this method should be able to produce both kinds of mazes.
	*/
	void Pathfinder::createRandomMaze()
	{
		for (int layer = 0; layer < NUM_LAYERS; layer++) {
			for(int row = 0; row < ROW_SIZE; row++) {
				for(int col = 0; col < COL_SIZE; col++) {
					maze_grid[layer][row][col] = rand() % 2;
				}	
			}
		}
		maze_grid[0][0][0] = 1;
		maze_grid[4][4][4] = 1;
	}
	//-----------------------------------------------------------------------------------------

	//Part 2-----------------------------------------------------------------------------------
	/*
	* importMaze
	*
	* Reads in a maze from a file with the given file name and stores it as the current maze.
	* Does nothing if the file does not exist or if the file's data does not represent a valid
	* maze.
	*
	* The file's contents must be of the format described above to be considered valid.
	*
	* Parameter:	file_name
	*				The name of the file containing a maze
	* Returns:		bool
	*				True if the maze is imported correctly; false otherwise
	*/
	bool Pathfinder::importMaze(string file_name)
	{
        int tempMaze[NUM_LAYERS][ROW_SIZE][COL_SIZE];
        string temp;
        ifstream infile (file_name);

        if(infile.is_open()) {
            cout << "Importing maze from file: " << file_name << endl;
            //read in the maze, checking for valid inputs
            for (int k=0; k<NUM_LAYERS; k++) {
                for (int i=0; i<ROW_SIZE; i++) {
                    for (int j=0; j<COL_SIZE; j++) {
                        if (infile.eof()) {
                            cout << "Invalid input: too short" << endl;
                            return false;
                        }
                        infile >> temp;                        
                        cout << temp << " ";
                        if (temp != "1" && temp != "0") {
                            cout << "Invalid maze, invalid inputs" << endl;
                            return false;
                        }
                        tempMaze[k][i][j] = stoi(temp);
                    }
                    cout << endl;
                }
                cout << endl;
            }

            //check if maze in too long
            while(!infile.eof()) {
                cout << "extra lines" << endl;
                infile >> temp;
                if (temp != " " && temp != "\n") {
                    cout << "Invalid maze, too long " << endl;
                    return false;
                }
            }

            //check if maze is has entrance and exit
            if(tempMaze[0][0][0] == 1 && tempMaze[NUM_LAYERS-1][ROW_SIZE-1][COL_SIZE-1] == 1) {
                for (int k=0; k<NUM_LAYERS; k++) {
                    for (int i=0; i<ROW_SIZE; i++) {
                        for (int j=0; j<COL_SIZE; j++) {
                            maze_grid[k][i][j] = tempMaze[k][i][j];
                        }
                    }
                }
            }
            else {
                cout << "Invalid maze, no entrance or exit " << endl;
                return false;
            }

            infile.close();
            cout << "Maze imported successfully" << endl;
            return true;
        }
        else {
            return false;
        }
	}
	//-----------------------------------------------------------------------------------------

	
	
	bool Pathfinder::find_maze_path(int grid[NUM_LAYERS][ROW_SIZE][COL_SIZE], int l, int r, int c) {
	  
        cout << "find_maze_path(" << l << ", " << r << ", " << c << ")" << endl;
        cout << this->toString() << endl;

        if (l < 0 || l >= NUM_LAYERS || r < 0 || r >= ROW_SIZE || c < 0 || c >= COL_SIZE) {
            return false;  // out of bounds
        }
        else if (grid[l][r][c] != BACKGROUND) {
            return false;  // cell is on barrier or dead end
        }
        else if ((l == NUM_LAYERS - 1) && (r == ROW_SIZE - 1) && (c == COL_SIZE - 1)) {
            solution.push_back("("+to_string(c)+", "+to_string(r)+", "+to_string(l)+")");
            cout << "found the exit" << endl;
            return true;  // found the exit
        }
        else {
            grid[l][r][c] = PATH;
            if (find_maze_path(grid,l, r - 1, c)
	            || find_maze_path(grid,l, r + 1, c)
	            || find_maze_path(grid,l, r, c - 1)
	            || find_maze_path(grid,l, r, c + 1 )
                || find_maze_path(grid,l+1, r, c)
                || find_maze_path(grid,l-1, r, c)) {
                solution.push_back("("+to_string(c)+", "+to_string(r)+", "+to_string(l)+")");
                return true;
            }
            else {
                grid[l][r][c] = TEMPORARY;  // dead end
                return false;
            }
        }
	}
	  //Part 3-----------------------------------------------------------------------------------
	/*
	* solveMaze
	*
	* Attempts to solve the current maze and returns a solution if one is found.
	*
	* A solution to a maze is a list of coordinates for the path from the entrance to the exit
	* (or an empty vector if no solution is found). This list cannot contain duplicates, and
	* any two consecutive coordinates in the list can only differ by 1 for only one
	* coordinate. The entrance cell (0, 0, 0) and the exit cell (4, 4, 4) should be included
	* in the solution. Each string in the solution vector must be of the format "(x, y, z)",
	* where x, y, and z are the integer coordinates of a cell.
	*
	* Understand that most mazes will contain multiple solutions
	*
	* Returns:		vector<string>
	*				A solution to the current maze, or an empty vector if none exists
	*/
	
	vector<string> Pathfinder::solveMaze()
	{
        solution.clear();
		int tempMaze[NUM_LAYERS][ROW_SIZE][COL_SIZE];
        for (int i = 0; i < NUM_LAYERS; i++) {
            for (int j = 0; j < ROW_SIZE; j++) {
                for (int k = 0; k < COL_SIZE; k++) {
                    tempMaze[i][j][k] = maze_grid[i][j][k];
                }
            }
        }
        find_maze_path(tempMaze,0,0,0);
		
        //reverse solution
        vector<string> temp_solution;
        for (int i = solution.size()-1; i >= 0; i--) {
            temp_solution.push_back(solution.at(i));
        }
        solution = temp_solution;
        
        for(auto s:solution) {
			cout <<s<<endl;
		}
	    return solution;
	}
