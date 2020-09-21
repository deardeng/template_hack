package lib

import (
	"jiajun/dedis"
	"sync"
	"time"
)

var NewsCachePool *sync.Pool

func init() {
	NewsCachePool = &sync.Pool{
		New: func() interface{} {
			return dedis.NewSimpleCache(dedis.NewStringOperation(), time.Second*15, dedis.Serilizer_JSON,
				dedis.NewCrossPolicy("^news\\d{1,5}$", time.Second*30),
			) // 指定序列化方式Gob
		},
	}
}

func NewsCache() *dedis.SimpleCache {
	return NewsCachePool.Get().(*dedis.SimpleCache)
}

func ReleaseNewsCache(cache *dedis.SimpleCache) {
	NewsCachePool.Put(cache)
}
