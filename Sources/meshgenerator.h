//
//  meshgenerator.h
//  New Project
//
//  Created by URJASVI SUTHAR on 9/14/21.
//  Copyright © 2021 the Kore Development Team. All rights reserved.
//

#include "global.h"

#define SIDE 20
#define VERTLENGTH 108*SIDE*SIDE*SIDE
#define INDSLENGTH 32*SIDE*SIDE*SIDE

typedef struct mesh {
    float vertices[VERTLENGTH];
    int indices[INDSLENGTH];
    size_t vertID;
    size_t indID;
}mesh;

KINC_FUNC void generateMesh(int *field, mesh *chunk);
KINC_FUNC void createTopFace(struct mesh *chunk, int voxel, int x, int y, int z);
KINC_FUNC void addVertice(struct mesh *chunk, float vert);
KINC_FUNC void addIndices(struct mesh *chunk);
KINC_FUNC int getVoxelAtPos(int *field, int x, int y, int z);
KINC_FUNC void setVoxelAtPos(int *field, int x, int y, int z, int voxel);
