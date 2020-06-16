#ifndef MATERIAL_H
#define MATERIAL_H

#include "util_functions.h"
#include "hittable.h"

class material {
	public:
		virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		) const = 0;
};

// Diffuse material
class lambertian : public material {
	public:
		lambertian(const color& a) : albedo(a) {}

		virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		) const {
			vec3 scatter_direction = rec.normal + random_unit_vector();
			scattered = ray(rec.p, scatter_direction);
			attenuation = albedo;
			return true;
		}

	public:
		color albedo;
};

// Metal material
class metal : public material {
	public:
		metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

		virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		) const {
			vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
			scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0);
		}

	public:
		color albedo;
		double fuzz;
};

// Refracting materal (like glass)
class dielectric : public material {
	public:
		dielectric(double ri) : ref_idx(ri) {}

		virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
		) const {
			attenuation = color(1.0, 1.0, 1.0);
			double etai_over_etat;
			if (rec.front_face) {
				etai_over_etat = 1.0 / ref_idx;
			}
			else {
				etai_over_etat = ref_idx;
			}

			vec3 unit_direction = unit_vector(r_in.direction());
			vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
			scattered = ray(rec.p, refracted);
			return true;
		}

		double ref_idx;
};

#endif
