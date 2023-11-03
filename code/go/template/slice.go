/*
 * @Author: gongluck
 * @Date: 2023-11-03 14:47:09
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-11-03 15:59:02
 */

package main

import (
	"log"
	"os"
	"text/template"
)

func makeslice(from, to int) []int {
	result := make([]int, to-from)
	for i := from; i < to; i++ {
		result[i-from] = i
	}
	return result
}

func main() {
	const text = `
data: {{printf "%#v" .}}
range:
{{- range . }}
	{{ printf "%#v" .}}
{{- end}}

{{- $numbers := makeslice 1 5 -}}
numbers: {{printf "%#v" .}}
range:
{{- range $numbers }}
	{{ printf "%#v" .}}
{{- end}}
`

	data := []string{"one", "two", "third"}

	t := template.Must(template.New("slice").Funcs(template.FuncMap{
		"makeslice": makeslice,
	}).Parse(text))

	err := t.Execute(os.Stdout, data)
	if err != nil {
		log.Println("executing template:", err)
	}

}
