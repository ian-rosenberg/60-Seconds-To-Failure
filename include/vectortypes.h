#pragma once

struct Vector2 {
	double x;
	double y;
};

struct Vector3{
	double x;
	double y;
	double z;
};

struct Vector4{
	double x;
	double y;
	double z;
	double w;
};

struct Rect{
	float x;
	float y;
	float w;
	float h;
};

Vector2 vector2(int x, int y);
Vector3 vector3(int x, int y, int z);
Vector4 vector4(int x, int y, int z, int w);

//From here to the end of the file, we use DJ Kehoe's defines

#define vector4_to_array(ar,vec)   (ar[0] = vec.x,ar[1] = vec.y,ar[2] = vec.z,ar[3] = vec.w)
#define vector3_to_array(ar,vec)   (ar[0] = vec.x,ar[1] = vec.y,ar[2] = vec.z)
#define vector2_to_array(ar,vec)   (ar[0] = vec.x,ar[1] = vec.y)

#define vector2_dot_product(a,b)      (a.x*b.x+a.y*b.y)

#define vector_in_rect(v, r)  ((v.x >= r.x)&&(v.x < (r.x + r.w))&&(v.y >= r.y)&&(v.y < (r.y + r.h)))

#define vector2_copy(dst,src)  (dst.x = src.x,dst.y = src.y)
#define vector3_copy(dst,src)  (dst.x = src.x,dst.y = src.y,dst.z = src.z)
#define vector4_copy(dst,src)  (dst.x = src.x,dst.y = src.y,dst.z = src.z,dst.w=src.w)

#define vector2_equal(a,b) ((a.x == b.x)&&(a.y == b.y))
#define vector3_equal(a,b) ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z))
#define vector4_equal(a,b) ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z)&&(a.w == b.w))

#define vector2_sub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y)
#define vector3_sub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y,dst.z=a.z-b.z)
#define vector4_sub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y,dst.z=a.z-b.z,dst.w=a.w-b.w)

#define vector2_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y)
#define vector3_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z)
#define vector4_add(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z,dst.w = a.w+b.w)

#define vector4_dot_product(a,b)      (a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w)
#define vector3_dot_product(a,b)      (a.x*b.x+a.y*b.y+a.z*b.z)
#define vector2_dot_product(a,b)      (a.x*b.x+a.y*b.y)

#define vector2_scale(dst,src,factor) (dst.x = src.x *(factor),\
                                         dst.y = src.y *(factor))
#define vector3_scale(dst,src,factor) (dst.x = src.x *(factor),\
                                         dst.y = src.y *(factor),\
                                         dst.z = src.z *(factor))
#define vector4_scale(dst,src,factor) (dst.x = src.x *(factor),\
                                         dst.y = src.y *(factor),\
                                         dst.z = src.z *(factor),\
                                         dst.w = src.w *(factor))

#define vector2_clear(a)       (a.x=0,a.y=0)
#define vector3_clear(a)       (a.x=0,a.y=0,a.z=0)
#define vector4_clear(a)       (a.x=0,a.y=0,a.z=0,a.w=0)

#define vector2_negate(dst,src)      (dst.x = -1 * src.x,dst.y = -1 * src.y)
#define vector3_negate(dst,src)      (dst.x = -src.x,dst.y = -src.y,dst.z = -src.z)
#define vector4_negate(dst,src)      (dst.x = -src.x,dst.y = -src.y,dst.z = -src.z,dst.w = -src.w)

#define vector2_set(v, a, b)  (v.x=(a), v.y=(b))
#define vector3_set(v, a, b, c)  (v.x=(a), v.y=(b), v.z=(c))
#define vector4_set(v, a, b, c,d)  (v.x=(a), v.y=(b), v.z=(c), v.w=(d))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define GF2D_EPSILON   1e-6f
#define GF2D_ROOT2  1.414213562
#define GF2D_2PI 6.283185308
#define GF2D_PI 3.141592654
#define GF2D_HALF_PI 1.570796327
/* conversion factor for converting from radians to degrees*/
#define GF2D_RADTODEG  57.295779513082

/* conversion factor for converting from degrees to radians*/
#define GF2D_DEGTORAD  0.017453292519943295769236907684886

typedef short unsigned int Bool;


/**
 * @brief random macro taken from Id Software's Quake 2 Source.
 * This macro exposes the more random bits from the rand() function
 * @return a random float between 0 and 1.0
 */
#define gf2d_random()  ((rand ()%1000)/(float)1000.0)

 /**
  * @brief random macro taken from Id Software's Quake 2 Source.
  * This macro exposes the more random bits from the rand() function
  * @return a random float between -1.0 and 1.0
  */
#define gf2d_crandom() (((float)((rand()%1000)/(float)1000.0) * 2.0) - 1.0)

  /**
   * basic operations
   */
#ifndef MIN
#define MIN(a,b)          (a<=b?a:b)
#endif

#ifndef MAX
#define MAX(a,b)          (a>=b?a:b)
#endif

#define gf2d_rect_set(r,a,b,c,d) (r.x=(a), r.y=(b), r.w=(c), r.h=(d))

/**
 * @brief sets the outvector to a unit vector podoubleing at the angle specified
 * @param out output.  Vector will be set to a unit vector rotated by radians
 * @param radians specify the angle of the vector to be set.
 */
void vector3_set_angle_by_radians(Vector3* out, float radians);

void vector2_set_angle_by_radians(Vector2* out, float radians);

/**
 * @brief returns the angle of a vector in 2D space.
 * @param x the x component of the vector
 * @param y the y component of the vector
 * @return the angle of the vector in radians.
 */
float vector_angle(float x, float y);

/**
 * @brief returns the angle of a vector in 2D space.
 * @param v the vector
 * @return the angle of the vector in radians.
 */
float vector2_angle(Vector2 v);

/**
 * @brief returns the angle between the two angles.
 * Takes doubleo account wraping around 0 and 360
 * units are degrees
 * @Param a one of the angles in question
 * @Param b one of the angles in question
 * @return the angle, in degrees, between the two angles
 */
float angle_between_degrees(float a, float b);

void angle_clamp_degrees(float* a);

Vector2* vector2_new();
Vector3* vector3_new();
Vector4* vector4_new();

Vector2* vector2_dup(Vector2 old);
Vector3* vector3_dup(Vector3 old);
Vector4* vector4_dup(Vector4 old);

void vector2_reflect(Vector2* out, Vector2 normal, Vector2 in);
void vector3_reflect(Vector3* out, Vector3 normal, Vector3 in);
void vector4_reflect(Vector4* out, Vector4 normal, Vector4 in);

float vector2_magnitude(Vector2 V);
float vector3_magnitude(Vector3 V);
float vector4_magnitude(Vector4 V);

Vector2 vector2_rotate(Vector2 in, float angle);

Bool vector2_distance_between_less_than(Vector2 p1, Vector2 p2, float size);
Bool vector3_distance_between_less_than(Vector3 p1, Vector3 p2, float size);
Bool vector4_distance_between_less_than(Vector4 p1, Vector4 p2, float size);

void vector2_set_magnitude(Vector2* V, float magnitude);
void vector3_set_magnitude(Vector3* V, float magnitude);
void vector4_set_magnitude(Vector4* V, float magnitude);

int vector2_magnitude_compare(Vector2 V, float size);
int vector3_magnitude_compare(Vector3 V, float size);
int vector4_magnitude_compare(Vector4 V, float size);

float vector2_magnitude_squared(Vector2 V);
float vector3_magnitude_squared(Vector3 V);
float vector4_magnitude_squared(Vector4 V);

void vector3_cross_product(Vector3* out, Vector3 v1, Vector3 v2);

double vector2_magnitude_between(Vector2 a, Vector2 b);
double vector3_magnitude_between(Vector3 a, Vector3 b);
double vector4_magnitude_between(Vector4 a, Vector4 b);

void vector2_normalize(Vector2* V);
void vector3_normalize(Vector3* V);
void vector4_normalize(Vector4* V);