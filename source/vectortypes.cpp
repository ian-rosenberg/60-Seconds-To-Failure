#include <memory>
#include "vectortypes.h"

Vector2 vector2(int x, int y)
{
    Vector2 vec;
    vector2_set(vec, x, y);
    return vec;
}

Vector3 vector3(int x, int y, int z)
{
    Vector3 vec;
    vector3_set(vec, x, y, z);
    return vec;
}

Vector4 vector4(int x, int y, int z, int w)
{
    Vector4 vec;
    vector4_set(vec, x, y, z, w);
    return vec;
}

Bool vector2_distance_between_less_than(Vector2 p1, Vector2 p2, float size)
{
    if (vector2_magnitude_between(p1, p2) < size)return 1;
    return 0;
}

Bool vector3_distance_between_less_than(Vector3 p1, Vector3 p2, float size)
{
    if (vector3_magnitude_between(p1, p2) < size)return 1;
    return 0;
}

Bool vector4_distance_between_less_than(Vector4 p1, Vector4 p2, float size)
{
    if (vector4_magnitude_between(p1, p2) < size)return 1;
    return 0;
}

float vector2_magnitude(Vector2 V)
{
    return sqrt(V.x * V.x + V.y * V.y);
}

float vector3_magnitude(Vector3 V)
{
    return sqrt(V.x * V.x + V.y * V.y + V.z * V.z);
}

float vector4_magnitude(Vector4 V)
{
    return sqrt(V.x * V.x + V.y * V.y + V.z * V.z + V.w * V.w);
}

float vector2_magnitude_squared(Vector2 V)
{
    return (V.x * V.x + V.y * V.y);
}

float vector3_magnitude_squared(Vector3 V)
{
    return (V.x * V.x + V.y * V.y + V.z * V.z);
}

float vector4_magnitude_squared(Vector4 V)
{
    return (V.x * V.x + V.y * V.y + V.z * V.z + V.w * V.w);
}

void vector2_set_magnitude(Vector2* V, float magnitude)
{
    if (!V)return;
    vector2_normalize(V);
    V->x *= magnitude;
    V->y *= magnitude;
}

void vector3_set_magnitude(Vector3* V, float magnitude)
{
    if (!V)return;
    vector3_normalize(V);
    V->x *= magnitude;
    V->y *= magnitude;
    V->z *= magnitude;
}

void vector4_set_magnitude(Vector4* V, float magnitude)
{
    if (!V)return;
    vector4_normalize(V);
    V->x *= magnitude;
    V->y *= magnitude;
    V->z *= magnitude;
    V->w *= magnitude;
}

double vector2_magnitude_between(Vector2 a, Vector2 b)
{
    Vector2 c;
    vector2_sub(c, a, b);
    return vector2_magnitude(c);
}

double vector3_magnitude_between(Vector3 a, Vector3 b)
{
    Vector3 c;
    vector3_sub(c, a, b);
    return vector3_magnitude(c);
}

double vector4_magnitude_between(Vector4 a, Vector4 b)
{
    Vector4 c;
    vector4_sub(c, a, b);
    return vector4_magnitude(c);
}

int vector2_magnitude_compare(Vector2 V, float size)
{
    float ms = vector2_magnitude_squared(V);
    float ss = size * size;
    if (ms < ss)return -1;
    if (ms == ss)return 0;
    return 1;
}

int vector3_magnitude_compare(Vector3 V, float size)
{
    float ms = vector3_magnitude_squared(V);
    float ss = size * size;
    if (ms < ss)return -1;
    if (ms == ss)return 0;
    return 1;
}

int vector4_magnitude_compare(Vector4 V, float size)
{
    float ms = vector4_magnitude_squared(V);
    float ss = size * size;
    if (ms < ss)return -1;
    if (ms == ss)return 0;
    return 1;
}


void vector2_set_angle_by_radians(Vector2* out, float radians)
{
    if (!out)return;
    out->x = cos(radians);
    out->y = sin(radians);
}

void vector3_set_angle_by_radians(Vector3* out, float radians)
{
    if (!out)return;
    out->x = cos(radians);
    out->y = sin(radians);
}

void vector2_normalize(Vector2* V)
{
    double M;
    if (!V)return;
    M = vector2_magnitude(*V);
    if (M == 0.0f)
    {
        return;
    }
    M = 1 / M;
    V->x *= M;
    V->y *= M;
}

void vector3_normalize(Vector3* V)
{
    float M;
    if (!V)return;
    M = vector3_magnitude(*V);
    if (M == 0.0f)
    {
        return;
    }
    M = 1 / M;
    V->x *= M;
    V->y *= M;
    V->z *= M;
}

void vector4_normalize(Vector4* V)
{
    float M;
    if (!V)return;
    M = vector4_magnitude(*V);
    if (M == 0.0f)
    {
        return;
    }
    M = 1 / M;
    V->x *= M;
    V->y *= M;
    V->z *= M;
    V->w *= M;
}

Vector2* vector2_dup(Vector2 old)
{
    Vector2* duped = NULL;
    duped = vector2_new();
    if (!duped)return NULL;
    vector2_copy((*duped), old);
    return duped;
}

Vector3* vector3_dup(Vector3 old)
{
    Vector3* duped = NULL;
    duped = vector3_new();
    if (!duped)return NULL;
    vector3_copy((*duped), old);
    return duped;
}

Vector4* vector4_dup(Vector4 old)
{
    Vector4* duped = NULL;
    duped = vector4_new();
    if (!duped)return NULL;
    vector4_copy((*duped), old);
    return duped;
}

Vector2* vector2_new()
{
    Vector2* vec = NULL;
    vec = (Vector2*)malloc(sizeof(Vector2));
    if (vec == NULL)
    {
        return NULL;
    }
    vector2_clear((*vec));
    return vec;
}

Vector3* vector3_new()
{
    Vector3* vec = NULL;
    vec = (Vector3*)malloc(sizeof(Vector3));
    if (vec == NULL)
    {
        return NULL;
    }
    vector3_clear((*vec));
    return vec;
}

Vector4* vector4_new()
{
    Vector4* vec = NULL;
    vec = (Vector4*)malloc(sizeof(Vector4));
    if (vec == NULL)
    {
        return NULL;
    }
    vector4_clear((*vec));
    return vec;
}

void vector3_project_to_plane(Vector3* dst, Vector3 p, Vector3 normal)
{
    float d, f;
    Vector3 n;
    float inv_denom;
    if (!dst)return;
    f = vector3_dot_product(normal, normal);
    if (!f)return;
    inv_denom = 1.0F / f;

    d = vector3_dot_product(normal, p) * inv_denom;

    n.x = normal.x * inv_denom;
    n.y = normal.y * inv_denom;
    n.z = normal.z * inv_denom;

    dst->x = p.z - d * n.x;
    dst->y = p.y - d * n.y;
    dst->z = p.x - d * n.z;
}


void vector3_perpendicular(Vector3* dst, Vector3 src)
{
    int pos;
    float minelem = 1.0F;
    Vector3 tempvec;
    if (!dst)return;
    /*
     * * find the smallest magnitude axially aligned vector
     */
    pos = 0;

    if (fabs(src.x) < minelem)
    {
        pos = 0;
        minelem = fabs(src.x);
    }
    if (fabs(src.y) < minelem)
    {
        pos = 1;
        minelem = fabs(src.y);
    }
    if (fabs(src.y) < minelem)
    {
        pos = 2;
        minelem = fabs(src.z);
    }

    tempvec.x = 0;
    tempvec.y = 0;
    tempvec.z = 0;

    switch (pos)
    {
    case 0:
        tempvec.x = 1;
        break;
    case 1:
        tempvec.y = 1;
        break;
    case 2:
        tempvec.z = 1;
        break;
    }

    /*
     * * project the point onto the plane defined by src
     */
    vector3_project_to_plane(dst, tempvec, src);

    /*
     * * normalize the result
     */
    vector3_normalize(dst);
}

void rotation_concacenate(float in1[3][3], float in2[3][3], float out[3][3])
{
    out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
        in1[0][2] * in2[2][0];
    out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
        in1[0][2] * in2[2][1];
    out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
        in1[0][2] * in2[2][2];
    out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
        in1[1][2] * in2[2][0];
    out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
        in1[1][2] * in2[2][1];
    out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
        in1[1][2] * in2[2][2];
    out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
        in1[2][2] * in2[2][0];
    out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
        in1[2][2] * in2[2][1];
    out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
        in1[2][2] * in2[2][2];
}

void vector3_rotate_about_vector(Vector3* dst, Vector3 dir, Vector3 point, float degrees)
{
    float m[3][3];
    float im[3][3];
    float zrot[3][3];
    float tmpmat[3][3];
    float rot[3][3];
    Vector3 vr, vup, vf;

    if (!dst)return;

    vf.x = dir.x;
    vf.y = dir.y;
    vf.z = dir.z;

    vector3_perpendicular(&vr, dir);
    vector3_cross_product(&vup, vr, vf);

    m[0][0] = vr.x;
    m[1][0] = vr.y;
    m[2][0] = vr.z;

    m[0][1] = vup.x;
    m[1][1] = vup.y;
    m[2][1] = vup.z;

    m[0][2] = vf.x;
    m[1][2] = vf.y;
    m[2][2] = vf.z;

    memcpy(im, m, sizeof(im));

    im[0][1] = m[1][0];
    im[0][2] = m[2][0];
    im[1][0] = m[0][1];
    im[1][2] = m[2][1];
    im[2][0] = m[0][2];
    im[2][1] = m[1][2];

    memset(zrot, 0, sizeof(zrot));
    zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;

    zrot[0][0] = cos((degrees * GF2D_DEGTORAD));
    zrot[0][1] = sin((degrees * GF2D_DEGTORAD));
    zrot[1][0] = -sin((degrees * GF2D_DEGTORAD));
    zrot[1][1] = cos((degrees * GF2D_DEGTORAD));

    rotation_concacenate(m, zrot, tmpmat);
    rotation_concacenate(tmpmat, im, rot);

    dst->x = rot[0][0] * point.x + rot[0][1] * point.y + rot[0][2] * point.z;
    dst->y = rot[1][0] * point.x + rot[1][1] * point.y + rot[1][2] * point.z;
    dst->z = rot[2][0] * point.x + rot[2][1] * point.y + rot[2][2] * point.z;
}

void vector3_rotate_about_x(Vector3* vect, float angle)
{
    Vector3 temp;
    if (!vect)return;

    angle = angle * GF2D_DEGTORAD;

    temp.x = vect->x;
    temp.y = (vect->y * cos(angle)) - (vect->z * sin(angle));
    temp.z = (vect->y * sin(angle)) + (vect->z * cos(angle));

    vect->x = temp.x;
    vect->y = temp.y;
    vect->z = temp.z;
}

void vector3_rotate_about_y(Vector3* vect, float angle)
{
    Vector3 temp;
    if (!vect)return;

    angle = angle * GF2D_DEGTORAD;

    temp.y = vect->y;
    temp.x = (vect->x * cos(angle)) + (vect->z * sin(angle));
    temp.z = (vect->x * sin(angle) * (-1)) + (vect->z * cos(angle));

    vect->x = temp.x;
    vect->y = temp.y;
    vect->z = temp.z;
}

void vector3_rotate_about_z(Vector3* vect, float angle)
{
    Vector3 temp;
    if (!vect)return;

    angle = angle * GF2D_DEGTORAD;

    temp.z = vect->z;
    temp.x = (vect->x * cos(angle)) - (vect->y * sin(angle));
    temp.y = (vect->x * sin(angle)) + (vect->y * cos(angle));

    vect->x = temp.x;
    vect->y = temp.y;
    vect->z = temp.z;
}


void vector3_angle_vectors(Vector3 angles, Vector3* forward, Vector3* right, Vector3* up)
{
    float angle;
    float sr, sp, sy, cr, cp, cy;

    angle = angles.x * (GF2D_DEGTORAD);
    sy = sin(angle);
    cy = cos(angle);
    angle = angles.y * (GF2D_DEGTORAD);
    sp = sin(angle);
    cp = cos(angle);
    angle = angles.z * (GF2D_DEGTORAD);
    sr = sin(angle);
    cr = cos(angle);

    if (forward)
    {
        forward->x = cp * cy;
        forward->y = cp * sy;
        forward->z = -sp;
    }
    if (right)
    {
        right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
        right->y = (-1 * sr * sp * sy + -1 * cr * cy);
        right->z = -1 * sr * cp;
    }
    if (up)
    {
        up->x = (cr * sp * cy + -sr * -sy);
        up->y = (cr * sp * sy + -sr * cy);
        up->z = cr * cp;
    }
}

float vector2_angle(Vector2 v)
{
    return vector_angle(v.x, v.y);
}

Vector2 vector2_rotate(Vector2 in, float angle)
{
    Vector2 out;
    out.x = in.x * cos(angle) - in.y * sin(angle); // now x is something different than original vector x
    out.y = in.x * sin(angle) + in.y * cos(angle);
    return out;
}

float vector_angle(float x, float y)
{
    float angle = atan2(y, x) + M_PI;
    float fraction = angle * 0.5 / M_PI;
    if (fraction >= 1.0)
    {
        fraction -= 1.0;
    }
    return (fraction * 360) - 180;
}

void angle_clamp_degrees(float* a)
{
    if (!a)return;
    while (*a >= 360)*a -= 360;
    while (*a < 0)*a += 360;
}

float angle_between_degrees(float a, float b)
{
    float angle;
    angle = fabs(a - b);
    while (angle >= 360)angle -= 360;
    while (angle < 0)angle += 360;
    if (angle > 180)angle -= 180;
    return angle;
}

void vector2_reflect(Vector2* out, Vector2 normal, Vector2 in)
{
    float f;
    f = vector2_dot_product(in, normal);
    out->x = in.x - (2 * normal.x * f);
    out->y = in.y - (2 * normal.y * f);
}

void vector3_reflect(Vector3* out, Vector3 normal, Vector3 in)
{
    float f;
    f = vector3_dot_product(in, normal);
    out->x = in.x - (2 * normal.x * f);
    out->y = in.y - (2 * normal.y * f);
    out->z = in.z - (2 * normal.z * f);
}

void vector4_reflect(Vector4* out, Vector4 normal, Vector4 in)
{
    float f;
    f = vector4_dot_product(in, normal);
    out->x = in.x - (2 * normal.x * f);
    out->y = in.y - (2 * normal.y * f);
    out->z = in.z - (2 * normal.z * f);
    out->w = in.w - (2 * normal.w * f);
}

void vector3_cross_product(Vector3* out, Vector3 v1, Vector3 v2)
{
    out->x = v1.y * v2.z - v1.z * v2.y;
    out->y = v1.z * v2.x - v1.x * v2.z;
    out->z = v1.x * v2.y - v1.y * v2.x;
}