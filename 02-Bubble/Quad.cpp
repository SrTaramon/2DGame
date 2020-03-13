#include <GL/glew.h>
#include <GL/gl.h>
#include "Quad.h"

#include <GL/glut.h>


Quad* Quad::createQuad(float x, float y, float width, float height, ShaderProgram& program)
{
	Quad* quad = new Quad(x, y, width, height, program);

	return quad;
}


Quad::Quad(float x, float y, float width, float height, ShaderProgram& program)
{
	float vertices[12] = { x, y, x + width, y, x + width, y + height, x, y, x + width, y + height, x, y + height };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);

}

void Quad::render() const
{

	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_LIGHTING);
	//glColor3f(1, 1, 1);
	//glRasterPos3f(0.0, 0.0, 0.0);
	char string[] = "ADEU!";
	//for (int i = 0; string[i] != '\0'; i++)
	//	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	void* pFont = GLUT_BITMAP_HELVETICA_12;  // also try _18

	glPushAttrib(GL_CURRENT_BIT);

	glColor3f(1, 1, 1);
	glDisable(GL_LIGHTING);
	// prepare to draw the text from the top-left corner
	glRasterPos2f(0, 0);

	// loop through the text
	for (const char* p = string; *p; p++)
		glutBitmapCharacter(pFont, *p);

	// This line was located in a stackechange answer on how to get colour set
	glPopAttrib();

	/*
	GLUT_BITMAP_8_BY_13
	GLUT_BITMAP_9_BY_15
	GLUT_BITMAP_TIMES_ROMAN_10
	GLUT_BITMAP_TIMES_ROMAN_24
	GLUT_BITMAP_HELVETICA_10
	GLUT_BITMAP_HELVETICA_12
	GLUT_BITMAP_HELVETICA_18
	*/

}

void drawText( const char* text, int iColour)
{

	void* pFont = GLUT_BITMAP_HELVETICA_12;  // also try _18

	glPushAttrib(GL_CURRENT_BIT);

	if (iColour == 0)
		glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */); //red
	else if (iColour == 1)
		glColor3f(0.0 /* red % */, 1.0 /* green % */, 0.0 /* blue % */); //green
	else if (iColour == 2)
		glColor3f(0.0 /* red % */, 0.0 /* green % */, 1.0 /* blue % */); //blue

	// prepare to draw the text from the top-left corner
	glRasterPos2f(0,0);

	// loop through the text
	for (const char* p = text; *p; p++)
		glutBitmapCharacter(pFont, *p);

	// This line was located in a stackechange answer on how to get colour set
	glPopAttrib();

}


void Quad::free()
{
	glDeleteBuffers(1, &vbo);
}