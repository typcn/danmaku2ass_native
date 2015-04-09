//
//  AssClass.cpp
//  danmaku2ass_native
//
//  Created by TYPCN on 2015/4/8.
//
//

#include "AssClass.hpp"
#include <string.h>
#include <iconv.h>
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
    
    srand((int)time(0));
    
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

void Ass::AppendComment(float appear_time,int comment_mode,int font_color,const char *content){
    int duration;

    string str = content;
    
    int ContentFontLen = (int)Utf8StringSize(str)*FontSize;
    
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

    string color = "";
    if(font_color != 16777215){
        if(font_color == 0x000000){
            color = "\\c&H000000&";
        }else if(font_color == 0xffffff){
            color = "\\c&HFFFFFF&";
        }else{
            int R = (font_color >> 16) & 0xff;
            int G = (font_color >> 8) & 0xff;
            int B = font_color & 0xff;
            char hexcolor[7];
            sprintf(hexcolor, "%02X%02X%02X",B,G,R);
            hexcolor[6] = '\0';
            string strcolor(hexcolor);
            color = "\\c&H" + strcolor + "&";
        }
    }

    stringstream ss;
    ss << "Dialogue: 2," << TS2t(appear_time) << "," << TS2t(appear_time + duration) << "," << style_name << ",,0000,0000,0000,,{" << effect << color << "}" << content;
    
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

template<typename _Iterator1, typename _Iterator2>
inline size_t IncUtf8StringIterator(_Iterator1& it, const _Iterator2& last) {
    if(it == last) return 0;
    unsigned char c;
    size_t res = 1;
    for(++it; last != it; ++it, ++res) {
        c = *it;
        if(!(c&0x80) || ((c&0xC0) == 0xC0)) break;
    }
    
    return res;
}

inline size_t Ass::Utf8StringSize(const std::string& str)  {
    size_t res = 0;
    std::string::const_iterator it = str.begin();
    for(; it != str.end(); IncUtf8StringIterator(it, str.end()))
        res++;
    
    return res;
}

static inline std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

void Ass::WriteToDisk(){
    typedef std::map<float, std::string>::iterator it_type;
    
    float MTime = 0;
    float TopTime = 0;
    float BottomTime = 0;
    
    int MROW = 0;
    int TopROW = 0;
    int BottomROW = 0;
    
    for(it_type iterator = comment_map.begin(); iterator != comment_map.end(); iterator++) {
        string r = iterator->second;
        
        if(r.find("[MROW]") != std::string::npos){
            float timeago =  iterator->first - MTime;
            if(timeago > 1){
                MROW = 0;
                MTime = iterator->first;
            }else{
                MROW++;
            }
            r = ReplaceAll(r,"[MROW]",to_string(MROW*FontSize));
        }else if(r.find("[TopROW]") != std::string::npos){
            float timeago =  iterator->first - TopTime;
            if(timeago > 1){
                TopROW = 0;
                TopTime = iterator->first;
            }else{
                TopROW++;
            }
            r = ReplaceAll(r,"[TopROW]",to_string(TopROW*FontSize));
        }else if(r.find("[BottomROW]") != std::string::npos){
            float timeago =  iterator->first - BottomTime;
            if(timeago > 1){
                BottomROW = 0;
                BottomTime = iterator->first;
            }else{
                BottomROW++;
            }
            r = ReplaceAll(r,"[BottomROW]",to_string(-BottomROW*FontSize));
        }else{
            continue;
        }
        
        out << r << endl;
    }
}
