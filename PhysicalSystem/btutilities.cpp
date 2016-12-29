
#include "Common.h"
#include <btBulletDynamicsCommon.h>


// Convert a quaternion into euler angles
btVector3 quaternionToEulerXYZ(const btQuaternion &quat)
{
    btVector3 euler;

    float w=quat.getW(); float x=quat.getX(); float y=quat.getY(); float z=quat.getZ();
    double sqw = w*w; double sqx = x*x; double sqy = y*y; double sqz = z*z;

    euler.setZ(static_cast<float>(atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))));
    euler.setX(static_cast<float>(atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))));
    euler.setY(static_cast<float>(asin(-2.0 * (x*z - y*w))));

    return euler;
}

//TODO:check
btVector3 quaternionToEuler(const btQuaternion &TQuat )
{

    btScalar W = TQuat.getW();
    btScalar X = TQuat.getX();
    btScalar Y = TQuat.getY();
    btScalar Z = TQuat.getZ();
    float WSquared = W * W;
    float XSquared = X * X;
    float YSquared = Y * Y;
    float ZSquared = Z * Z;

    btVector3 TEuler;

    TEuler.setX(atan2f(2.0f * (Y * Z + X * W), -XSquared - YSquared + ZSquared + WSquared));
    TEuler.setY(asinf(-2.0f * (X * Z - Y * W)));
    TEuler.setZ(atan2f(2.0f * (X * Y + Z * W), XSquared - YSquared - ZSquared + WSquared));
    TEuler *= RADTODEG;

    return TEuler;
}


// Get the yaw from a quaternion
float quaternionToYaw(const btQuaternion &quat)
{
    float fTx  = 2.0f*quat.x();
    float fTy  = 2.0f*quat.y();
    float fTz  = 2.0f*quat.z();
    float fTwy = fTy*quat.w();
    float fTxx = fTx*quat.x();
    float fTxz = fTz*quat.x();
    float fTyy = fTy*quat.y();

    return atan2(fTxz+fTwy, 1.0f-(fTxx+fTyy));
}

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) {
        x0 = x1 = - 0.5 * b / a;
    }
    else {
        float q = (b > 0) ?
                    -0.5 * (b + sqrt(discr)) :
                    -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }

    return true;
}

bool rayTriangleIntersect(
        const vec3 &orig, const vec3 &dir,
        const vec3 &v0, const vec3 &v1, const vec3 &v2,
        float &t, float &u, float &v)
{
    //    vec3 v0v1 = v1 - v0;
    //    vec3 v0v2 = v2 - v0;
    //    vec3 pvec = dir.crossProduct(v0v2);
    //    float det = v0v1.dotProduct(pvec);

    //    // ray and triangle are parallel if det is close to 0
    //    if (fabs(det) < kEpsilon) return false;

    //    float invDet = 1 / det;

    //    vec3 tvec = orig - v0;
    //    u = tvec.dotProduct(pvec) * invDet;
    //    if (u < 0 || u > 1) return false;

    //    vec3 qvec = tvec.crossProduct(v0v1);
    //    v = dir.dotProduct(qvec) * invDet;
    //    if (v < 0 || u + v > 1) return false;

    //    t = v0v2.dotProduct(qvec) * invDet;

    //    return (t > 0) ? true : false;
    return true;
}
