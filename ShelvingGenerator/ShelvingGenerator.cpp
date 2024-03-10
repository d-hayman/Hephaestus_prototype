/*********************************************************
* svg_box.c
*
* Generate an SVG file for a box.
*
* Author David Hayman
* Using Mitre Box code from Mike Field <hamster@snap.net.nz>
*
********************************************************/

#include "stdafx.h"
#include "ShelfUnit.h"

/*******************************************************************
* Settings that control the size of the box - set as static
* values because I can't be bothered with argument parsing
*******************************************************************/
/* Shelves dimensions (int 1/100ths of an inch */
static int iShelfHeight = 550;
static int iShelfWidth = 600;
static int iShelfDepth = 500;
static int iShelfThickness = 68;
static int iShelfHangover = 15;
static int iShelfCount = 3;
static int iBitDiameter = 13;

/*******************************************************************/
/* For getting the correct space characters in the polyline object */
static int polyFirst;
/*******************************************************************/

static void PolyStart(FILE *fp) {
	fprintf(fp,"<polyline points=\"");
	polyFirst = 1;
}

static void PolyPoint(FILE *fp, int x, int y) {
	if (!polyFirst)
		fprintf(fp, " ");
	polyFirst = 0;
	fprintf(fp, "%i,%i", x, y);
}

static void PolyEnd(FILE *fp) {
	fprintf(fp, "\" fill=\"black\" stroke=\"black\" stroke-width=\"3\" />\r\n");
}

//walls are (sh+st)*sc + ho X sd
void WallPanel(FILE *fp, int sh, int sd, int st, int ho, int sc, int bd) {
	int i;
	int x = 0, y = 0;
	PolyStart(fp);

	//draw the bottom line
	PolyPoint(fp,x, y);
	x += sd;
	PolyPoint(fp,x, y);

	//draw the shelf slots
	for (i = 0; i < sc; i++) {
		y += sh;
		PolyPoint(fp,x, y);
		x -= sd / 2 + bd / 2;
		PolyPoint(fp,x, y);
		y += bd;
		PolyPoint(fp,x, y);
		x += bd / 2;
		PolyPoint(fp, x, y);
		y += st - 2 * bd;
		PolyPoint(fp,x, y);
		x -= bd / 2;
		PolyPoint(fp, x, y);
		y += bd;
		PolyPoint(fp, x, y);
		x += sd / 2 + bd / 2;
		PolyPoint(fp,x, y);
	}

	//draw the top
	y += ho;
	PolyPoint(fp,x, y);
	x -= sd;
	PolyPoint(fp,x, y);

	//draw the back
	y -= (ho + sc*(sh + st));
	PolyPoint(fp,x, y);

	PolyEnd(fp);
}

//Shelves are (st+ho)*2+sw X sd
void ShelfPanel(FILE *fp, int sw, int sd, int st, int ho, int bd) {
	int i;
	int x = 0, y = 0;
	PolyStart(fp);

	//draw the bottom line
	PolyPoint(fp, x, y);
	x += 2*(st+ho)+sw;
	PolyPoint(fp, x, y);

	//draw one side
	y += sd;
	PolyPoint(fp, x, y);

	//draw the notches and shelf interface
	x -= ho;//left
	PolyPoint(fp, x, y);
	y -= sd / 2 + bd / 2;//up (notch + bit
	PolyPoint(fp, x, y);
	x -= bd;//left (make bit subnotch)
	PolyPoint(fp, x, y);
	y += bd / 2; // finish subnotch
	PolyPoint(fp, x, y);
	x -= st - bd * 2;//left
	PolyPoint(fp, x, y);
	y -= bd / 2; // finish subnotch
	PolyPoint(fp, x, y);
	x -= bd;//left (make bit subnotch)
	PolyPoint(fp, x, y);
	y += sd / 2 + bd / 2;//down
	PolyPoint(fp, x, y);
	x -= sw;//left across the shelf
	PolyPoint(fp, x, y);
	y -= sd / 2 + bd / 2;//up (notch + bit
	PolyPoint(fp, x, y);
	x -= bd;//left (make bit subnotch)
	PolyPoint(fp, x, y);
	y += bd / 2; // finish subnotch
	PolyPoint(fp, x, y);
	x -= st - bd * 2;//left
	PolyPoint(fp, x, y);
	y -= bd / 2; // finish subnotch
	PolyPoint(fp, x, y);
	x -= bd;//left (make bit subnotch)
	PolyPoint(fp, x, y);
	y += sd / 2 + bd / 2;//down
	PolyPoint(fp, x, y);
	x -= ho;//left
	PolyPoint(fp, x, y);

	//draw the other side
	y -= sd;
	PolyPoint(fp, x, y);

	PolyEnd(fp);
}

void StartDoc(FILE **fp, char* filename, int w, int h) {
	*fp = fopen(filename, "w");
	w = (w + 99) / 100 * 100;
	h = (h + 99) / 100 * 100;
	fprintf(*fp, "<?xml version=\"1.0\" standalone=\"no\"?>\r\n");
	fprintf(*fp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\r\n");
	fprintf(*fp, "<svg width=\"%iin\" height=\"%iin\" viewBox=\"0 0 %i %i\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\r\n", w / 100, h / 100, w, h);

}

void EndDoc(FILE **fp) {
	fprintf(*fp, "</svg>\r\n");
	fclose(*fp);
}

int main(int argc, char *argv[])
{
	FILE *fp;

	StartDoc(&fp, "Walls.svg", iShelfDepth, iShelfHangover + iShelfCount*(iShelfHeight + iShelfThickness));

	WallPanel(fp, iShelfHeight, iShelfDepth, iShelfThickness, iShelfHangover, iShelfCount, iBitDiameter);

	EndDoc(&fp);

	StartDoc(&fp, "Shelves.svg", 2*(iShelfHangover+iShelfThickness)+iShelfWidth, iShelfDepth);

	ShelfPanel(fp, iShelfWidth, iShelfDepth, iShelfThickness, iShelfHangover, iBitDiameter);

	EndDoc(&fp);

	return 0;
}
