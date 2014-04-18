package test

// fibonacci is a function that returns
// a function that returns an int.
func fibonacci() func() int {
    x := 0
    y := 0
    return func() int {
        n := x + y
        x = y
        if n == 0 {
            y = 1
        } else {
            y = n
        }
        return y
    }
}

