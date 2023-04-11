#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <stdlib.h>
#include <GL/GL.h>

#include "vmath.h"
#include "common.h"
#include "constants.h"


namespace sphere
{

class Mesh
{

    unsigned short *elements;
    float *verts;
    float *norms;
    float *texCoords;
    
    int numElements;
    int maxElements;
    int numVertices;

    int iNumIndices;
    
    GLuint vbo_position;
    GLuint vbo_normal;
    GLuint vbo_texture;
    GLuint vbo_index;
    GLuint vao;

public:
    inline void allocate(int numIndices)
    {
        // function declaration
        void cleanupMeshData();

        // code
        // first cleanup, if not initially empty
        this->cleanupMeshData();
        
        maxElements = numIndices;
        numElements = 0;
        numVertices = 0;
        
        iNumIndices=numIndices/3;
        
        elements = new unsigned short[iNumIndices * 3 * 2]; // 3 is x,y,z and 2 is sizeof short
        verts = new float[iNumIndices * 3 * 4]; // 3 is x,y,z and 4 is sizeof float
        norms = new float[iNumIndices * 3 * 4]; // 3 is x,y,z and 4 is sizeof float
        texCoords = new float[iNumIndices * 2 * 4]; // 2 is s,t and 4 is sizeof float
    }

    // Add 3 vertices, 3 normal and 2  i.e. one triangle to the geometry.
    // This searches the current list for identical vertices (exactly or nearly) and
    // if one is found, it is added to the index array.
    // if not, it is added to both the index array and the vertex array.
    inline void addTriangle(float **single_vertex, float **single_normal, float **single_texture)
    {
        //functions declarations
        void normalizeVector(float* single_normal);
        bool isFoundIdentical(float a, float b, float tollerence);
        void cleanupMeshData();

        //variable declarations
        const float diff = 0.00001;
        int i, j;
        // code
        // normals should be of unit length
        this->normalizeVector(single_normal[0]);
        this->normalizeVector(single_normal[1]);
        this->normalizeVector(single_normal[2]);
        
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < numVertices; j++) //for the first ever iteration of 'j', numVertices will be 0 because of it's initialization in the parameterized constructor
            {
                if (this->isFoundIdentical(verts[j * 3], single_vertex[i][0], diff) &&
                    this->isFoundIdentical(verts[(j * 3) + 1], single_vertex[i][1], diff) &&
                    this->isFoundIdentical(verts[(j * 3) + 2], single_vertex[i][2], diff) &&
                    
                    this->isFoundIdentical(norms[j * 3], single_normal[i][0], diff) &&
                    this->isFoundIdentical(norms[(j * 3) + 1], single_normal[i][1], diff) &&
                    this->isFoundIdentical(norms[(j * 3) + 2], single_normal[i][2], diff) &&
                    
                    this->isFoundIdentical(texCoords[j * 2], single_texture[i][0], diff) &&
                    this->isFoundIdentical(texCoords[(j * 2) + 1], single_texture[i][1], diff))
                {
                    elements[numElements] = j;
                    numElements++;
                    break;
                }
            }
            
            //If the single vertex, normal and texture do not match with the given, then add the corressponding triangle to the end of the list
            if (j == numVertices && numVertices < maxElements && numElements < maxElements)
            {
                verts[numVertices * 3] = single_vertex[i][0];
                verts[(numVertices * 3) + 1] = single_vertex[i][1];
                verts[(numVertices * 3) + 2] = single_vertex[i][2];
                
                norms[numVertices * 3] = single_normal[i][0];
                norms[(numVertices * 3) + 1] = single_normal[i][1];
                norms[(numVertices * 3) + 2] = single_normal[i][2];
                
                texCoords[numVertices * 2] = single_texture[i][0];
                texCoords[(numVertices * 2) + 1] = single_texture[i][1];
                
                elements[numElements] = numVertices; //adding the index to the end of the list of elements/indices
                numElements++; //incrementing the 'end' of the list
                numVertices++; //incrementing count of vertices
            }
        }
    }
    
    inline void prepareToDraw()
    {
        // vao
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        
        // vbo for position
        glGenBuffers(1, &vbo_position);
        glBindBuffer(GL_ARRAY_BUFFER,vbo_position);
        glBufferData(GL_ARRAY_BUFFER, (iNumIndices * 3 * 4),verts,GL_STATIC_DRAW);

        glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION,
                               3,
                               GL_FLOAT,
                               false,0,0);
        glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // vbo for normals
        glGenBuffers(1, &vbo_normal);
        glBindBuffer(GL_ARRAY_BUFFER,vbo_normal);
        glBufferData(GL_ARRAY_BUFFER,
                        (iNumIndices * 3 * 4),
                        norms,
                        GL_STATIC_DRAW);
        glVertexAttribPointer(DOMAIN_ATTRIBUTE_NORMAL,
                               3,
                               GL_FLOAT,
                               false,0,0);
        glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_NORMAL);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        
        // vbo for texture
        glGenBuffers(1, &vbo_texture);
        glBindBuffer(GL_ARRAY_BUFFER,vbo_texture);
        glBufferData(GL_ARRAY_BUFFER,
                      (iNumIndices * 2 * 4),
                      texCoords,
                      GL_STATIC_DRAW);
        glVertexAttribPointer(DOMAIN_ATTRIBUTE_TEXTURE0,
                               2, // 2 is for S,T co-ordinates in our texCoords array
                               GL_FLOAT,
                               false,0,0);
        glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TEXTURE0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        
        // vbo for index
        glGenBuffers(1, &vbo_index);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo_index);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      (iNumIndices * 3 * 2),
                      elements,
                      GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        
        glBindVertexArray(0);
        
        // after sending data to GPU, now we can free our arrays
        cleanupMeshData();
    }
    
    inline void draw()
    {
        // code
        // bind vao
        glBindVertexArray(vao);

        // draw

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index);
        glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

        // unbind vao
        glBindVertexArray(0);
    }
    
// private:    
    inline int getIndexCount()
    {
        // code
        return(numElements);
    }
    
    inline int getVertexCount()
    {
        // code
        return(numVertices);
    }
    
    inline void normalizeVector(float *v)
    {
        // code
        
        // square the vector length
        float squaredVectorLength=(v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]);
        
        // get square root of above 'squared vector length'
        float squareRootOfSquaredVectorLength=sqrtf(squaredVectorLength);
        
        // scale the vector with 1/squareRootOfSquaredVectorLength
        v[0] = v[0] * 1.0/squareRootOfSquaredVectorLength;
        v[1] = v[1] * 1.0/squareRootOfSquaredVectorLength;
        v[2] = v[2] * 1.0/squareRootOfSquaredVectorLength;
    }

    inline bool isFoundIdentical(float val1, float val2, float diff)
    {

        // code
        if(fabs(val1 - val2) < diff)
            return(true);
        else
            return(false);
    }

    inline void cleanupMeshData()
    {
        // code
        if(elements!=NULL)
        {
            delete[] elements;
            elements=NULL;
        }
        
        if(verts!=NULL)
        {
            delete[] verts;
            verts=NULL;
        }
        
        if(norms!=NULL)
        {
            delete[] norms;
            norms=NULL;
        }
        
        if(texCoords!=NULL)
        {
            delete[] texCoords;
            texCoords=NULL;
        }
    }
    
    inline void deallocate()
    {
        // code
        if(vao)
        {
            glDeleteVertexArrays(1, &vao);
            vao=0;
        }
        
        if(vbo_index)
        {
            glDeleteBuffers(1, &vbo_index);
            vbo_index=0;
        }
        
        if(vbo_texture)
        {
            glDeleteBuffers(1, &vbo_texture);
            vbo_texture=0;
        }
        
        if(vbo_normal)
        {
            glDeleteBuffers(1, &vbo_normal);
            vbo_normal=0;
        }
        
        if(vbo_position)
        {
            glDeleteBuffers(1, &vbo_position);
            vbo_position=0;
        }
    }

};

void makeSphere(Mesh& sphereMesh, float fRadius, int iSlices, int iStacks)
{
    // code
    float drho = float(DOMAIN_PI) / float(iStacks);
    float dtheta = 2.0 * float(DOMAIN_PI) / float(iSlices);
    float ds = 1.0 / float(iSlices);
    float dt = 1.0 / float(iStacks);
    float t = 1.0;
    float s = 0.0;
    int i=0;
    int j=0;
    
    sphereMesh.allocate(iSlices * iStacks * 6); // 6 - vertices of 2 triangles ?
    
    for (i = 0; i < iStacks; i++)
    {
        float rho = float(i * drho);
        float srho = float(sin(rho));
        float crho = float(cos(rho));
        float srhodrho = float(sin(rho + drho));
        float crhodrho = float(cos(rho + drho));
        
        // Many sources of OpenGL sphere drawing code uses a triangle fan
        // for the caps of the sphere. This however introduces texturing
        // artifacts at the poles on some OpenGL implementations
        s = 0.0;
        
        // initialization of three 2-D arrays, two are 4 x 3 and one is 4 x 2
        float** vertex=new float*[4]; // 4 rows
        for(int a=0;a<4;a++)
            vertex[a]=new float[3]; // 3 columns

        float** normal=new float*[4]; // 4 rows
        for(int a=0;a<4;a++)
            normal[a]=new float[3]; // 3 columns
        float** texture=new float*[4];
        for(int a=0;a<4;a++)
            texture[a]=new float[2]; // 3 columns

        for ( j = 0; j < iSlices; j++)
        {
            float theta = (j == iSlices) ? 0.0 : j * dtheta;
            float stheta = float(-sin(theta));
            float ctheta = float(cos(theta));
            
            float x = stheta * srho;
            float y = ctheta * srho;
            float z = crho;
            
            texture[0][0] = s;
            texture[0][1] = t;
            normal[0][0] = x;
            normal[0][1] = y;
            normal[0][2] = z;
            vertex[0][0] = x * fRadius;
            vertex[0][1] = y * fRadius;
            vertex[0][2] = z * fRadius;
            
            x = stheta * srhodrho;
            y = ctheta * srhodrho;
            z = crhodrho;
            
            texture[1][0] = s;
            texture[1][1] = t - dt;
            normal[1][0] = x;
            normal[1][1] = y;
            normal[1][2] = z;
            vertex[1][0] = x * fRadius;
            vertex[1][1] = y * fRadius;
            vertex[1][2] = z * fRadius;
            
            theta = ((j+1) == iSlices) ? 0.0 : (j+1) * dtheta;
            stheta = float(-sin(theta));
            ctheta = float(cos(theta));
            
            x = stheta * srho;
            y = ctheta * srho;
            z = crho;
            
            s += ds;
            texture[2][0] = s;
            texture[2][1] = t;
            normal[2][0] = x;
            normal[2][1] = y;
            normal[2][2] = z;
            vertex[2][0] = x * fRadius;
            vertex[2][1] = y * fRadius;
            vertex[2][2] = z * fRadius;
            
            x = stheta * srhodrho;
            y = ctheta * srhodrho;
            z = crhodrho;
            
            texture[3][0] = s;
            texture[3][1] = t - dt;
            normal[3][0] = x;
            normal[3][1] = y;
            normal[3][2] = z;
            vertex[3][0] = x * fRadius;
            vertex[3][1] = y * fRadius;
            vertex[3][2] = z * fRadius;
            
            sphereMesh.addTriangle(vertex, normal, texture);
            
            // Rearrange for next triangle
            vertex[0][0]=vertex[1][0];
            vertex[0][1]=vertex[1][1];
            vertex[0][2]=vertex[1][2];
            normal[0][0]=normal[1][0];
            normal[0][1]=normal[1][1];
            normal[0][2]=normal[1][2];
            texture[0][0]=texture[1][0];
            texture[0][1]=texture[1][1];
            
            vertex[1][0]=vertex[3][0];
            vertex[1][1]=vertex[3][1];
            vertex[1][2]=vertex[3][2];
            normal[1][0]=normal[3][0];
            normal[1][1]=normal[3][1];
            normal[1][2]=normal[3][2];
            texture[1][0]=texture[3][0];
            texture[1][1]=texture[3][1];
            
            sphereMesh.addTriangle(vertex, normal, texture);
        }
        t -= dt;
    }

    sphereMesh.prepareToDraw();
}

} // namespace

#endif /* __SPHERE_H__ */
