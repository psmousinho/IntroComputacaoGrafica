#ifndef _MYGL_H_
#define _MYGL_H_

#include <math.h>
#include "definitions.h"
#include "pipeline.hpp"

typedef struct {
	int x,y;
} pixel;

typedef int color[4];

color red = {255,0,0,255};
color green = {0,255,0,255};
color blue = {0,0,255,255};
color black = {0,0,0,255};

//#######################################################
	//primitivas:
 
void putPixel(int x, int y, color c) {
	if(x < 0 || x >= IMAGE_WIDTH || y < 0 || y >= IMAGE_HEIGHT) {
		return;
	}
	FBptr[x*4 + y*4*IMAGE_WIDTH] = c[0];
	FBptr[x*4 + y*4*IMAGE_WIDTH + 1] = c[1];
	FBptr[x*4 + y*4*IMAGE_WIDTH + 2] = c[2];
	FBptr[x*4 + y*4*IMAGE_WIDTH + 3] = c[3];
}

void putPixel(pixel p, color c) {
	putPixel(p.x,p.y,c);
}

void clear() {
	for(int i = 0; i < IMAGE_WIDTH; i++) {
		for(int j = 0; j < IMAGE_HEIGHT; j++) {
			putPixel(i,j,black);
		}
	}
}

void interpolar(float per,color atual, color inicial, color fin) {
	for(int i = 0; i < 4; i++) {
		atual[i] = (per * inicial[i]) + ((1-per) * fin[i]);	
	}
} 

void drawLine(pixel i,pixel f, color c) {
	// Coordenadas iniciais
	int x = i.x;
	int y = i.y;
	//Deslocamentos
	int dx = f.x - i.x;
	int dy = f.y - i.y;
	
	if(abs(dx) > abs(dy)){
		//1 Octante
		if(dx > 0 && dy > 0) {
			int d = 2*dy - dx;
			int e_inc = 2*dy;
			int ne_inc = 2*(dy - dx);
			
			putPixel(i,c);			
			while(x < f.x){
				if(d <= 0){
					x++;
					d += e_inc;			
				}
				else{
					x++;
					y++;
					d += ne_inc;			
				}
				putPixel(x,y,c);
			}
		}

		//4 e 5 Octantes 
		else if(dx < 0) {
			drawLine(f,i,c);
		}

		//8 Octante e Linhas Horizontais
		else {
			int d = 2*dy + dx;
			int e_inc = 2*dy;
			int se_inc = 2*(dy + dx);
			
			putPixel(i,c);			
			while(x < f.x){
				if(d <= 0){
					x++;
					y--;
					d += se_inc;			
				}
				else{
					x++;
					d += e_inc;			
				}
				putPixel(x,y,c);
			}
		}
	}

	else {
		//7 Octante
		if(dx > 0 && dy < 0) {
			int d = dy + 2*dx;
			int s_inc =  2*dx;
			int se_inc = 2*(dy + dx);
			
			putPixel(x,y,c);
			while(y > f.y){
				if(d <= 0){
					y--;
					d += s_inc;
				}
				else {
					x++;
					y--;
					d += se_inc;
				}
				putPixel(x,y,c);
			}
		}

		//3 e 6 Octantes
		else if(dx < 0 || (i.x == f.x && i.y > f.y)){
			drawLine(f,i,c);
		}

		//2 Octante e linah verticais
		else {
			int d = dy + 2*-dx;
			int n_inc =  2*-dx;
			int ne_inc = 2*(dy - dx);
			
			putPixel(x,y,c);
			while(y < f.y){
				if(d <= 0){
					y++;
					x++;
					d += ne_inc;
				}
				else {
					y++;
					d += n_inc;
				}
				putPixel(x,y,c);
			}	
		}

	} 
}

void drawLine2(int xi,int yi, int xf, int yf, color c) {
	// Coordenadas iniciais
	int x = xi;
	int y = yi;
	//Deslocamentos
	int dx = xf - xi;
	int dy = yf - yi;
	
	if(abs(dx) > abs(dy)){
		//1 Octante
		if(dx > 0 && dy > 0) {
			int d = 2*dy - dx;
			int e_inc = 2*dy;
			int ne_inc = 2*(dy - dx);
			
			putPixel(x,y,c);			
			while(x < xf){
				if(d <= 0){
					x++;
					d += e_inc;			
				}
				else{
					x++;
					y++;
					d += ne_inc;			
				}
				putPixel(x,y,c);
			}
		}

		//4 e 5 Octantes 
		else if(dx < 0) {
			drawLine2(xf,yf,xi,yi,c);
		}

		//8 Octante e Linhas Horizontais
		else {
			int d = 2*dy + dx;
			int e_inc = 2*dy;
			int se_inc = 2*(dy + dx);
			
			putPixel(x,y,c);			
			while(x < xf){
				if(d <= 0){
					x++;
					y--;
					d += se_inc;			
				}
				else{
					x++;
					d += e_inc;			
				}
				putPixel(x,y,c);
			}
		}
	}

	else {
		//7 Octante
		if(dx > 0 && dy < 0) {
			int d = dy + 2*dx;
			int s_inc =  2*dx;
			int se_inc = 2*(dy + dx);
			
			putPixel(x,y,c);
			while(y > yf){
				if(d <= 0){
					y--;
					d += s_inc;
				}
				else {
					x++;
					y--;
					d += se_inc;
				}
				putPixel(x,y,c);
			}
		}

		//3 e 6 Octantes
		else if(dx < 0 || (xi == xf && yi > yf)){
			drawLine2(xf,yf,xi,yi,c);
		}

		//2 Octante e linah verticais
		else {
			int d = dy + 2*-dx;
			int n_inc =  2*-dx;
			int ne_inc = 2*(dy - dx);
			
			putPixel(x,y,c);
			while(y < yf){
				if(d <= 0){
					y++;
					x++;
					d += ne_inc;
				}
				else {
					y++;
					d += n_inc;
				}
				putPixel(x,y,c);
			}	
		}

	} 
}

void drawLineInterpolado(pixel i, pixel f, color cInicial, color cFinal) {
	//X e Y iniciais 
	int x = i.x;
	int y = i.y;
	//Deslocamentos
	int dx = f.x - i.x;
	int dy = f.y - i.y;

	//Variaveis de Interpolacao
	float per;
	float desTotal = sqrt(pow(dx,2) + pow(dy,2));
	color atual;
	
	if(abs(dx) > abs(dy)){
		//1 Octante
		if(dx > 0 && dy > 0) {
			int d = 2*dy - dx;
			int e_inc = 2*dy;
			int ne_inc = 2*(dy - dx);
			
			putPixel(i,cInicial);			
			while(x < f.x) {
				if(d <= 0) {
					x++;
					d += e_inc;			
				}
				else {
					x++;
					y++;
					d += ne_inc;			
				}

				per = sqrt((f.x - x)*(f.x - x) + (f.y - y) *(f.y - y) )  / desTotal;
				interpolar(per,atual,cInicial,cFinal);
				putPixel(x,y,atual);
				
			}
		}
		
		//4 e 5 octante
		else if(dx < 0) {
			drawLineInterpolado(f,i,cFinal,cInicial);
		}

		//8 Octante e Linhas horizontais
		else {
			int d = 2*dy + dx;
			int e_inc = 2*dy;
			int se_inc = 2*(dy + dx);
			
			putPixel(i,cInicial);			
			while(x < f.x){
				if(d <= 0){
					x++;
					y--;
					d += se_inc;			
				}
				else{
					x++;
					d += e_inc;			
				}
				
				per = sqrt((f.x - x)*(f.x - x) + (f.y - y) *(f.y - y) )  / desTotal;
				interpolar(per,atual,cInicial,cFinal);
				putPixel(x,y,atual);

			}
		}
	}

	else {
		//7 Octante
		if(dx > 0 && dy < 0) {
			int d = dy + 2*dx;
			int s_inc =  2*dx;
			int se_inc = 2*(dy + dx);
			
			putPixel(i,cInicial);
			while( y > f.y){
				if(d <= 0){
					y--;
					d += s_inc;
				}
				else {
					x++;
					y--;
					d += se_inc;
				}

				per = sqrt((f.x - x)*(f.x - x) + (f.y - y) *(f.y - y) )  / desTotal;
				interpolar(per,atual,cInicial,cFinal);
				putPixel(x,y,atual);

			}
			
		}

		//3 e 6 Octante
		else if(dx < 0 || (i.x == f.x && i.y > f.y)){
			drawLineInterpolado(f,i,cFinal,cInicial);
		}

		//2 Octante e Linhas Verticais
		else {
			int d = dy + 2*-dx;
			int n_inc =  2*-dx;
			int ne_inc = 2*(dy - dx);
			
			putPixel(i,cInicial);
			while(y < f.y){
				if(d <= 0){
					x++;
					y++;
					d += ne_inc;
				}
				else {
					y++;
					d += n_inc;
				}
				
				per = sqrt((f.x - x)*(f.x - x) + (f.y - y) *(f.y - y) )  / desTotal;
				interpolar(per,atual,cInicial,cFinal);
				putPixel(x,y,atual);
			}
			
		}

	} 
}

void drawTriangle(pixel v1,pixel v2, pixel v3, color c) {
	drawLine(v1,v2,c);
	drawLine(v1,v3,c);
	drawLine(v2,v3,c);
}

void drawTriangleInterpolado(pixel v1 ,color c1 ,pixel v2 ,color c2 , pixel v3,color c3) {
	drawLineInterpolado(v1,v2,c1,c2);
	drawLineInterpolado(v1,v3,c1,c3);
	drawLineInterpolado(v2,v3,c2,c3);
}

//#######################################################
	//Uteis:

void demo() {
	pixel i;
	pixel m;
	pixel f;
	
	i.x = 256;
	i.y = 156;
	m.x = 156;
	m.y = 356;
	f.x = 356;
	f.y = 356;
	color white = {255,255,255,255};
	drawTriangle(i,m,f,white);

	i.x = 0;
	i.y = 325;
	f.x = 206;
	f.y = 256;
	drawLine(i,f,white);
	
	i.x = 290;
	i.y = 225;
	m.x = 315;
	m.y = 275;
	color gray = {50,50,50,255};
	drawLineInterpolado(f,i,white,gray);
	drawLineInterpolado(f,m,white,gray);

	pixel aux;
	for (int c = 0,d = 0; c < 50; c++){		
		aux.x = i.x + d;
		aux.y = i.y + c;		
		drawLineInterpolado(f,aux,white,gray);
		if(c % 2 == 0) {
			d++;		
		} 
	}

	pixel aux2;
	aux2.x = 511;
	i.x++;
	for (int s = 0; s < 6; s++){
		color c;
		switch(s) {
			case 0:
			c[0] = 254;
			c[1] = 2;
			c[2] = 49;
			c[3] = 255;
			break;	
			case 1:
			c[0] = 254;
			c[1] = 148;
			c[2] = 13;
			c[3] = 255;
			break;
			case 2:
			c[0] = 251;
			c[1] = 245;
			c[2] = 11;
			c[3] = 255;
			break;
			case 3:
			c[0] = 88;
			c[1] = 253;
			c[2] = 101;
			c[3] = 255;
			break;
			case 4:
			c[0] = 46;
			c[1] = 184;
			c[2] = 255;
			c[3] = 255;
			break;
			case 5:
			c[0] = 180;
			c[1] = 147;
			c[2] = 251;
			c[3] = 255;
			break;
		}
		
		for(int k = 0,j = 0; k <= 8; k++) {
			aux.x = i.x + j;
			aux.y =	i.y + k;
			aux2.y = aux.x + 10  ;
			drawLine(aux,aux2,c);
			if(k%2 == 0) {
				j++;		
			}
		}
		i.x+=4;
		i.y+= 8;
	}
}

void drawAxis() {
	pixel pi ,pf;
	pi.x = pf.x = 256;
	pi.y = 0;
	pf.y = 511;
	drawLine(pi,pf, red);
	pi.y = pf.y = 256;
	pi.x = 0;
	pf.x = 511;
	drawLine(pi,pf, green);
}

void cube(Pipeline p) {
	std::vector<glm::vec4> vertices(8);
	vertices[0] = glm::vec4(-1,-1,-1, 1);
	vertices[1] = glm::vec4(-1,-1, 1, 1);
	vertices[2] = glm::vec4( 1,-1,-1, 1);
	vertices[3] = glm::vec4( 1,-1, 1, 1);
	vertices[4] = glm::vec4(-1, 1,-1, 1);
	vertices[5] = glm::vec4(-1, 1, 1, 1);
	vertices[6] = glm::vec4( 1, 1,-1, 1);
	vertices[7] = glm::vec4( 1, 1, 1, 1);

	vertices = p.transformMesh(vertices);

	pixel i = {(int)vertices[0].x,(int)vertices[0].y};
	pixel f = {(int)vertices[1].x,(int)vertices[1].y};
	drawLine(i,f,blue);
	i = {(int)vertices[0].x,(int)vertices[0].y};
	f = {(int)vertices[2].x,(int)vertices[2].y};
	drawLine(i,f,blue);
	i = {(int)vertices[1].x,(int)vertices[1].y};
	f = {(int)vertices[3].x,(int)vertices[3].y};
	drawLine(i,f,blue);
	i = {(int)vertices[2].x,(int)vertices[2].y};
	f = {(int)vertices[3].x,(int)vertices[3].y};
	drawLine(i,f,blue);
	i = {(int)vertices[4].x,(int)vertices[4].y};
	f = {(int)vertices[5].x,(int)vertices[5].y};
	drawLine(i,f,blue);
	i = {(int)vertices[4].x,(int)vertices[4].y};
	f = {(int)vertices[6].x,(int)vertices[6].y};
	drawLine(i,f,blue);
	i = {(int)vertices[5].x,(int)vertices[5].y};
	f = {(int)vertices[7].x,(int)vertices[7].y};
	drawLine(i,f,blue);
	i = {(int)vertices[6].x,(int)vertices[6].y};
	f = {(int)vertices[7].x,(int)vertices[7].y};
	drawLine(i,f,blue);
	i = {(int)vertices[0].x,(int)vertices[0].y};
	f = {(int)vertices[4].x,(int)vertices[4].y};
	drawLine(i,f,blue);
	i = {(int)vertices[1].x,(int)vertices[1].y};
	f = {(int)vertices[5].x,(int)vertices[5].y};
	drawLine(i,f,blue);
	i = {(int)vertices[2].x,(int)vertices[2].y};
	f = {(int)vertices[6].x,(int)vertices[6].y};
	drawLine(i,f,blue);
	i = {(int)vertices[3].x,(int)vertices[3].y};
	f = {(int)vertices[7].x,(int)vertices[7].y};
	drawLine(i,f,blue);
}

void drawMesh(std::vector<glm::vec4> vertices) {
	pixel pi,pm,pf;
	for (int i = 0; i < vertices.size(); i+=3) {
		pi.x = (int) vertices[i].x;
		pi.y = (int) vertices[i].y;
		pm.x = (int) vertices[i+1].x;
		pm.y = (int) vertices[i+1].y;
		pf.x = (int) vertices[i+2].x;
		pf.y = (int) vertices[i+2].y;
		drawTriangle(pi,pm,pf,blue);
	}
}

#endif // _MYGL_H_
