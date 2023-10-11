#pragma once
#include "ray.h"
#include "util.h"

// Lights are an emitting material that can be put on any geometry
enum material_enum {
	LAMBERTIAN,
	METAL,
	DIELECTRIC,
	LIGHT,
	CONSTANT_DENSITY_MEDIUM_MATERIAL
};

// To avoid circular dependency between material.h and geometry.h
struct hit_record; 
struct scene_object;

color emitted(const hit_record& rec);
bool lambertian_scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered_ray, double& pdf);
double lambertian_scatter_pdf(const ray& ray_in, const hit_record& rec, const ray& scattered_ray);
bool metallic_reflection(const ray& ray_in, const hit_record& rec, color& attenuation, ray& reflected_ray, double metallic_fuzz);
double reflectance(double cosine, double ref_idx);
bool dielectric_refraction(const ray& ray_in, const hit_record& rec, color& attenuation, ray& reflected_ray, double refraction_index);
bool constant_density_medium_scatter(const hit_record& rec, color& attenuation, ray& ray_out);