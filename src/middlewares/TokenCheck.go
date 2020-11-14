package middlewares

import (
	"fmt"
	"github.com/gin-gonic/gin"
)

type TokenCheck struct{

}

func NewTokenCheck() *TokenCheck {
	return &TokenCheck{}
}

func (this *TokenCheck)OnRequest(ctx *gin.Context)error{
	if ctx.Query("token") == ""{
		//goft.Throw("token required", 503, ctx)
		return fmt.Errorf("token required")
	}
	return nil
}

func (this *TokenCheck)OnResponse(result interface{})(interface{}, error){
	return result, nil
}