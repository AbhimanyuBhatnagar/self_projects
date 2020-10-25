/******************************************************
Title: Intelligent Tic-Tac-Toe
Description: Program to implement the game Tic-Tac-Toe 
using Reactive Artificial Intelligence so that the computer 
can play the game intelligently against a human player.
Author: Abhimanyu Bhatnagar
Class: XII-D
******************************************************/
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <process.h>
#include <fstream.h>
#include <stdio.h>
#include <string.h>
#include <iomanip.h>
#include <dos.h>
#include <time.h>

// The class Matrix contains the data grid and
// the methods required to manipulate it and compute
// the computer's moves
class Matrix
{
	int grid[3][3];
	char conv(int y);
	int look_forward(int player_val);
	public:
	Matrix();
	void setGridPoint(int row, int col, int value);
	int getGridPoint(int row, int col);
	int diag2();
	int diag1();
	int columnsum(int column_num);
	int rowsum(int row_num);
	void drawfig();
	int compute_move(int move_count);
};

// The class User is used to store historical
// results of a user
class User
{
	int serial_num;
	char name[100];
	int num_games;
	int num_wins;
	int num_draws;
	char time_last_played[100];
	public:
	User();
	User(int,char*,int,int,int,char*);
	int get_serial_num();
	char* get_name();
	void set_name(char*);
	int get_num_games();
	void set_num_games(int);
	int get_num_wins();
	void set_num_wins(int);
	int get_num_draws();
	void set_num_draws(int);
	char* get_time_last_played();
	void set_time_last_played(char*);
	void display();
};

// The class History is used to manage the historical
// dataset of all users. It contains methods to handle
// files for storing user data and manipulating them.
class History
{
	User current_user;
	int new_user;
	void display_header();
	char* History::currentDateTime();
	void offer_update();
	public:
	int find_current_user();
	int update_current_user_result(int);
	int display();
};

// Basic constuctor
User::User()
{
	serial_num = 0;
	num_games = 0;
	num_wins = 0;
	num_draws = 0;
	strcpy(name,"");
	strcpy(time_last_played,"");
}

// Constructor with initialization values of all
// data members
User::User(int s, char* n, int ng, int nw, int nd, char* lp)
{
	serial_num = s;
	num_games = ng;
	num_wins = nw;
	num_draws = nd;
	strcpy(name,n);
	strcpy(time_last_played,lp);
}

// Accessor method to retrieve serial number
int User::get_serial_num()
{
	return serial_num;
}

// Accessor method to retrieve name
char* User::get_name()
{
	return name;
}

// Mutator method to update name
void User::set_name(char *n)
{
	strcpy(name,n);
}
// Accessor method to retrieve number of games played
int User::get_num_games()
{
	return num_games;
}

// Mutator method to update the number of games played
void User::set_num_games(int ng)
{
	num_games = ng;
}

// Accessor method to retrieve number of wins
int User::get_num_wins()
{
	return num_wins;
}

// Mutator method to update the number of wins
void User::set_num_wins(int nw)
{
	num_wins = nw;
}

// Accessor method to retrieve number of games drawn
int User::get_num_draws()
{
	return num_draws;
}

// Mutator method to update the number of games drawn
void User::set_num_draws(int nd)
{
	num_draws = nd;
}

// Accessor method to retrieve time_last_played
char* User::get_time_last_played()
{
	return time_last_played;
}

// Mutator method to update time_last_played
void User::set_time_last_played(char *lp)
{
	strcpy(time_last_played,lp);
}


// Method to display a user
void User::display()
{
	cout.setf(ios::left);
	cout << setw(5) << serial_num;
	cout << setw(26) << name;
	cout << setw(7) << num_games;
	cout << setw(5) << num_wins;
	cout << setw(8) << num_games-num_wins-num_draws;
	cout << setw(7) << num_draws;
	cout << setw(20) << time_last_played << endl;
	cout.setf(ios::right);
}

// Method used to identify the current player. The
// player is shown the history of all players and
// asked for his/her serial number if present in 
// the history list. If the player provides a serial 
// number, the that user is chosen. Otherwise the
// player is asked to provide his/her name and a
// new user is created
int History::find_current_user()
{
	clrscr();
	int max_serial_num = 0, current_serial_num, user_count = 0;
	User hist_user;
	fstream fhist;
	fhist.open("tic_hist.dat",ios::in|ios::binary);
	if (!fhist)
	{
		// Create the data file if it does not exist
		fhist.open("tic_hist.dat",ios::out|ios::in|ios::binary);
	}
	// display_header();
	// Display the list of all players
	while (!fhist.eof())
	{
		fhist.read((char *) &hist_user, sizeof(User));
		if (fhist.eof())
		{
			// The latest read encountered eof. Hence break.
			break;
		}
		if (user_count % 5 == 0)
		{
			if (user_count!=0) {
				cout << endl << "Press any key to continue" << endl;
				getch();
			}
			clrscr();
			display_header();
		}
		user_count++;
		hist_user.display();
		if (hist_user.get_serial_num() > max_serial_num)
		{
			max_serial_num = hist_user.get_serial_num();
		}
	}
	if (user_count)
	{
		cout << "\nIf your name is on the list, please enter your sequence number." << endl;
		cout << "Otherwise enter 0" << endl;
		cin >> current_serial_num;
	}
	else
	{
		cout << "Empty list" << endl << endl;
		current_serial_num = 0;
	}
	if (current_serial_num > 0 && current_serial_num <= max_serial_num)
	{
		fhist.clear();
		fhist.seekg(0);
		while (!fhist.eof())
		{
			fhist.read((char *) &hist_user, sizeof(User));
			if (fhist.eof())
			{
				break;
			}
			if (hist_user.get_serial_num() == current_serial_num)
			{
				// Found a matching user. Set that to current user/
				current_user = hist_user;
				offer_update();
				new_user = 0;
				fhist.close();
				return 0;
			}
		}
		fhist.close();
	}

	// Create a new user
	char name[256];
	current_serial_num = max_serial_num + 1;
	cout << "Please enter your name" << endl;
	gets(name);
	name[25] = '\0';   // Truncate the name to 25 characters
	current_user = User(current_serial_num, name, 0, 0, 0, currentDateTime());
	new_user = 1;
	return 0;
}
// Method to allow use to update his/her name or reset
// his/her record
void History::offer_update()
{
	char update_choice = '1';
	while ('1' == update_choice || '2' == update_choice)
	{
		cout << "If you want to correct your name, enter 1" << endl;
		cout << "If you want to reset your record, enter 2" << endl;
		cout << "If you want to play, enter any other key" << endl;
		cin >> update_choice;
		if ('1' == update_choice)
		{

			char new_name[256];
			cout << "Please enter your name" << endl;
			gets(new_name);
			new_name[25] = '\0';   // Truncate the name to 25 characters
			current_user.set_name(new_name);
			cout << "Your name has been updated" << endl;
		} else if ('2' == update_choice)
		{
			current_user.set_num_games(0);
			current_user.set_num_wins(0);
			current_user.set_num_draws(0);
			current_user.set_time_last_played("");
			cout << "Your record has been reset" << endl;
		}
	}
}


// Update records of the current user with the game
// result and then update the history data file.
int History::update_current_user_result(int result)
{
	fstream fhist, ftemp;
	User hist_user;

	current_user.set_time_last_played(currentDateTime());
	current_user.set_num_games(current_user.get_num_games()+1);
	if (1==result)
	{
		// User won
		current_user.set_num_wins(current_user.get_num_wins()+1);
	}
	if (2==result)
	{
		// Game was drawn
		current_user.set_num_draws(current_user.get_num_draws()+1);
	}
	fhist.open("tic_hist.dat",ios::in|ios::binary);  // Open the hsitory data file
	ftemp.open("tic_hist.tmp",ios::out|ios::binary); // Open a temporary file
	if (!fhist || !ftemp)
	{
		cout << "Unable to open history file" << endl;
		return 1;
	}
	while (!fhist.eof())
	{
		fhist.read((char *) &hist_user, sizeof(User));
		if (fhist.eof())
		{
			// The latest read encountered eof. Hence break.
			break;
		}
		if (hist_user.get_serial_num() == current_user.get_serial_num())
		{
			// Store the updated record of the current
			// user instead of the historical record
			ftemp.write((char *) &current_user, sizeof(User));
		}
		else
		{
			// Store the historical record unchanged
			// since it doesn't belong to current user
			ftemp.write((char *) &hist_user, sizeof(User));
		}
	}
	if (new_user)
	{
			// This is a new user. So write his/her
			// record after all historical records
			// have been written
	      ftemp.write((char *) &current_user, sizeof(User));
	}
	fhist.close();
	ftemp.close();
	rename("tic_hist.dat","tic_hist.del");  // Rename historical data file as .del (deleted) file
	rename("tic_hist.tmp","tic_hist.dat");  // Rename temp file as historical data file
	remove("tic_hist.del");  // Now actually delete del file. This way we won't use historical file even if program crashes midway
	return 0;
}

// Method to return currentDate & time
// Uses time(), localtime() and strftime() from time.h
char* History::currentDateTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[100];
	tstruct = *localtime(&now);
	strftime(buf,sizeof(buf),"%Y-%m-%d %X", &tstruct);
	return buf;
}
// Display header of historical table
void History::display_header()
{
	cout << setw(44) << "History of Players" << endl;
	for (int i=0;i<77;i++)
	{
		cout << "-";
	}
	cout << endl;
	cout.setf(ios::left);
	cout << setw(5) << "Seq";
	cout << setw(26) << "Name";
	cout << setw(7) << "Num";
	cout << setw(5) << "Num";
	cout << setw(8) << "Num";
	cout << setw(7) << "Num";
	cout << setw(20) << "Last played" << endl;

	cout << setw(5) << "no.";
	cout << setw(26) << "";
	cout << setw(7) << "games";
	cout << setw(5) << "wins";
	cout << setw(8) << "losses";
	cout << setw(7) << "draws" << endl;
	for (i=0;i<77;i++)
	{
		cout << "-";
	}
	cout << endl;
	cout.setf(ios::right);
}

// Display the historical data of all players present
// in historical data file
int History::display()
{

	User hist_user;
	fstream fhist;
	int count = 0;
	clrscr();
	fhist.open("tic_hist.dat",ios::in|ios::binary);
	if (!fhist)
	{
		cout << "Unable to open history file" << endl;
		return 1;
	}
	display_header();
	while (!fhist.eof())
	{
		fhist.read((char *) &hist_user, sizeof(User));
		if (fhist.eof())
		{
			// The latest read encountered eof. Hence break.
			break;
		}
		if (count % 5 == 0)
		{
			if (count!=0) {
				cout << endl << "Press any key to continue" << endl;
				getch();
			}
			clrscr();
			display_header();
		}
		count++;
		hist_user.display();
	}
	fhist.close();
	return 0;
}

// Constructor for Matrix. It initializes the 
// game grid by storing 0s in all positions
Matrix::Matrix()
{
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			grid[i][j]=0;
		}
	}
}

// Mutator method to update a point on the grid.
void Matrix::setGridPoint(int row, int col, int value)
{
	grid[row][col] = value;
}

// Accessor method to retrieve a point on the grid
int Matrix::getGridPoint(int row, int col)
{
	return (grid[row][col]);
}

// Draw the grid representing the current status of the
// game on the screen
void Matrix::drawfig()
{
	clrscr();
	int n=179;
	char z=n;
	cout<<"\n\n\n\t\t\t\t"<<z<<"\t"<<z<<endl;
	cout<<"\t\t\t    "<<conv(grid[0][0])<<"   "<<z<<"   "<<conv(grid[0][1])<<"   "<<z<<"   "<<conv(grid[0][2])<<"  " <<endl;
	cout<<"\t\t\t  ______"<<z<<"_______"<<z<<"______"<<endl;
	cout<<"\t\t\t\t"<<z<<"\t"<<z<<endl;
	cout<<"\t\t\t    "<<conv(grid[1][0])<<"   "<<z<<"   "<<conv(grid[1][1])<<"   "<<z<<"   "<<conv(grid[1][2])<<"  " <<endl;
	cout<<"\t\t\t  ______"<<z<<"_______"<<z<<"______"<<endl;
	cout<<"\t\t\t\t"<<z<<"\t"<<z<<endl;
	cout<<"\t\t\t    "<<conv(grid[2][0])<<"   "<<z<<"   "<<conv(grid[2][1])<<"   "<<z<<"   "<<conv(grid[2][2])<<"  " <<endl;
	cout<<"\t\t\t\t"<<z<<"\t"<<z<<endl;
	return ;
}

// Convert the internal representation of game status
// to the external representation of 'X', 'O' or blank
char Matrix::conv(int y)
{
	char z;
	if(0==y)
	{
		z=' ';
	}
	if(1==y)
	{
		z='X';
	}
	if(-1==y)
	{
	       z='O';
	}
	return z;
}

// Compute the sum of all values in the row row_num
int Matrix::rowsum(int row_num)
{
	int sum;
	sum=0;
	for(int i=0;i<3;i++)
	{
		sum=grid[row_num][i]+sum;
	}
	return sum;
}

// Compute the sum of all values in the 
// column column_num
int Matrix::columnsum(int column_num)
{
	int sum;
	sum=0;
	for(int i=0;i<3;i++)
	{
		sum=grid[i][column_num]+sum;
	}
	return sum;
}

// Compute the sum of all values in the diagonal
// from top left to bottom right
int Matrix::diag1()
{
	int sum=0;
	for(int i=0;i<3;i++)
	{
		sum += grid[i][i];
	}
	return sum;
}

// Compute the sum of all values in the diagonal 
// from bottom left to top right
int Matrix::diag2()
{
	int sum=0;
	for(int i=0;i<3;i++)
	{
		int j=2-i;
		sum += grid[i][j];
	}
	return sum;
}

// Compute the computer's next move
int Matrix::compute_move(int move_count)
{
	int row, column, move_index, look_forward_val;
	if(diag1()==-2)
	{
		for(int i=0;i<3;i++)
		{
			if(grid[i][i]==0)
			{
				row=i;
				column=i;
				cout << "Attacking " << row << ", " << column << endl;
				return row*3+column;
			}
		}
	}
	if(diag2()==-2)
	{
		for(int i=0;i<3;i++)
		{
			int j=2-i;
			if(grid[i][j]==0)
			{
				row=i;
				column=j;
				cout << "Attacking " << row << ", " << column << endl;
				return row*3+column;
			}
		}
	}
	for(int t=0;t<3;t++)
	{
		// Attack loop
		if(-2==rowsum(t))
		{
			for(int i=0;i<3;i++)
			{
				if(grid[t][i]==0)
				{
					row=t;
					column=i;
					cout << "Attacking " << row << ", " << column << endl;
					return row*3+column;
				}
			}
		}
		if(-2==columnsum(t))
		{
			for(int i=0;i<3;i++)
			{
				if(grid[i][t]==0)
				{
					column=t;
					row=i;
					cout << "Attacking " << row << ", " << column << endl;
					return row*3+column;
				}
			}
		}
	}
	if(diag1()==2)
	{
		for(int i=0;i<3;i++)
		{
			if(grid[i][i]==0)
			{
				row=i;
				column=i;
				cout << "Defending " << row << ", " << column << endl;
				return row*3+column;
			}
		}
	}
	if(diag2()==2)
	{
		for(int i=0;i<3;i++)
		{
			int j=2-i;
			if(grid[i][j]==0)
			{
				row=i;
				column=j;
				cout << "Defending " << row << ", " << column << endl;
				return row*3+column;
			}
		}
	}
	for(t=0;t<3;t++)
	{
		// defence loop
		if(2==rowsum(t))
		{
			for(int i=0;i<3;i++)
			{
				if(grid[t][i]==0)
				{
					row=t;
					column=i;
					cout << "Defending " << row << ", " << column << endl;
					return row*3+column;
				}
			}
		}
		if(2==columnsum(t))
		{
			for(int i=0;i<3;i++)
			{
				if(grid[i][t]==0)
				{
					column=t;
					row=i;
					cout << "Defending " << row << ", " << column << endl;
					return row*3+column;
				}
			}
		}

	}
	// Special case of opponent occupying opposite corners
	if(move_count==3 && ((grid[0][0]==1 && grid[2][2]==1)||(grid[2][0]==1 && grid[0][2]==1)))
	{
		for(int j=1;j<8;j+=2)
		{
			row=j/3;
			column=j%3;
			if(grid[row][column]==0)
			{
				cout << "Defending against diagonal" << endl;
				return j;
			}
		}
	}
	look_forward_val = look_forward(-1);
	if (look_forward_val > -1)
	{
		cout << "Look forward attacking" << endl;
		return look_forward_val;
	}
	look_forward_val = look_forward(1);
	if (look_forward_val > -1)
	{
		cout << "Look forward defending" << endl;
		return look_forward_val;
	}
	if (move_count <= 2)
	{
		// In initial moves, choose a position on diagonal
		if (0==grid[1][1]) {
			// If center is free, choose that
			cout << "Choosing center" << endl;
			return 4;
		}
		do
		{
			row = random (3);
			if (random(2)==0)
			{
				column = row;
			} else {
				column = 2-row;
			}
		}
		while (grid[row][column]!=0);
		cout << "Choosing diagonal position" << endl;
		return row*3+column;
	}
	do
	{
		row=random(3);
		column=random(3);
	} while ( grid[row][column] != 0);
	cout << "Random choice " << row << ", " << column << endl;
	move_index = row*3+column;
	return move_index;
}

// This function takes as input the game matrix and
// integer player_val which is -1 if it is the
// computer's move and 1 for the human player's move.
// It checks checks all possible moves that can be made
// by the player. If any potential move will results in
// a situation where the player gets 2 marks in 2 rows,
// columns or diagonals, it returns the index for the
// move. Otherwise it returns -1.
int Matrix::look_forward(int player_val)
{
	int row, column, double_count;
	for (row = 0; row < 3; row++)
	{
		for (column = 0; column < 3; column++)
		{
			if (grid[row][column]==0)
			{
				// Simulate a move on the square if it is unmarked
				grid[row][column] = player_val;
				double_count = 0;
				double_count+= (rowsum(0)==2*player_val)?1:0;
				double_count+= (rowsum(1)==2*player_val)?1:0;
				double_count+= (rowsum(2)==2*player_val)?1:0;
				double_count+= (columnsum(0)==2*player_val)?1:0;
				double_count+= (columnsum(1)==2*player_val)?1:0;
				double_count+= (columnsum(2)==2*player_val)?1:0;
				double_count+= (diag1()==2*player_val)?1:0;
				double_count+= (diag2()==2*player_val)?1:0;
				grid[row][column] = 0;  // Reverse the simulated move
				if (double_count >= 2)
				{
					return 3*row+column;
				}
			}
		}
	}
	return -1;
}

// Method to display the game banner
void display_banner()
{
	// Flash the banner four times
	for (int k=0;k<4;k++) 
	{
		clrscr();
		cout << endl << endl;
		cout << " ###" << endl;
		cout << "  #    #    #  #####  ######  #       #       #   ####   ######  #    #  #####" << endl;
		cout << "  #    ##   #    #    #       #       #       #  #    #  #       ##   #    #" << endl;
		cout << "  #    # #  #    #    #####   #       #       #  #       #####   # #  #    #" << endl;
		cout << "  #    #  # #    #    #       #       #       #  #  ###  #       #  # #    #" << endl;
		cout << "  #    #   ##    #    #       #       #       #  #    #  #       #   ##    #" << endl;
		cout << " ###   #    #    #    ######  ######  ######  #   ####   ######  #    #    #" << endl << endl;
		cout << "#######                    #######                      #######" << endl;
		cout << "   #     #     ####           #      ##     ####           #     ####   ######" << endl;
		cout << "   #     #    #    #          #     #  #   #    #          #    #    #  #" << endl;
		cout << "   #     #    #       ####    #    #    #  #       ####    #    #    #  ####" << endl;
		cout << "   #     #    #               #    ######  #               #    #    #  #" << endl;
		cout << "   #     #    #    #          #    #    #  #    #          #    #    #  #" << endl;
		cout << "   #     #     ####           #    #    #   ####           #     ####   ######" << endl;
		delay(1000);  // Wait for 1000 ms = 1 sec before clearing the screen
		clrscr();
		delay(300);   // Wait for 300 ms with blank screen before drawing the banner again, thus giving flashing effect
	}
}

int main()
{
	unsigned int seedval;
	time_t t;
	seedval = (unsigned) time(&t);
	srand(seedval);

	int i, j, row, column, move_count, count;
	int first_mover = rand()%2; // Randomly choose who has the 1st move
	int game_end = 0, game_result = 0;

	// Display the game banner
	display_banner();

	// Display player history & identify current player
	History hist;
	clrscr();
	hist.find_current_user();

	clrscr();
	// Instantiate an object of class Matrix
	// The squares on which neither player has marked a
	// move are represented by 0. The squares marked by
	// the human player are represented internally
	// as 1. The squares marked by the computer are
	// represented internally as -1
	Matrix gameMatrix;
	gameMatrix.drawfig();  // Draw the initial grid on screen
	for(move_count=0;move_count<9;move_count++)
	{
		if (move_count%2 == first_mover)
		{
			// Human player's move. Collect input.
			count = 0;
			do
			{
				if (count >0)
				{
					cout <<"Invalid move. Please enter again" << endl;
				}
				count += 1;
				cout<<"Enter the row number \n";
				cin>>row;
				if(row == 15)
				{
					// Hack to exit midway. Not published.
					exit(0);
				}
				row -= 1;
				cout<<"Enter the column number \n";
				cin>>column;
				column -= 1;
			} while ( row>=3 || \
				  column>=3 || \
				  row<0 || \
				  column<0 || \
				  gameMatrix.getGridPoint(row,column)!=0);
			gameMatrix.setGridPoint(row,column,1);  // Mark the move location with +1
		}
		else
		{
			// Computer's move. Calculate the move.
			int move_index = gameMatrix.compute_move(move_count);
			row = move_index/3;
			column = move_index%3;
			gameMatrix.setGridPoint(row,column,-1);  // Mark the move location with -1
		}
		// getch();
		gameMatrix.drawfig();
		// Check if either player has won the game. If yes announce the result
		for(int t=0;t<3;t++)
		{
			if(3==gameMatrix.rowsum(t))
			{
				cout<<"You win"<<endl;
				game_end=1;
				game_result = 1;
				break;
			}
			if(-3==gameMatrix.rowsum(t))
			{
				cout<<"Computer wins"<<endl;
				game_end=1;
				break;
			}
			if(3==gameMatrix.columnsum(t))
			{
				cout<<"You win"<<endl;
				game_end=1;
				game_result = 1;
				break;
			}
			if(-3==gameMatrix.columnsum(t))
			{
				cout<<"Computer wins"<<endl;
				game_end=1;
				break;
			}
		}
		if(3==gameMatrix.diag1())
		{
			cout<<"You win"<<endl;
			game_end=1;
			game_result = 1;
			break;
		}
		if(-3==gameMatrix.diag1())
		{
			cout<<"Computer wins"<<endl;
			game_end=1;
			break;
		}
		if(3==gameMatrix.diag2())
		{
			cout<<"You win"<<endl;
			game_end=1;
			game_result = 1;
			break;
		}
		if(-3==gameMatrix.diag2())
		{
			cout<<"Computer wins"<<endl;
			game_end=1;
			break;
		}

		if(game_end==1)
		{
			break;
		}
	}
	// If all moves are over without either player
	// winning, the game is a draw
	if(0==game_end)
	{
		cout<<"The game is a draw"<<endl;
		game_result = 2;
	}

	// Update the historical data file with the result
	// of the current game
	hist.update_current_user_result(game_result);
	cout << endl << "Press any key to continue" << endl;
	getch();

	// Display updated historical results
	hist.display();
	cout << endl << "Press any key to continue" << endl;
	getch();
	return 0;
}
