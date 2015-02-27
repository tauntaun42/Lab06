#include "Maze.h"
#include "Color.h"
#include "Rect.h"

#include <windows.h>  //for the sleep function

#include <iostream>
using namespace std;

Maze::Maze(Matrix* mz)
{
   maze = mz;
   height = maze->getNumRows();
   width = maze->getNumCols();

   WALL = 0;
   SPACE = 1;
   TRIED = 2;
   BACKTRACK = 3;
   PATH = 4;
}

Maze::~Maze()
{
   delete maze;
}

void Maze::addListener(Update* g)
{
   gui = g;
}

bool Maze::solve()
{
   bool done = traverse();
   return done;
}

//backing through the maze, setting the color to BACKTRACK
Cell* Maze::processBackTrack(StackLinked<Cell>* stack)
{
   //DO THIS
   //you may need to back through several cells

   Cell* top_cell = stack->peek();  
   //top_cell is NULL if the stack is empty
   //top_cell's direction is DEAD_END if you need to keep backtracking
   
   while (top_cell->getDir() == DEAD_END)  //need to back track
   {
      maze->setElement(top_cell->getRow(), top_cell->getCol(), BACKTRACK);
      stack->pop();

      //remove the cell and set the maze location to BACKTRACK (the maze is a Matrix)
      //look at the next cell

     	top_cell = stack->peek();




      Sleep(75);      //slow down the maze traversal
      gui->update();  //update whenever the color of a cell has been changed
   }

   return top_cell;
}

bool Maze::isSolved(Cell* curr_cell, StackLinked<Cell>* stack)
{
   //DO THIS
   //get row and col from curr_cell
	
	int x = curr_cell->getRow();
   	int y = curr_cell->getCol();
   	int end = maze->getElement(x,y);


   //have you solved the maze? (check that we are at the bottom right maze location and that it is a SPACE
   if (y == width && x == height &&  end == SPACE)  
   {


      //set the maze location to TRIED
	 maze->setElement(x,y, TRIED);

      //push curr_cell
	 stack->push(curr_cell);

      gui->update();
      //return the appropriate boolean
	return true;
   }

	else
	return false;
   //return the appropriate boolean
}

//backing through the maze, setting the solution color to PATH
void Maze::processSolution(StackLinked<Cell>* stack)
{
   //DO THIS
   //the stack has the solution path stored
   while(!stack->isEmpty())
   {
      //get the next cell from the stack
	  Cell* point = stack->pop();
	  int h = point->getRow();
	  int w = point->getCol();

      //update the maze location to PATH

	maze->setElement(h, w, PATH);

      gui->update();
   }
}

bool Maze::traverse()
{
   //DO THIS
   //complete several sections in this method

   bool done = false; //assume we are not done unless proven otherwise
   StackLinked<Cell> stack;

   maze->setElement(1, 1, TRIED);
   gui->update();

   Cell* start_cell = new Cell(1, 1);
   stack.push(start_cell);  //start from the top left corner

   while(!stack.isEmpty())
   {
      Cell* top_cell = processBackTrack(&stack);
      if (top_cell == NULL) break;  //no solution (back tracked all the way to the beginning)

      //call a method in the Cell class to give you a new Cell in a new direction relative to       top_cell (initially, DOWN)
      //DO THIS
      Cell* curr_cell = top_cell->nextCell();





      //does this new Cell solve the maze?
      done = isSolved(curr_cell, &stack);

      if (done) break;

      //DO THIS
      //get the row and col from curr_cell
      int row = curr_cell->getRow();
      int col = curr_cell->getCol();

      //check that the current maze location corresponds to SPACE, otherwise delete it
      if (maze->getElement(row, col) == 1)
      {
         //update the maze location to TRIED
         //put the cell on the stack (move forward through the maze)

         maze->setElement(row, col, TRIED);
         stack.push(curr_cell);

         Sleep(75);  //slow down the maze traversal
         gui->update();
      }
      else //look for a different route 
      {
         //DO THIS
         //delete the cell

	curr_cell = NULL;

      }
   }

   //did we make it to the bottom right?
   if (done)
   {
      processSolution(&stack);
   }
   else
   {
      cout << "No solution." << endl;
   }

   return done;
}

void Maze::mouseClicked(int x, int y)
{}

void Maze::draw(Cairo::RefPtr<Cairo::Context> cr, int width, int height)
{
   int rows = maze->getNumRows();
   int cols = maze->getNumCols();
   int cell_width = (int) (((double) width)/cols + 0.5);
   int cell_height = (int) (((double) height)/rows + 0.5);

   Color red(1.0, 0.0, 0.0);
   Rect redRect(&red, cell_width, cell_height);

   Color green(0.0, 1.0, 0.0);
   Rect greenRect(&green, cell_width, cell_height);

   Color blue(0.0, 0.0, 1.0);
   Rect blueRect(&blue, cell_width, cell_height);

   Color white(1.0, 1.0, 1.0);
   Rect whiteRect(&white, cell_width, cell_height);

   Color black(0.0, 0.0, 0.0);
   Rect blackRect(&black, cell_width, cell_height);
 
   for (int i = 1; i <= rows; i++)
   {
      for (int j = 1; j <= cols; j++)
      {
         int val = (int) maze->getElement(i, j);
         int x_pixel = (j - 1) * cell_width + cell_width/2;
         int y_pixel = (i - 1) * cell_height + cell_height/2;

         if (val == WALL)
         {
            blackRect.draw(cr, x_pixel, y_pixel);
         }
         else if (val == SPACE)
         {
            whiteRect.draw(cr, x_pixel, y_pixel);
         }
         else if (val == TRIED)
         {
            blueRect.draw(cr, x_pixel, y_pixel);
         }
         else if (val == BACKTRACK)
         {
            redRect.draw(cr, x_pixel, y_pixel);
         }
         else if (val == PATH)
         {
            greenRect.draw(cr, x_pixel, y_pixel);
         }
      }
   }
}
