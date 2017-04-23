package neogo

/*
#cgo LDFLAGS: /usr/local/lib/libOgmaNeo.so
#cgo LDFLAGS: /opt/intel/opencl/lib64/libOpenCL.so
#cgo CPPFLAGS: -I/usr/local/include/ogmaneo
#include "neogo.h"
*/
import "C"

// Resources ...
type Resources interface {
	Free()
	Ptr() rsrcPtr
}

// NewResources allocates a new CL Resources
// and returns an ogmaneo::Resources*
func NewResources(platform, device int) Resources {
	p := C.rsrc_new(C.int(platform), C.int(device))
	return rsrcPtr(uint64(p))
}

type rsrcPtr uintptr

// Free frees the underlying object.
func (r rsrcPtr) Free() {
	C.rsrc_free(C.uint64_t(r))
}

// Ptr returns the underlying pointer.
func (r rsrcPtr) Ptr() rsrcPtr { return r }

// Architect ...
type Architect interface {
	Free()
	Ptr() archPtr
	Initialize(x int, r Resources)
	AddInputLayer(x, y int) ParameterModifier
	AddHigherLayer(x, y int) ParameterModifier
	GenerateHierarchy() Hierarchy
}

// NewArchitect allocates a new Architect and
// returns an ogmaneo::Architect*
func NewArchitect() Architect {
	p := C.arch_new()
	return archPtr(uint64(p))
}

type archPtr uintptr

// Free frees the underlying object.
func (a archPtr) Free() {
	C.arch_free(C.uint64_t(a))
}

// Ptr returns the underlying pointer.
func (a archPtr) Ptr() archPtr { return a }

// Initialize an architect with rng seed x and archPtr
// r. This method will take ownership of r once called,
// _do not_ reuse r with any other architects. Likewise,
// do not manually call Free() on r if Initialize has
// been called.
func (a archPtr) Initialize(x int, r Resources) {
	C.arch_initialize(C.uint64_t(a), C.int(x), C.uint64_t(r.Ptr()))
}

func (a archPtr) AddInputLayer(x, y int) ParameterModifier {
	sv := NewV2i(x, y)
	p := C.arch_addInputLayer(C.uint64_t(a.Ptr()), C.uint64_t(sv))
	return pModPtr(p)
}

func (a archPtr) AddHigherLayer(x, y int) ParameterModifier {
	sv := NewV2i(x, y)
	p := C.arch_addHigherLayer(C.uint64_t(a.Ptr()), C.uint64_t(sv))
	return pModPtr(p)
}

func (a archPtr) GenerateHierarchy() Hierarchy {
	p := C.arch_generateHierarchy(C.uint64_t(a))
	return hrchPtr(p)
}

// ParameterModifier
type ParameterModifier interface {
	SetValue(name string, val interface{}) ParameterModifier
	Free()
	Ptr() pModPtr
}

type pModPtr uintptr

func (p pModPtr) SetValue(name string, val interface{}) ParameterModifier {
	switch val.(type) {
	case string:
		rp := C.pMod_setValStr(
			C.uint64_t(p),
			C.CString(name),
			C.CString(val.(string)))
		return pModPtr(rp)
	case float32:
		rp := C.pMod_setValF(
			C.uint64_t(p),
			C.CString(name),
			C.float(val.(float32)))
		return pModPtr(rp)
	case float64:
		rp := C.pMod_setValF(
			C.uint64_t(p),
			C.CString(name),
			C.float(val.(float64)))
		return pModPtr(rp)
	case v2iPtr:
		rp := C.pMod_setValV2i(
			C.uint64_t(p),
			C.CString(name),
			C.uint64_t(val.(v2iPtr)))
		return pModPtr(rp)
	case v2fPtr:
		rp := C.pMod_setValV2f(
			C.uint64_t(p),
			C.CString(name),
			C.uint64_t(val.(v2fPtr)))
		return pModPtr(rp)
	}

	panic("unknown type: val in SetValue(name string, val interface{})")
}

// Free frees the underlying memory.
func (p pModPtr) Free() {
	C.pMod_free(C.uint64_t(p))
}

// Ptr returns the underlying pointer.
func (p pModPtr) Ptr() pModPtr { return p }

// Vec2i ...
type v2iPtr uintptr

func NewV2i(x, y int) v2iPtr {
	v := C.v2i_new(C.int(x), C.int(y))
	return v2iPtr(v)
}

func (v v2iPtr) Free() {
	C.v2i_free(C.uint64_t(v))
}

// Vec2f ...
type v2fPtr uintptr

func NewV2f(x, y float32) v2fPtr {
	v := C.v2f_new(C.float(x), C.float(y))
	return v2fPtr(v)
}

// Free frees the underlying memory.
func (v v2fPtr) Free() {
	C.v2f_free(C.uint64_t(v))
}

// Hierarchy ...
type Hierarchy interface {
	Free()
	Ptr() hrchPtr
	Activate(in vf2dPtr)
	Learn(in vf2dPtr)
	GetPredictions() vf2dPtr
}
type hrchPtr uintptr

// Free frees the underlying object.
func (h hrchPtr) Free() {
	C.hrch_free(C.uint64_t(h))
}

// Ptr returns the underlying pointer.
func (h hrchPtr) Ptr() hrchPtr { return h }

// Activate does a forward pass.
func (h hrchPtr) Activate(in vf2dPtr) {
	C.hrch_activate(C.uint64_t(h), C.uint64_t(in))
}

// Learn does a backwards pass.
func (h hrchPtr) Learn(in vf2dPtr) {
	C.hrch_learn(C.uint64_t(h), C.uint64_t(in))
}

// GetPredictions returns current predictions.
func (h hrchPtr) GetPredictions() vf2dPtr {
	p := C.hrch_getPredictions(C.uint64_t(h))
	return vf2dPtr(p)
}

// ValueField2D ...
type vf2dPtr uintptr

func NewVF2D(size v2iPtr) vf2dPtr {
	p := C.vf2d_new(C.uint64_t(size))
	return vf2dPtr(p)
}

func MakeVF2D(in [][]float32) vf2dPtr {
	d1 := make([]float32, len(in)*len(in[0]))
	for x := range in {
		for y := range in[x] {
			d1[x+y*len(in)] = in[x][y]
		}
	}

	p := C.vf2d_fromArr(
		(*C.float)(&d1[0]),
		C.int(len(in)),
		C.int(len(in[0])))

	// make sure GC doesn't grab d1 before we ready
	d1 = make([]float32, 0)

	return vf2dPtr(p)
}

func (vf vf2dPtr) Free() {
	C.vf2d_free(C.uint64_t(vf))
}

func (vf vf2dPtr) Slice(x, y int) [][]float32 {
	raw := make([]float32, x*y)
	C.vf2d_toArr(C.uint64_t(vf), (*C.float)(&raw[0]))

	ret := make([][]float32, x)
	for i := range ret {
		ret[i] = make([]float32, y)
	}

	for i := 0; i < x; i++ {
		for j := 0; j < y; j++ {
			ret[i][j] = raw[i+j*x]
		}
	}

	return ret
}
