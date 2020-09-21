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
			return dedis.NewSimpleCache(dedis.NewStringOperation(), time.Second*15)
		},
	}
}

func NewsCache() *dedis.SimpleCache {
	return NewsCachePool.Get().(*dedis.SimpleCache)
}

func ReleaseNewsCache(cache *dedis.SimpleCache) {
	NewsCachePool.Put(cache)
}
