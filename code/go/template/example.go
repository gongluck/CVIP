/*
 * @Author: gongluck
 * @Date: 2023-10-30 15:28:12
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-10-30 15:51:02
 */

package main

import (
	"log"
	"os"
	"text/template"
)

func main() {
	// Define a template.
	/* {{- 表示删除所有尾部空格符 */
	/*  -}} 表示删除所有前部空格符 */
	/* {{.}} 在with块中匹配成with后的参数 */
	const letter = `
Dear {{.Name}},
{{if .Attended}}
It was a pleasure to see you at the wedding.
{{- else}}
It is a shame you couldn't make it to the wedding.																
{{- end}}
{{with .Gift -}}
    Thank you for the lovely {{.}}. 
{{end}}
Best wishes,
Josie
`

	// Prepare some data to insert into the template.
	type Recipient struct {
		Name, Gift string
		Attended   bool
	}
	var recipients = []Recipient{
		{"Aunt Mildred", "bone china tea set", true},
		{"Uncle John", "moleskin pants", false},
		{"Cousin Rodney", "", false},
	}

	// Create a new template and parse the letter into it.
	t := template.Must(template.New("letter").Parse(letter))

	// Execute the template for each recipient.
	for _, r := range recipients {
		err := t.Execute(os.Stdout, r)
		if err != nil {
			log.Println("executing template:", err)
		}
	}

}
