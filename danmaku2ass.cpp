#include <stdio.h>
#include <string.h>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

int GetCommentType(string headline){
	cout << headline;
}

void danmaku2ass(const char *infile,const char *outfile,int width,int height,const char *font,float fontsize,float alpha,float duration_marquee,float duration_still){
	ifstream in;
	in.open(infile, ios::in );
	string headline;
	in >> headline;
	GetCommentType(headline);
}

int main(int argc,char *argv[]){
	printf("Starting danmaku2ass native\n");

	map<string,string> args;

	int count;
	for (count=0; count<argc; count++){
		char *param = argv[count];
		char *str = strchr(param,'='); // Get value for param
		if(str){
			int pos = (int)(str - param); // Get position of "="
			char keybuf[pos-1];
			strncpy(keybuf, param + 1, (size_t)pos-1); // Get key for param
			keybuf[pos-1] = '\0';
			args[keybuf] = str+1;
		}else{
			continue;
		}
	}

	danmaku2ass(args["in"].c_str(),args["out"].c_str(),stoi(args["w"]),stoi(args["h"]),args["font"].c_str(),stof(args["fontsize"]),stof(args["alpha"]),stof(args["dm"]),stof(args["ds"]));

	return 0;
}