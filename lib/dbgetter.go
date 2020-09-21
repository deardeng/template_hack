package lib

import (
	"encoding/json"
	"jiajun/dedis"
	"log"
)

func NewDBGetter(id string) dedis.DBGetterFunc {
	log.Println("get from db")
	return func() string {
		newsModel := NewNewsModel()
		Gorm.Table("mynews").Where("id=?", id).Find(newsModel)
		b, _ := json.Marshal(newsModel)
		return string(b)
	}
}
