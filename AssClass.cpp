//
//  AssClass.cpp
//  danmaku2ass_native
//
//  Created by TYPCN on 2015/4/8.
//
//

#include "AssClass.hpp"
#include <iostream>

using namespace std;

void Ass::init(const char *filename){
    out.open(filename);
}

void Ass::WriteHead(int width,int height,const char *font,float fontsize,float alpha){
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
    out << "Style: Danmaku2ASS_3c1a, " << // Style name
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

void Ass::AppendComment(float appear_time,int comment_mode,int font_size,const char *font_color,const char *content){
    
}

int Ass::round_int( double r ) {
    return (r > 0.0) ? (r + 0.5) : (r - 0.5);
}

void Ass::WriteToDisk(){
    
}
