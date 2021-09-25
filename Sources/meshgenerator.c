//
//  meshgenerator.c
//  New Project
//
//  Created by URJASVI SUTHAR on 9/14/21.
//  Copyright © 2021 the Kore Development Team. All rights reserved.
//

#include "meshgenerator.h"
#include <stdio.h>

void generateMesh(int *field, mesh *chunk){

    for (int x = 0; x < SIDE; x++) {
        for(int y = 0; y < SIDE; y++){
            for(int z = 0; z < SIDE; z++){
                int voxel = getVoxelAtPos(field, x, y, z);
                createTopFace(chunk, voxel, x, y, z);
            }
        }
    }
}

void createTopFace(struct mesh *chunk, int voxel, int x, int y, int z){
    addVertice(chunk, x+1); addVertice(chunk, y+1); addVertice(chunk, z);
    addVertice(chunk, x); addVertice(chunk, y+1); addVertice(chunk, z);
    addVertice(chunk, x); addVertice(chunk, y+1); addVertice(chunk, z+1);
    
    addVertice(chunk, x+1); addVertice(chunk, y+1); addVertice(chunk, z);
    addVertice(chunk, x); addVertice(chunk, y+1); addVertice(chunk, z+1);
    addVertice(chunk, x+1); addVertice(chunk, y+1); addVertice(chunk, z+1);

    addIndices(chunk);
    addIndices(chunk);
    addIndices(chunk);
    addIndices(chunk);
    addIndices(chunk);
    addIndices(chunk);
}

void addVertice(struct mesh *chunk, float vert){
    chunk->vertices[chunk->vertID] = vert;
    chunk->vertID += 1;
}

void addIndices(struct mesh *chunk){
    chunk->indices[chunk->indID] = (int)chunk->indID;
    chunk->indID += 1;
}

int getVoxelAtPos(int *field, int x, int y, int z){
    return field[x + y * SIDE + z * SIDE * SIDE];
}

void setVoxelAtPos(int *field, int x, int y, int z, int voxel){
    field[x + y * SIDE + z * SIDE * SIDE] = voxel;
}
