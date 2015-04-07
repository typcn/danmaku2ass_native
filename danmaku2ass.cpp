#include <stdio.h>
#include <string.h>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

/*
    Get comment type
    
    headline: the first line of comment file
    
    Return:
        1 - Acfun
        2 - Bilibili
        3 - Niconico
*/

int GetCommentType(string headline){
    if(headline.find("xml version=\"1.0\" encoding=\"UTF-8\"?><p") != std::string::npos){
        return 3;
    }else if(headline.find("!-- BoonSutazioData=") != std::string::npos){
        return 3;
    }else if(headline.find("xml version=\"1.0\" encoding=\"UTF-8\"?><i") != std::string::npos){
        return 2;
    }else if(headline.find("xml version=\"1.0\" encoding=\"utf-8\"?><i") != std::string::npos){
        return 2;
    }else if(headline.find("xml version=\"1.0\" encoding=\"Utf-8\"?>\n") != std::string::npos){
        return 2;
    }else if(headline.find("\"commentList\":[") != std::string::npos){
        return 1;
    }else{
        return 0;
    }
}

/*
    Convert comments to .ass subtitle
 
    infile: comment file path
    outfile: output file path
    width: video width
    height: video height
    font: font name
    alpha: comment alpha
    duration_marquee:Duration of scrolling comment
    duration_still:Duration of still comment
*/

void danmaku2ass(const char *infile,const char *outfile,int width,int height,const char *font,float fontsize,float alpha,float duration_marquee,float duration_still){
    std::ifstream input(infile);
    string headline;
    getline( input, headline);
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

    danmaku2ass(
        args["in"].c_str(), // Input file ( must be utf-8 )
        args["out"].c_str(), // Output file
        stoi(args["w"]), // Video width
        stoi(args["h"]), // Video height
        args["font"].c_str(), // Comment Font
        stof(args["fontsize"]), // Font Size
        stof(args["alpha"]), // Comment Alpha
        stof(args["dm"]), // Duration of scrolling comment
        stof(args["ds"]) // Duration of still comment
    );

	return 0;
}