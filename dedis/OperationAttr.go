package dedis

import (
	"fmt"
	"time"
)

const(
	ATTR_EXPRIRE = "expr" // 过期时间
	ATTR_NX = "nx" // setnx
	ATTR_XX = "xx" // setxx
)

type empty struct{}

type  OperationAttr struct{
	Name string
	Value interface{}
}

type OperationAttrs []*OperationAttr

func WithExpire(t time.Duration)*OperationAttr{
	return &OperationAttr{Name: ATTR_EXPRIRE, Value: t}
}

func WithNX()*OperationAttr{
	return &OperationAttr{Name: ATTR_NX, Value: empty{}}
}

func WithXX()*OperationAttr{
	return &OperationAttr{Name: ATTR_XX,Value: empty{}}
}

func (this OperationAttrs)Find(name string)*InterfaceResult{
	for _, attr := range this{
		if attr.Name == name{
			return NewInterfaceResult(attr.Value, nil)
		}

	}
	return NewInterfaceResult(nil, fmt.Errorf("OperationAttrs found error:%s", name))
}