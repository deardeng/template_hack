package controllers

import (
	"github.com/gin-gonic/gin"
	"github.com/shenyisyn/goft-gin/goft"
	"time"
)

type IndexController struct{}

func NewIndexController()*IndexController{
	return &IndexController{}
}

func getTimestamp() (beginTimeNum, endTimeNum int64) {
	timeStr := time.Now().Format("2006-01-02")
	t, _ := time.ParseInLocation("2006-01-02", timeStr, time.Local)
	beginTimeNum = t.Unix()
	endTimeNum = beginTimeNum + 86400
	return beginTimeNum, endTimeNum
}

func(this* IndexController)Index(ctx *gin.Context)goft.Json{

	//goft.Throw("test exception", 500, ctx)
	//return gin.H{"result":"index"}
	begin, end:= getTimestamp()
	return gin.H{"begin":begin, "end":end}
}

func(this* IndexController)Name()string{
	return "indexController"
}

func(this* IndexController)Build(goft *goft.Goft){
	goft.Handle("GET", "/", this.Index)
}
