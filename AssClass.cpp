//
//  AssClass.cpp
//  danmaku2ass_native
//
//  Created by TYPCN on 2015/4/8.
//
//

#include "AssClass.hpp"
#include "string.h"
#include <iostream>
#include <sstream>

using namespace std;

void Ass::init(const char *filename){
    out.open(filename);
}

void Ass::SetDuration(int dm,int ds){
    duration_marquee = dm;
    duration_still = ds;
}

void Ass::WriteHead(int width,int height,const char *font,float fontsize,float alpha){
    
    srand(time(0));
    
    FontSize = fontsize;
    VideoHeight = height;
    VideoWidth = width;
    
    // Write a♂ss head info
    out << "[Script Info]\nScript Updated By: Danmaku2ASS_native (https://github.com/typcn/danmaku2ass_native)\nScriptType: v4.00+" << endl;
    out << "PlayResX: " << width << endl;
    out << "PlayResY: " << height << endl;
    out << "Aspect Ratio: " << width << ":" << height << endl;
    out << "Collisions: Normal" << endl;
    out << "WrapStyle: 2" << endl;
    out << "ScaledBorderAndShadow: yes" << endl;
    out << "YCbCr Matrix: TV.601" << endl << endl;
    
    char alpha_hex[3];
    sprintf(alpha_hex, "%02X", 255-round_int(alpha*255));
    
    // Write ass styles , maybe disorder , See https://en.wikipedia.org/wiki/SubStation_Alpha
    out << "[V4+ Styles]"<< endl;
    out << "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding"<< endl;
    
    // Get Style name
    stringstream ss;
    ss << "TYCM_" << rand() % (int)(9999 + 1);
    ss >> style_name;
    
    out << "Style: " <<
    style_name <<  "," << // Style name
    font << ", " << // Font name
    fontsize << ", " << // Font size
    "&H"<< alpha_hex <<"FFFFFF, " << // Primary Color
    "&H"<< alpha_hex <<"FFFFFF, " << // Secondary Color
    "&H"<< alpha_hex <<"000000, " << // Outline Color
    "&H"<< alpha_hex <<"000000, " << // Back Color
    "0, 0, 0, 0, 100, 100, 0.00, 0.00, 1, 1, 0, 7, 0, 0, 0, 0" << endl << endl;
    
    out << "[Events]" << endl;
    out << "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text" << endl;
}

void Ass::AppendComment(float appear_time,int comment_mode,const char *font_color,const char *content){
    int duration;
    int ContentFontLen =(int)strlen(content)*FontSize;
    
    char effect[30];
    if(comment_mode < 4){
        sprintf(effect,"\\move(%d, [MROW], %d, [MROW])",VideoWidth,-ContentFontLen);
        duration = duration_marquee;
    }else if(comment_mode == 4){
        sprintf(effect,"\\an2\\pos(%d,[BottomROW])",VideoWidth/2);
        duration = duration_still;
    }else if(comment_mode == 5){
        sprintf(effect,"\\an8\\pos(%d,[TopROW])",VideoWidth/2);
        duration = duration_still;
    }else{
        return;
    }
    stringstream ss;
    ss << "Dialogue: 2," << TS2t(appear_time) << "," << TS2t(appear_time + duration) << "," << style_name << ",,0000,0000,0000,,{" << effect << "}" << content;
    
    comment_map.insert(std::pair<float, std::string>(appear_time,ss.str()));
    
}

int Ass::round_int( double r ) {
    return (r > 0.0) ? (r + 0.5) : (r - 0.5);
}

inline string Ass::TS2t(double timestamp){
    int ts=timestamp*100;
    int hour,minute,second,centsecond;
    hour = ts/36000;
    minute = ts%36000;
    second = minute%6000;
    minute = minute/6000;
    centsecond = second%100;
    second = second/100;
    char buff[20];
    sprintf(buff,"%d:%02d:%02d.%02d", hour,minute,second,centsecond);
    return string(buff);
}

void Ass::WriteToDisk(){
    typedef std::map<float, std::string>::iterator it_type;
    for(it_type iterator = comment_map.begin(); iterator != comment_map.end(); iterator++) {
        out << iterator->second << endl;
    }
}
