package dedis

type InterfaceResult struct {
	Result interface{}
	Err    error
}

func NewInterfaceResult(result interface{}, err error) *InterfaceResult {
	return &InterfaceResult{Result: result, Err: err}
}

func (this *InterfaceResult) UnWrap() interface{} {
	if this.Err != nil {
		panic(this.Err)
	}
	return this.Result
}

func (this *InterfaceResult) UnWrap_Or(value interface{}) interface{} {
	if this.Err != nil {
		return value
	}
	return this.Result
}
