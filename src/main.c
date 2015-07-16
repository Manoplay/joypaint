/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <linux/joystick.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <memory.h>
	int joy_fd;
#define JOY_DEV "/dev/input/js1"
#define ALT_JOY_DEV "/dev/input/js0"
int menu(){
bool chosen;
	bool movt = true;
	//int joy_fd;
	int highlight = 0;
WINDOW *w;
	w = newwin(6, 20, LINES/2-3, COLS/2-10);
	box(w,0,0);
	mvwprintw(w,0,1,"MENU");
	wrefresh(w);

mvwprintw(w,1,1,"Menu");
struct JS_DATA_TYPE js;
		/* open the joystick */
	/*if( ( joy_fd = open( JOY_DEV, O_RDONLY ) ) < 0 )
	{
		printf( "Couldn't open joystick device %s. Using %s.\n", JOY_DEV,ALT_JOY_DEV );
		if( ( joy_fd = open( ALT_JOY_DEV, O_RDONLY ) ) < 0 )
		{
		printf( "Couldn't open joystick device %s\n", ALT_JOY_DEV );
		endwin();
		return;
		}
	}*/
while (!(chosen)){
if( read( joy_fd, &js, JS_RETURN ) != JS_RETURN )
		{
			printw( "\nFailed read\n" );
		}
	if(js.y > 200 && movt){
			movt = false;
			++highlight;
		}else if(js.y == 128){
			movt = true;
		}else if(js.y < 128 && movt){
			movt = false;
			if(highlight==0){highlight==2;}else{
			--highlight;}
		}
		if(js.buttons == 2){
			chosen = true;
			wborder(w, ' ', ' ', ' ',' ',' ',' ',' ',' ');
			wclear(w);
			wrefresh(w);
			delwin(w);
			return highlight;
		}
//RENDERIZZAZIONE
switch(highlight){
case 0:
wattron(w,A_REVERSE);
mvwprintw(w,1,1,"Exit");
wattroff(w,A_REVERSE);
mvwprintw(w,2,1,"Clean");
mvwprintw(w,3,1,"Save");
wrefresh(w);
break;
case 1:
wattron(w,A_REVERSE);
mvwprintw(w,2,1,"Clean");
wattroff(w,A_REVERSE);
mvwprintw(w,1,1,"Exit");
mvwprintw(w,3,1,"Save");
wrefresh(w);
break;
case 2:
wattron(w,A_REVERSE);
mvwprintw(w,3,1,"Save");
wattroff(w,A_REVERSE);
mvwprintw(w,1,1,"Exit");
mvwprintw(w,2,1,"Clean");
wrefresh(w);
break;
}
}
}

int main(int argc, const char **argv[])
{

	int xpos = 1;
	int ypos = 1;
	bool movt = true;
	bool ymovt = true;

	struct JS_DATA_TYPE js;
		/* open the joystick */
	if( ( joy_fd = open( JOY_DEV, O_RDONLY ) ) < 0 )
	{
		printf( "Couldn't open joystick device %s. Using %s.\n", JOY_DEV,ALT_JOY_DEV );
		if( ( joy_fd = open( ALT_JOY_DEV, O_RDONLY ) ) < 0 )
		{
		printf( "Couldn't open joystick device %s\n", ALT_JOY_DEV );
		endwin();
		return;
		}
	}
initscr();
curs_set(2);
	noecho();
getch();
while(1){

/* infinite loop */
			/* read the joystick state into js */
		if( read( joy_fd, &js, JS_RETURN ) != JS_RETURN )
		{
			printw( "\nFailed read\n" );
		}

			/* print the results */
		/*printf("X: % 4d  Y: % 4d  B1: %1d  B2: %1d  \r"
				,js.x	// X axis
				,js.y	// Y axis
				,(js.buttons & 1) ? 1 : 0	// button 1 
				,(js.buttons & 2) ? 1 : 0 );	// button 2 */
		if(js.x > 200 && movt){
			movt = false;
			xpos = xpos +1;
			move(ypos,xpos);
			refresh();
		}else if(js.x == 128){
			movt = true;
		}else if(js.x < 128 && movt){
			movt = false;
			xpos = xpos - 1;
			move(ypos,xpos);
			refresh();
		}
		if(js.y > 200 && ymovt){
			ymovt = false;
			++ypos;
			move(ypos,xpos);
			refresh();
		}else if(js.y == 128){
			ymovt = true;
		}else if(js.y < 128 && ymovt){
			ymovt = false;
			--ypos;
			move(ypos,xpos);
			refresh();
		}
		if(js.buttons == 2){
			mvprintw(ypos,xpos,"#");
			move(ypos,xpos);
			refresh();
		}else if(js.buttons == 1){
			int choose = menu();
			refresh();
			switch(choose){
			case 0:
			close(joy_fd);
			endwin();
			return 0;
			break;
			case 1:
			clear();
			refresh();
			break;
			case 2:
			if(getlogin() == "root"){
			sysyem("sudo /usr/share/joypaint/save.sh");	
			}else{
			system("tty > .tt; . /usr/share/joypaint/init.sh | echo $USER_PASSWD | sudo -S /usr/share/joypaint/save.sh; rm .tt");
			}
			break;
			}
		}}
		if( read( joy_fd, &js, JS_RETURN ) != JS_RETURN )
		{
			printw( "\nFailed read\n" );
		}

	close(joy_fd);	/* too bad we never get here */
endwin();
	return 0;
}
