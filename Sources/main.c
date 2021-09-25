#include <kinc/graphics4/graphics.h>
#include <kinc/graphics4/indexbuffer.h>
#include <kinc/graphics4/pipeline.h>
#include <kinc/graphics4/shader.h>
#include <kinc/graphics4/vertexbuffer.h>
#include <kinc/io/filereader.h>
#include <kinc/system.h>
#include <kinc/input/mouse.h>
#include <kinc/input/keyboard.h>
#include <kinc/math/core.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "meshgenerator.h"
#include "mat4.h"
#include "vector4.h"
#include "vector3.h"

static kinc_g4_shader_t vertex_shader;
static kinc_g4_shader_t fragment_shader;
static kinc_g4_pipeline_t pipeline;
static kinc_g4_vertex_buffer_t vertexBuffer;
static kinc_g4_index_buffer_t indexBuffer;

static mat4_t projection;
static mat4_t view;
static mat4_t model;
static kinc_g4_constant_location_t model_mat_cl;
static kinc_g4_constant_location_t view_mat_cl;
static kinc_g4_constant_location_t projection_mat_cl;

//static float horizontalAngle = 3.14;
//static float verticalAngle = -0.52;

static vec3_t cameraLook;
static vec3_t cameraPos;
static vec3_t cameraUp;
//static vec3_t cameraRight;
//static vec3_t cameraDirection;

#define HEAP_SIZE 1024 * 1024
static uint8_t *heap = NULL;
static size_t heap_top = 0;

static void *allocate(size_t size) {
    size_t old_top = heap_top;
    heap_top += size;
    assert(heap_top <= HEAP_SIZE);
    return &heap[old_top];
}

static void update(void) {

    kinc_g4_begin(0);
    kinc_g4_clear(KINC_G4_CLEAR_COLOR, 0, 0.0f, 0);

    kinc_g4_set_pipeline(&pipeline);
    
    kinc_g4_set_vertex_buffer(&vertexBuffer);
    kinc_g4_set_index_buffer(&indexBuffer);

    kinc_matrix4x4_t model_matrix = mat4_to_kinc_matrix4x4(&model);
    kinc_g4_set_matrix4(model_mat_cl, &model_matrix);
    
    kinc_matrix4x4_t view_matrix = mat4_to_kinc_matrix4x4(&view);
    kinc_g4_set_matrix4(view_mat_cl, &view_matrix);

    kinc_matrix4x4_t projection_matrix = mat4_to_kinc_matrix4x4(&projection);
    kinc_g4_set_matrix4(projection_mat_cl, &projection_matrix);

    kinc_g4_draw_indexed_vertices();

    kinc_g4_end(0);
    kinc_g4_swap_buffers();
}

static void load_shader(const char *filename, kinc_g4_shader_t *shader, kinc_g4_shader_type_t shader_type) {
    kinc_file_reader_t file;
    kinc_file_reader_open(&file, filename, KINC_FILE_TYPE_ASSET);
    size_t data_size = kinc_file_reader_size(&file);
    uint8_t *data = allocate(data_size);
    kinc_file_reader_read(&file, data, data_size);
    kinc_file_reader_close(&file);
    kinc_g4_shader_init(shader, data, data_size, shader_type);
}

int kickstart(int argc, char **argv) {
    kinc_init("Shader", 1024, 768, NULL, NULL);
    kinc_set_update_callback(update);

    heap = (uint8_t *)malloc(HEAP_SIZE);
    assert(heap != NULL);

    load_shader("shade.vert", &vertex_shader, KINC_G4_SHADER_TYPE_VERTEX);
    load_shader("shade.frag", &fragment_shader, KINC_G4_SHADER_TYPE_FRAGMENT);

    kinc_g4_vertex_structure_t structure;
    kinc_g4_vertex_structure_init(&structure);
    kinc_g4_vertex_structure_add(&structure, "pos", KINC_G4_VERTEX_DATA_FLOAT3);
    kinc_g4_vertex_structure_add(&structure, "nor", KINC_G4_VERTEX_DATA_FLOAT3);
    kinc_g4_pipeline_init(&pipeline);
    pipeline.vertex_shader = &vertex_shader;
    pipeline.fragment_shader = &fragment_shader;
    pipeline.input_layout[0] = &structure;
    pipeline.input_layout[1] = NULL;
    pipeline.depth_write = true;
    pipeline.depth_mode = KINC_G4_COMPARE_LESS;
    pipeline.cull_mode = KINC_G4_CULL_NOTHING;
    kinc_g4_pipeline_compile(&pipeline);
    
    model_mat_cl = kinc_g4_pipeline_get_constant_location(&pipeline, "M");
    view_mat_cl = kinc_g4_pipeline_get_constant_location(&pipeline, "V");
    projection_mat_cl = kinc_g4_pipeline_get_constant_location(&pipeline, "P");

    
    vec3_set(&cameraUp, 0.0, 1.0, 0.0);
    vec3_set(&cameraPos, 4, 3, 3);
    vec3_set(&cameraLook, 0.0, 0.0, 0.0);
    mat4_look_at(&view, &cameraPos, &cameraLook, &cameraUp);
    mat4_persp(&projection, 45.0, 4.0/3.0, 0.1, 100.0);
    mat4_identity(&model);
    
    int ver[] = {
            -1.0,-1.0,-1.0,
            -1.0,-1.0, 1.0,
            -1.0, 1.0, 1.0,
             1.0, 1.0,-1.0,
            -1.0,-1.0,-1.0,
            -1.0, 1.0,-1.0,
             1.0,-1.0, 1.0,
            -1.0,-1.0,-1.0,
             1.0,-1.0,-1.0,
             1.0, 1.0,-1.0,
             1.0,-1.0,-1.0,
            -1.0,-1.0,-1.0,
            -1.0,-1.0,-1.0,
            -1.0, 1.0, 1.0,
            -1.0, 1.0,-1.0,
             1.0,-1.0, 1.0,
            -1.0,-1.0, 1.0,
            -1.0,-1.0,-1.0,
            -1.0, 1.0, 1.0,
            -1.0,-1.0, 1.0,
             1.0,-1.0, 1.0,
             1.0, 1.0, 1.0,
             1.0,-1.0,-1.0,
             1.0, 1.0,-1.0,
             1.0,-1.0,-1.0,
             1.0, 1.0, 1.0,
             1.0,-1.0, 1.0,
             1.0, 1.0, 1.0,
             1.0, 1.0,-1.0,
            -1.0, 1.0,-1.0,
             1.0, 1.0, 1.0,
            -1.0, 1.0,-1.0,
            -1.0, 1.0, 1.0,
             1.0, 1.0, 1.0,
            -1.0, 1.0, 1.0,
             1.0,-1.0, 1.0
    };
    int indices[]= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};
    float normals[] = {-1,0,0,-1,0,0,-1,0,0,-0,0,-1,-0,0,-1,-0,0,-1,0,-1,-0,0,-1,-0,0,-1,-0,0,-0,-1,0,-0,-1,0,-0,-1,-1,0,0,-1,0,0,-1,0,0,-0,-1,0,-0,-1,0,-0,-1,0,0,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,4,0,0,0,1,0,0,1,0,0,1};
    

    kinc_g4_vertex_buffer_init(&vertexBuffer, 36, &structure, KINC_G4_USAGE_STATIC, 0);
    {
        float *v = kinc_g4_vertex_buffer_lock_all(&vertexBuffer);

        for (int i = 0; i < 36; i++) {
            v[i*6] = ver[i*3];
            v[i*6+1] = ver[i*3+1];
            v[i*6+2] = ver[i*3+2];
            v[i*6+3] = normals[i*3];
            v[i*6+4] = normals[i*3+1];
            v[i*6+5] = normals[i*3+2];
        }

        kinc_g4_vertex_buffer_unlock_all(&vertexBuffer);
    }

    kinc_g4_index_buffer_init(&indexBuffer, 36, KINC_G4_INDEX_BUFFER_FORMAT_32BIT, KINC_G4_USAGE_STATIC);
    {
        int *i = kinc_g4_index_buffer_lock(&indexBuffer);
        
        for (int j = 0; j < 36; j++) {
            i[j] = indices[j];
        }

        kinc_g4_index_buffer_unlock(&indexBuffer);
    }

    kinc_start();

    return 0;
}
