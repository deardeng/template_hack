package main

import (
	"github.com/shenyisyn/goft-gin/goft"
	"go_gin/src/controllers"
	"go_gin/src/middlewares"
)

func main(){
	goft.
		Ignite().
		Attach(middlewares.NewTokenCheck()).
		Mount("v1", controllers.NewIndexController()).
		Launch()
}
