#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define W 2000
#define H 2000
#define max_N 200
FILE* file = 0;
int saved_image_count = 0;
char file_name[50];
int file_number = 0;
int L; //layers
int N;
int neurons_in_layer[30] = {9,9,9,9,9,9,9};
double p[max_N][2];
int d[max_N];
int C[max_N][max_N];
void start_svg();
void finish_svg();
typedef struct {
	int r;
	int g;
	int b;
} color;

color red = {255,0,0};
color green = {0,255,0};
color blue = {0,0,255};
color white = {255,255,255};
color black = {0,0,0};
color gray = {50,50,50};
color background = {0,255,0};
typedef struct {
	int x;
	int y;
} point;
point origin = {0,0};
point corner = {W,H};


color random_color() {
	color c;
	c.r = rand()%256;
	c.g = rand()%256;
	c.b = rand()%256;
	return c;
}
void install_connections()
{
    int j = 0;
    L = 0;
    while (neurons_in_layer[j] > 0)
    {
        ++L;
        ++j;
    }
    N = 0;
    for (int l = 0; l < L; l++)
    {
        for (int i = 0; i < neurons_in_layer[l]; i++)
        {
            d[N] = l;
            ++N;
        }
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (d[i] - d[j] == 1)
                C[i][j] = 1;
            else
                C[i][j] = 0;
        }
    }
}

void install_structure(int w, int h)
{
    float in_fit = 30;
    float dx = (w-in_fit)/(L-1);
    float dy;
    int c = 0;
    float adjust = 1;
    for (int i = 0; i < L; i++)
    {
        dy = H/neurons_in_layer[i]-10;
        adjust = 0;
        for (int j = 0; j < neurons_in_layer[i]; j++)
        {
            p[c][0] = in_fit/2+i*dx;
            p[c][1] = w/2 + adjust*dy;
            if (j%2 == 0) adjust = abs(adjust) + 1; else adjust *= -1;
            c++;
        }
    }
}

void filled_rectangle(point p, int w, int h, color c) {
    fprintf(file, "<rect x = \"%d\" y = \"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(%d,%d,%d)\"/>\n", 
        p.x,p.y,w,h,c.r,c.g,c.b);
}

void line(point p, point q, color c, int t) {
	fprintf(file,"<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:rgb(%d,%d,%d);stroke-width:%d\"/>\n",
		p.x,p.y,q.x,q.y,c.r,c.g,c.b,t);
}
void filled_circle(point p, int r, color c) {
	fprintf(file,"<circle cx=\"%d\" cy=\"%d\" r=\"%d\" style=\"fill:rgb(%d,%d,%d)\"/>\n",
		p.x,p.y,r,c.r,c.g,c.b);
}
void empty_circle(point p, int r, color c) {
	fprintf(file,"<circle cx=\"%d\" cy=\"%d\" r=\"%d\" stroke=\"rgb(%d,%d,%d)\"/>\n",
        p.x,p.y,r,c.r,c.g,c.b);
}
void empty_circles(int n) {
	point center;
	int radius;
	color c;
	for (int i = 0; i < n; i++) { 
		center.x = rand()%W;
		center.y = rand()%H;
		radius = rand()% (H/10);
		c.r = rand()%256;
		c.g = rand()%256;
		c.b = rand()%256;
		empty_circle(center, radius, c);
	}
}
void filled_circles(int n) {
    point center;
    int radius;
    color c;
    for (int i = 0; i < n; i++) { 
        center.x = rand()%W;
        center.y = rand()%H;
        radius = rand()% (H/20);
        c.r = rand()%256;
        c.g = rand()%256;
        c.b = rand()%256;
        filled_circle(center, radius, c);
    }
}

void plot_structure(int left, int down)
{
    int brightness = 0;
	point p1, p2;
	color c;
	int t,r;
    for (int i= 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if (C[i][j]) {
				p1.x = (int)floor(left + p[i][0]);
				p1.y = (int)floor(down + p[i][1]);
				p2.x = (int)floor(left + p[j][0]);
				p2.y = (int)floor(down + p[j][1]);
				c = random_color(); //white;
				t = rand()%5;
            	line(p1,p2,c,t);
           	}
		}
	}
	for (int i = 0; i < N; i++) {
        c = random_color();
        p1.x = left + p[i][0];
		p1.y = down + p[i][1];
		r = rand()%20 + 2;
        filled_circle(p1, r, c);
    }
}

int main() {
	sprintf(file_name, "image%d.svg",saved_image_count);
    file =fopen(file_name,"w");
	start_svg(W,H);

	//add_rectangle(origin,500,500, red);
	//line(origin, corner, red, 10);
	//empty_circle(origin, 700, red);
	//filled_circles(400);
	install_connections();
	install_structure(W,H);
	filled_rectangle(origin, W, H, background);
	plot_structure(0,0);
	finish_svg();
	saved_image_count++;
    fclose(file);
}


void start_svg(int w,int h) {
    fprintf(file, "<svg xmlns=\"http:\/\/www.w3.org/2000/svg\" width=\"%d\" height=\"%d\">\n", w,h);
}
void finish_svg() {
    fprintf(file,"</svg>");
}


//<line x1="0" y1="0" x2="200" y2="200" style="stroke:rgb(255,0,0);stroke-width:2" />
