// ZeroJudge d094 (UVa 478) - 點是否(嚴格)落在圖形內(矩形/圓/三角形), 邊上不算.
#include <bits/stdc++.h>
using namespace std;
struct Fig{ char type; double a,b,c,d,e,f; };
double crs(double ax,double ay,double bx,double by,double cx,double cy){
    return (ax-cx)*(by-cy)-(bx-cx)*(ay-cy);
}
int main(){
    vector<Fig> figs;
    string tok;
    while(cin>>tok){
        if(tok=="*") break;
        Fig g; g.type=tok[0];
        if(g.type=='r'){ cin>>g.a>>g.b>>g.c>>g.d; }        // 左上(a,b) 右下(c,d)
        else if(g.type=='c'){ cin>>g.a>>g.b>>g.c; }        // 圓心(a,b) 半徑 c
        else { cin>>g.a>>g.b>>g.c>>g.d>>g.e>>g.f; }        // 三頂點
        figs.push_back(g);
    }
    double px,py; int pi=0;
    while(cin>>px>>py){
        if(px==9999.9 && py==9999.9) break;
        pi++;
        bool any=false;
        for(size_t j=0;j<figs.size();j++){
            Fig&g=figs[j]; bool in=false;
            if(g.type=='r'){
                double x1=min(g.a,g.c), x2=max(g.a,g.c), y1=min(g.b,g.d), y2=max(g.b,g.d);
                in = (px>x1&&px<x2&&py>y1&&py<y2);
            } else if(g.type=='c'){
                double dx=px-g.a, dy=py-g.b;
                in = (dx*dx+dy*dy < g.c*g.c);
            } else {
                double d1=crs(px,py,g.a,g.b,g.c,g.d);
                double d2=crs(px,py,g.c,g.d,g.e,g.f);
                double d3=crs(px,py,g.e,g.f,g.a,g.b);
                in = (d1>0&&d2>0&&d3>0)||(d1<0&&d2<0&&d3<0);
            }
            if(in){ printf("Point %d is contained in figure %d\n", pi, (int)j+1); any=true; }
        }
        if(!any) printf("Point %d is not contained in any figure\n", pi);
    }
    return 0;
}
