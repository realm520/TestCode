package test

import (
    "testing"
)

func TestFabonacci(t *testing.T) {
	f := fibonacci()
	v := f()
    if v != 1 {
    	t.Errorf("fibonacci(%v) = %v, want %v", 0, v, 1)
    }
    if v = f(); v != 1 {
    	t.Errorf("fibonacci(%v) = %v, want %v", 1, v, 1)
    }
    if v = f(); v != 2 {
    	t.Errorf("fibonacci(%v) = %v, want %v", 2, v, 2)
    }
    if v = f(); v != 3 {
    	t.Errorf("fibonacci(%v) = %v, want %v", 3, v, 3)
    }
    if v = f(); v != 5 {
    	t.Errorf("fibonacci(%v) = %v, want %v", 4, v, 5)
    }
    if v = f(); v != 8 {
    	t.Errorf("fibonacci(%v) = %v, want %v", 5, v, 8)
    }
    if v = f(); v != 13 {
    	t.Errorf("fibonacci(%v) = %v, want %v", 6, v, 13)
    }
    if v = f(); v != 21 {
    	t.Errorf("fibonacci(%v) = %v, want %v", 7, v, 21)
    }
    if v = f(); v != 34 {
    	t.Errorf("fibonacci(%v) = %v, want %v", 8, v, 34)
    }
    if v = f(); v != 55 {
    	t.Errorf("fibonacci(%v) = %v, want %v", 9, v, 55)
    }
    if v = f(); v != 89 {
    	t.Errorf("fibonacci(%v) = %v, want %v", 10, v, 89)
    }
}

