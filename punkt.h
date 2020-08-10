#ifndef PUNKT_H
#define PUNKT_H


class punkt
{
public:
    punkt();
    int x;
    int y;
    punkt &operator =(const punkt &p){
        this->x = p.x;
        this->y = p.y;
        return *this;
    }
    bool operator<(const punkt &p){
        return (this->y < p.y);
    }
};

#endif // PUNKT_H
