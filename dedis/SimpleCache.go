package dedis

import (
	"bytes"
	"encoding/gob"
	"encoding/json"
	"time"
)

const (
	Serilizer_JSON = "json"
	Serilizer_GOB  = "gob"
)

type DBGetterFunc func() interface{}

type SimpleCache struct {
	Operation *StringOperation // 操作类
	Expire    time.Duration    // 过期时间
	DBGetter  DBGetterFunc     // 一旦缓存中没有 db获取的方法
	Serilizer string           // 序列化方式
}

func NewSimpleCache(operation *StringOperation, expire time.Duration, serilizer string) *SimpleCache {
	return &SimpleCache{Operation: operation, Expire: expire, Serilizer: serilizer}
}

func (this *SimpleCache) SetCache(key string, value interface{}) {
	this.Operation.Set(key, value, WithExpire(this.Expire)).UnWrap()
}

func (this *SimpleCache) GetCacheForObject(key string, obj interface{}) interface{} {
	ret := this.GetCache(key)
	if ret == nil {
		return nil
	}
	if this.Serilizer == Serilizer_JSON {
		err := json.Unmarshal([]byte(ret.(string)), obj)
		if err != nil {
			return nil
		}
	} else if this.Serilizer == Serilizer_GOB {
		var buf = &bytes.Buffer{}
		buf.WriteString(ret.(string))
		dec := gob.NewDecoder(buf)
		if dec.Decode(obj) != nil {
			return nil
		}
	}
	return nil
}

func (this *SimpleCache) GetCache(key string) (ret interface{}) {
	obj := this.DBGetter()
	if this.Serilizer == Serilizer_JSON {
		f := func() string {
			b, err := json.Marshal(obj)
			if err != nil {
				return ""
			}
			return string(b)
		}
		ret = this.Operation.Get(key).Unwrap_Or_Else(f)
		this.SetCache(key, ret)
	} else if this.Serilizer == Serilizer_GOB {
		f := func() string {
			var buf = &bytes.Buffer{}
			enc := gob.NewEncoder(buf)
			if err := enc.Encode(obj); err != nil {
				return ""
			}
			return buf.String()
		}
		ret = this.Operation.Get(key).Unwrap_Or_Else(f)
		this.SetCache(key, ret)
	}
	return
}
