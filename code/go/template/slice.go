/*
 * @Author: gongluck
 * @Date: 2023-11-03 14:47:09
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-11-06 15:56:12
 */

package main

import (
	"log"
	"os"
	"text/template"
)

func makeslice(from, to int) ([]int, error) {
	result := make([]int, to-from)
	for i := from; i < to; i++ {
		result[i-from] = i
	}
	return result, nil
}

func appendslice(s []int, v int) ([]int, error) {
	return append(s, v), nil
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
{{- $numbers = appendslice $numbers 100 -}}
numbers: {{printf "%#v" .}}
range:
{{- range $numbers }}
	{{ printf "%#v" .}}
{{- end}}
`

	data := []string{"one", "two", "third"}

	t := template.Must(template.New("slice").Funcs(template.FuncMap{
		"makeslice":   makeslice,
		"appendslice": appendslice,
	}).Parse(text))

	err := t.Execute(os.Stdout, data)
	if err != nil {
		log.Println("executing template:", err)
	}
}
