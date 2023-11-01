/*
 * @Author: gongluck
 * @Date: 2023-10-30 15:52:32
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-10-30 16:03:43
 */

package main

import (
	"log"
	"os"
	"strings"
	"text/template"
)

func main() {
	const (
		// {{block "list" .}} 的存在是为了在 master 模板中使用 "list" 模板块的定义。
		master  = `Names:{{block "list" .}}{{"\n"}}{{range .}}{{println "-" .}}{{end}}{{end}}`
		overlay = `{{define "list"}} {{join . ", "}}{{end}} `
	)
	var (
		funcs     = template.FuncMap{"join": strings.Join}
		guardians = []string{"Gamora", "Groot", "Nebula", "Rocket", "Star-Lord"}
	)
	masterTmpl, err := template.New("master").Funcs(funcs).Parse(master)
	if err != nil {
		log.Fatal(err)
	}
	overlayTmpl, err := template.Must(masterTmpl.Clone()).Parse(overlay)
	if err != nil {
		log.Fatal(err)
	}
	if err := masterTmpl.Execute(os.Stdout, guardians); err != nil {
		log.Fatal(err)
	}
	if err := overlayTmpl.Execute(os.Stdout, guardians); err != nil {
		log.Fatal(err)
	}
}
