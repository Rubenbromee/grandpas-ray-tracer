#pragma once
#include "ray.h"
#include "util.h"

// Lights are an emitting material that can be put on any geometry
enum material_enum {
	LAMBERTIAN,
	METAL,
	DIELECTRIC,
	LIGHT
};

struct hit_record; // To avoid circular dependency between material.h and geometry.h

color emitted(const hit_record& rec);
bool lambertian_scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered_ray, double& pdf);
double lambertian_scatter_pdf(const ray& ray_in, const hit_record& rec, const ray& scattered_ray);
bool metallic_reflection(const ray& ray_in, const hit_record& rec, color& attenuation, ray& reflected_ray, double metallic_fuzz);
double reflectance(double cosine, double ref_idx);
bool dielectric_refraction(const ray& ray_in, const hit_record& rec, color& attenuation, ray& reflected_ray, double refraction_index);