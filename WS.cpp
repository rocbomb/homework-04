#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define WORDLEN 12
#define MID 15
#define UP 0
#define UP_R 1
#define RIGHT 2
#define DOWN_R 3
#define DOWN 4
#define DOWN_L 5
#define LEFT 6
#define UP_L 7

int debug = 1;
char dictionary[100][WORDLEN];
char Word[100][100];
int dictSign[100]={0};
int dictSize;
int a[2];

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
	}
	printf("Dictionary size is %d\n", dictSize);

	for(i=0; i<dictSize; i++){
		printf("%s\n",dictionary[i]);
	}
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
			dictSign[i] = 1;
		}
	}
	if(all == 0)
		return -1;
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

void ws_maker(){
	//printf("%d\n",getMax());
	char *s = dictionary[0];
	input(MID,MID,s,DOWN);
	//printf("%s\n",s);
	
	for(int i=0,j=1; i<dictSize-1;i++){
		int judge=0;
		s = dictionary[i+1];

		if(debug){
			printf("%d %s\n",i,s);
		}
		for(; ;j++){
			getWhere(j);
			int startD = (getDirect(a[0],a[1])+1)%8;
			for(int k=1; k<=8; k++){
				if(input(a[0],a[1],s,startD%8)){
					judge = 1;
					break;
				}
			}
			if(judge == 1)
				break;
		}
	}
}



int main(int argc, char **argv)
{
	//printf("sb%s%d\n",*argv,argc);
	/* read in the dictionary */
	readDictionary(argv[1]);
	ws_maker();
	//input(8,2,"love",UP);
	//input(8,1,"fove",UP_R);
	if(debug)
	for(int i=0; i<30; i++){
		for(int j=0; j<30; j++)
			printf("%C",Word[i][j]);
		printf("\n");
	}

}

