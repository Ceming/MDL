/*========== my_main.c ==========
  This is the only file you need to modify in order
  to get a working mdl project (for now).
  my_main.c will serve as the interpreter for mdl.
  When an mdl script goes through a lexer and parser, 
  the resulting operations will be in the array op[].
  Your job is to go through each entry in op and perform
  the required action from the list below:
  push: push a new origin matrix onto the origin stack
  pop: remove the top matrix on the origin stack
  move/scale/rotate: create a transformation matrix 
                     based on the provided values, then 
		     multiply the current top of the
		     origins stack by it.
  box/sphere/torus: create a solid object based on the
                    provided values. Store that in a 
		    temporary matrix, multiply it by the
		    current top of the origins stack, then
		    call draw_polygons.
  line: create a line based on the provided values. Store 
        that in a temporary matrix, multiply it by the
	current top of the origins stack, then call draw_lines.
  save: call save_extension with the provided filename
  display: view the image live
  
  jdyrlandweaver
  =========================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parser.h"
#include "symtab.h"
#include "y.tab.h"

#include "matrix.h"
#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "stack.h"

void my_main() {

  int i;
  struct matrix *tmp;
  struct stack *s;
  screen t;
  color g;
  
  s = new_stack();
  tmp = new_matrix(4, 1000);
  clear_screen( t );

  for (i=0;i<lastop;i++) {  
    switch (op[i].opcode) {
      while ( fgets(line, sizeof(line), f) != NULL ) {
	line[strlen(line)-1]='\0';
	//printf(":%s:\n",line);
	
	double xvals[4];
	double yvals[4];
	double zvals[4];
	double r, r1;
	double theta;
	struct matrix *tmp;]
	char axis;
	int type;
	double step = 0.10;
    
	if ( strncmp(line, "push", strlen(line)) == 0){
	  push(stacks);
	}

	else if ( strncmp(line, "pop", strlen(line)) == 0){
	  pop(stacks);
	}
    
	else if ( strncmp(line, "box", strlen(line)) == 0 ) {
	  fgets(line, sizeof(line), f);
	  //printf("BOX\t%s", line);

	  sscanf(line, "%lf %lf %lf %lf %lf %lf",
		 xvals, yvals, zvals,
		 xvals+1, yvals+1, zvals+1);
	  add_box( tmpPoly, xvals[0], yvals[0], zvals[0],
		   xvals[1], yvals[1], zvals[1]);
	  matrix_mult(stacks->data[stacks->top], tmpPoly);
	  draw_polygons(tmpPoly, s, c);
	  tmpPoly->lastcol = 0;
	}//end of box

	else if ( strncmp(line, "sphere", strlen(line)) == 0 ) {
	  fgets(line, sizeof(line), f);
	  //printf("SPHERE\t%s", line);

	  sscanf(line, "%lf %lf %lf %lf",
		 xvals, yvals, zvals, &r);
	  add_sphere( tmpPoly, xvals[0], yvals[0], zvals[0], r, step);
	  matrix_mult(stacks->data[stacks->top], tmpPoly);
	  draw_polygons(tmpPoly, s, c);
	  tmpPoly->lastcol = 0;
	}//end of sphere

	else if ( strncmp(line, "torus", strlen(line)) == 0 ) {
	  fgets(line, sizeof(line), f);
	  //printf("torus\t%s", line);

	  sscanf(line, "%lf %lf %lf %lf %lf",
		 xvals, yvals, zvals, &r, &r1);
	  add_torus( tmpPoly, xvals[0], yvals[0], zvals[0], r, r1, step);
	  matrix_mult(stacks->data[stacks->top], tmpPoly);
	  draw_polygons(tmpPoly, s, c);
	  tmpPoly->lastcol = 0;
	}//end of torus

	else if ( strncmp(line, "circle", strlen(line)) == 0 ) {
	  fgets(line, sizeof(line), f);
	  //printf("CIRCLE\t%s", line);

	  sscanf(line, "%lf %lf %lf %lf",
		 xvals, yvals, zvals, &r);
	  add_circle( tmpEdge, xvals[0], yvals[0], zvals[0], r, step);
	  matrix_mult(stacks->data[stacks->top], tmpEdge);
	  draw_lines(tmpEdge, s, c);
	  tmpEdge->lastcol = 0;
	}//end of circle

	else if ( strncmp(line, "hermite", strlen(line)) == 0 ||
		  strncmp(line, "bezier", strlen(line)) == 0 ) {
	  if (strncmp(line, "hermite", strlen(line)) == 0 )
	    type = HERMITE;
	  else
	    type = BEZIER;
      
	  fgets(line, sizeof(line), f);
	  //printf("CURVE\t%s", line);

	  sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf",
		 xvals, yvals, xvals+1, yvals+1,
		 xvals+2, yvals+2, xvals+3, yvals+3);
	  /* printf("%lf %lf %lf %lf %lf %lf %lf %lf\n", */
	  /* 	     xvals[0], yvals[0], */
	  /* 	     xvals[1], yvals[1], */
	  /* 	     xvals[2], yvals[2], */
	  /* 	     xvals[3], yvals[3]); */
      
	  //printf("%d\n", type);
	  add_curve( tmpEdge, xvals[0], yvals[0], xvals[1], yvals[1],
		     xvals[2], yvals[2], xvals[3], yvals[3], step, type);
	  matrix_mult(stacks->data[stacks->top], tmpEdge);
	  draw_lines(tmpEdge, s, c);
	  tmpEdge->lastcol = 0;
	}//end of curve
    
	else if ( strncmp(line, "line", strlen(line)) == 0 ) {
	  fgets(line, sizeof(line), f);
	  //printf("LINE\t%s", line);

	  sscanf(line, "%lf %lf %lf %lf %lf %lf",
		 xvals, yvals, zvals,
		 xvals+1, yvals+1, zvals+1);
	  /*printf("%lf %lf %lf %lf %lf %lf",
	    xvals[0], yvals[0], zvals[0],
	    xvals[1], yvals[1], zvals[1]) */
	  add_edge(tmpEdge, xvals[0], yvals[0], zvals[0],
		   xvals[1], yvals[1], zvals[1]);
	  matrix_mult(stacks->data[stacks->top], tmpEdge);
	  draw_lines(tmpEdge, s, c);
	  tmp->lastcol = 0;
	}//end line

	else if ( strncmp(line, "scale", strlen(line)) == 0 ) {
	  fgets(line, sizeof(line), f);
	  //printf("SCALE\t%s", line);
	  sscanf(line, "%lf %lf %lf",
		 xvals, yvals, zvals);
	  /* printf("%lf %lf %lf\n", */
	  /* 	xvals[0], yvals[0], zvals[0]); */ 
	  tmp = make_scale( xvals[0], yvals[0], zvals[0]);
	  matrix_mult(stacks->data[stacks->top], tmp);
	  copy_matrix(tmp, stacks->data[stacks->top]);
	}//end scale

	else if ( strncmp(line, "move", strlen(line)) == 0 ) {
	  fgets(line, sizeof(line), f);
	  //printf("MOVE\t%s", line);
	  sscanf(line, "%lf %lf %lf",
		 xvals, yvals, zvals);
	  /* printf("%lf %lf %lf\n", */
	  /* 	xvals[0], yvals[0], zvals[0]); */ 
	  tmp = make_translate( xvals[0], yvals[0], zvals[0]);
	  matrix_mult(stacks->data[stacks->top], tmp);
	  copy_matrix(tmp, stacks->data[stacks->top]);
	}//end translate

	else if ( strncmp(line, "rotate", strlen(line)) == 0 ) {
	  fgets(line, sizeof(line), f);
	  //printf("Rotate\t%s", line);
	  sscanf(line, "%c %lf",
		 &axis, &theta);      
	  /* printf("%c %lf\n", */
	  /* 	axis, theta); */
	  theta = theta * (M_PI / 180);
	  if ( axis == 'x' )
	    tmp = make_rotX( theta );
	  else if ( axis == 'y' )
	    tmp = make_rotY( theta );
	  else 
	    tmp = make_rotZ( theta );
      
	  matrix_mult(stacks->data[stacks->top], tmp);
	  copy_matrix(tmp, stacks->data[stacks->top]);
	}//end rotate

	else if ( strncmp(line, "clear", strlen(line)) == 0 ) {
	  //printf("clear\t%s", line);
	  edges->lastcol = 0;
	}//end clear

	else if ( strncmp(line, "ident", strlen(line)) == 0 ) {
	  //printf("IDENT\t%s", line);
	  ident(transform);
	}//end ident
    
	/*else if ( strncmp(line, "apply", strlen(line)) == 0 ) {
	//printf("APPLY\t%s", line);
	matrix_mult(transform, edges);
	}//end apply
	*/
    
	else if ( strncmp(line, "display", strlen(line)) == 0 ) {
	  //printf("DISPLAY\t%s", line);
	  display( s );
	}//end display

	else if ( strncmp(line, "save", strlen(line)) == 0 ) {
	  fgets(line, sizeof(line), f);
	  *strchr(line, '\n') = 0;
	  //printf("SAVE\t%s\n", line);
	  save_extension(s, line);
	}//end save
    }
  }
}
