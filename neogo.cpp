// neogo.cpp
#include "neogo.h"
#include <neo/Architect.h>
#include <neo/Hierarchy.h>
#include <stdint.h>
#include <vector>

/* Resources
 * new / free
 */
uint64_t rsrc_new(int pform, int device) {
	ogmaneo::Resources * res = new ogmaneo::Resources();
	res->create(ogmaneo::ComputeSystem::_cpu, pform, device);
	return (uint64_t) res;
}

void rsrc_free(uint64_t r) {
	ogmaneo::Resources * res = (ogmaneo::Resources*) r;
	delete res;
}

/* Architect
 * new / free
 * initialize
 * addInputLayer / addHigherLayer
 * generateHierarchy
 */
uint64_t arch_new() {
	ogmaneo::Architect * arch = new ogmaneo::Architect();
	return (uint64_t) arch;
}

void arch_free(uint64_t a) {
	ogmaneo::Architect * arch = (ogmaneo::Architect*) a;
	delete arch;
}

void arch_initialize(uint64_t a, int x, uint64_t r) {
	ogmaneo::Architect * arch = (ogmaneo::Architect*) a;
	ogmaneo::Resources * raw_r = (ogmaneo::Resources*) r;
	std::shared_ptr<ogmaneo::Resources> res(raw_r);	
	arch->initialize(x, res);
}

uint64_t arch_addInputLayer(uint64_t a, uint64_t v) {
	ogmaneo::Architect * arch = (ogmaneo::Architect*) a;
	ogmaneo::Vec2i * sv = (ogmaneo::Vec2i*) v;
	ogmaneo::ParameterModifier * par = new ogmaneo::ParameterModifier();

	*par = arch->addInputLayer(*sv);
	return (uint64_t) par;
}

uint64_t arch_addHigherLayer(uint64_t a, uint64_t v) {
	ogmaneo::Architect * arch = (ogmaneo::Architect*) a;
	ogmaneo::Vec2i * sv = (ogmaneo::Vec2i*) v;
	ogmaneo::ParameterModifier * par = new ogmaneo::ParameterModifier();

	*par = arch->addHigherLayer(*sv, ogmaneo::_chunk);
	return (uint64_t) par;
}

uint64_t arch_generateHierarchy(uint64_t a) {
	ogmaneo::Architect * arch = (ogmaneo::Architect*) a;
	ogmaneo::Hierarchy * hrch = new ogmaneo::Hierarchy();

	std::shared_ptr<ogmaneo::Hierarchy> hs = arch->generateHierarchy();
	*hrch = *hs;
	return (uint64_t) hrch;
}

/* ParameterModifier
 * free
 * setVal[ Str | F | V2i | V2f ]
 */
void pMod_free(uint64_t p) {
	ogmaneo::ParameterModifier * pr = (ogmaneo::ParameterModifier*) p;
	delete pr;
}

uint64_t pMod_setValStr(uint64_t p, char * name, char * val) {
	ogmaneo::ParameterModifier * pr = (ogmaneo::ParameterModifier*) p;

	*pr = pr->setValue(name, val);
	return (uint64_t) pr;
}

uint64_t pMod_setValF(uint64_t p, char * name, float val) {
	ogmaneo::ParameterModifier * pr = (ogmaneo::ParameterModifier*) p;

	*pr = pr->setValue(name, val);
	return (uint64_t) pr;
}

uint64_t pMod_setValV2i(uint64_t p, char * name, uint64_t val) {
	ogmaneo::ParameterModifier * pr = (ogmaneo::ParameterModifier*) p;
	ogmaneo::Vec2i * sval = (ogmaneo::Vec2i*) val;

	*pr = pr->setValue(name, *sval);
	return (uint64_t) pr;
}

uint64_t pMod_setValV2f(uint64_t p, char * name, uint64_t val) {
	ogmaneo::ParameterModifier * pr = (ogmaneo::ParameterModifier*) p;
	ogmaneo::Vec2f * sval = (ogmaneo::Vec2f*) val;

	*pr = pr->setValue(name, *sval);
	return (uint64_t) pr;
}

/* Vec2i
 * new / free
 */
uint64_t v2i_new(int x, int y) {
	ogmaneo::Vec2i * sv = new ogmaneo::Vec2i(x, y);
	return (uint64_t) sv;
}

void v2i_free(uint64_t v) {
	ogmaneo::Vec2i * sv = (ogmaneo::Vec2i*) v;
	delete sv;
}

/* Vec2f
 * new / free
 */
uint64_t v2f_new(float x, float y) {
	ogmaneo::Vec2f * sv = new ogmaneo::Vec2f(x, y);
	return (uint64_t) sv;
}

void v2f_free(uint64_t v) {
	ogmaneo::Vec2f * sv = (ogmaneo::Vec2f*) v;
	delete sv;
}

/* Hierarchy
 * free
 * activate / learn
 * getPredictions
 */
void hrch_free(uint64_t h) {
	ogmaneo::Hierarchy * hrch = (ogmaneo::Hierarchy*) h;
	delete hrch;
}

void hrch_activate(uint64_t h, uint64_t in) {
	ogmaneo::Hierarchy * hrch = (ogmaneo::Hierarchy*) h;
	ogmaneo::ValueField2D * inv = (ogmaneo::ValueField2D*) in;

	std::vector<ogmaneo::ValueField2D> feed =
		std::vector<ogmaneo::ValueField2D>{ *inv };
	hrch->activate(feed);
}

void hrch_learn(uint64_t h, uint64_t in) {
	ogmaneo::Hierarchy * hrch = (ogmaneo::Hierarchy*) h;
	ogmaneo::ValueField2D * inv = (ogmaneo::ValueField2D*) in;

	std::vector<ogmaneo::ValueField2D> feed =
		std::vector<ogmaneo::ValueField2D>{ *inv };
	hrch->learn(feed);
}

uint64_t hrch_getPredictions(uint64_t h) {
	ogmaneo::Hierarchy * hrch = (ogmaneo::Hierarchy*) h;
	ogmaneo::ValueField2D * pred = new ogmaneo::ValueField2D;

	*pred = hrch->getPredictions().front();
	return (uint64_t) pred;
}

/* ValueField2D
 * new / free
 * fromArr / toArr
 */
uint64_t vf2d_new(uint64_t size) {
	ogmaneo::Vec2i * sizev = (ogmaneo::Vec2i*) size;
	ogmaneo::ValueField2D * vf = new ogmaneo::ValueField2D();

	vf->create(*sizev);
	return (uint64_t) vf;
}

void vf2d_free(uint64_t v) {
	ogmaneo::ValueField2D * vf = (ogmaneo::ValueField2D*) v;
	delete vf;
}

uint64_t vf2d_fromArr(float * arr, int x, int y) {
	ogmaneo::Vec2i * sv = new ogmaneo::Vec2i(x, y);
	ogmaneo::ValueField2D * vf = new ogmaneo::ValueField2D();
	vf->create(*sv);

	ogmaneo::Vec2i pos;
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			pos = ogmaneo::Vec2i(i, j);
			float val = arr[i+j*x];
			vf->setValue(pos, val);
		}
	}

	return (uint64_t) vf;
}

void vf2d_toArr(uint64_t v, float * arr) {
	// from v -> arr
	ogmaneo::ValueField2D * vf = (ogmaneo::ValueField2D*) v;
	std::vector<float> data = vf->getData();
	
	// iterate through data, one -> one with arr
	for (int i = 0; i < data.size(); i++) {
		arr[i] = data[i];
	}
}
