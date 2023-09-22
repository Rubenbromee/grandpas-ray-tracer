#pragma once
#include "ray.h"
#include "util.h"

enum material_enum {
	LAMBERTIAN,
	METAL,
	DIELECTRIC
};

struct hit_record; // To avoid circular dependency between material.h and geometry.h

bool lambertian_scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered_ray);
bool metallic_reflection(const ray& ray_in, const hit_record& rec, color& attenuation, ray& reflected_ray, double metallic_fuzz);
double reflectance(double cosine, double ref_idx);
bool dielectric_refraction(const ray& ray_in, const hit_record& rec, color& attenuation, ray& reflected_ray, double refraction_index);