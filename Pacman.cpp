#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

char tmp_map[29][53];
void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
char cpy[29][53];
char map[29][53] = {
	"+##################################################+",
	"|                        ##                        |",
	"|   #####    #######     ##      ######    #####   |",
	"|   #####    #######     ##      ######    #####   |",
	"|                                                  |",
	"|   #####   ##      ############      #    #####   |",	
	"|           ##           ##           #            |",	
	"|########   #########    ##    ########    ########|",	
	"|########   ##                        #    ########|",	
	"|                                                  |",	
	"|  #######    #   ###          ###    #   #######  |",
	"|  #######    #   ################    #   #######  |",
	"|  #######    #                       #   #######  |",
	"|                 ################                 |",
	"|    ##########         ##           ##########    |",
	"|    ##                 ##                   ##    |",
	"|    ##    ##########   ##   #############   ##    |",
	"|                                                  |",
	"|#######    ##    ################     ##   #######|",
	"|           ##    ################     ##          |",
	"|           ##           ##            ##          |",
	"|           ##           ##            ##          |",
	"|     ##############     ##     ##############     |",	
	"|                                                  |",
	"+##################################################+",};

void ShowMap()//in map ra ngoai man hinh
{
	for(int i = 0; i < 29; i++) {
		printf("%s\n",map[i] );
	}
}
void cpymap(){
	memcpy( cpy, map, sizeof(map) );
}
void gotoxy( short x, short y )
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE) ;
    COORD position = { x, y } ;

    SetConsoleCursorPosition( hStdout, position ) ;
}



struct walk {
	long walk_count;
	long x;
	long y;
	long back;
};

struct target {
	long x;
	long y;
};

vector<target> walk_que;

vector<walk> BFSArray;

void AddArray( long x, long y, long wc , long back ){//tao ra ma tran toa do ghost
	if( tmp_map[y][x] == ' ' || tmp_map[y][x] == '*' ){
		tmp_map[y][x] = '#';
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.walk_count = wc;
		tmp.back = back;
		BFSArray.push_back( tmp );
	}
}

void FindPath( long sx, long sy, long x, long y ){ // hunting hero
	memcpy( tmp_map, map, sizeof(map) );
	BFSArray.clear();
	walk tmp;
	tmp.x = sx;
	tmp.y = sy;
	tmp.walk_count = 0;
	tmp.back = -1;
	BFSArray.push_back( tmp );

	int i = 0;
	while( i < BFSArray.size() ){
		if( BFSArray[i].x == x && BFSArray[i].y == y ){
			walk_que.clear();
			target tmp2;
			while( BFSArray[i].walk_count != 0 ){
				tmp2.x = BFSArray[i].x;
				tmp2.y = BFSArray[i].y;
				walk_que.push_back( tmp2 );
				i = BFSArray[i].back;
			}

			break;
		}

		AddArray( BFSArray[i].x+1, BFSArray[i].y, BFSArray[i].walk_count+1, i );
		AddArray( BFSArray[i].x-1, BFSArray[i].y, BFSArray[i].walk_count+1, i );
		AddArray( BFSArray[i].x, BFSArray[i].y+1, BFSArray[i].walk_count+1, i );
		AddArray( BFSArray[i].x, BFSArray[i].y-1, BFSArray[i].walk_count+1, i );
		i++;
	}

	BFSArray.clear();
}


int main(int arc, char * argv[])
{	
	cpymap();
	ofstream outFile("C:\\Users\\minha\\OneDrive\\Desktop\\name & score.txt");
	string name;
	resizeConsole(1000, 1500);
	bool restart = true;
	while (restart == true){
		system("cls");
		memcpy( map, cpy, sizeof(cpy) );
	    bool running = true;
		long x = 25; //toa do x cua Hero
		long y = 23; //toa do y cua Hero
		long old_x; //vi tri x cu cua Hero
		long old_y; //Vi tri x cu cua Hero
	
		long ex = 1; //toa do x cua ghost
		long ey = 1;  //toa do y cua ghost
	
	
		long pts = 0; //diem so
	
		printf("Instruction:\n1. Arrow Keys to move your hero\n2. Eat the dots produced by the Eater to gain poins\n3. Don't get caught by the Eater\n\n");
		printf("H -> Hard\nN -> Normal\nE -> Easy\n\nChoose Level : ");
	
		char diffi;
		int speedmod = 3;
	
		cin >> diffi;
	
		if( diffi == 'N' ){
			speedmod = 2;
		}else if( diffi == 'H' ){
			speedmod = 1;
		}
	
		system("cls");
	    ShowMap();
	
		gotoxy( x, y ); cout << "H";
	
		int frame = 0;
		
		FindPath( ex,ey,x,y );
	
		while( running ){
			gotoxy( x, y ); cout << " ";
	
			old_x = x;
			old_y = y;
			// bat dau nhan su kien ban phim
			if ( GetAsyncKeyState( VK_UP ) ){
				if( map[y-1][x] == '*' ){ y--; pts++; } else
				if( map[y-1][x] == ' ' ) y--;
			}
			if ( GetAsyncKeyState( VK_DOWN ) ){
				if( map[y+1][x] == '*' ){ y++; pts++; } else
				if( map[y+1][x] == ' ' ) y++;
			}
			if ( GetAsyncKeyState( VK_LEFT ) ){
				if( map[y][x-1] == '*' ){ x--; pts++; } else
				if( map[y][x-1] == ' ' ) x--;
			}
			if ( GetAsyncKeyState( VK_RIGHT ) ){
				if( map[y][x+1] == '*' ){ x++; pts++; } else
				if( map[y][x+1] == ' ' ) x++;
			}
	
			if( old_x != x || old_y != y ){
				FindPath( ex,ey,x,y );
			}
	
			gotoxy( x,y ); cout << "H";
	
			map[ey][ex] = '*';
			gotoxy( ex, ey ); cout << "*";
	
			if( frame%speedmod == 0 && walk_que.size() != 0 ){
				ex = walk_que.back().x;
				ey = walk_que.back().y;
				walk_que.pop_back();
			}
	
			gotoxy( ex, ey ); cout << "E";
			if( (ex == x && ey == y) ){
				break; // khi ghost bat dc Hero
			}
	
	
			gotoxy( 53, 29 );
			gotoxy( 53, 1 ); cout << pts;
			Sleep( 80 );
			frame++;
		}
	
		system("cls");
		printf("You Lose and your score is : %i", pts );
		cout << "\nYour name: "; cin >> name;
		outFile << name << " - " << pts << "\n";
		cout << "\nIf you want to restart?";
		cout << "\nY -> yes \nN -> no\n";
		char key;
		cin >> key;
		if (key == 'Y'|| key == 'y') restart = true;
		else if (key == 'N' || key == 'n') break;
		system("cls");
	}
	return 0;
}
