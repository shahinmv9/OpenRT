#include "PrimPlane.h"
#include "ray.h"
#include "Transform.h"

namespace rt {
	
	bool CPrimPlane::intersect(Ray& ray) const
	{
		float t = (m_origin - ray.org).dot(m_normal) / ray.dir.dot(m_normal);
		if (t < Epsilon || t > ray.t) return false;
		ray.t = t;
		// ray.u = 0;
		// ray.v = 0;
		ray.hit = shared_from_this();
		return true;
	}

	bool CPrimPlane::if_intersect(const Ray& ray) const
	{
		float t = (m_origin - ray.org).dot(m_normal) / ray.dir.dot(m_normal);
		if (t < Epsilon || t > ray.t) return false;
		return true;
	}

	void CPrimPlane::transform(const Mat& t)
	{
		// Transform origin
		m_origin = CTransform::point(m_origin, t);

		// Transform normals
		Mat t1 = t.inv().t();
		m_normal = normalize(CTransform::vector(m_normal, t1));
	}

	Vec2f CPrimPlane::getTextureCoords(const Ray& ray) const
	{
		Vec3f mu;
		for (int i = 0; i < 3; i++)
			if (m_normal.val[i] >= 1.0f / sqrtf(3)) {
				mu.val[i] = 0;
				mu.val[(i + 1) % 3] = 1;
				mu.val[(i + 2) % 3] = m_normal.val[(i + 2) % 3] > Epsilon ? -m_normal.val[(i + 1) % 3] / m_normal.val[(i + 2) % 3] : 0;
				break;
			}
		mu = normalize(mu);
		Vec3f mv = normalize(m_normal.cross(mu));
		
		Vec3f h = ray.hitPoint() - m_origin;

		return Vec2f(h.dot(mu), h.dot(mv));
	}

	CBoundingBox CPrimPlane::calcBounds(void) const
	{
		float inf = std::numeric_limits<float>::infinity();
		Vec3f minPoint = Vec3f::all(-inf);
		Vec3f maxPoint = Vec3f::all(inf);
		for (int i = 0; i < 3; i++)
			if (m_normal.val[i] == 1) {
				minPoint.val[i] = m_origin.val[i];
				maxPoint.val[i] = m_origin.val[i];
				break;
			}
		return CBoundingBox(minPoint, maxPoint);
	}
}
