package lib

type NewsModel struct{
	NewID int `gorm:"column:id" json:"id"`
	NewsTitle string `gorm:"column:newstitle" json:"title"`
	NewContent string `gorm:"column:newcontent" json:"content"`
	NewViews int `gorm:"column:views" json:"views"`
	NewTime int64 `gorm:"column:addtime" json:"addtime"`
}

func NewNewsModel() *NewsModel{
	return &NewsModel{}
}