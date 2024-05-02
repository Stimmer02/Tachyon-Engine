#include "Color.h"
bool Color::operator==(const Color &other){
    if(R == other.R && G == other.G && B == other.B && A == other.A){
        return true;
    }
    return false;
}
