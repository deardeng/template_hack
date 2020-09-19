package main

import (
	"fmt"
	"strconv"
)

type GoPool struct{
	MaxLimit int
	tokenChan chan struct{}
}

type GoPoolOption func(*GoPool)

func WithMaxLimit(max int)GoPoolOption{
	return func(gp *GoPool){
		gp.MaxLimit = max
		gp.tokenChan = make(chan struct{}, gp.MaxLimit)

		for i := 0; i < gp.MaxLimit; i++{
			gp.tokenChan <- struct{}{}
		}
	}
}

func NewGoPool(options ...GoPoolOption)*GoPool{
	p := &GoPool{}
	for _, o := range options{
		o(p)
	}
	return p
}

// Submit will wait a token, and then execute fn
func (gp *GoPool) Submit(fn func(args ... interface{}), args ...interface{}){
	token := <-gp.tokenChan // if there are no tokens, we'll block here

	go func(args ... interface{}){
		fn(args...)
		gp.tokenChan <- token
	}(args...)
}

// Wait will wait all the tasks executed, and then return
func (gp *GoPool) Wait(){
	for i := 0; i < gp.MaxLimit; i++{
		<-gp.tokenChan
	}

	close(gp.tokenChan)
}

func main(){
	goPool := NewGoPool(WithMaxLimit(10))
	defer goPool.Wait()

	for i:=0; i<10000;i++ {
		goPool.Submit(func(args ...interface{}){
			argStr := ""
			for _, v := range args{
				argStr += strconv.Itoa(v.(int)) + " "
			}
			fmt.Printf("{%s}\n", argStr)
		}, i, i+100)
	}
}

/*
gopool.Submit 在令牌不足时，会阻塞当前调用(因此Go runtime会执行其他不阻塞的代码)
gopool.Wait() 会等到回收所有令牌之后，才返回
这样就可以实现我们的需求了，例如并发3个Goroutine执行任务。
 */