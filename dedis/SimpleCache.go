package dedis

import "time"

type DBGettFunc func() string

type SimpleCache struct{
	Operation *StringOperation
	Expire time.Duration
	DBGetter DBGettFunc
}

func NewSimpleCache(operation *StringOperation, expire time.Duration) *SimpleCache {
	return &SimpleCache{Operation: operation, Expire: expire}
}

func (this *SimpleCache)SetCache(key string, value interface{}){
	this.Operation.Set(key, value, WithExpire(this.Expire)).UnWrap()
}

func (this *SimpleCache)GetCache(key string)(ret interface{}){
	ret = this.Operation.Get(key).Unwrap_Or_Else(this.DBGetter)
	this.SetCache(key, ret)
	return
}