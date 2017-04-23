package main

import (
	"fmt"

	o "github.com/nytopop/neogo"
)

func main() {
	// allocate resources & architect
	r := o.NewResources(0, 0)
	a := o.NewArchitect()
	defer a.Free()

	// r is converted to a shared_ptr, a takes ownership
	a.Initialize(1234, r)

	// input layer
	a.AddInputLayer(1, 1).
		SetValue("in_p_alpha", 0.02).
		SetValue("in_p_radius", 16.0)

	fmt.Printf("res  : 0x%x\narch : 0x%x\n",
		r.Ptr(), a.Ptr())

	// 6 hidden layers using chunk encoders
	for i := 0; i < 6; i++ {
		a.AddHigherLayer(64, 64).
			SetValue("sfc_chunkSize", o.NewV2i(12, 12)).
			SetValue("sfc_ff_radius", 12.0).
			SetValue("hl_poolSteps", 2.0).
			SetValue("p_alpha", 0.08).
			SetValue("p_beta", 0.16).
			SetValue("p_radius", 12.0).Free()
		i = i
	}

	h := a.GenerateHierarchy()
	defer h.Free()
	fmt.Printf("hrch : 0x%x\n\n", h.Ptr())

	for i := 0; i < 8; i++ {
		// Create a ValueField2D from [][]float32
		dat := make([][]float32, 1)
		dat[0] = make([]float32, 1)
		dat[0][0] = float32(i)
		in := o.MakeVF2D(dat)

		// activate
		h.Activate(in)
		fmt.Printf("Activated %02d; ", i)

		// learn
		h.Learn(in)
		fmt.Printf("Learned %02d; ", i)

		// free the ValueField2D
		in.Free()
		fmt.Printf("Freed %02d;\n", i)

		// get predictions
		p := h.GetPredictions().Slice(1, 1)
		fmt.Printf("this %.2f; next %.2f\n\n", dat[0][0], p[0][0])
	}
}
