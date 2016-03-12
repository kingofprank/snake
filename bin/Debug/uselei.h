#ifndef USELEI_H_INCLUDED
#define USELEI_H_INCLUDED

class point
{
public:
    int x,y;
    point operator +(point s)
    {
        point pp;
        pp.x = s.x + x;
        pp.y = s.y + y;
        return pp;
    }

};

class snack
{
public:
    point body[76];
    int tail;
    int head;
    int bodylen;
};


class giveplayer
{
public:
    int smap[40][40];
    snack p1;
    snack p2;
    point food;
    int mpsize;
}playerget;


#endif // USELEI_H_INCLUDED
