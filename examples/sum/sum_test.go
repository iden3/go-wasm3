package main

import (
	"io/ioutil"
	"testing"

	wasm3 "github.com/matiasinsaurralde/go-wasm3"
)

var (
	wasmBytes []byte
)

func init() {
	var err error
	wasmBytes, err = ioutil.ReadFile(wasmFilename)
	if err != nil {
		panic(err)
	}
}

func TestSum(t *testing.T) {
	env := wasm3.NewEnvironment()
	runtime := wasm3.NewRuntime(env, 64*1024)
	_, err := runtime.Load(wasmBytes)
	if err != nil {
		t.Fatal(err)
	}
	fn, err := runtime.FindFunction(fnName)
	if err != nil {
		t.Fatal(err)
	}
	fn("1", "2")
}

func BenchmarkSum(b *testing.B) {
	for n := 0; n < b.N; n++ {
		env := wasm3.NewEnvironment()
		runtime := wasm3.NewRuntime(env, 64*1024)
		_, err := runtime.Load(wasmBytes)
		if err != nil {
			b.Fatal(err)
		}
		fn, err := runtime.FindFunction(fnName)
		if err != nil {
			b.Fatal(err)
		}
		fn("1", "2")
	}
}

func BenchmarkSumReentrant(b *testing.B) {
	env := wasm3.NewEnvironment()
	runtime := wasm3.NewRuntime(env, 64*1024)
	_, err := runtime.Load(wasmBytes)
	if err != nil {
		b.Fatal(err)
	}
	fn, err := runtime.FindFunction(fnName)
	if err != nil {
		b.Fatal(err)
	}
	for n := 0; n < b.N; n++ {
		fn("1", "2")
	}
}
