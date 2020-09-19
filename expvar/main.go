package main

import (
	"expvar"
	"net/http"
	"time"
)

func main(){
	lastAccess := expvar.NewString("lastAccess")
	http.HandleFunc("/access", func(w http.ResponseWriter, req *http.Request){
		w.Write([]byte("hello"))
		lastAccess.Set(time.Now().String())
	})
	http.ListenAndServe(":8881", nil)
}

/*
$ http :8080/debug/vars | grep lastAccess
"lastAccess": "2020-04-12 09:52:14.127334409 +0800 CST m=+15.783055827",
$ http :8080/
HTTP/1.1 200 OK
Content-Length: 5
Content-Type: text/plain; charset=utf-8
Date: Sun, 12 Apr 2020 01:52:27 GMT

hello

$ http :8080/debug/vars | grep lastAccess
"lastAccess": "2020-04-12 09:52:27.533390067 +0800 CST m=+29.189111480",

 */


/*
// 从 NewString 入手
lastAccess := expvar.NewString("lastAccess")

func NewString(name string) *String {
	v := new(String)
	Publish(name, v)
	return v
}

// 看看String的定义
type String struct {
	s atomic.Value // string
}

// 看看Publish的定义
// Publish declares a named exported variable. This should be called from a
// package's init function when it creates its Vars. If the name is already
// registered then this will log.Panic.
func Publish(name string, v Var) {
	if _, dup := vars.LoadOrStore(name, v); dup {
		log.Panicln("Reuse of exported var name:", name)
	}
	varKeysMu.Lock()
	defer varKeysMu.Unlock()
	varKeys = append(varKeys, name)
	sort.Strings(varKeys)
}

// 可以看到 Var 是一个接口
// Var is an abstract type for all exported variables.
type Var interface {
	// String returns a valid JSON value for the variable.
	// Types with String methods that do not return valid JSON
	// (such as time.Time) must not be used as a Var.
	String() string
}

// vars 和 varKeys 的定义是这样的
// All published variables.
var (
	vars      sync.Map // map[string]Var
	varKeysMu sync.RWMutex
	varKeys   []string // sorted
)

// 所以可以看到，逻辑就是每次把数值存储到 `vars`，`vars` 是一个map[string]Var类型的map。因为map迭代时是无序的，
// 所以有 `varKeys` 用来排序，这样输出的时候，就可以每次都有序输出

// 来看看如何暴露这些变量
func expvarHandler(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json; charset=utf-8")
	fmt.Fprintf(w, "{\n")
	first := true
	Do(func(kv KeyValue) {
		if !first {
			fmt.Fprintf(w, ",\n")
		}
		first = false
		fmt.Fprintf(w, "%q: %s", kv.Key, kv.Value)
	})
	fmt.Fprintf(w, "\n}\n")
}

// 可以看到，很粗暴，先输出 `{\n`，然后输出key value的字符串，最后输出 `\n}\n`，相当于代码拼接JSON
// Do这个函数用来迭代
// Do calls f for each exported variable.
// The global variable map is locked during the iteration,
// but existing entries may be concurrently updated.
func Do(f func(KeyValue)) {
	varKeysMu.RLock()
	defer varKeysMu.RUnlock()
	for _, k := range varKeys {
		val, _ := vars.Load(k)
		f(KeyValue{k, val.(Var)})
	}
}

// 就如我们前面所说，按 varKeys 的顺序来迭代，然后依次执行传入的函数
 */