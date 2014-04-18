package main

import (
    "fmt"
    "time"
)

type MyError struct {
    When time.Time
    What string
}

func (e *MyError) Error() string {
    return fmt.Sprintf("at %v, %s",
        e.When, e.What)
}

func run() error {
    return &MyError{
        time.Now(),
        "it didn't work",
    }
}

type ErrNegativeSqrt float64

func (e ErrNegativeSqrt) Error() string {
    return fmt.Sprintf("cannot Sqrt negative number: %v", float64(e))
}

func Sqrt(f float64) (float64, error) {
    if f < 0 {
        fmt.Println(f)
        var e ErrNegativeSqrt
        e = ErrNegativeSqrt(f)
        return 0, e
    }
    return 0, nil
}

func deferOder1() {
	fmt.Println("order1")
}

func deferOder2() {
	fmt.Println("order2")
}

func deferOder3() {
	fmt.Println("order3")
}

func main() {
    if err := run(); err != nil {
        fmt.Println(err)
    }
    defer deferOder1()
    defer deferOder2()
    defer deferOder3()
    fmt.Println(Sqrt(2))
    fmt.Println(Sqrt(-2))
}