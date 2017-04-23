#ifndef NEOGO_H
#define NEOGO_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
	// Resources
	uint64_t rsrc_new(int pform, int device);
	void rsrc_free(uint64_t r);

	// Architect
	uint64_t arch_new();
	void arch_free(uint64_t a);
	void arch_initialize(uint64_t a, int x, uint64_t r);
	uint64_t arch_addInputLayer(uint64_t a, uint64_t v);
	uint64_t arch_addHigherLayer(uint64_t a, uint64_t v);
	uint64_t arch_generateHierarchy(uint64_t a);

	// ParameterModifier
	void pMod_free(uint64_t p);
	uint64_t pMod_setValStr(uint64_t p, char * name, char * val);
	uint64_t pMod_setValF(uint64_t p, char * name, float val);
	uint64_t pMod_setValV2i(uint64_t p, char * name, uint64_t val);
	uint64_t pMod_setValV2f(uint64_t p, char * name, uint64_t val);

	// Vec2i
	uint64_t v2i_new(int x, int y);
	void v2i_free(uint64_t v);

	// Vec2f
	uint64_t v2f_new(float x, float y);
	void v2f_free(uint64_t v);

	// Hierarchy
	void hrch_free(uint64_t h);
	void hrch_activate(uint64_t h, uint64_t in);
	void hrch_learn(uint64_t h, uint64_t in);
	uint64_t hrch_getPredictions(uint64_t h);

	// ValueField2D
	uint64_t vf2d_new(uint64_t size);
	void vf2d_free(uint64_t v);
	uint64_t vf2d_fromArr(float * arr, int x, int y);
	void vf2d_toArr(uint64_t v, float * arr);
#ifdef __cplusplus
}
#endif
#endif
