#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cmath>

#include "aabb.h"
#include "shape.h"
#include "vec3.h"

// Barycentric record for the triangle family. Slides p.27-28:
// p = (1 - beta - gamma)*a + beta*b + gamma*c, so (beta, gamma) locates the hit
// inside the triangle. beta and gamma are also the texture coordinates once
// you get to that chapter.
class TriangleHitStruct : public HitStruct {
public:
    double beta = 0;
    double gamma = 0;
};

// One triangle, intersected by solving the 3x3 system on slides p.40-41 for the
// three unknowns (beta, gamma, t) and applying Cramer's rule (p.42-43).
class triangle : public Shape {
public:
    triangle(const point3& v0, const point3& v1, const point3& v2)
        : v0(v0), v1(v1), v2(v2),
          va_minus_vb(v0 - v1),   // the (a, b, c) coefficients, slide p.42
          va_minus_vc(v0 - v2),   // the (d, e, f) coefficients, slide p.42
          n(cross(v1 - v0, v2 - v0)) {
        degenerate = n.length_squared() == 0;
        n = degenerate ? vec3(0, 0, 0) : unit_vector(n);
        box = aabb(min3(v0, v1, v2), max3(v0, v1, v2));
    }

    bool bounding_box(aabb& bounds) const override {
        bounds = box;
        return true;
    }

    bool intersect(const ray& r, double tmin, double& tmax, HitStruct& hit) const override {
        if (degenerate) return false;

        auto& thit = static_cast<TriangleHitStruct&>(hit);

        const auto& ro = r.origin();
        const auto& rd = r.direction();

        // Slide p.42: a = (xa-xb), b = (ya-yb), c = (za-zb)
        //              d = (xa-xc), e = (ya-yc), f = (za-zc)
        double a = va_minus_vb.x(), b = va_minus_vb.y(), c = va_minus_vb.z();
        double d = va_minus_vc.x(), e = va_minus_vc.y(), f = va_minus_vc.z();

        // Slide p.42: g = xrd, h = yrd, i = zrd
        double g = rd.x(), h = rd.y(), i = rd.z();

        // Slide p.42 prints j = (xa-xe), k = (ya-ye), l = (za-ze), but `e` is a
        // coefficient, not a vertex. From the matrix on slide p.41 these are
        // (xa-xro) etc -- the right-hand side is a - ro.
        double j = v0.x() - ro.x(), k = v0.y() - ro.y(), l = v0.z() - ro.z();

        double M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
        if (std::fabs(M) < k_parallel_epsilon) return false;  // ray parallel to the plane

        double beta  = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g)) / M;
        double gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c)) / M;

        // Slide p.43 gives t = (-f(ak-jb) + e(jc-al) + d(bl-kc)) / M, which has a
        // sign error -- it returns -t for a front-facing hit, so every triangle
        // then fails the `t < tmin` test below. Cramer's rule on the same
        // system, replacing the third column with the right-hand side, gives
        // the expression used here.
        double t = (a*(e*l - f*k) - d*(b*l - k*c) + j*(b*f - e*c)) / M;

        if (t < tmin || t > tmax) return false;
        if (gamma < 0 || gamma > 1) return false;
        if (beta < 0 || beta > 1 - gamma) return false;

        hit.set_t(t);
        hit.set_p(r.at(t));
        hit.set_face_normal(r, n);

        thit.beta = beta;
        thit.gamma = gamma;

        return true;
    }

private:
    // A ray parallel to the triangle's plane makes M vanish. The threshold is
    // relative to nothing in particular, so treat it as a scale-free guard: any
    // triangle small enough for M to fall under it is far below a pixel.
    static constexpr double k_parallel_epsilon = 1e-12;

    point3 v0, v1, v2;
    vec3 va_minus_vb, va_minus_vc;
    vec3 n;          // unit geometric normal
    bool degenerate; // vertices collinear (or coincident)
    aabb box;

    static point3 min3(const point3& p, const point3& q, const point3& r) {
        return point3(std::fmin(p.x(), std::fmin(q.x(), r.x())),
                      std::fmin(p.y(), std::fmin(q.y(), r.y())),
                      std::fmin(p.z(), std::fmin(q.z(), r.z())));
    }

    static point3 max3(const point3& p, const point3& q, const point3& r) {
        return point3(std::fmax(p.x(), std::fmax(q.x(), r.x())),
                      std::fmax(p.y(), std::fmax(q.y(), r.y())),
                      std::fmax(p.z(), std::fmax(q.z(), r.z())));
    }
};

#endif
