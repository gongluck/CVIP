/*
 * @Author: gongluck
 * @Date: 2023-10-30 16:05:37
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-11-06 14:33:27
 */

package main

import (
	"errors"
	"log"
	"os"
	"strings"
	"text/template"
)

// 内部函数
func function(a, b, c int) (int, string, error) {
	if a == 0 {
		return 0, "failed", errors.New("number is 0")
	}
	return a + b + c, "succeed", nil
}

func main() {
	c := 100
	// First we create a FuncMap with which to register the function.
	funcMap := template.FuncMap{
		// The name "title" is what the function will be called in the template text.
		"title": strings.Title,
		"testfunc": func(a, b int) (int, error) { // 映射进模板的函数
			ret, _, err := function(a, b, c) // 捕获外部的变量c
			return ret, err
		},
	}

	c = 101

	// A simple template definition to test our function.
	// We print the input text several ways:
	// - the original
	// - title-cased
	// - title-cased and then printed with %q
	// - printed with %q and then title-cased.
	const templateText = `
Input: {{printf "%q" .}}
Output 0: {{title .}}
Output 1: {{title . | printf "%q"}}
Output 2: {{printf "%q" . | title}}
Output 3: {{ testfunc 1 2 | print}}
Output 4: {{ testfunc 0 2 | print}}
Output 5: {{ testfunc 1 2 | print}}
`

	// Create a template, add the function map, and parse the text.
	tmpl, err := template.New("titleTest").Funcs(funcMap).Parse(templateText)
	if err != nil {
		log.Fatalf("parsing: %s", err)
	}

	// Run the template to verify the output.
	err = tmpl.Execute(os.Stdout, "the go programming language")
	if err != nil {
		log.Fatalf("execution: %s", err)
	}

}
