//
//  mat4.c
//  New Project
//
//  Created by URJASVI SUTHAR on 9/16/21.
//  Copyright © 2021 the Kore Development Team. All rights reserved.
//

#include "mat4.h"

#include "vector4.h"
#include "vector3.h"
//#include "quat.h"
#include <string.h>
#include <kinc/log.h>
#include <stdio.h>

#include <kinc/math/core.h>


void mat4_translation(mat4_t* self,float x,float y,float z){
    self->_00 = 1; self->_10 = 0; self->_20 = 0; self->_30 = x;
    self->_01 = 0; self->_11 = 1; self->_21 = 0; self->_31 = y;
    self->_02 = 0; self->_12 = 0; self->_22 = 1; self->_32 = z;
    self->_03 = 0; self->_13 = 0; self->_23 = 0; self->_33 = 1;
}

void mat4_identity(mat4_t* self){
    self->_00 = 1; self->_10 = 0; self->_20 = 0; self->_30 = 0;
    self->_01 = 0; self->_11 = 1; self->_21 = 0; self->_31 = 0;
    self->_02 = 0; self->_12 = 0; self->_22 = 1; self->_32 = 0;
    self->_03 = 0; self->_13 = 0; self->_23 = 0; self->_33 = 1;
}

void mat4_scale(mat4_t* self,float x,float y,float z){
    self->_00 = x; self->_10 = 0; self->_20 = 0; self->_30 = 0;
    self->_01 = 0; self->_11 = y; self->_21 = 0; self->_31 = 0;
    self->_02 = 0; self->_12 = 0; self->_22 = z; self->_32 = 0;
    self->_03 = 0; self->_13 = 0; self->_23 = 0; self->_33 = 1;
}

void mat4_rotationX(mat4_t* self,float alpha){
    float ca = kinc_cos(alpha);
    float sa = kinc_sin(alpha);
    self->_00 = 1; self->_10 = 0; self->_20 = 0; self->_30 = 0;
    self->_01 = 0; self->_11 = ca; self->_21 = -sa; self->_31 = 0;
    self->_02 = 0; self->_12 = sa; self->_22 = ca; self->_32 = 0;
    self->_03 = 0; self->_13 = 0; self->_23 = 0; self->_33 = 1;
}

void mat4_rotationY(mat4_t* self,float alpha){
    float ca = kinc_cos(alpha);
    float sa = kinc_sin(alpha);
    self->_00 = ca; self->_10 = 0; self->_20 = sa; self->_30 = 0;
    self->_01 = 0; self->_11 = 1; self->_21 = 0; self->_31 = 0;
    self->_02 = -sa; self->_12 = 0; self->_22 = ca; self->_32 = 0;
    self->_03 = 0; self->_13 = 0; self->_23 = 0; self->_33 = 1;
}

void mat4_rotationZ(mat4_t* self,float alpha){
    float ca = kinc_cos(alpha);
    float sa = kinc_sin(alpha);
    self->_00 = ca; self->_10 = -sa; self->_20 = 0; self->_30 = 0;
    self->_01 = sa; self->_11 = ca; self->_21 = 0; self->_31 = 0;
    self->_02 = 0; self->_12 = 0; self->_22 = 1; self->_32 = 0;
    self->_03 = 0; self->_13 = 0; self->_23 = 0; self->_33 = 1;
}

void mat4_rotation(mat4_t* self,float yaw, float pitch, float roll){
    float sy = kinc_sin(yaw);
    float cy = kinc_cos(yaw);
    float sx = kinc_sin(pitch);
    float cx = kinc_cos(pitch);
    float sz = kinc_sin(roll);
    float cz = kinc_cos(roll);
    self->_00 = cx * cy; self->_10 = cx * sy * sz - sx * cz; self->_20 = cx * sy * cz + sx * sz; self->_30 = 0;
    self->_01 = sx * cy; self->_11 = sx * sy * sz + cx * cz; self->_21 = sx * sy * cz - cx * sz; self->_31 = 0;
    self->_02 = -sy; self->_12 = cy * sz; self->_22 = cy * cz; self->_32 = 0;
    self->_03 = 0; self->_13 = 0; self->_23 = 0; self->_33 = 1;
}


void mat4_ortho(mat4_t* self,float left,float right,float bottom,float top,float zn,float zf){
    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    float tz = -(zf + zn) / (zf - zn);

    self->_00 = 2 / (right - left); self->_10 = 0; self->_20 = 0; self->_30 = tx;
    self->_01 = 0; self->_11 = 2 / (top - bottom); self->_21 = 0; self->_31 = ty;
    self->_02 = 0; self->_12 = 0; self->_22 = -2 / (zf - zn); self->_32 = tz;
    self->_03 = 0; self->_13 = 0; self->_23 = 0; self->_33 = 1;
}


void mat4_persp(mat4_t* self, float fovY, float aspect, float zn, float zf) {
    float uh = 1.0f / kinc_tan(fovY / 2);
    float uw = uh / aspect;

    self->_00 = uw; self->_10 = 0; self->_20 = 0; self->_30 = 0;
    self->_01 = 0; self->_11 = uh; self->_21 = 0; self->_31 = 0;
    self->_02 = 0; self->_12 = 0; self->_22 = (zf + zn) / (zn - zf); self->_32 = 2 * zf * zn / (zn - zf);
    self->_03 = 0; self->_13 = 0; self->_23 = -1; self->_33 = 0;

}

void mat4_look_at(mat4_t* self, vec3_t* eye, vec3_t* at, vec3_t* up) {
//    printf("eye: %f, %f, %f\n", eye->x, eye->y, eye->z);
//    printf("at: %f, %f, %f\n", at->x, at->y, at->z);
//    printf("up: %f, %f, %f\n", up->x, up->y, up->z);
    vec3_sub(at,eye);
    vec3_normalize(at);

    vec3_t zaxis = {at->x, at->y, at->z};
//    memcpy(&zaxis,at,sizeof(vec3_t));

    vec3_t xaxis = {zaxis.x, zaxis.y, zaxis.z};
//    memcpy(&xaxis,&zaxis,sizeof(vec3_t));
    vec3_cross(&xaxis,up);
    vec3_normalize(&xaxis);

    vec3_t yaxis = {xaxis.x, xaxis.y, xaxis.z};
//    memcpy(&yaxis,&xaxis,sizeof(vec3_t));
    vec3_cross(&yaxis,&zaxis);


    self->_00 = xaxis.x;  self->_10 = xaxis.y;  self->_20 = xaxis.z;  self->_30 = -vec3_dot(&xaxis,eye);
    self->_01 = yaxis.x;  self->_11 = yaxis.y;  self->_21 = yaxis.z;  self->_31 = -vec3_dot(&yaxis,eye);
    self->_02 = -zaxis.x; self->_12 = -zaxis.y; self->_22 = -zaxis.z; self->_32 = vec3_dot(&zaxis,eye);
    self->_03 = 0;        self->_13 = 0;        self->_23 = 0;        self->_33 = 1;
}

void mat4_add(mat4_t* self,mat4_t* other){
    self->_00 += other->_00;
    self->_10 += other->_10;
    self->_20 += other->_20;
    self->_30 += other->_30;
    self->_01 += other->_01;
    self->_11 += other->_11;
    self->_21 += other->_21;
    self->_31 += other->_31;
    self->_02 += other->_02;
    self->_12 += other->_12;
    self->_22 += other->_22;
    self->_32 += other->_32;
    self->_03 += other->_03;
    self->_13 += other->_13;
    self->_23 += other->_23;
    self->_33 += other->_33;
}
void mat4_sub(mat4_t* self,mat4_t* other){
    self->_00 -= other->_00;
    self->_10 -= other->_10;
    self->_20 -= other->_20;
    self->_30 -= other->_30;
    self->_01 -= other->_01;
    self->_11 -= other->_11;
    self->_21 -= other->_21;
    self->_31 -= other->_31;
    self->_02 -= other->_02;
    self->_12 -= other->_12;
    self->_22 -= other->_22;
    self->_32 -= other->_32;
    self->_03 -= other->_03;
    self->_13 -= other->_13;
    self->_23 -= other->_23;
    self->_33 -= other->_33;
}

void mat4_mult(mat4_t* self,mat4_t* other){
    mat4_t m = *other;
    self->_00 = self->_00 * m._00 + self->_10 * m._01 + self->_20 * m._02 + self->_30 * m._03;
    self->_10 = self->_00 * m._10 + self->_10 * m._11 + self->_20 * m._12 + self->_30 * m._13;
    self->_20 = self->_00 * m._20 + self->_10 * m._21 + self->_20 * m._22 + self->_30 * m._23;
    self->_30 = self->_00 * m._30 + self->_10 * m._31 + self->_20 * m._32 + self->_30 * m._33;
    self->_01 = self->_01 * m._00 + self->_11 * m._01 + self->_21 * m._02 + self->_31 * m._03;
    self->_11 = self->_01 * m._10 + self->_11 * m._11 + self->_21 * m._12 + self->_31 * m._13;
    self->_21 = self->_01 * m._20 + self->_11 * m._21 + self->_21 * m._22 + self->_31 * m._23;
    self->_31 = self->_01 * m._30 + self->_11 * m._31 + self->_21 * m._32 + self->_31 * m._33;
    self->_02 = self->_02 * m._00 + self->_12 * m._01 + self->_22 * m._02 + self->_32 * m._03;
    self->_12 = self->_02 * m._10 + self->_12 * m._11 + self->_22 * m._12 + self->_32 * m._13;
    self->_22 = self->_02 * m._20 + self->_12 * m._21 + self->_22 * m._22 + self->_32 * m._23;
    self->_32 = self->_02 * m._30 + self->_12 * m._31 + self->_22 * m._32 + self->_32 * m._33;
    self->_03 = self->_03 * m._00 + self->_13 * m._01 + self->_23 * m._02 + self->_33 * m._03;
    self->_13 = self->_03 * m._10 + self->_13 * m._11 + self->_23 * m._12 + self->_33 * m._13;
    self->_23 = self->_03 * m._20 + self->_13 * m._21 + self->_23 * m._22 + self->_33 * m._23;
    self->_33 = self->_03 * m._30 + self->_13 * m._31 + self->_23 * m._32 + self->_33 * m._33;
}


void mat4_multvec(mat4_t* self,vec4_t* value, vec4_t* product){
    product->x = self->_00 * value->x + self->_10 * value->y + self->_20 * value->z + self->_30 * value->w;
    product->y = self->_01 * value->x + self->_11 * value->y + self->_21 * value->z + self->_31 * value->w;
    product->z = self->_02 * value->x + self->_12 * value->y + self->_22 * value->z + self->_32 * value->w;
    product->w = self->_03 * value->x + self->_13 * value->y + self->_23 * value->z + self->_33 * value->w;
}

void mat4_multf(mat4_t* self,float value){
    self->_00 *= value; self->_10 *= value; self->_20 *= value; self->_30 *= value;
    self->_01 *= value; self->_11 *= value; self->_21 *= value; self->_31 *= value;
    self->_02 *= value; self->_12 *= value; self->_22 *= value; self->_32 *= value;
    self->_03 *= value; self->_13 *= value; self->_23 *= value; self->_33 *= value;
}

float mat4_cofactor(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8) {
    return m0 * (m4 * m8 - m5 * m7) - m1 * (m3 * m8 - m5 * m6) + m2 * (m3 * m7 - m4 * m6);
}

float mat4_determinant(mat4_t* self){
    float c00 = mat4_cofactor(self->_11, self->_21, self->_31, self->_12, self->_22, self->_32, self->_13, self->_23, self->_33);
    float c01 = mat4_cofactor(self->_10, self->_20, self->_30, self->_12, self->_22, self->_32, self->_13, self->_23, self->_33);
    float c02 = mat4_cofactor(self->_10, self->_20, self->_30, self->_11, self->_21, self->_31, self->_13, self->_23, self->_33);
    float c03 = mat4_cofactor(self->_10, self->_20, self->_30, self->_11, self->_21, self->_31, self->_12, self->_22, self->_32);
    return self->_00 * c00 - self->_01 * c01 + self->_02 * c02 - self->_03 * c03;
}

void mat4_inverse(mat4_t* self, mat4_t* result){

    float c00 = mat4_cofactor(self->_11, self->_21, self->_31, self->_12, self->_22, self->_32, self->_13, self->_23, self->_33);
    float c01 = mat4_cofactor(self->_10, self->_20, self->_30, self->_12, self->_22, self->_32, self->_13, self->_23, self->_33);
    float c02 = mat4_cofactor(self->_10, self->_20, self->_30, self->_11, self->_21, self->_31, self->_13, self->_23, self->_33);
    float c03 = mat4_cofactor(self->_10, self->_20, self->_30, self->_11, self->_21, self->_31, self->_12, self->_22, self->_32);

    float det = self->_00 * c00 - self->_01 * c01 + self->_02 * c02 - self->_03 * c03;
    if (kinc_abs(det) < 0.000001) {
        kinc_log(KINC_LOG_LEVEL_ERROR, "determinant is too small, will set to identity");
        mat4_identity(result);
        return;
    }

    float c10 = mat4_cofactor(self->_01, self->_21, self->_31, self->_02, self->_22, self->_32, self->_03, self->_23, self->_33);
    float c11 = mat4_cofactor(self->_00, self->_20, self->_30, self->_02, self->_22, self->_32, self->_03, self->_23, self->_33);
    float c12 = mat4_cofactor(self->_00, self->_20, self->_30, self->_01, self->_21, self->_31, self->_03, self->_23, self->_33);
    float c13 = mat4_cofactor(self->_00, self->_20, self->_30, self->_01, self->_21, self->_31, self->_02, self->_22, self->_32);

    float c20 = mat4_cofactor(self->_01, self->_11, self->_31, self->_02, self->_12, self->_32, self->_03, self->_13, self->_33);
    float c21 = mat4_cofactor(self->_00, self->_10, self->_30, self->_02, self->_12, self->_32, self->_03, self->_13, self->_33);
    float c22 = mat4_cofactor(self->_00, self->_10, self->_30, self->_01, self->_11, self->_31, self->_03, self->_13, self->_33);
    float c23 = mat4_cofactor(self->_00, self->_10, self->_30, self->_01, self->_11, self->_31, self->_02, self->_12, self->_32);

    float c30 = mat4_cofactor(self->_01, self->_11, self->_21, self->_02, self->_12, self->_22, self->_03, self->_13, self->_23);
    float c31 = mat4_cofactor(self->_00, self->_10, self->_20, self->_02, self->_12, self->_22, self->_03, self->_13, self->_23);
    float c32 = mat4_cofactor(self->_00, self->_10, self->_20, self->_01, self->_11, self->_21, self->_03, self->_13, self->_23);
    float c33 = mat4_cofactor(self->_00, self->_10, self->_20, self->_01, self->_11, self->_21, self->_02, self->_12, self->_22);

    float invdet = 1.0f / det;
    result->_00 = c00 * invdet; result->_10 = -c01 * invdet; result->_20 = c02 * invdet; result->_30 = -c03 * invdet;
    result->_01 = -c10 * invdet; result->_11 = c11 * invdet; result->_21 = -c12 * invdet; result->_31 = c13 * invdet;
    result->_02 = c20 * invdet; result->_12 = -c21 * invdet; result->_22 = c22 * invdet; result->_32 = -c23 * invdet;
    result->_03 = -c30 * invdet; result->_13 = c31 * invdet; result->_23 = -c32 * invdet; result->_33 = c33 * invdet;

}


void mat4_multmats3x4(mat4_t* self,mat4_t* a,mat4_t* b) {
    float a00 = a->_00; float a01 = a->_01; float a02 = a->_02; float a03 = a->_03;
    float a10 = a->_10; float a11 = a->_11; float a12 = a->_12; float a13 = a->_13;
    float a20 = a->_20; float a21 = a->_21; float a22 = a->_22; float a23 = a->_23;
    float a30 = a->_30; float a31 = a->_31; float a32 = a->_32; float a33 = a->_33;

    float b0 = b->_00; float b1 = b->_10; float b2 = b->_20; float b3 = b->_30;
    self->_00 = a00 * b0 + a01 * b1 + a02 * b2 + a03 * b3;
    self->_10 = a10 * b0 + a11 * b1 + a12 * b2 + a13 * b3;
    self->_20 = a20 * b0 + a21 * b1 + a22 * b2 + a23 * b3;
    self->_30 = a30 * b0 + a31 * b1 + a32 * b2 + a33 * b3;

    b0 = b->_01; b1 = b->_11; b2 = b->_21; b3 = b->_31;
    self->_01 = a00 * b0 + a01 * b1 + a02 * b2 + a03 * b3;
    self->_11 = a10 * b0 + a11 * b1 + a12 * b2 + a13 * b3;
    self->_21 = a20 * b0 + a21 * b1 + a22 * b2 + a23 * b3;
    self->_31 = a30 * b0 + a31 * b1 + a32 * b2 + a33 * b3;

    b0 = b->_02; b1 = b->_12; b2 = b->_22; b3 = b->_32;
    self->_02 = a00 * b0 + a01 * b1 + a02 * b2 + a03 * b3;
    self->_12 = a10 * b0 + a11 * b1 + a12 * b2 + a13 * b3;
    self->_22 = a20 * b0 + a21 * b1 + a22 * b2 + a23 * b3;
    self->_32 = a30 * b0 + a31 * b1 + a32 * b2 + a33 * b3;

    self->_03 = 0;
    self->_13 = 0;
    self->_23 = 0;
    self->_33 = 1;
}

//void mat4_compose(mat4_t* self, vec4_t* loc, quat_t* quat,vec4_t* sc) {
//    mat4_fromQuat(self,quat);
//    mat4_scale(self,sc->x,sc->y,sc->z);
//    mat4_setLoc(self,loc);
//}

//void mat4_decompose(mat4_t* self, vec4_t* loc, quat_t* quat,vec4_t* scale) {
//    loc->x = self->_30; loc->y = self->_31; loc->z = self->_32;
//    vec4_t helpVec = {0};
//    vec4_set3(&helpVec,self->_00, self->_01, self->_02);
//    scale->x = vec4_length(&helpVec);
//    vec4_set3(&helpVec,self->_10, self->_11, self->_12);
//    scale->y = vec4_length(&helpVec);
//    vec4_set3(&helpVec,self->_20, self->_21, self->_22);
//    scale->z = vec4_length(&helpVec);
//    if (mat4_determinant(self) < 0.0) scale->x = -scale->x;
//    float invs = 1.0 / scale->x; // Scale the rotation part
//    mat4_t helpMat = {0};
//    helpMat._00 = self->_00 * invs;
//    helpMat._01 = self->_01 * invs;
//    helpMat._02 = self->_02 * invs;
//    invs = 1.0f / scale->y;
//    helpMat._10 = self->_10 * invs;
//    helpMat._11 = self->_11 * invs;
//    helpMat._12 = self->_12 * invs;
//    invs = 1.0f / scale->z;
//    helpMat._20 = self->_20 * invs;
//    helpMat._21 = self->_21 * invs;
//    helpMat._22 = self->_22 * invs;
//    quat_fromRotationMat(quat,&helpMat);
//
//}

void mat4_setLoc(mat4_t* self, vec4_t* v) {
    self->_30 = v->x;
    self->_31 = v->y;
    self->_32 = v->z;
    
}

//void mat4_fromQuat( mat4_t* self, quat_t* q) {
//    float x = q->x; float y = q->y; float z = q->z; float w = q->w;
//    float x2 = x + x; float y2 = y + y; float z2 = z + z;
//    float xx = x * x2; float xy = x * y2; float xz = x * z2;
//    float yy = y * y2; float yz = y * z2; float zz = z * z2;
//    float wx = w * x2; float wy = w * y2; float wz = w * z2;
//
//    self->_00 = 1.0f - (yy + zz);
//    self->_10 = xy - wz;
//    self->_20 = xz + wy;
//
//    self->_01 = xy + wz;
//    self->_11 = 1.0f - (xx + zz);
//    self->_21 = yz - wx;
//
//    self->_02 = xz - wy;
//    self->_12 = yz + wx;
//    self->_22 = 1.0f - (xx + yy);
//
//    self->_03 = 0.0f;
//    self->_13 = 0.0f;
//    self->_23 = 0.0f;
//    self->_30 = 0.0f;
//    self->_31 = 0.0f;
//    self->_32 = 0.0f;
//    self->_33 = 1.0f;
//
//
//}

void mat4_translate(mat4_t* self, float x, float y, float z) {
    self->_00 += x * self->_03; self->_01 += y * self->_03; self->_02 += z * self->_03;
    self->_10 += x * self->_13; self->_11 += y * self->_13; self->_12 += z * self->_13;
    self->_20 += x * self->_23; self->_21 += y * self->_23; self->_22 += z * self->_23;
    self->_30 += x * self->_33; self->_31 += y * self->_33; self->_32 += z * self->_33;
}

void mat4_applyScale(mat4_t* self,vec4_t* v) {
    float x = v->x; float y = v->y; float z = v->z;
    self->_00 *= x;
    self->_01 *= x;
    self->_02 *= x;
    self->_03 *= x;
    self->_10 *= y;
    self->_11 *= y;
    self->_12 *= y;
    self->_13 *= y;
    self->_20 *= z;
    self->_21 *= z;
    self->_22 *= z;
    self->_23 *= z;
}

void mat4_transpose(mat4_t* self) {
    float  f = self->_01; self->_01 = self->_10; self->_10 = f;
    f = self->_02; self->_02 = self->_20; self->_20 = f;
    f = self->_03; self->_03 = self->_30; self->_30 = f;
    f = self->_12; self->_12 = self->_21; self->_21 = f;
    f = self->_13; self->_13 = self->_31; self->_31 = f;
    f = self->_23; self->_23 = self->_32; self->_32 = f;
    
}

void mat4_transpose3x3(mat4_t* self) {
    float  f = self->_01; self->_01 = self->_10; self->_10 = f;
    f = self->_02; self->_02 = self->_20; self->_20 = f;
    f = self->_12; self->_12 = self->_21; self->_21 = f;

}

void mat4_setF32(mat4_t* self,float* a,int offset) {
    self->_00 = a[0 + offset]; self->_10 = a[1 + offset]; self->_20 = a[2 + offset]; self->_30 = a[3 + offset];
    self->_01 = a[4 + offset]; self->_11 = a[5 + offset]; self->_21 = a[6 + offset]; self->_31 = a[7 + offset];
    self->_02 = a[8 + offset]; self->_12 = a[9 + offset]; self->_22 = a[10 + offset]; self->_32 = a[11 + offset];
    self->_03 = a[12 + offset]; self->_13 = a[13 + offset]; self->_23 = a[14 + offset]; self->_33 = a[15 + offset];
    
}

void mat4_setFrom(mat4_t* self, mat4_t* other) {
    mat4_t m = *other;
    self->_00 = m._00; self->_01 = m._01; self->_02 = m._02; self->_03 = m._03;
    self->_10 = m._10; self->_11 = m._11; self->_12 = m._12; self->_13 = m._13;
    self->_20 = m._20; self->_21 = m._21; self->_22 = m._22; self->_23 = m._23;
    self->_30 = m._30; self->_31 = m._31; self->_32 = m._32; self->_33 = m._33;
    
}

void mat4_getLoc(mat4_t* self,vec4_t* v) {
    v->x = self->_30;
    v->y = self->_31;
    v->z = self->_32;
    v->w = self->_33;
}

void mat4_getScale(mat4_t* self,vec4_t* v) {
   
    v->x = kinc_sqrt(self->_00 * self->_00 + self->_10 * self->_10 + self->_20 * self->_20);
    v->y = kinc_sqrt(self->_01 * self->_01 + self->_11 * self->_11 + self->_21 * self->_21);
    v->z = kinc_sqrt(self->_02 * self->_02 + self->_12 * self->_12 + self->_22 * self->_22);
}

void mat4_toRotation(mat4_t* self) {
    vec4_t helpVec;
    vec4_set3(&helpVec,self->_00, self->_01, self->_02);
    float scale = 1.0 / vec4_length(&helpVec);
    self->_00 = self->_00 * scale;
    self->_01 = self->_01 * scale;
    self->_02 = self->_02 * scale;
    vec4_set3(&helpVec,self->_10, self->_11, self->_12);
    scale = 1.0 / vec4_length(&helpVec);
    self->_10 = self->_10 * scale;
    self->_11 = self->_11 * scale;
    self->_12 = self->_12 * scale;
    vec4_set3(&helpVec,self->_20, self->_21, self->_22);
    scale = 1.0 / vec4_length(&helpVec);
    self->_20 = self->_20 * scale;
    self->_21 = self->_21 * scale;
    self->_22 = self->_22 * scale;
    self->_03 = 0.0;
    self->_13 = 0.0;
    self->_23 = 0.0;
    self->_30 = 0.0;
    self->_31 = 0.0;
    self->_32 = 0.0;
    self->_33 = 1.0;
}

//void applyQuat(mat4_t* self,quat_t* q) {
//    mat4_t helpMat = {0};
//    mat4_fromQuat(&helpMat,q);
//    mat4_mult(self,&helpMat);
//}

void  mat4_right(mat4_t* self,vec4_t* v){ vec4_set3(v,self->_00, self->_01, self->_02); }

/**
 * Sets the vector to the positive y axis of the space defined by this matrix.
 * @param self This matrix
 * @param v The vector to be set to the look vector
**/
void  mat4_look(mat4_t* self,vec4_t* v){ vec4_set3(v,self->_10, self->_11, self->_12); }

/**
 * Sets the vector to the positive z axis of the space defined by this matrix.
 * @param self This matrix
 * @param v The vector to be set to the up vector
**/
void mat4_up(mat4_t* self,vec4_t* v){ vec4_set3(v,self->_20, self->_21, self->_22); }

kinc_matrix4x4_t mat4_to_kinc_matrix4x4(mat4_t* self){
    kinc_matrix4x4_t out;
    out.m[0] = self->_00;
    out.m[1] = self->_10;
    out.m[2] = self->_20;
    out.m[3] = self->_30;
    out.m[4] = self->_01;
    out.m[5] = self->_11;
    out.m[6] = self->_21;
    out.m[7] = self->_31;
    out.m[8] = self->_02;
    out.m[9] = self->_12;
    out.m[10] = self->_22;
    out.m[11] = self->_32;
    out.m[12] = self->_03;
    out.m[13] = self->_13;
    out.m[14] = self->_23;
    out.m[15] = self->_33;
    return out;
}

void mat4_print(mat4_t* self){
    printf("[\n%f, %f, %f, %f \n", self->_00, self->_10, self->_20, self->_30);
    printf("%f, %f, %f, %f \n", self->_01, self->_11, self->_21, self->_31);
    printf("%f, %f, %f, %f \n", self->_02, self->_12, self->_22, self->_32);
    printf("%f, %f, %f, %f \n]", self->_03, self->_13, self->_23, self->_33);
}
