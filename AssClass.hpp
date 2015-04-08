//
//  AssClass.h
//  danmaku2ass_native
//
//  Created by TYPCN on 2015/4/8.
//
//

#ifndef __danmaku2ass_native__AssClass__
#define __danmaku2ass_native__AssClass__

#include <stdio.h>
#include <fstream>
#include <map>

class Ass{
private:
    std::ofstream out;
    std::map <int, std::string> comment_map;
    int round_int( double r );
public:
    void init(const char *filename);
    void WriteHead(int width,int height,const char *font,float fontsize,float alpha);
    void AppendComment(float appear_time,int comment_mode,int font_size,const char *font_color,const char *content);
    void WriteToDisk();
};

#endif /* defined(__danmaku2ass_native__AssClass__) */
