#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define WORDLEN 12
#define MID 20
#define UP 0
#define UP_R 1
#define RIGHT 2
#define DOWN_R 3
#define DOWN 4
#define DOWN_L 5
#define LEFT 6
#define UP_L 7
#define MAXMATRIX MID*2
int debug = 1;
char dictionary[100][WORDLEN];
char Word[100][100];
char answer[100][100];
int dictSign[100]={0};
int dictSize;
int a[2];

int  max_x,max_y,min_x,min_y;
int  max_x_new,max_y_new,min_x_new,min_y_new;

static void
readDictionary(char *dictName)
{
	FILE *h;
	char s[80];
	int i;

	dictSize = 0;
	h = fopen(dictName, "rt");
	fgets(s, 80, h);

	while (!feof(h)) {
		s[strlen(s)-1] = '\0';  /* get rid of newline */
			strcpy(dictionary[dictSize++], s);
		fgets(s, 80, h);
		for(int i=0; i< strlen(dictionary[dictSize-1]); i++){
			 dictionary[dictSize-1][i] = toupper(dictionary[dictSize-1][i]);
		}
	}
	printf("Dictionary size is %d\n", dictSize);

	// for(i=0; i<dictSize; i++){
	// 	printf("%s\n",dictionary[i]);
	// }
}

int getMax(){
	int max=0,which=0;
	int all=0;
	for(int i=0; i<dictSize; i++){
		if(dictSign[i] == 1)
			continue;
		int len = strlen(dictionary[i]);
		if(max < len){
			max = len;
			which = i;
			all = 1;
		}
	}
	if(all == 0)
		return 0;
	dictSign[which] = 1;
	return which;
}


bool input(int x,int y,char *s,int direction){
	int len = strlen(s);

	switch(direction){
	case UP :
		for(int i=0; i<len; i++){
			if(s[i] != Word[x-i][y] && Word[x-i][y] != 0)
				return false;
		}
		for(int i=0; i<len; i++)
			Word[x-i][y]=s[i];
		break;
	case UP_R:
		for(int i=0; i<len; i++){
			if(s[i] != Word[x-i][y+i] && Word[x-i][y+i] != 0)
				return false;
		}
		for(int i=0; i<len; i++)
			Word[x-i][y+i]=s[i];
		break;
	case RIGHT:
		for(int i=0; i<len; i++){
			if(s[i] != Word[x][y+i] && Word[x][y+i] != 0)
				return false;
		}
		for(int i=0; i<len; i++)
			Word[x][y+i]=s[i];
		break;
	case DOWN_R:
		for(int i=0; i<len; i++){
			if(s[i] != Word[x+i][y+i] && Word[x+i][y+i] != 0)
				return false;
		}
		for(int i=0; i<len; i++)
			Word[x+i][y+i]=s[i];
		break;
	case DOWN:
		for(int i=0; i<len; i++){
			if(s[i] != Word[x+i][y] && Word[x+i][y] != 0)
				return false;
		}
		for(int i=0; i<len; i++)
			Word[x+i][y]=s[i];
		break;
	case DOWN_L:
		for(int i=0; i<len; i++){
			if(s[i] != Word[x+i][y-i] && Word[x+i][y-i] != 0)
				return false;
		}
		for(int i=0; i<len; i++)
			Word[x+i][y-i]=s[i];
		break;
	case LEFT:
		for(int i=0; i<len; i++){
			if(s[i] != Word[x][y-i] && Word[x][y-i] != 0)
				return false;
		}
		for(int i=0; i<len; i++)
			Word[x][y-i]=s[i];
		break;
	case UP_L:
		for(int i=0; i<len; i++){
			if(s[i] != Word[x-i][y-i] && Word[x-i][y-i] != 0)
				return false;
		}
		for(int i=0; i<len; i++)
			Word[x-i][y-i]=s[i];
		break;
	}
	return true;
}

void getWhere(int n){
	int layer;
	int n8=0;
	for(int i=1; ;i++){
		n8 += i*8;
		if(n <= n8){
			layer = i;
			n8 -= i*8;
			n = n-n8;
			int many = (n-1)/(i*2);

			switch(many){
				case 0:
					a[0] = MID-i-1+n;
					a[1] = MID-i;
					break;
				case 1:
					a[1] = MID-i+(n-1)%(i*2);
					a[0] = MID+i;
					break;
				case 2:
					a[0] = MID+i-(n-1)%(i*2);
					a[1] = MID+i;
					break;
				case 3:
					a[1] = MID+i-(n-1)%(i*2);
					a[0] = MID-i;
					break;
			}
			break;
		}
	}
}
int getDirect(int x, int y){
	if(x>MID){
		if(y>MID)
			return DOWN_L;
		else if(y==MID)
			return LEFT;
		else
			return UP_L;
	}
	else if(x == MID){
		if(y > MID)
			return DOWN;
		else
			return UP;
	}
	else{
		if(y>MID)
			return DOWN_R;
		else if(y==MID)
			return RIGHT;
		else
			return UP_R;
	}
}

void getMaxAndMin_X_Y(){
	for(int i=0; i<MAXMATRIX; i++){
		for(int j=0; j<MAXMATRIX; j++){
			if(Word[i][j] != 0){
				if(i > max_x)
					max_x = i;
				if( i< min_x )
					min_x = i;
				if(j> max_y)
					max_y = j;
				if( j < min_y)
					min_y = j;
			}
		}

	}
}

int isbetter(int x,int y,char *s,int direction){
	int len = strlen(s);
	int timers = 0;
	int kong = 0;
	switch(direction){
	case UP :
		max_x_new = max_x;
		max_y_new = max_y;
		min_x_new = min_x<x-len+1? min_x : x-len+1;
		min_y_new = min_y;

		for(int i=0; i<len; i++){
			if(Word[x-i][y] == 0){
				kong++;
				continue;
			}
			if(s[i] == Word[x-i][y])
				timers++;
			else 
				return -1;
		}



		if(kong == len)
			return -2;
		return timers;
	case UP_R:

		max_x_new = max_x;
		max_y_new = max_y<y+len-1? y-len-1:max_y;
		min_x_new = min_x<x-len+1? min_x : x-len+1;
		min_y_new = min_y;

		for(int i=0; i<len; i++){
			if(Word[x-i][y+i] == 0){
				kong++;
				continue;
			}
			if(s[i] == Word[x-i][y+i])
				timers++;
			else 
				return -1;
		}

		if(kong == len)
			return -2;
		return timers;
	case RIGHT:
		max_x_new = max_x;
		max_y_new = max_y<y+len-1? y-len-1:max_y;
		min_x_new = min_x;
		min_y_new = min_y;
		for(int i=0; i<len; i++){
			if(Word[x][y+i] == 0){
				kong++;
				continue;
			}
			if(s[i] == Word[x][y+i])
				timers++;
			else 
				return -1;
		}
		if(kong == len)
			return -2;
		return timers;
	case DOWN_R:
		max_x_new = max_x<x+len-1? x+len-1:max_x;
		max_y_new = max_y<y+len-1? y+len-1:max_y;
		min_x_new = min_x;
		min_y_new = min_y;
		for(int i=0; i<len; i++){
			if(Word[x+i][y+i] == 0)
				{ kong++; continue; }
			if(s[i] == Word[x+i][y+i])
				timers++;
			else 
				return -1;
		}
		if(kong == len)
			return -2;
		return timers;
	case DOWN:
		max_x_new = max_x<x+len-1? x+len-1:max_x;
		max_y_new = max_y;
		min_x_new = min_x;
		min_y_new = min_y;
		for(int i=0; i<len; i++){
			if(Word[x+i][y] == 0)
				{ kong++; continue; }
			if(s[i] == Word[x+i][y])
				timers++;
			else 
				return -1;
		}
		if(kong == len)
			return -2;
		return timers;
	case DOWN_L:
		max_x_new = max_x<x+len-1? x+len-1:max_x;
		max_y_new = max_y;
		min_x_new = min_x;
		min_y_new = min_y;
		for(int i=0; i<len; i++){
			if(Word[x+i][y-i] == 0)
				{ kong++; continue; }
			if(s[i] == Word[x+i][y-i])
				timers++;
			else 
				return -1;
		}
		if(kong == len)
			return -2;
		return timers;
	case LEFT:
		max_x_new = max_x;
		max_y_new = max_y;
		min_x_new = min_x;
		min_y_new = min_y<y-len+1?min_y:y-len+1;
		for(int i=0; i<len; i++){
			if(Word[x][y-i] == 0)
				{ kong++; continue; }
			if(s[i] == Word[x][y-i])
				timers++;
			else 
				return -1;
		}
		if(kong == len)
			return -2;
		return timers;
	case UP_L:
		max_x_new = max_x;
		max_y_new = max_y;
		min_x_new = min_x<x-len+1? min_x : x-len+1;
		min_y_new = min_y<y-len+1?min_y:y-len+1;
		for(int i=0; i<len; i++){
			if(Word[x-i][y-i] == 0)
				{ kong++; continue; }
			if(s[i] == Word[x-i][y-i])
				timers++;
			else 
				return -1;
		}
		if(kong == len)
			return -2;
		return timers;
	}
}

void ws_maker(){
	//printf("%d\n",getMax());
	char *s = dictionary[getMax()];
	int len = strlen(s);
	input(MID-len/2,MID,s,DOWN);
	printf("0 %s\n",s);

	for(int i=0; i<dictSize-1-4;i++){
		int judge=0;
		s = dictionary[getMax()];

		getMaxAndMin_X_Y();
		int max=-1,x=0,y=0,d=0,sizeX = 1000, sizeY = 1000;
		for(int j=1;j<1000 ;j++){
			getWhere(j);
			int xxx,kongtime=0;      //xxx isbetter 返回
			int bd = getDirect(a[0],a[1]);
			for(int k=0; k<8; k++){

				xxx = isbetter(a[0],a[1],s,(bd+k-1)%8);

				if(xxx == -2){
					kongtime++;
					xxx = 0;
				}
				if(xxx == -1)
					continue;

				if(max_x - min_x > max_y - min_y){
					if(max_x_new <= max_x && min_x_new >= min_x){
						if(max_y_new - min_y_new < sizeY){
							x = a[0];
							y = a[1];
							d = (bd+k-1)%8;
							max = xxx;
							sizeY = max_y_new - min_y_new;
						}
						else if( max_y_new - min_y_new == sizeY ){
							if(max < xxx){
								x = a[0];
								y = a[1];
								d = (bd+k-1)%8;
								max = xxx;
							}
						}
					}

				}
				else{
					if(max_y_new <= max_y && min_y_new >= min_y){
						if(max_x_new - min_x_new < sizeX){
							x = a[0];
							y = a[1];
							d = (bd+k-1)%8;
							max = xxx;
							sizeX = max_x_new - min_x_new;
						}
						else if( max_x_new - min_x_new == sizeX ){
							if(max < xxx){
								x = a[0];
								y = a[1];
								d = (bd+k-1)%8;
								max = xxx;
							}
						}
					}
				}


				
			}
			if(kongtime == 8)
				break;
		}
		// if(debug){
		// 	printf("%d %d %d %s %d\n",i+1,x,y,s,d);
		// }
		input(x,y,s,d);
	}
}

void setlastFour(){
	getMaxAndMin_X_Y();
	int lenth = max_x -min_x > max_y - min_y ? max_x -min_x : max_y - min_y;
	int r = 4;

	if(input(min_x,min_y,dictionary[getMax()],RIGHT))
			r--;
		else if(input(min_x,min_y,dictionary[getMax()],DOWN))
			r--;
	if(input(min_x+lenth,min_y,dictionary[getMax()],RIGHT))
			r--;
		else if(input(min_x+lenth,min_y,dictionary[getMax()],UP))
			r--;
	if(input(min_x,min_y+lenth,dictionary[getMax()],LEFT))
			r--;
		else if(input(min_x,min_y+lenth,dictionary[getMax()],DOWN))
			r--;

	if(input(min_x+lenth,min_y+lenth,dictionary[getMax()],LEFT))
			r--;
		else if(input(min_x+lenth,min_y+lenth,dictionary[getMax()],UP))
			r--;
	if(r == 0)
		printf("$$$END$$$\n");
	//inputlast(r);
}



int main(int argc, char **argv)
{	max_x=max_y=min_x=min_y=MID;

	readDictionary(*argv +1);
	ws_maker();
	setlastFour();
	getMaxAndMin_X_Y();
	if(debug)
	for(int i=0; i<MAXMATRIX; i++){
		for(int j=0; j<MAXMATRIX; j++)
			printf("%C",Word[i][j]);
		printf("\n");
	}
	FILE *h;
	h = fopen("answer.txt", "w+");
	for(int i=0; i<max_x - min_x +1 ; i++){
		for(int j=0; j<max_y - min_y +1; j++){
			answer[i][j] = Word[min_x+i][min_y+j];
			if(answer[i][j] == 0)
				answer[i][j] = 'Z';
			printf("%c ", answer[i][j]);
			fputc(answer[i][j],h);
		}
		printf("\n");
		fputc('\n',h);
	}


	printf("%d %d %d %d\n",max_x ,min_x, max_y ,min_y);

}

