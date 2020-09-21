package dedis

import "regexp"

type CachePolicy interface {
	Before(key string)
}

// 缓存穿透 策略
type CrossPolicy struct {
	KeyRegx string // 检查key的正则
}

func NewCrossPolicy(keyRegx string) *CrossPolicy {
	return &CrossPolicy{KeyRegx: keyRegx}
}

func (this *CrossPolicy) Before(key string) {
	if !regexp.MustCompile(this.KeyRegx).MatchString(key) {
		panic("error cache key")
	}
}
