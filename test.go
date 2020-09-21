package main

import (
	"github.com/gin-gonic/gin"
	"jiajun/dedis"
	"jiajun/lib"
	"log"
	"time"
)

func main() {
	iter := dedis.NewStringOperation().MGet("abc", "abcd").Iter()
	for iter.HasNext() {
		log.Println(iter.Next())
	}

	log.Println(dedis.NewStringOperation().
		Set("deardeng", "dengxin",
			dedis.WithExpire(100*time.Second),
			dedis.WithXX(), // setnx
		))

	r := gin.New()
	r.Handle("GET", "/news/:id", func(context *gin.Context) {
		// 新闻缓存，假设我们认为他的过期时间 = 15s
		newsCache := lib.NewsCache()
		defer lib.ReleaseNewsCache(newsCache)

		// 新闻的缓存key: news123 news101
		newsID := context.Param("id")
		newsCache.DBGetter = lib.NewDBGetter(newsID)

		//context.Header("Content-type", "application/json")
		//context.String(200, newsCache.GetCache("news"+newsID).(string))
		newsModel := lib.NewNewsModel()
		newsCache.GetCacheForObject("news"+newsID, newsModel)
		context.JSON(200, newsModel)
	})
	r.Run(":8080")
}
