package lib

import (
	"jiajun/redis/dedis"
	"log"
)

func NewDBGetter(id string) dedis.DBGetterFunc {
	return func() interface{} {
		log.Println("get from db")
		newsModel := NewNewsModel()
		Gorm.Table("mynews").Where("id=?", id).Find(newsModel)
		if newsModel.NewID == 0 {
			return nil
		}
		return newsModel
	}
}
