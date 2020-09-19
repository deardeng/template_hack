package main

import (
	"encoding/json"
	"jiajun/dedis"
	"jiajun/lib"
	"log"
	"time"
)

func main(){
	iter := dedis.NewStringOperation().MGet("abc","abcd").Iter()
	for iter.HasNext(){
		log.Println(iter.Next())
	}

	log.Println(dedis.NewStringOperation().
		Set("deardeng", "dengxin",
		dedis.WithExpire(100*time.Second),
		dedis.WithXX(), // setnx
		))

	// 新闻缓存，假设我们认为他的过期时间 = 15s
	newsCache := dedis.NewSimpleCache(dedis.NewStringOperation(), time.Second*15)

	// 新闻的缓存key: news123 news101
	newsID := 1
	newsCache.DBGetter = func()string{
		log.Println("get from db")
		newsModel := lib.NewNewsModel()
		lib.Gorm.Table("mynews").Where("id=?", newsID).Find(newsModel)
		b, _:= json.Marshal(newsModel)
		return string(b)
	}
	log.Println(newsCache.GetCache("news123").(*lib.NewsModel).NewsTitle)
}